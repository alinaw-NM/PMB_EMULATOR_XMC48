"""
can_scrape_v2.py  (xlsx_crawl.py)
==================================
Parses PMB CAN Protocol v2 - Data ID.csv and generates:
  1. #define REG_* constants for all active Data IDs
  2. build_response() switch-case bodies
  3. process_write() switch-case bodies

Usage:
    python3 xlsx_crawl.py > generated_v2.c

Output sections printed to stdout:
    === #define REG_* CONSTANTS ===
    === READ 12-BIT TELEMETRY ===         (0x00-0x1A)
    === READ STATUS BIT-MAPPED ===        (0x1B-0x1E)
    === READ POWER ENABLE ===             (0x1F-0x23)
    === READ 16-BIT OFFSETS ===           (0x30-0x4A)
    === READ 16-BIT SCALES ===            (0x50-0x6A)
    === READ 16-BIT EU ===                (0x70-0x8A)
    === READ DERIVED EU ===               (0x90-0x91)
    === READ ERROR CODE ===               (0xA0)
    === READ FAULT REGISTERS ===          (0xA8-0xAA)
    === READ 16-BIT LIMITS ===            (0xB0-0xCF)
    === READ LIMIT FAULT FLAGS ===        (0xD0-0xD1, 0xD8-0xD9)
    === READ BUILD INFO + WRITE PROT ===  (0x1F8, 0x1FA)
    === WRITE POWER ENABLE ===
    === WRITE 16-BIT OFFSETS ===
    === WRITE 16-BIT SCALES ===
    === WRITE 16-BIT LIMITS ===
    === WRITE FAULT REGISTERS ===
    === WRITE LIMIT FAULT FLAGS ===
    === WRITE BUILD INFO + WRITE PROT ===
"""

import csv
import os

# ---------------------------------------------------------------------------
# Load CSV (no external dependencies — uses stdlib csv)
# ---------------------------------------------------------------------------
CSV_PATH = os.path.join(os.path.dirname(__file__), '..', '..',
                        'PMB CAN Protocol v2 - Data ID.csv')

# Column indices (0-based):
#   0  = Section label (only on first row of each section, else empty)
#   1  = Data ID hex string  e.g. "0x00"
#   2  = Register name
#   5  = Read/Write
#   7  = Byte 8  (MSB, always packet counter / RSVD)
#  ...
#  14  = Byte 1  (LSB, maps to data[0] in firmware)
#  16  = Bit width
#  17  = Sign

RESERVED_IDS = {
    '0xAB', '0xAD', '0xAE', '0xAF',
    '0xE0', '0xF0', '0x100', '0x180',
    '0x1A0', '0x1E0', '0x1E8', '0x200', '0x300',
}


def parse_rows(csv_path):
    rows = []
    with open(csv_path, newline='', encoding='utf-8') as f:
        reader = csv.reader(f)
        for i, row in enumerate(reader):
            if i == 0:          # header line
                continue
            if len(row) < 18:   # pad short rows
                row += [''] * (18 - len(row))
            data_id = row[1].strip()
            if not data_id.startswith('0x'):
                continue
            rows.append({
                'id':    data_id,
                'name':  row[2].strip(),
                'rw':    row[5].strip(),
                'b1':    row[14],   # data[0]
                'b2':    row[13],   # data[1]
                'b3':    row[12],   # data[2]
                'b4':    row[11],   # data[3]
                'b5':    row[10],   # data[4]
                'b6':    row[9],    # data[5]
                'width': row[16].strip(),
                'sign':  row[17].strip(),
            })
    return rows


all_rows = parse_rows(CSV_PATH)


# ---------------------------------------------------------------------------
# #define REG_* generation (data-driven from CSV)
# ---------------------------------------------------------------------------

def _define_name(reg_name):
    """Convert register name to REG_<UPPER_SNAKE> identifier."""
    import re
    s = reg_name.upper()
    s = re.sub(r'[^A-Z0-9]+', '_', s)
    s = s.strip('_')
    return f"REG_{s}"


def gen_defines(rows):
    """Emit #define REG_* 0xNN for all active (non-reserved) Data IDs."""
    lines = ["/* === #define REG_* CONSTANTS === */"]
    for r in rows:
        if r['id'] in RESERVED_IDS:
            continue
        if 'Reserved' in r['name'] or r['name'] in ('nan', ''):
            continue
        macro = _define_name(r['name'])
        lines.append(f"#define {macro:<50s} ({r['id']}U)")
    return "\n".join(lines)


