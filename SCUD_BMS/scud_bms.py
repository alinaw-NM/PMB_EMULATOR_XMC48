# python scud_bms.py --port COM6 --log
"""SCUD BMS RS485 reader -- CID2=0x42 analog telemetry.

Implements the YDT 1363.3-style ASCII frame protocol used by SCUD BMS modules
(reference: "(对外简化版) SCUD BMS485 协议 A2 修订版 20251015.pdf" in
SCUDBMS_TOOLS_V1.0/).

Wire format (all chars between SOI and EOI are ASCII hex):

    ~ VER(2) ADR(2) CID1(2) CID2(2) LENGTH(4) INFO(LENID) CHKSUM(4) \r

Where LENGTH = (LCHKSUM<<12) | LENID, LENID = number of ASCII chars in INFO,
and CHKSUM is the 16-bit two's complement of the sum of all ASCII byte values
between SOI and CHKSUM (exclusive on both ends).

Serial defaults match the SCUDBMS_TOOLS GUI (default.ini): 115200 8N1, no parity.
The protocol spec itself quotes 9600 but the GUI overrides via .ini; we follow
the hardware-confirmed setting.

Smoke test:
    python scud_bms.py                       # default COM6 @ 115200
    python scud_bms.py --port COM7 --once    # one read, then exit
    python scud_bms.py --dump                # also print raw RX/TX frames
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

# Import-safe without pyserial so the framing layer can be tested standalone.
try:
    import serial  # type: ignore
except ImportError:  # pragma: no cover
    serial = None  # type: ignore[assignment]


SOI = b"~"
EOI = b"\r"
VER_DEFAULT = 0x22       # protocol v2.2 ("22H")
ADR_DEFAULT = 0x00       # pack address (user confirmed = 0)
CID1_BMS    = 0x4A       # BMS device type
CID2_ANALOG = 0x42       # get analog telemetry
CID2_ALARM  = 0x44       # get alarm/event flags (遥信量信息)
ANALOG_COMMAND_GROUP = 0x01  # "pack 1" -- matches manual example


def _lchksum(lenid: int) -> int:
    """4-bit LCHKSUM over the 12-bit LENID (sum nibbles, ~+1)."""
    s = ((lenid >> 8) & 0xF) + ((lenid >> 4) & 0xF) + (lenid & 0xF)
    return (-s) & 0xF


def _length_field(info_ascii_len: int) -> int:
    if info_ascii_len > 0xFFF:
        raise ValueError("INFO too long (LENID is 12-bit)")
    return (_lchksum(info_ascii_len) << 12) | (info_ascii_len & 0xFFF)


def _chksum(body_ascii: bytes) -> int:
    """16-bit two's complement of ASCII byte sum over VER+ADR+CID1+CID2+LENGTH+INFO."""
    return (-sum(body_ascii)) & 0xFFFF


def encode_frame(cid1: int, cid2: int, info_bytes: bytes,
                 ver: int = VER_DEFAULT, addr: int = ADR_DEFAULT) -> bytes:
    info_ascii = info_bytes.hex().upper().encode("ascii")
    length = _length_field(len(info_ascii))
    body = (
        f"{ver:02X}{addr:02X}{cid1:02X}{cid2:02X}{length:04X}"
    ).encode("ascii") + info_ascii
    return SOI + body + f"{_chksum(body):04X}".encode("ascii") + EOI


@dataclasses.dataclass
class Frame:
    ver: int
    addr: int
    cid1: int
    cid2: int      # in a response this is the RTN code (0x00 = OK)
    info: bytes    # decoded INFO bytes (not ASCII)


def parse_frame(raw: bytes) -> Frame:
    """Parse a complete on-wire frame (SOI...EOI inclusive)."""
    if not raw.startswith(SOI) or not raw.endswith(EOI):
        raise ValueError(f"bad framing: {raw!r}")
    body = raw[1:-1]
    if len(body) < 12 + 4:
        raise ValueError(f"frame too short: {raw!r}")

    chksum_ascii = body[-4:]
    body_no_chk  = body[:-4]
    expected = _chksum(body_no_chk)
    got = int(chksum_ascii, 16)
    if expected != got:
        raise ValueError(f"CHKSUM mismatch: expected {expected:04X}, got {got:04X}")

    ver    = int(body_no_chk[0:2],  16)
    addr   = int(body_no_chk[2:4],  16)
    cid1   = int(body_no_chk[4:6],  16)
    cid2   = int(body_no_chk[6:8],  16)
    length = int(body_no_chk[8:12], 16)
    lenid  = length & 0xFFF
    if _lchksum(lenid) != (length >> 12):
        raise ValueError(f"LCHKSUM mismatch on LENGTH {length:04X}")

    info_ascii = body_no_chk[12:12 + lenid]
    if len(info_ascii) != lenid:
        raise ValueError(f"INFO length {len(info_ascii)} != LENID {lenid}")
    info = bytes.fromhex(info_ascii.decode("ascii")) if lenid else b""
    return Frame(ver, addr, cid1, cid2, info)


