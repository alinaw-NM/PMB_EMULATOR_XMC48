# To log iwth standalone changer: python morelle_bms.py --channel PCAN_USBBUS2 --battery-addr 0x02 --log
"""Morelle Falcon 15 EB BMS — CAN bus client (250 kbps, 29-bit IDs).

Polls Data IDs 0x90 / 0x91 / 0x92 / 0x93 / 0x95 / 0x96 / 0x98 over the
Morelle proprietary request-response protocol (see
``CANOpen_morelle/260519_morelle_Battery_v1.0 (1).pdf`` p5-8). Exposes the
same ``AnalogTelemetry`` / ``AlarmEvents`` shape as ``scud_bms.py`` so the
safety loop in ``deltaq_web.py`` can treat both BMS types identically.

Wire format (extended 29-bit CAN IDs, 8-byte payload):

    Request  (host → battery):   0x18 <DataID> <BattAddr> <HostAddr>
    Response (battery → host):   0x18 <DataID> <HostAddr> <BattAddr>

The host sends an 8-byte all-zero payload. Multi-frame responses (0x95 cell
voltages, 0x96 cell temps) carry a 1-based frame number in byte 0.

Smoke test:
    python morelle_bms.py --channel PCAN_USBBUS2 --once
"""

from __future__ import annotations

import argparse
import csv
import dataclasses
import os
import threading
import time
from datetime import datetime
from typing import Optional

# Import-safe without python-can so the framing layer can be unit-tested.
try:
    import can  # type: ignore
except ImportError:  # pragma: no cover
    can = None  # type: ignore[assignment]


_PCAN_USB_HANDLES: dict[str, int] = {
    f"PCAN_USBBUS{i}": 0x50 + i for i in range(1, 9)
}


def pcan_uninit_channel(channel: str) -> None:
    """Uninitialize a PCAN USB channel before opening it.

    PCAN retains channel state across process restarts, so Initialize()
    returns PCAN_ERROR_INITIALIZE unless Uninitialize() is called first.
    Uninitializing a clean channel is harmless.
    """
    handle = _PCAN_USB_HANDLES.get(channel.upper())
    if handle is None:
        return
    try:
        from PCANBasic import PCANBasic  # type: ignore[import]
        PCANBasic().Uninitialize(handle)
    except Exception:
        pass


PRIORITY          = 0x18
HOST_ADDR_DEFAULT = 0x40
BATT_ADDR_DEFAULT = 0x01

DID_PACK_TOTALS   = 0x90
DID_CELL_EXTREMES = 0x91
DID_TEMP_EXTREMES = 0x92
DID_MOS_STATE     = 0x93
DID_CELL_VOLTAGES = 0x95
DID_CELL_TEMPS    = 0x96
DID_FAULT_STATUS  = 0x98

# DIDs that produce exactly one response frame — safe to request in a burst.
# 0x95 / 0x96 are multi-frame and remain sequential after the burst.
_BURST_DIDS: tuple[int, ...] = (
    DID_PACK_TOTALS, DID_CELL_EXTREMES, DID_MOS_STATE, DID_FAULT_STATUS
)

TEMP_OFFSET     = 40         # 0x92 / 0x96: °C = raw - 40
CURRENT_OFFSET  = 30000      # 0x90: A = (raw - 30000) * 0.1
DEFAULT_CELLS   = 13         # Falcon 15 EB = 13S3P
DEFAULT_TEMPS   = 4          # datasheet "4-sensor pack"
DEFAULT_BITRATE = 250000     # datasheet section "CAN Bus Settings"


def make_request_id(data_id: int, batt_addr: int, host_addr: int) -> int:
    return ((PRIORITY & 0xFF) << 24) | ((data_id & 0xFF) << 16) \
         | ((batt_addr & 0xFF) << 8) | (host_addr & 0xFF)