# ---------------------------------------------------------------------------
# Helper generators for switch-case bodies
# ---------------------------------------------------------------------------

def gen_read_12(reg_list, struct_prefix):
    """reg_list: [(hex_id_int, name_str, [field0, field1, field2_or_None])]"""
    lines = []
    for hex_id, name, fields in reg_list:
        lines.append(f"        case 0x{hex_id:02X}U: /* {name} */")
        for i, f in enumerate(fields):
            if f:
                lines.append(
                    f"            pack12(&data[{i*2}], &data[{i*2+1}], "
                    f"{struct_prefix}{f});"
                )
        lines.append("            break;")
    return "\n".join(lines)


def gen_read_16(reg_list, struct_prefix, comment=""):
    """reg_list: [(hex_id_int, [field0, field1, field2_or_None])]"""
    lines = []
    for hex_id, fields in reg_list:
        tag = f"0x{hex_id:02X}" if hex_id <= 0xFF else f"0x{hex_id:03X}"
        lines.append(f"        case {tag}U: /* {comment}{tag} */")
        for i, f in enumerate(fields):
            if f:
                lines.append(
                    f"            pack16(&data[{i*2}], &data[{i*2+1}], "
                    f"(uint16_t){struct_prefix}{f});"
                )
        lines.append("            break;")
    return "\n".join(lines)


def gen_write_16(reg_list, struct_prefix, cast="(int16_t)"):
    lines = []
    for hex_id, fields in reg_list:
        tag = f"0x{hex_id:02X}" if hex_id <= 0xFF else f"0x{hex_id:03X}"
        lines.append(f"        case {tag}U:")
        for i, f in enumerate(fields):
            if f:
                lines.append(
                    f"            {struct_prefix}{f} = "
                    f"{cast}unpack16(data[{i*2}], data[{i*2+1}]);"
                )
        lines.append("            break;")
    return "\n".join(lines)


# ---------------------------------------------------------------------------
# Register definitions
# Field order in each tuple: [Byte1-field, Byte3-field, Byte5-field]
#   = [data[0]/data[1], data[2]/data[3], data[4]/data[5]]
# Field names use sanitized C identifiers:
#   24V_*  → v24_*    12V_*  → v12_*    5V_*   → v5_*
#   3.3V_* → v3v3_*   1.2V_* → 1v2_*   2.5V_* → 2v5_*   3.3V → v3v3
# ---------------------------------------------------------------------------

# --- 12-bit telemetry (0x00-0x1A) ---
registers_12bit = [
    (0x00, "Telemetry Group 1",  ["chg_curr",          "batt1_curr",         "batt2_curr"]),
    (0x01, "Telemetry Group 2",  ["ll_imon",            "rl_imon",            "la_imon"]),
    (0x02, "Telemetry Group 3",  ["ra_imon",            "tr_imon",            "chg_in"]),
    (0x03, "Telemetry Group 4",  ["hv",                 "batt1_in",           "batt2_in"]),
    (0x04, "Telemetry Group 5",  ["nk_imon",            "mot_bus",            "chg_hv"]),
    (0x05, "Telemetry Group 6",  ["user_hv",            "user_hv_curr",       None]),
    (0x06, "Telemetry Group 7",  ["pc_24v",             "pc_24v_imon",        None]),
    (0x07, "Telemetry Group 8",  ["user_24v",           "user_24v_imon",      None]),
    (0x08, "Telemetry Group 9",  ["pc_a_12v",           "pc_a_12v_imon",      None]),
    (0x09, "Telemetry Group 10", ["pc_b_12v",           "pc_b_12v_imon",      None]),
    (0x0A, "Telemetry Group 11", ["v24_imon0",          "v24_imon1",          "v24_imon2"]),
    (0x0B, "Telemetry Group 12", ["v12_imon0",          "v12_imon1",          "v12_imon2"]),
    (0x0C, "Telemetry Group 13", ["v24_bus_voltage",    "v12_bus_a_voltage",  None]),
    (0x0D, "Telemetry Group 14", ["v5",                 "v5_curr",            None]),
    (0x0E, "Telemetry Group 15", ["v3v3",               "v3v3_curr",          None]),
    (0x0F, "Telemetry Group 16", ["aux_24v",            "aux_24v_curr",       None]),
    (0x10, "Telemetry Group 17", ["aux_12v",            "aux_12v_curr",       None]),
    (0x11, "Telemetry Group 18", ["estop_rec_a_12v",    "estop_rec_b_12v",    None]),
    (0x12, "Telemetry Group 19", ["v12_b_imon3",        "v12_b_imon4",        None]),
    (0x13, "Telemetry Group 20", ["hv_or",              "mb_sp_imon",         None]),
    (0x14, "Telemetry Group 21", ["temp0",              "temp1",              "temp2"]),
    (0x15, "Telemetry Group 22", ["temp3",              "temp4",              "temp5"]),
    (0x16, "Telemetry Group 23", ["ex_temp0",           "ex_temp1",           None]),
    (0x17, "Telemetry Group 24", ["lidar_a_12v",        "lidar_b_12v",        None]),
    (0x18, "Telemetry Group 25", ["sto_24v",            "chg_hvil_out",       None]),
    (0x19, "Telemetry Group 26", ["eth_2v5",            "eth_1v2",            None]),
    (0x1A, "Telemetry Group 27", ["ecat_3v3",           "ecat_1v2",           None]),
]