@dataclasses.dataclass
class AnalogTelemetry:
    """Decoded CID2=0x42 response payload.

    Layout was derived from the manual's worked example (67-byte INFO with
    SOC byte 10 = 0x2F = 47%) and confirmed by sane V/RMC/FCC values.
    """
    data_flag:     int
    pack_num:      int
    total_v:       float          # V        -- from 4-byte LE mV
    current_a:     float          # A signed -- from 4-byte LE mA, + = chg
    soc:           int            # %
    soh:           int            # %
    cycle_count:   int
    rmc_ah:        float          # Ah       -- from 4-byte LE (10 mAh units)
    fcc_ah:        float          # Ah
    design_cap_ah: float          # Ah
    cell_mv:       list[int]      # per-cell, mV
    cell_high_idx: int
    cell_low_idx:  int
    cell_vdiff_mv: int            # mV
    cell_temps_c:  list[int]      # signed, 1 degC/LSB  ("N" group)
    other_temps_c: list[int]      # signed, 1 degC/LSB  ("k" group)

    @property
    def all_temps_c(self) -> list[int]:
        return self.cell_temps_c + self.other_temps_c

    @property
    def m(self) -> int:
        return len(self.cell_mv)

    @property
    def n(self) -> int:
        return len(self.cell_temps_c)


def parse_analog_info(info: bytes) -> AnalogTelemetry:
    if len(info) < 27:
        raise ValueError(f"analog INFO too short: {len(info)} bytes")

    data_flag    = info[0]
    pack_num     = info[1]
    total_v_mv   = int.from_bytes(info[2:6],   "little", signed=False)
    current_ma   = int.from_bytes(info[6:10],  "little", signed=True)
    soc          = info[10]
    soh          = info[11]
    cycle_count  = int.from_bytes(info[12:14], "little", signed=False)
    # Capacity is in mAh on this SCUD pack (manual quotes "/100 = 10mAh units",
    # but the SCUDBMS GUI shows /1000 = Ah, verified against live frames).
    rmc_mah      = int.from_bytes(info[14:18], "little", signed=False)
    fcc_mah      = int.from_bytes(info[18:22], "little", signed=False)
    design_mah   = int.from_bytes(info[22:26], "little", signed=False)

    m = info[26]
    cells_end = 27 + 2 * m
    if len(info) < cells_end + 4:
        raise ValueError(f"INFO truncated mid-cells: M={m}, len={len(info)}")
    cell_mv = [
        int.from_bytes(info[27 + 2 * i:27 + 2 * i + 2], "little", signed=False)
        for i in range(m)
    ]

    off = cells_end
    high_idx     = info[off]; off += 1
    low_idx      = info[off]; off += 1
    vdiff_mv     = int.from_bytes(info[off:off + 2], "little", signed=False)
    off += 2

    n = info[off]; off += 1
    if len(info) < off + 2 * n:
        raise ValueError(f"INFO truncated mid-temps: N={n}, off={off}")
    cell_temps = [
        int.from_bytes(info[off + 2 * i:off + 2 * i + 2], "little", signed=True)
        for i in range(n)
    ]
    off += 2 * n

    # Second temperature group ("other temps"): a 1-byte count k followed by
    # k 2-byte signed values. On the SCUD pack these are extra cell sensors
    # (the GUI's "Cell Temp 3 / 4"). Consumers covering every sensor should
    # read AnalogTelemetry.all_temps_c, not cell_temps_c alone.
    other_temps: list[int] = []
    if off < len(info):
        k = info[off]; off += 1
        if len(info) >= off + 2 * k:
            other_temps = [
                int.from_bytes(info[off + 2 * i:off + 2 * i + 2], "little",
                               signed=True)
                for i in range(k)
            ]

    return AnalogTelemetry(
        data_flag=data_flag,
        pack_num=pack_num,
        total_v=total_v_mv / 1000.0,
        current_a=current_ma / 1000.0,
        soc=soc,
        soh=soh,
        cycle_count=cycle_count,
        rmc_ah=rmc_mah / 1000.0,
        fcc_ah=fcc_mah / 1000.0,
        design_cap_ah=design_mah / 1000.0,
        cell_mv=cell_mv,
        cell_high_idx=high_idx,
        cell_low_idx=low_idx,
        cell_vdiff_mv=vdiff_mv,
        cell_temps_c=cell_temps,
        other_temps_c=other_temps,
    )