def make_response_id(data_id: int, batt_addr: int, host_addr: int) -> int:
    return ((PRIORITY & 0xFF) << 24) | ((data_id & 0xFF) << 16) \
         | ((host_addr & 0xFF) << 8) | (batt_addr & 0xFF)


# Same shape as scud_bms.AnalogTelemetry so deltaq_web._evaluate_safety
# and CSV logging don't care which BMS supplied the snapshot.
@dataclasses.dataclass
class AnalogTelemetry:
    data_flag:     int
    pack_num:      int
    total_v:       float
    current_a:     float
    soc:           int
    rmc_ah:        float
    cell_mv:       list[int]
    cell_high_idx: int
    cell_low_idx:  int
    cell_vdiff_mv: int
    cell_temps_c:  list[int]
    other_temps_c: list[int]
    # Fields the Morelle protocol does NOT expose — left as None rather
    # than fabricated, so the dashboard can render "—" instead of an
    # arbitrary value. Kept in the dataclass so callers can rely on the
    # same attribute names across BMS types (SCUD populates them).
    soh:           Optional[int]   = None
    cycle_count:   Optional[int]   = None
    fcc_ah:        Optional[float] = None
    design_cap_ah: Optional[float] = None
    # Morelle-only (0x93). SCUD's AnalogTelemetry does not have these;
    # deltaq_web reads them with getattr.
    charge_mos:    Optional[bool] = None
    discharge_mos: Optional[bool] = None
    bms_state:     Optional[str]  = None   # "idle" / "charging" / "discharging"
    life_counter:  Optional[int]  = None   # wrapping uint8 from 0x93 byte 3

    @property
    def all_temps_c(self) -> list[int]:
        return self.cell_temps_c + self.other_temps_c


# 0x98 fault layout from the Morelle datasheet (p7-8).
# Tuple: (byte_idx, bit, name, is_protection).
_FAULT_BITS: list[tuple[int, int, str, bool]] = [
    (0, 0, "cell over-voltage alarm",            False),
    (0, 1, "cell over-voltage protection",       True),
    (0, 2, "cell under-voltage alarm",           False),
    (0, 3, "cell under-voltage protection",      True),
    (0, 4, "pack over-voltage alarm",            False),
    (0, 5, "pack over-voltage protection",       True),
    (0, 6, "pack under-voltage alarm",           False),
    (0, 7, "pack under-voltage protection",      True),
    (1, 0, "charge over-temp alarm",             False),
    (1, 1, "charge over-temp protection",        True),
    (1, 2, "charge under-temp alarm",            False),
    (1, 3, "charge under-temp protection",       True),
    (1, 4, "discharge over-temp alarm",          False),
    (1, 5, "discharge over-temp protection",     True),
    (1, 6, "discharge under-temp alarm",         False),
    (1, 7, "discharge under-temp protection",    True),
    (2, 0, "charge over-current alarm",          False),
    (2, 1, "charge over-current protection",     True),
    (2, 2, "discharge over-current alarm",       False),
    (2, 3, "discharge over-current protection",  True),
    (2, 6, "SOC low alarm 1",                    False),
    (2, 7, "SOC low alarm 2",                    False),
    (3, 0, "cell voltage difference alarm 1",    False),
    (3, 1, "cell voltage difference alarm 2",    False),
    (3, 2, "temperature difference alarm 1",     False),
    (3, 3, "temperature difference alarm 2",     False),
    (4, 0, "charge MOS over-temp protection",    True),
    (4, 1, "discharge MOS over-temp protection", True),
]


@dataclasses.dataclass
class AlarmEvents:
    """Decoded 0x98 fault payload — kept as raw bytes so callers can both
    log the wire bytes and walk the bits via active() / protections()."""
    raw: bytes = b"\x00" * 8

    def _walk(self):
        for byte_idx, bit, name, is_prot in _FAULT_BITS:
            if byte_idx < len(self.raw) and (self.raw[byte_idx] >> bit) & 1:
                yield name, is_prot

    def active(self) -> list[str]:
        return [name for name, _ in self._walk()]

    def protections(self) -> list[str]:
        return [name for name, is_prot in self._walk() if is_prot]

    def as_bytes(self) -> list[int]:
        return list(self.raw)