# --- Status bit-mapped read-only (0x1B-0x1E) ---
registers_status = [
    (0x1B, "COMP_PDG",
        [("st->comp_pdg[0]", 0), ("st->comp_pdg[1]", 1), ("st->comp_pdg[2]", 2)]),
    (0x1C, "PDG_FAULT_GRP1",
        [("st->pdg_fault_grp1[0]", 0), ("st->pdg_fault_grp1[1]", 1), ("st->pdg_fault_grp1[2]", 2)]),
    (0x1D, "PDG_FAULT_GRP2",
        [("st->pdg_fault_grp2[0]", 0), ("st->pdg_fault_grp2[1]", 1)]),
    (0x1E, "PDG_FAULT_GRP3",
        [("st->pdg_fault_grp3[0]", 0), ("st->pdg_fault_grp3[1]", 1)]),
]

# --- Power Enable bit-mapped R/W (0x1F-0x23) ---
registers_pwr_en = [
    (0x1F, "PWR_EN_GRP1",
        [("pe->grp1", 0)]),
    (0x20, "PWR_EN_GRP2",
        [("pe->grp2[0]", 0), ("pe->grp2[1]", 1), ("pe->grp2[2]", 2)]),
    (0x21, "PWR_EN_GRP3",
        [("pe->grp3[0]", 0), ("pe->grp3[1]", 1), ("pe->grp3[2]", 2), ("pe->grp3[3]", 3)]),
    (0x22, "PERIPH_INPUTS",      # Read Only
        [("pe->periph_inputs", 0)]),
    (0x23, "PWR_EN_GRP4",
        [("pe->grp4[0]", 0), ("pe->grp4[1]", 1), ("pe->grp4[2]", 2)]),
]

# --- Calibration offsets (0x30-0x4A): 16-bit signed LE ---
registers_16bit_ofs = [
    (0x30, ["chg_curr_ofs",        "batt1_curr_ofs",       "batt2_curr_ofs"]),
    (0x31, ["ll_imon_ofs",         "rl_imon_ofs",          "la_imon_ofs"]),
    (0x32, ["ra_imon_ofs",         "tr_imon_ofs",          "chg_in_ofs"]),
    (0x33, ["hv_ofs",              "batt1_in_ofs",         "batt2_in_ofs"]),
    (0x34, ["nk_imon_ofs",         "mot_bus_ofs",          "chg_hv_ofs"]),
    (0x35, ["user_hv_ofs",         "user_hv_curr_ofs",     None]),
    (0x36, ["pc_24v_ofs",          "pc_24v_imon_ofs",      None]),
    (0x37, ["user_24v_ofs",        "user_24v_imon_ofs",    None]),
    (0x38, ["pc_a_12v_ofs",        "pc_a_12v_imon_ofs",    None]),
    (0x39, ["pc_b_12v_ofs",        "pc_b_12v_imon_ofs",    None]),
    (0x3A, ["v24_imon0_ofs",       "v24_imon1_ofs",        "v24_imon2_ofs"]),
    (0x3B, ["v12_imon0_ofs",       "v12_imon1_ofs",        "v12_imon2_ofs"]),
    (0x3C, ["v24_ofs",             "v12_ofs",              None]),
    (0x3D, ["v5_ofs",              "v5_curr_ofs",          None]),
    (0x3E, ["v3v3_ofs",            "v3v3_curr_ofs",        None]),
    (0x3F, ["aux_24v_ofs",         "aux_24v_curr_ofs",     None]),
    (0x40, ["aux_12v_ofs",         "aux_12v_curr_ofs",     None]),
    (0x41, ["estop_rec_a_12v_ofs", "estop_rec_b_12v_ofs",  None]),
    (0x42, ["v12_b_imon3_ofs",     "v12_b_imon4_ofs",      None]),
    (0x43, ["hv_or_ofs",           "mb_sp_imon_ofs",       None]),
    (0x44, ["temp0_ofs",           "temp1_ofs",            "temp2_ofs"]),
    (0x45, ["temp3_ofs",           "temp4_ofs",            "temp5_ofs"]),
    (0x46, ["ex_temp0_ofs",        "ex_temp1_ofs",         None]),
    (0x47, ["lidar_a_12v_ofs",     "lidar_b_12v_ofs",      None]),
    (0x48, ["sto_24v_ofs",         "chg_hvil_out_ofs",     None]),
    (0x49, ["eth_2v5_ofs",         "eth_1v2_ofs",          None]),
    (0x4A, ["ecat_3v3_ofs",        "ecat_1v2_ofs",         None]),
]