# === Alarm/event decoder (CID2=0x44) ===================================
# Bit definitions from SCUD BMS485 protocol A2 §A.5.2.2.
# Each entry: (bit, name, is_protection). "Protection" bits mean the BMS
# is actively protecting (disconnect/limit) and warrant an immediate
# safety trip. "Alarm" bits are warnings only.

_VOLTAGE_EVENTS = [
    (0, "cell over-voltage alarm",        False),
    (1, "cell over-voltage protection",   True),
    (2, "cell under-voltage alarm",       False),
    (3, "cell under-voltage protection",  True),
    (4, "pack over-voltage alarm",        False),
    (5, "pack over-voltage protection",   True),
    (6, "pack under-voltage alarm",       False),
    (7, "pack under-voltage protection",  True),
]
_CURRENT_EVENTS = [
    (0, "charge current alarm",           False),
    (1, "charge over-current 1",          True),
    (2, "charge over-current 2",          True),
    (3, "discharge current alarm",        False),
    (4, "discharge over-current 1",       True),
    (5, "discharge over-current 2",       True),
    (6, "output short circuit",           True),
    (7, "over-current lockout",           True),
]
_CELL_TEMP_EVENTS = [
    (0, "charge high-temp alarm",         False),
    (1, "charge over-temp protection",    True),
    (2, "charge low-temp alarm",          False),
    (3, "charge under-temp protection",   True),
    (4, "discharge high-temp alarm",      False),
    (5, "discharge over-temp protection", True),
    (6, "discharge low-temp alarm",       False),
    (7, "discharge under-temp protection",True),
]
_OTHER_TEMP_EVENTS = [
    (0, "control board high-temp alarm",  False),
    (1, "control board over-temp",        True),
    (2, "power board high-temp alarm",    False),
    (3, "power board over-temp",          True),
]
_CAPACITY_EVENTS = [
    (0, "low capacity alarm",             False),
    (1, "charge over-capacity alarm",     False),
    (2, "charge over-capacity protection",True),
    (3, "charge timeout protection",      True),
]
_OTHER1_EVENTS = [
    (0, "cell vdiff alarm",               False),
    (1, "cell vdiff protection",          True),
    (2, "cell critically low voltage",    True),
    (3, "NCT disconnect",                 True),
    (4, "AFE device exception",           True),
    (5, "AFE comm exception",             True),
    (6, "EEPROM comm exception",          True),
]
_OTHER2_EVENTS = [
    (0, "charge loop exception",          True),
    (1, "discharge loop exception",       True),
]

_EVENT_BYTE_LAYOUT = [
    ("voltage",    _VOLTAGE_EVENTS),
    ("current",    _CURRENT_EVENTS),
    ("cell_temp",  _CELL_TEMP_EVENTS),
    ("other_temp", _OTHER_TEMP_EVENTS),
    ("capacity",   _CAPACITY_EVENTS),
    ("other1",     _OTHER1_EVENTS),
    ("other2",     _OTHER2_EVENTS),
]


@dataclasses.dataclass
class AlarmEvents:
    """Decoded CID2=0x44 response — seven event bytes, each a bitfield."""
    voltage:    int = 0
    current:    int = 0
    cell_temp:  int = 0
    other_temp: int = 0
    capacity:   int = 0
    other1:     int = 0
    other2:     int = 0

    def _walk(self):
        for attr, bits in _EVENT_BYTE_LAYOUT:
            byte = getattr(self, attr)
            for bit, name, is_protection in bits:
                if byte & (1 << bit):
                    yield name, is_protection

    def active(self) -> list[str]:
        return [name for name, _ in self._walk()]

    def protections(self) -> list[str]:
        return [name for name, is_prot in self._walk() if is_prot]

    def as_bytes(self) -> list[int]:
        return [self.voltage, self.current, self.cell_temp, self.other_temp,
                self.capacity, self.other1, self.other2]