def parse_fault_info(info: bytes) -> AlarmEvents:
    return AlarmEvents(raw=bytes(info))


def parse_pack_totals(data: bytes) -> tuple[float, float, int]:
    """0x90 -> (pack_v, current_a, soc_percent)."""
    pack_v = int.from_bytes(data[0:2], "big") * 0.1
    i_raw  = int.from_bytes(data[4:6], "big")
    current_a = (i_raw - CURRENT_OFFSET) * 0.1
    soc = int(round(int.from_bytes(data[6:8], "big") * 0.1))
    return pack_v, current_a, soc


def parse_cell_extremes(data: bytes) -> tuple[int, int, int, int]:
    """0x91 -> (vmax_mv, hi_idx_0based, vmin_mv, lo_idx_0based)."""
    vmax = int.from_bytes(data[0:2], "big")
    hi_i = max(0, data[2] - 1)
    vmin = int.from_bytes(data[3:5], "big")
    lo_i = max(0, data[5] - 1)
    return vmax, hi_i, vmin, lo_i


class MorelleBms:
    """Polling client for the Morelle Falcon 15 EB BMS over CAN.

    Owns its own python-can ``Bus``. The Morelle BMS runs at 250 kbps with
    29-bit extended IDs, so it cannot share the 125 kbps Delta-Q charger
    bus — supply a separate adapter channel.
    """

    def __init__(self, channel: str, *,
                 interface: str = "pcan",
                 bitrate: int = DEFAULT_BITRATE,
                 host_addr: int = HOST_ADDR_DEFAULT,
                 battery_addr: int = BATT_ADDR_DEFAULT,
                 num_cells: int = DEFAULT_CELLS,
                 num_temps: int = DEFAULT_TEMPS,
                 recv_timeout_s: float = 0.2,
                 txn_timeout_s: float = 0.4,
                 dump: bool = False):
        self.channel        = channel
        self.interface      = interface
        self.bitrate        = bitrate
        self.host_addr      = host_addr
        self.battery_addr   = battery_addr
        self.num_cells      = num_cells
        self.num_temps      = num_temps
        self.recv_timeout_s = recv_timeout_s
        self.txn_timeout_s  = txn_timeout_s
        self.dump           = dump
        self._bus: Optional["can.BusABC"] = None  # type: ignore[name-defined]
        self._lock = threading.Lock()
        self._stop = threading.Event()
        self._thread: Optional[threading.Thread] = None
        self._latest: Optional[AnalogTelemetry] = None
        self._latest_at: float = 0.0
        self._latest_alarms: Optional[AlarmEvents] = None
        self._latest_alarms_at: float = 0.0

    # ---- lifecycle ----------------------------------------------------

    def open(self) -> None:
        if can is None:
            raise RuntimeError(
                "python-can is not installed — run `pip install python-can`"
            )
        if self._bus is not None:
            return
        if self.interface == "pcan":
            pcan_uninit_channel(self.channel)
        if self.interface == "virtual":
            self._bus = can.interface.Bus(channel=self.channel,
                                          interface="virtual")
        else:
            self._bus = can.interface.Bus(channel=self.channel,
                                          interface=self.interface,
                                          bitrate=self.bitrate)

    def close(self) -> None:
        if self._thread is not None:
            self._stop.set()
            self._thread.join(timeout=2.0)
            self._thread = None
        if self._bus is not None:
            try:
                self._bus.shutdown()
            except Exception:
                pass
            self._bus = None

    def __enter__(self):
        self.open()
        return self

    def __exit__(self, *_):
        self.close()

    # ---- low-level I/O ------------------------------------------------

    def _send_request(self, data_id: int) -> None:
        if self._bus is None:
            raise RuntimeError("bus not open")
        arb = make_request_id(data_id, self.battery_addr, self.host_addr)
        msg = can.Message(arbitration_id=arb, data=bytes(8),
                          is_extended_id=True)
        if self.dump:
            print(f"  TX -> 0x{arb:08X}  {bytes(msg.data).hex()}")
        self._bus.send(msg)

    def _recv_response(self, data_id: int, *, want_frames: int = 1) -> list[bytes]:
        """Collect up to ``want_frames`` response payloads matching ``data_id``.

        Drops other frames silently (likely stragglers from a previous
        cycle). Returns whatever it has when the overall timeout expires.
        """
        if self._bus is None:
            raise RuntimeError("bus not open")
        want_id = make_response_id(data_id, self.battery_addr, self.host_addr)
        deadline = time.time() + self.txn_timeout_s
        out: list[bytes] = []
        while time.time() < deadline and len(out) < want_frames:
            remaining = max(0.0, deadline - time.time())
            msg = self._bus.recv(timeout=min(self.recv_timeout_s, remaining))
            if msg is None:
                continue
            if msg.arbitration_id == want_id and msg.is_extended_id:
                if self.dump:
                    print(f"  RX <- 0x{msg.arbitration_id:08X}  "
                          f"{bytes(msg.data).hex()}")
                out.append(bytes(msg.data))
        return out

    def _txn(self, data_id: int, *, want_frames: int = 1) -> list[bytes]:
        self._send_request(data_id)
        return self._recv_response(data_id, want_frames=want_frames)

    def _recv_burst(self, data_ids: tuple[int, ...]) -> dict[int, bytes]:
        """Send all requests at once, collect single-frame responses in one pass.

        Returns {data_id: payload} for each response that arrived before the
        shared txn_timeout. Missing entries mean the BMS did not respond in time.
        """
        if self._bus is None:
            raise RuntimeError("bus not open")
        want = {
            make_response_id(did, self.battery_addr, self.host_addr): did
            for did in data_ids
        }
        for did in data_ids:
            self._send_request(did)
        results: dict[int, bytes] = {}
        deadline = time.time() + self.txn_timeout_s
        while time.time() < deadline and len(results) < len(data_ids):
            remaining = max(0.0, deadline - time.time())
            msg = self._bus.recv(timeout=min(self.recv_timeout_s, remaining))
            if msg is None or not msg.is_extended_id:
                continue
            did = want.get(msg.arbitration_id)
            if did is not None and did not in results:
                if self.dump:
                    print(f"  RX(burst) <- 0x{msg.arbitration_id:08X}  "
                          f"{bytes(msg.data).hex()}")
                results[did] = bytes(msg.data)
        return results

    # ---- per-DataID readers -------------------------------------------

    def _read_pack_totals(self) -> tuple[float, float, int]:
        frames = self._txn(DID_PACK_TOTALS)
        if not frames:
            raise TimeoutError("no 0x90 response")
        return parse_pack_totals(frames[0])

    def _read_cell_extremes(self) -> tuple[int, int, int, int]:
        frames = self._txn(DID_CELL_EXTREMES)
        if not frames:
            raise TimeoutError("no 0x91 response")
        return parse_cell_extremes(frames[0])

    def _read_mos_state(self) -> tuple[int, int, bool, bool, str]:
        """0x93 -> (bms_life_counter, remaining_mah, charge_mos_on,
        discharge_mos_on, state).

        Byte layout (datasheet p6): 0=state, 1=charge MOS, 2=discharge MOS,
        3=life counter, 4-7=remaining capacity (mAh, uint32 BE).
        """
        frames = self._txn(DID_MOS_STATE)
        if not frames:
            raise TimeoutError("no 0x93 response")
        d = frames[0]
        state_raw = d[0]
        charge_mos    = bool(d[1])
        discharge_mos = bool(d[2])
        life = d[3]
        rmc_mah = int.from_bytes(d[4:8], "big")
        state = {0: "idle", 1: "charging", 2: "discharging"}.get(
            state_raw, f"unknown({state_raw})")
        return life, rmc_mah, charge_mos, discharge_mos, state

    def _read_cell_voltages(self) -> list[int]:
        """0x95: one request triggers a series of frames.

        Frame layout: byte0 = 1-based frame#, bytes1-2 / 3-4 / 5-6 = three
        uint16-BE cell mV. Frame N carries cells (3N-2 .. 3N). 13 cells -> 5
        frames, the last carries cell 13 only.
        """
        want = (self.num_cells + 2) // 3
        frames = self._txn(DID_CELL_VOLTAGES, want_frames=want)
        cells = [0] * self.num_cells
        seen = [False] * self.num_cells
        for d in frames:
            frame_no = d[0]
            if frame_no == 0 or frame_no == 0xFF:
                continue
            base = (frame_no - 1) * 3
            for k in range(3):
                idx = base + k
                if idx >= self.num_cells:
                    break
                cells[idx] = int.from_bytes(d[1 + k * 2:3 + k * 2], "big")
                seen[idx] = True
        if not all(seen):
            raise TimeoutError(
                f"0x95 incomplete: got {sum(seen)} of {self.num_cells} cells"
            )
        return cells

    def _read_cell_temps(self) -> list[int]:
        """0x96: byte0 = frame#, bytes 1..7 = seven uint8 temps (raw - 40)."""
        want = (self.num_temps + 6) // 7
        frames = self._txn(DID_CELL_TEMPS, want_frames=want)
        temps: list[Optional[int]] = [None] * self.num_temps
        for d in frames:
            frame_no = d[0]
            if frame_no == 0 or frame_no == 0xFF:
                continue
            base = (frame_no - 1) * 7
            for k in range(7):
                idx = base + k
                if idx >= self.num_temps:
                    break
                temps[idx] = d[1 + k] - TEMP_OFFSET
        if any(t is None for t in temps):
            raise TimeoutError(f"0x96 incomplete: {temps}")
        return [int(t) for t in temps]  # type: ignore[arg-type]

    def _read_faults(self) -> AlarmEvents:
        frames = self._txn(DID_FAULT_STATUS)
        if not frames:
            raise TimeoutError("no 0x98 response")
        return parse_fault_info(frames[0])

    # ---- composite snapshot -------------------------------------------

    def read_snapshot(self, burst: bool = True) -> tuple[AnalogTelemetry, AlarmEvents]:
        """Read a full snapshot.

        burst=True (default): sends the 4 single-frame requests together and
        collects responses in one shared timeout window (~49 ms on Falcon 15 EB
        vs ~69 ms sequential).

        burst=False: fully sequential, one request-response at a time.
        """
        if burst:
            got = self._recv_burst(_BURST_DIDS)
            missing = [f"0x{d:02X}" for d in _BURST_DIDS if d not in got]
            if missing:
                raise TimeoutError(f"burst: no response for {', '.join(missing)}")
            pack_v, pack_i, soc = parse_pack_totals(got[DID_PACK_TOTALS])
            vmax, hi_i, vmin, lo_i = parse_cell_extremes(got[DID_CELL_EXTREMES])
            d = got[DID_MOS_STATE]
            state_raw = d[0]
            chg_mos = bool(d[1])
            dis_mos = bool(d[2])
            life    = d[3]
            rmc_mah = int.from_bytes(d[4:8], "big")
            state = {0: "idle", 1: "charging", 2: "discharging"}.get(
                state_raw, f"unknown({state_raw})")
            alarms = parse_fault_info(got[DID_FAULT_STATUS])
        else:
            pack_v, pack_i, soc = self._read_pack_totals()
            vmax, hi_i, vmin, lo_i = self._read_cell_extremes()
            life, rmc_mah, chg_mos, dis_mos, state = self._read_mos_state()
            alarms = self._read_faults()
        cells = self._read_cell_voltages()
        temps = self._read_cell_temps()
        analog = AnalogTelemetry(
            data_flag=0,
            pack_num=self.battery_addr,
            total_v=pack_v,
            current_a=pack_i,
            soc=soc,
            rmc_ah=rmc_mah / 1000.0,
            cell_mv=cells,
            cell_high_idx=hi_i,
            cell_low_idx=lo_i,
            cell_vdiff_mv=max(0, vmax - vmin),
            cell_temps_c=temps[:3],
            other_temps_c=temps[3:],
            # soh / cycle_count / fcc_ah / design_cap_ah: not exposed by
            # the Morelle protocol — left as None (dataclass defaults).
            charge_mos=chg_mos,
            discharge_mos=dis_mos,
            bms_state=state,
            life_counter=life,
        )
        return analog, alarms

    # ---- background poller --------------------------------------------

    def start_polling(self, interval_s: float = 0.0) -> None:
        """Start the background poll thread.

        interval_s=0 (default): no sleep between snapshots — polls as fast as
        the BMS responds.
        """
        if self._thread is not None:
            return
        self._stop.clear()
        self._thread = threading.Thread(target=self._poll_loop,
                                        args=(interval_s,),
                                        name="morelle-poll", daemon=True)
        self._thread.start()

    def _poll_loop(self, interval_s: float) -> None:
        while not self._stop.is_set():
            try:
                analog, alarms = self.read_snapshot()
            except Exception as e:
                if self.dump:
                    print(f"  !! poll error: {e}")
            else:
                with self._lock:
                    now = time.time()
                    self._latest = analog
                    self._latest_at = now
                    self._latest_alarms = alarms
                    self._latest_alarms_at = now
            if interval_s > 0 and self._stop.wait(interval_s):
                break

    def latest(self, max_age_s: float = 2.0) -> Optional[AnalogTelemetry]:
        with self._lock:
            if self._latest is None:
                return None
            if time.time() - self._latest_at > max_age_s:
                return None
            return self._latest

    def latest_alarms(self, max_age_s: float = 5.0) -> Optional[AlarmEvents]:
        with self._lock:
            if self._latest_alarms is None:
                return None
            if time.time() - self._latest_alarms_at > max_age_s:
                return None
            return self._latest_alarms