# --- Calibration scales (0x50-0x6A): same layout, _scl suffix ---
registers_16bit_scl = [
    (h + 0x20, [f.replace("_ofs", "_scl") if f else None for f in fields])
    for h, fields in registers_16bit_ofs
]

# --- Engineering units (0x70-0x8A): 16-bit signed LE ---
registers_16bit_eu = [
    (0x70, ["chg_curr_eu",         "batt1_curr_eu",        "batt2_curr_eu"]),
    (0x71, ["ll_imon_eu",          "rl_imon_eu",           "la_imon_eu"]),
    (0x72, ["ra_imon_eu",          "tr_imon_eu",           "chg_in_eu"]),
    (0x73, ["hv_eu",               "batt1_in_eu",          "batt2_in_eu"]),
    (0x74, ["nk_imon_eu",          "mot_bus_eu",           "chg_hv_eu"]),
    (0x75, ["user_hv_eu",          "user_hv_curr_eu",      None]),
    (0x76, ["pc_24v_eu",           "pc_24v_imon_eu",       None]),
    (0x77, ["user_24v_eu",         "user_24v_imon_eu",     None]),
    (0x78, ["pc_a_12v_eu",         "pc_a_12v_imon_eu",     None]),
    (0x79, ["pc_b_12v_eu",         "pc_b_12v_imon_eu",     None]),
    (0x7A, ["v24_imon0_eu",        "v24_imon1_eu",         "v24_imon2_eu"]),
    (0x7B, ["v12_imon0_eu",        "v12_imon1_eu",         "v12_imon2_eu"]),
    (0x7C, ["v24_eu",              "v12_eu",               None]),
    (0x7D, ["v5_eu",               "v5_curr_eu",           None]),
    (0x7E, ["v3v3_eu",             "v3v3_curr_eu",         None]),
    (0x7F, ["aux_24v_eu",          "aux_24v_curr_eu",      None]),
    (0x80, ["aux_12v_eu",          "aux_12v_curr_eu",      None]),
    (0x81, ["estop_rec_a_12v_eu",  "estop_rec_b_12v_eu",   None]),
    (0x82, ["v12_b_imon3_eu",      "v12_b_imon4_eu",       None]),
    (0x83, ["hv_or_eu",            "mb_sp_imon_eu",        None]),
    (0x84, ["temp0_eu",            "temp1_eu",             "temp2_eu"]),
    (0x85, ["temp3_eu",            "temp4_eu",             "temp5_eu"]),
    (0x86, ["ex_temp0_eu",         "ex_temp1_eu",          None]),
    (0x87, ["lidar_a_12v_eu",      "lidar_b_12v_eu",       None]),
    (0x88, ["sto_24v_eu",          "chg_hvil_out_eu",      None]),
    (0x89, ["eth_2v5_eu",          "eth_1v2_eu",           None]),
    (0x8A, ["ecat_3v3_eu",         "ecat_1v2_eu",          None]),
]

# --- Derived EU (0x90-0x91): 16-bit signed LE ---
registers_derived_eu = [
    (0x90, ["v12_a_total_curr_eu", "v12_b_total_curr_eu",  "v24_total_curr_eu"]),
    (0x91, ["batt_total_curr_eu",  None,                   None]),
]