def parse_alarm_info(info: bytes) -> AlarmEvents:
    """Parse CID2=0x44 response INFO (§A.5.2.2). Tolerates trailing bytes."""
    if len(info) < 9:
        raise ValueError(f"alarm INFO too short: {len(info)} bytes")
    # info[0] = DATA_FLAG, info[1] = PACK group position
    return AlarmEvents(
        voltage=info[2],
        current=info[3],
        cell_temp=info[4],
        other_temp=info[5],
        capacity=info[6],
        other1=info[7],
        other2=info[8],
    )


def encode_alarm_info(events: AlarmEvents,
                      data_flag: int = 0x00, pack_num: int = 1) -> bytes:
    """Inverse of parse_alarm_info — used by the fake_bms test harness."""
    out = bytearray()
    out.append(data_flag & 0xFF)
    out.append(pack_num & 0xFF)
    out += bytes(events.as_bytes())
    out.append(0x00)  # 'other event 1' / reserved — protocol shows a 10th byte
    return bytes(out)


def encode_analog_info(
    *,
    data_flag: int = 0x00,
    pack_num: int = 1,
    total_v: float,
    current_a: float,
    soc: int,
    soh: int = 100,
    cycle_count: int = 0,
    rmc_ah: float,
    fcc_ah: float,
    design_cap_ah: float,
    cell_mv: list[int],
    cell_high_idx: int = 0,
    cell_low_idx: int = 0,
    cell_vdiff_mv: int,
    cell_temps_c: list[int],
    other_temps_c: list[int],
) -> bytes:
    """Inverse of parse_analog_info — used by the fake_bms test harness."""
    out = bytearray()
    out.append(data_flag & 0xFF)
    out.append(pack_num & 0xFF)
    out += int(round(total_v * 1000)).to_bytes(4, "little", signed=False)
    out += int(round(current_a * 1000)).to_bytes(4, "little", signed=True)
    out.append(soc & 0xFF)
    out.append(soh & 0xFF)
    out += (cycle_count & 0xFFFF).to_bytes(2, "little", signed=False)
    out += int(round(rmc_ah * 1000)).to_bytes(4, "little", signed=False)
    out += int(round(fcc_ah * 1000)).to_bytes(4, "little", signed=False)
    out += int(round(design_cap_ah * 1000)).to_bytes(4, "little", signed=False)
    out.append(len(cell_mv) & 0xFF)
    for mv in cell_mv:
        out += int(mv).to_bytes(2, "little", signed=False)
    out.append(cell_high_idx & 0xFF)
    out.append(cell_low_idx & 0xFF)
    out += (int(cell_vdiff_mv) & 0xFFFF).to_bytes(2, "little", signed=False)
    out.append(len(cell_temps_c) & 0xFF)
    for t in cell_temps_c:
        out += int(t).to_bytes(2, "little", signed=True)
    out.append(len(other_temps_c) & 0xFF)
    for t in other_temps_c:
        out += int(t).to_bytes(2, "little", signed=True)
    return bytes(out)