_LOG_HEADERS = [
    "timestamp", "elapsed_s",
    "bms_voltage_v", "bms_current_a", "bms_soc",
    "bms_rmc_ah",
    "bms_vdiff_mv", "computed_vdiff_mv",
    "cell_voltages_mv", "cell_temps_c", "other_temps_c",
    "charge_mos", "discharge_mos", "bms_state", "life_counter",
    "bms_event_bytes", "bms_protections", "bms_alarms",
]


def _csv_row(t: AnalogTelemetry, alarms: AlarmEvents, elapsed_s: float) -> list:
    computed_vdiff = (max(t.cell_mv) - min(t.cell_mv)) if t.cell_mv else 0
    prots = set(alarms.protections())
    return [
        datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")[:-3],
        f"{elapsed_s:.2f}",
        f"{t.total_v:.3f}",
        f"{t.current_a:.3f}",
        t.soc,
        f"{t.rmc_ah:.3f}",
        t.cell_vdiff_mv,
        computed_vdiff,
        ";".join(str(v) for v in t.cell_mv),
        ";".join(str(v) for v in t.cell_temps_c),
        ";".join(str(v) for v in t.other_temps_c),
        ("" if t.charge_mos    is None else int(t.charge_mos)),
        ("" if t.discharge_mos is None else int(t.discharge_mos)),
        t.bms_state or "",
        ("" if t.life_counter  is None else t.life_counter),
        " ".join(f"{b:02X}" for b in alarms.as_bytes()),
        ";".join(alarms.protections()),
        ";".join(n for n in alarms.active() if n not in prots),
    ]