# --- Soft/negative limits (0xB0-0xCF): 16-bit signed LE ---
registers_16bit_lim = [
    (0xB0, ["chg_curr_lim",        "batt1_curr_plim",      "batt2_curr_plim"]),
    (0xB1, ["ll_imon_lim",         "rl_imon_lim",          "la_imon_lim"]),
    (0xB2, ["ra_imon_lim",         "tr_imon_lim",          "chg_in_lim"]),
    (0xB3, ["hv_lim",              "batt1_in_lim",         "batt2_in_lim"]),
    (0xB4, ["nk_imon_lim",         "mot_bus_lim",          "chg_hv_lim"]),
    (0xB5, ["user_hv_lim",         "user_hv_curr_lim",     None]),
    (0xB6, ["pc_24v_lim",          "pc_24v_imon_lim",      None]),
    (0xB7, ["user_24v_lim",        "user_24v_imon_lim",    None]),
    (0xB8, ["pc_a_12v_lim",        "pc_a_12v_imon_lim",    None]),
    (0xB9, ["pc_b_12v_lim",        "pc_b_12v_imon_lim",    None]),
    (0xBA, ["v24_imon0_lim",       "v24_imon1_lim",        "v24_imon2_lim"]),
    (0xBB, ["v12_imon0_lim",       "v12_imon1_lim",        "v12_imon2_lim"]),
    (0xBC, ["v24_lim",             "v12_lim",              None]),
    (0xBD, ["v5_lim",              "v5_curr_lim",          None]),
    (0xBE, ["v3v3_lim",            "v3v3_curr_lim",        None]),
    (0xBF, ["aux_24v_lim",         "aux_24v_curr_lim",     None]),
    (0xC0, ["aux_12v_lim",         "aux_12v_curr_lim",     None]),
    (0xC1, ["estop_rec_a_12v_lim", "estop_rec_b_12v_lim",  None]),
    (0xC2, ["v12_b_imon3_lim",     "v12_b_imon4_lim",      None]),
    (0xC3, ["hv_or_lim",           "mb_sp_imon_lim",       None]),
    (0xC4, ["temp0_plim",          "temp1_plim",           "temp2_plim"]),
    (0xC5, ["temp3_plim",          "temp4_plim",           "temp5_plim"]),
    (0xC6, ["ex_temp0_plim",       "ex_temp1_plim",        None]),
    (0xC7, ["lidar_a_12v_lim",     "lidar_b_12v_lim",      None]),
    (0xC8, ["sto_24v_lim",         "chg_hvil_out_lim",     None]),
    (0xC9, ["eth_2v5_lim",         "eth_1v2_lim",          None]),
    (0xCA, ["ecat_3v3_lim",        "ecat_1v2_lim",         None]),
    (0xCB, ["v12_a_total_curr_lim","v12_b_total_curr_lim", "v24_total_curr_lim"]),
    (0xCC, [None,                  "batt1_curr_nlim",      "batt2_curr_nlim"]),
    (0xCD, ["temp0_nlim",          "temp1_nlim",           "temp2_nlim"]),
    (0xCE, ["temp3_nlim",          "temp4_nlim",           "temp5_nlim"]),
    (0xCF, ["ex_temp0_nlim",       "ex_temp1_nlim",        None]),
]

# ---------------------------------------------------------------------------
# Print all output sections
# ---------------------------------------------------------------------------

print(f"/* Rows parsed from CSV: {len(all_rows)} */\n")

# --- #define REG_* ---
print(gen_defines(all_rows))
print()

print("/* ============================================================")
print(" * READ: build_response() switch-case bodies")
print(" * ============================================================ */\n")

print("/* === READ 12-BIT TELEMETRY (0x00-0x1A) === */")
print(gen_read_12(registers_12bit, "t->"))
print()

print("/* === READ STATUS BIT-MAPPED (0x1B-0x1E) === */")
for hex_id, name, fields in registers_status:
    print(f"        case 0x{hex_id:02X}U: /* {name} */")
    print("            { PMB_Status_t *st = &g_regs.status;")
    for expr, idx in fields:
        print(f"              data[{idx}] = {expr};")
    print("            } break;")
print()