class ScudBms:
    """Synchronous request/response client + optional background poller."""

    def __init__(self, port: str = "COM6", baud: int = 115200,
                 addr: int = ADR_DEFAULT, timeout_s: float = 1.0,
                 dump: bool = False):
        self.port = port
        self.baud = baud
        self.addr = addr
        self.timeout_s = timeout_s
        self.dump = dump
        self._ser: Optional[serial.Serial] = None
        self._latest: Optional[AnalogTelemetry] = None
        self._latest_at: float = 0.0
        self._latest_alarms: Optional[AlarmEvents] = None
        self._latest_alarms_at: float = 0.0
        self._lock = threading.Lock()
        self._stop = threading.Event()
        self._thread: Optional[threading.Thread] = None

    def open(self) -> None:
        if serial is None:
            raise RuntimeError(
                "pyserial is not installed -- run `pip install pyserial`"
            )
        if self._ser is None:
            self._ser = serial.Serial(
                self.port, baudrate=self.baud,
                bytesize=8, parity=serial.PARITY_NONE, stopbits=1,
                timeout=self.timeout_s,
            )

    def close(self) -> None:
        if self._thread is not None:
            self._stop.set()
            self._thread.join(timeout=2.0)
            self._thread = None
        if self._ser is not None:
            self._ser.close()
            self._ser = None

    def __enter__(self):
        self.open()
        return self

    def __exit__(self, *_):
        self.close()

    def read_analog(self,
                    command_group: int = ANALOG_COMMAND_GROUP) -> AnalogTelemetry:
        """Send CID2=0x42 and parse the response."""
        frame = self._txn(CID2_ANALOG, command_group)
        return parse_analog_info(frame.info)

    def read_alarms(self,
                    command_group: int = ANALOG_COMMAND_GROUP) -> AlarmEvents:
        """Send CID2=0x44 (遥信量) and parse the response."""
        frame = self._txn(CID2_ALARM, command_group)
        return parse_alarm_info(frame.info)

    def _txn(self, cid2: int, command_group: int) -> Frame:
        if self._ser is None:
            raise RuntimeError("port not open")
        tx = encode_frame(CID1_BMS, cid2, bytes([command_group]), addr=self.addr)
        if self.dump:
            print(f"  TX -> {tx!r}")
        self._ser.reset_input_buffer()
        self._ser.write(tx)
        rx = self._read_frame()
        if self.dump:
            print(f"  RX <- {rx!r}")
        frame = parse_frame(rx)
        if frame.cid2 != 0x00:
            raise RuntimeError(f"BMS returned RTN 0x{frame.cid2:02X} (not OK)")
        return frame

    def _read_frame(self) -> bytes:
        """Read bytes until EOI ('\\r') or timeout."""
        assert self._ser is not None
        deadline = time.time() + self.timeout_s
        buf = bytearray()
        while time.time() < deadline:
            b = self._ser.read(1)
            if not b:
                continue
            if b == SOI:
                buf.extend(b)
                break
        else:
            raise TimeoutError("no SOI before timeout")
        while time.time() < deadline:
            b = self._ser.read(1)
            if not b:
                continue
            buf.extend(b)
            if b == EOI:
                return bytes(buf)
        raise TimeoutError("no EOI before timeout")

    def start_polling(self, interval_s: float = 0.5) -> None:
        """Background thread polls read_analog(); read result via latest()."""
        if self._thread is not None:
            return
        self._stop.clear()
        self._thread = threading.Thread(
            target=self._poll_loop, args=(interval_s,), daemon=True,
        )
        self._thread.start()

    def _poll_loop(self, interval_s: float) -> None:
        while not self._stop.is_set():
            try:
                tel = self.read_analog()
            except Exception as e:
                if self.dump:
                    print(f"  !! analog poll error: {e}")
            else:
                with self._lock:
                    self._latest = tel
                    self._latest_at = time.time()
            # TODO(perf): rate-limit alarm reads (e.g. only every Nth cycle).
            # Polling alarms every cycle adds ~100–200 ms of lag on top of analog,
            # which forces SAFETY_DISCREPANCY_STRIKES to carry more headroom than
            # it otherwise would. Alarms change slowly — every ~2 s is plenty.
            try:
                alarms = self.read_alarms()
            except Exception as e:
                if self.dump:
                    print(f"  !! alarm poll error: {e}")
            else:
                with self._lock:
                    self._latest_alarms = alarms
                    self._latest_alarms_at = time.time()
            self._stop.wait(interval_s)

    def latest(self, max_age_s: float = 2.0) -> Optional[AnalogTelemetry]:
        """Most recent telemetry, or None if stale/missing."""
        with self._lock:
            if self._latest is None:
                return None
            if time.time() - self._latest_at > max_age_s:
                return None
            return self._latest

    def latest_alarms(self, max_age_s: float = 5.0) -> Optional[AlarmEvents]:
        """Most recent alarm/event bytes, or None if stale/missing."""
        with self._lock:
            if self._latest_alarms is None:
                return None
            if time.time() - self._latest_alarms_at > max_age_s:
                return None
            return self._latest_alarms


def _print(t: AnalogTelemetry) -> None:
    temps = t.all_temps_c
    def temp(i: int) -> str:
        return f"{temps[i]:>3d}C" if i < len(temps) else " --C"
    print(
        f"[{time.strftime('%H:%M:%S')}]  "
        f"V={t.total_v:6.2f}V  I={t.current_a:+7.3f}A  "
        f"SOC={t.soc:3d}%  "
        f"Vdiff={t.cell_vdiff_mv:4d}mV  "
        f"RMC={t.rmc_ah:6.2f}Ah  FCC={t.fcc_ah:6.2f}Ah  "
        f"T0={temp(0)}  T1={temp(1)}  T2={temp(2)}  T3={temp(3)}"
    )