def _print(t: AnalogTelemetry) -> None:
    temps = t.all_temps_c
    def temp(i: int) -> str:
        return f"{temps[i]:>3d}C" if i < len(temps) else " --C"
    print(
        f"[{time.strftime('%H:%M:%S')}]  "
        f"V={t.total_v:6.2f}V  I={t.current_a:+7.3f}A  SOC={t.soc:3d}%  "
        f"Vdiff={t.cell_vdiff_mv:4d}mV  RMC={t.rmc_ah:6.2f}Ah  "
        f"T0={temp(0)}  T1={temp(1)}  T2={temp(2)}  T3={temp(3)}"
    )


def main() -> None:
    ap = argparse.ArgumentParser(
        description="Morelle Falcon 15 EB BMS CAN telemetry reader (bench tool)")
    ap.add_argument("--interface", default="pcan")
    ap.add_argument("--channel", required=True,
                    help="python-can channel (e.g. PCAN_USBBUS2)")
    ap.add_argument("--bitrate", type=int, default=DEFAULT_BITRATE)
    ap.add_argument("--battery-addr", type=lambda s: int(s, 0),
                    default=BATT_ADDR_DEFAULT,
                    help="Battery CAN address (0x01 or 0x02)")
    ap.add_argument("--host-addr", type=lambda s: int(s, 0),
                    default=HOST_ADDR_DEFAULT)
    ap.add_argument("--interval", type=float, default=0.0,
                    help="Sleep between snapshots in seconds (0 = as fast as BMS responds)")
    ap.add_argument("--once", action="store_true")
    ap.add_argument("--burst", action="store_true",
                    help="Enable burst mode (send all single-frame requests at once; "
                         "test against your BMS before relying on it)")
    ap.add_argument("--dump", action="store_true")
    ap.add_argument("--log", action="store_true",
                    help="Log telemetry to a timestamped CSV in logs/ next to this script")
    args = ap.parse_args()

    bms = MorelleBms(channel=args.channel, interface=args.interface,
                     bitrate=args.bitrate,
                     host_addr=args.host_addr,
                     battery_addr=args.battery_addr,
                     dump=args.dump)
    print(f"Opening {args.interface} {args.channel} @ {args.bitrate}  "
          f"(host=0x{args.host_addr:02X} batt=0x{args.battery_addr:02X})")
    with bms:
        if args.once:
            a, _ = bms.read_snapshot(burst=args.burst)
            _print(a)
            return

        csv_fh = csv_writer = None
        if args.log:
            log_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "logs")
            os.makedirs(log_dir, exist_ok=True)
            fname = (f"morelle_0x{args.battery_addr:02x}_"
                     + datetime.now().strftime("%Y%m%d_%H%M%S") + ".csv")
            csv_path = os.path.join(log_dir, fname)
            csv_fh = open(csv_path, "w", newline="", encoding="utf-8")
            csv_writer = csv.writer(csv_fh)
            csv_writer.writerow(_LOG_HEADERS)
            csv_fh.flush()
            print(f"Logging to {csv_path}")

        session_start = time.time()
        try:
            while True:
                try:
                    t0 = time.time()
                    a, alarms = bms.read_snapshot(burst=args.burst)
                    elapsed_ms = (time.time() - t0) * 1000
                    _print(a)
                    print(f"  snapshot in {elapsed_ms:.0f} ms  "
                          f"({'burst' if args.burst else 'sequential'})")
                    if csv_writer is not None:
                        csv_writer.writerow(
                            _csv_row(a, alarms, time.time() - session_start))
                        csv_fh.flush()
                except Exception as e:
                    print(f"  !! {type(e).__name__}: {e}")
                if args.interval > 0:
                    time.sleep(args.interval)
        except KeyboardInterrupt:
            print("\nStopped.")
        finally:
            if csv_fh is not None:
                csv_fh.close()


if __name__ == "__main__":
    main()