print("/* === READ POWER ENABLE (0x1F-0x23) === */")
for hex_id, name, fields in registers_pwr_en:
    print(f"        case 0x{hex_id:02X}U: /* {name} */")
    print("            { PMB_PowerEnable_t *pe = &g_regs.pwr_enable;")
    for expr, idx in fields:
        print(f"              data[{idx}] = {expr};")
    print("            } break;")
print()

print("/* === READ 16-BIT OFFSETS (0x30-0x4A) === */")
print(gen_read_16(registers_16bit_ofs, "r->calib_offset.", "OFS "))
print()

print("/* === READ 16-BIT SCALES (0x50-0x6A) === */")
print(gen_read_16(registers_16bit_scl, "r->calib_scale.", "SCL "))
print()

print("/* === READ 16-BIT EU (0x70-0x8A) === */")
print(gen_read_16(registers_16bit_eu, "r->eu.", "EU "))
print()

print("/* === READ DERIVED EU (0x90-0x91) === */")
print(gen_read_16(registers_derived_eu, "r->eu.", "DERIVED "))
print()

print("/* === READ ERROR CODE (0xA0) === */")
print("""\
        case 0xA0U: /* ERROR_CODE[31:0] in bytes 4-1 (32-bit unsigned LE) */
            data[0] = (uint8_t)(g_regs.error_code         & 0xFFU);
            data[1] = (uint8_t)((g_regs.error_code >>  8U) & 0xFFU);
            data[2] = (uint8_t)((g_regs.error_code >> 16U) & 0xFFU);
            data[3] = (uint8_t)((g_regs.error_code >> 24U) & 0xFFU);
            break;""")
print()

print("/* === READ FAULT REGISTERS (0xA8-0xAA) === */")
print("""\
        case 0xA8U: /* FAULT_REG1 */
            data[0] = g_regs.fault_regs.reg1;
            break;
        case 0xA9U: /* FAULT_REG2 */
            data[0] = g_regs.fault_regs.reg2[0];
            data[1] = g_regs.fault_regs.reg2[1];
            data[2] = g_regs.fault_regs.reg2[2];
            break;
        case 0xAAU: /* FAULT_REG3 */
            data[0] = g_regs.fault_regs.reg3[0];
            data[1] = g_regs.fault_regs.reg3[1];
            data[2] = g_regs.fault_regs.reg3[2];
            data[3] = g_regs.fault_regs.reg3[3];
            break;""")
print()

print("/* === READ 16-BIT LIMITS (0xB0-0xCF) === */")
print(gen_read_16(registers_16bit_lim, "r->soft_limits.", "LIM "))
print()

print("/* === READ LIMIT FAULT FLAGS (0xD0-0xD1, 0xD8-0xD9) === */")
print("""\
        case 0xD0U: /* Soft Limit Fault Group 1: 6 bytes */
            memcpy(&data[0], g_regs.lim_faults.soft_grp1, 6U);
            break;
        case 0xD1U: /* Soft Limit Fault Group 2: 3 bytes */
            data[0] = g_regs.lim_faults.soft_grp2[0];
            data[1] = g_regs.lim_faults.soft_grp2[1];
            data[2] = g_regs.lim_faults.soft_grp2[2];
            break;
        case 0xD8U: /* Hard Limit Fault Group 1: 6 bytes */
            memcpy(&data[0], g_regs.lim_faults.hard_grp1, 6U);
            break;
        case 0xD9U: /* Hard Limit Fault Group 2: 3 bytes */
            data[0] = g_regs.lim_faults.hard_grp2[0];
            data[1] = g_regs.lim_faults.hard_grp2[1];
            data[2] = g_regs.lim_faults.hard_grp2[2];
            break;""")
print()

print("/* === READ BUILD INFO + WRITE PROTECT (0x1F8, 0x1FA) === */")
print("""\
        case 0x1F8U: /* HARDWARE_REVISION[B2-1], FIRMWARE_REVISION[B4-3] */
            pack16(&data[0], &data[1], g_regs.build_info.hw_rev);
            pack16(&data[2], &data[3], g_regs.build_info.fw_rev);
            break;
        case 0x1FAU: /* Write Protect bytes 1-5 */
            data[0] = g_regs.write_protect.b1;
            data[1] = g_regs.write_protect.b2;
            data[2] = g_regs.write_protect.b3;
            data[3] = g_regs.write_protect.b4;
            data[4] = g_regs.write_protect.b5;
            break;""")
print()