_CSV_HEADERS = [
    "timestamp", "elapsed_s",
    "bms_voltage_v", "bms_current_a", "bms_soc", "bms_soh",
    "bms_rmc_ah", "bms_fcc_ah", "bms_cycle_count", "bms_life_counter",
    "bms_charge_mos", "bms_discharge_mos", "bms_state",
    "bms_vdiff_mv", "computed_vdiff_mv",
    "cell_voltages_mv", "all_temps_c", "start_temps_c",
    "charger_voltage_v", "charger_current_a",
    "charger_status_raw", "charger_cycle_type", "charger_indication",
    "charger_on", "charger_ac_detected", "charger_alarm", "charger_fault",
    "charger_fault_kind", "charger_fault_code", "charger_fault_raw",
    "bms_event_bytes", "bms_protections", "bms_alarms",
    "safety_fault",
]


def _csv_row(t: AnalogTelemetry, elapsed: float, start_temps: list[int]) -> list:
    computed_vdiff = max(t.cell_mv) - min(t.cell_mv) if t.cell_mv else 0
    return [
        datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")[:-3],
        f"{elapsed:.2f}",
        f"{t.total_v:.3f}", f"{t.current_a:.3f}",
        t.soc, t.soh,
        f"{t.rmc_ah:.3f}", f"{t.fcc_ah:.3f}",
        t.cycle_count, "",     # life_counter: not in SCUD protocol
        "", "", "",            # charge_mos, discharge_mos, bms_state
        t.cell_vdiff_mv, computed_vdiff,
        ";".join(str(v) for v in t.cell_mv),
        ";".join(str(v) for v in t.all_temps_c),
        ";".join(str(v) for v in start_temps),
        "", "",                # charger_voltage_v, charger_current_a
        "", "", "", "", "", "", "", "", "", "",  # charger fields
        "", "", "",            # bms_event_bytes, bms_protections, bms_alarms
        "",                    # safety_fault
    ]


def main() -> None:
    ap = argparse.ArgumentParser(description="SCUD BMS analog telemetry reader")
    ap.add_argument("--port", default="COM6")
    ap.add_argument("--baud", type=int, default=115200)
    ap.add_argument("--addr", type=lambda s: int(s, 0), default=ADR_DEFAULT,
                    help="BMS address byte (default 0)")
    ap.add_argument("--interval", type=float, default=1.0,
                    help="Seconds between polls (default 1.0)")
    ap.add_argument("--once", action="store_true",
                    help="Single read then exit")
    ap.add_argument("--dump", action="store_true",
                    help="Print raw TX/RX frames")
    ap.add_argument("--log", action="store_true",
                    help="Log telemetry to a timestamped CSV in logs/ next to this script")
    args = ap.parse_args()

    csv_writer = None
    csv_fh = None
    if args.log and not args.once:
        log_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "logs")
        os.makedirs(log_dir, exist_ok=True)
        fname = "scud_" + datetime.now().strftime("%Y%m%d_%H%M%S") + ".csv"
        csv_path = os.path.join(log_dir, fname)
        csv_fh = open(csv_path, "w", newline="", encoding="utf-8")
        csv_writer = csv.writer(csv_fh)
        csv_writer.writerow(_CSV_HEADERS)
        csv_fh.flush()
        print(f"Logging to {csv_path}")

    started_at = time.time()
    start_temps: list[int] = []

    print(f"Opening {args.port} @ {args.baud} 8N1  (BMS addr=0x{args.addr:02X})")
    try:
        with ScudBms(port=args.port, baud=args.baud, addr=args.addr,
                     dump=args.dump) as bms:
            if args.once:
                _print(bms.read_analog())
                return
            try:
                while True:
                    try:
                        tel = bms.read_analog()
                        _print(tel)
                        if csv_writer is not None:
                            if not start_temps:
                                start_temps = list(tel.all_temps_c)
                            csv_writer.writerow(
                                _csv_row(tel, time.time() - started_at, start_temps))
                            csv_fh.flush()
                    except Exception as e:
                        print(f"  !! {type(e).__name__}: {e}")
                    time.sleep(args.interval)
            except KeyboardInterrupt:
                print("\nStopped.")
    finally:
        if csv_fh is not None:
            csv_fh.close()


if __name__ == "__main__":
    main()