print("/* ============================================================")
print(" * WRITE: process_write() switch-case bodies")
print(" * ============================================================ */\n")

print("/* === WRITE POWER ENABLE (0x1F-0x23, except 0x22 read-only) === */")
for hex_id, name, fields in registers_pwr_en:
    if name == "PERIPH_INPUTS":
        continue
    print(f"        case 0x{hex_id:02X}U: /* {name} */")
    print("            { PMB_PowerEnable_t *pe = &g_regs.pwr_enable;")
    for expr, idx in fields:
        print(f"              {expr} = data[{idx}];")
    print("            } break;")
print()

print("/* === WRITE 16-BIT OFFSETS (0x30-0x4A) === */")
print(gen_write_16(registers_16bit_ofs, "r->calib_offset."))
print()

print("/* === WRITE 16-BIT SCALES (0x50-0x6A) === */")
print(gen_write_16(registers_16bit_scl, "r->calib_scale."))
print()

print("/* === WRITE 16-BIT LIMITS (0xB0-0xCF) === */")
print(gen_write_16(registers_16bit_lim, "r->soft_limits."))
print()

print("/* === WRITE FAULT REGISTERS (0xA8-0xAA): writing 1 clears === */")
print("""\
        case 0xA8U:
            g_regs.fault_regs.reg1    &= ~data[0];
            break;
        case 0xA9U:
            g_regs.fault_regs.reg2[0] &= ~data[0];
            g_regs.fault_regs.reg2[1] &= ~data[1];
            g_regs.fault_regs.reg2[2] &= ~data[2];
            break;
        case 0xAAU:
            g_regs.fault_regs.reg3[0] &= ~data[0];
            g_regs.fault_regs.reg3[1] &= ~data[1];
            g_regs.fault_regs.reg3[2] &= ~data[2];
            g_regs.fault_regs.reg3[3] &= ~data[3];
            break;""")
print()

print("/* === WRITE LIMIT FAULT FLAGS (0xD0-0xD1, 0xD8-0xD9): clear-on-write-1 === */")
print("""\
        case 0xD0U:
            for (int _i = 0; _i < 6; _i++) g_regs.lim_faults.soft_grp1[_i] &= ~data[_i];
            break;
        case 0xD1U:
            g_regs.lim_faults.soft_grp2[0] &= ~data[0];
            g_regs.lim_faults.soft_grp2[1] &= ~data[1];
            g_regs.lim_faults.soft_grp2[2] &= ~data[2];
            break;
        case 0xD8U:
            for (int _i = 0; _i < 6; _i++) g_regs.lim_faults.hard_grp1[_i] &= ~data[_i];
            break;
        case 0xD9U:
            g_regs.lim_faults.hard_grp2[0] &= ~data[0];
            g_regs.lim_faults.hard_grp2[1] &= ~data[1];
            g_regs.lim_faults.hard_grp2[2] &= ~data[2];
            break;""")
print()

print("/* === WRITE BUILD INFO + WRITE PROTECT (0x1F8, 0x1FA) === */")
print("""\
        case 0x1F8U:
            g_regs.build_info.hw_rev = unpack16(data[0], data[1]);
            g_regs.build_info.fw_rev = unpack16(data[2], data[3]);
            break;
        case 0x1FAU:
            g_regs.write_protect.b1 = data[0];
            g_regs.write_protect.b2 = data[1];
            g_regs.write_protect.b3 = data[2];
            g_regs.write_protect.b4 = data[3];
            g_regs.write_protect.b5 = data[4];
            break;""")
print()

# ---------------------------------------------------------------------------
# Summary
# ---------------------------------------------------------------------------
active = [r for r in all_rows
          if r['id'] not in RESERVED_IDS
          and 'Reserved' not in r['name']
          and r['rw'] not in ('nan', '')]

read_count = (len(registers_12bit) + len(registers_status) + len(registers_pwr_en) +
              len(registers_16bit_ofs) + len(registers_16bit_scl) +
              len(registers_16bit_eu) + len(registers_derived_eu) + 1 +
              3 + len(registers_16bit_lim) + 4 + 2)
write_count = (len(registers_pwr_en) - 1 +
               len(registers_16bit_ofs) + len(registers_16bit_scl) +
               len(registers_16bit_lim) + 3 + 4 + 2)

print(f"/* Summary: {read_count} read cases, {write_count} write cases, "
      f"{len(active)} active registers parsed from CSV */")
