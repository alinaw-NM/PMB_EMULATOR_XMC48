/**
 * @file pmb_can_emulator.c
 * @brief PMB CAN Protocol Emulator — complete implementation for XMC4800
 *
 * Every Data ID from PMB_CAN_Protocol_v1.xlsx is decoded here.
 * Byte index convention (matches the spec):
 *   data[0] = Byte 1 (first transmitted byte, LSB position in spec)
 *   data[7] = Byte 8 (last transmitted byte, MSB position in spec)
 *   data[7]:data[6] = packet counter (Bytes 8-7, reserved per spec)
 *   data[5]:data[4] = Bytes 6-5  (highest data field)
 *   data[3]:data[2] = Bytes 4-3  (middle data field)
 *   data[1]:data[0] = Bytes 2-1  (lowest data field)
 *
 * 12-bit encoding (B2-1):
 *   data[0] = FIELD[7:0]
 *   data[1] = 0x00 | (FIELD[11:8] & 0x0F)   i.e. {4b0000, FIELD[11:8]}
 *
 * 16-bit encoding (B2-1), little-endian:
 *   data[0] = FIELD[7:0]
 *   data[1] = FIELD[15:8]
 *
 * Read/Write distinction:
 *   Fixture sends an 8-byte frame to PMB address (0xC0).
 *   A frame with all data bytes zero is treated as a read request.
 *   A frame with non-zero data bytes is treated as a write request.
 *   PMB always responds with the current register value.
 */

#include "pmb_can_emulator.h"

/* =========================================================================
 * MODULE STATE
 * ========================================================================= */
static PMB_Registers_t g_regs;

/* =========================================================================
 * BYTE PACKING HELPERS
 * ========================================================================= */

/** Pack a 12-bit unsigned value into two consecutive data bytes. */
static inline void pack12(uint8_t *lo, uint8_t *hi, uint16_t val)
{
    *lo = (uint8_t)(val & 0xFFU);
    *hi = (uint8_t)((val >> 8U) & 0x0FU);
}

/** Unpack a 12-bit unsigned value from two consecutive data bytes. */
static inline uint16_t unpack12(uint8_t lo, uint8_t hi)
{
    return (uint16_t)(((uint16_t)(hi & 0x0FU) << 8U) | lo);
}

/**
 * Pack a 16-bit value (signed or unsigned) into two consecutive data bytes,
 * little-endian (lo byte first, as in the spec).
 */
static inline void pack16(uint8_t *lo, uint8_t *hi, uint16_t val)
{
    *lo = (uint8_t)(val & 0xFFU);
    *hi = (uint8_t)(val >> 8U);
}

/** Unpack a 16-bit little-endian value from two consecutive data bytes. */
static inline uint16_t unpack16(uint8_t lo, uint8_t hi)
{
    return (uint16_t)(((uint16_t)hi << 8U) | lo);
}

/** Compute even parity for a byte's data bits [7:1]; returns 0 or 1 for bit 0. */
static uint8_t even_parity(uint8_t byte_without_parity)
{
    uint8_t p = 0U;
    uint8_t b = byte_without_parity >> 1U; /* data is in bits [7:1] */
    while (b) { p ^= (b & 1U); b >>= 1U; }
    return p;
}

/* =========================================================================
 * WRITE PROTECT CHECKS
 * ========================================================================= */
static bool write_allowed(uint16_t data_id)
{
    PMB_WriteProtect_t *wp = &g_regs.write_protect;

    /* Write Protect itself always writable */
    if (data_id == REG_WRITE_PROTECT) return true;

    /* Calibration offsets */
    if (data_id >= REG_ADC_OFS_GRP1 && data_id <= REG_ADC_OFS_GRP27)
        return (wp->b1 & WP_OFS_BIT) != 0U;

    /* Calibration scales */
    if (data_id >= REG_ADC_SCL_GRP1 && data_id <= REG_ADC_SCL_GRP27)
        return (wp->b1 & WP_SCL_BIT) != 0U;

    /* Soft limits */
    if ((data_id >= REG_PLIM_GRP1 && data_id <= REG_NLIM_GRP5))
        return (wp->b1 & WP_LIM_BIT) != 0U;

    /* Build info */
    if (data_id == REG_HW_FW_REVISION)
        return (wp->b1 & WP_REV_BIT) != 0U;

    /* Power Enable — individual byte protection */
    if (data_id == REG_PWR_EN_GRP1) return (wp->b2 & 0x01U) != 0U;
    if (data_id == REG_PWR_EN_GRP2) return (wp->b3 & 0x07U) != 0U;
    if (data_id == REG_PWR_EN_GRP3) return (wp->b4 & 0x0FU) != 0U;
    if (data_id == REG_PWR_EN_GRP4) return (wp->b5 & 0x07U) != 0U;

    /* Fault registers and limit fault flags — always writable (not protected) */
    if (data_id == REG_FAULT_REG1 || data_id == REG_FAULT_REG2 || data_id == REG_FAULT_REG3)
        return true;
    if (data_id == REG_SOFT_LIM_FLT_GRP1 || data_id == REG_SOFT_LIM_FLT_GRP2)
        return true;
    if (data_id == REG_HARD_LIM_FLT_GRP1 || data_id == REG_HARD_LIM_FLT_GRP2)
        return true;

    return false; /* Read-only or unknown */
}

/* =========================================================================
 * BUILD RESPONSE FRAME
 * Fills data[0..5] with register content.  data[6..7] = packet counter.
 * ========================================================================= */
static void build_response(uint16_t data_id, uint8_t data[8])
{
    PMB_Telemetry_t    *t  = &g_regs.telemetry;
    PMB_CalibOffset_t  *co = &g_regs.calib_offset;
    PMB_CalibScale_t   *cs = &g_regs.calib_scale;
    PMB_EU_t           *eu = &g_regs.eu;
    PMB_SoftLimits_t   *sl = &g_regs.soft_limits;
    PMB_Status_t       *st = &g_regs.status;
    PMB_PowerEnable_t  *pe = &g_regs.pwr_enable;
    PMB_FaultRegs_t    *fr = &g_regs.fault_regs;
    PMB_LimFaults_t    *lf = &g_regs.lim_faults;

    memset(data, 0, 8U);
    /* Packet counter in bytes 8-7 (data[7]:data[6]) */
    data[7] = (uint8_t)(g_regs.packet_counter >> 8U);
    data[6] = (uint8_t)(g_regs.packet_counter & 0xFFU);
    g_regs.packet_counter++;

    switch (data_id)
    {
    /* ------------------------------------------------------------------
     * TELEMETRY 0x00-0x1A  (12-bit unsigned, B2-1, B4-3, B6-5)
     * ------------------------------------------------------------------ */
        case 0x00U: /* CHG_CURR[B2-1], BATT1_CURR[B4-3], BATT2_CURR[B6-5] */
            pack12(&data[0], &data[1], t->chg_curr);
            pack12(&data[2], &data[3], t->batt1_curr);
            pack12(&data[4], &data[5], t->batt2_curr);
            break;
        case 0x01U: /* LL_IMON, RL_IMON, LA_IMON */
            pack12(&data[0], &data[1], t->ll_imon);
            pack12(&data[2], &data[3], t->rl_imon);
            pack12(&data[4], &data[5], t->la_imon);
            break;
        case 0x02U: /* RA_IMON, TR_IMON, CHG_IN */
            pack12(&data[0], &data[1], t->ra_imon);
            pack12(&data[2], &data[3], t->tr_imon);
            pack12(&data[4], &data[5], t->chg_in);
            break;
        case 0x03U: /* HV, BATT1_IN, BATT2_IN */
            pack12(&data[0], &data[1], t->hv);
            pack12(&data[2], &data[3], t->batt1_in);
            pack12(&data[4], &data[5], t->batt2_in);
            break;
        case 0x04U: /* NK_IMON, MOT_BUS, CHG_HV */
            pack12(&data[0], &data[1], t->nk_imon);
            pack12(&data[2], &data[3], t->mot_bus);
            pack12(&data[4], &data[5], t->chg_hv);
            break;
        case 0x05U: /* USER_HV[B2-1], USER_HV_CURR[B4-3] */
            pack12(&data[0], &data[1], t->user_hv);
            pack12(&data[2], &data[3], t->user_hv_curr);
            break;
        case 0x06U: /* PC_24V[B2-1], PC_24V_IMON[B4-3] */
            pack12(&data[0], &data[1], t->pc_24v);
            pack12(&data[2], &data[3], t->pc_24v_imon);
            break;
        case 0x07U: /* USER_24V, USER_24V_IMON */
            pack12(&data[0], &data[1], t->user_24v);
            pack12(&data[2], &data[3], t->user_24v_imon);
            break;
        case 0x08U: /* PC_A_12V, PC_A_12V_IMON */
            pack12(&data[0], &data[1], t->pc_a_12v);
            pack12(&data[2], &data[3], t->pc_a_12v_imon);
            break;
        case 0x09U: /* PC_B_12V, PC_B_12V_IMON */
            pack12(&data[0], &data[1], t->pc_b_12v);
            pack12(&data[2], &data[3], t->pc_b_12v_imon);
            break;
        case 0x0AU: /* 24V_IMON0, 24V_IMON1, 24V_IMON2 */
            pack12(&data[0], &data[1], t->v24_imon0);
            pack12(&data[2], &data[3], t->v24_imon1);
            pack12(&data[4], &data[5], t->v24_imon2);
            break;
        case 0x0BU: /* 12V_IMON0, 12V_IMON1, 12V_IMON2 */
            pack12(&data[0], &data[1], t->v12_imon0);
            pack12(&data[2], &data[3], t->v12_imon1);
            pack12(&data[4], &data[5], t->v12_imon2);
            break;
        case 0x0CU: /* 24V_BUS_VOLTAGE[B2-1], 12V_BUS_A_VOLTAGE[B4-3] */
            pack12(&data[0], &data[1], t->v24_bus_voltage);
            pack12(&data[2], &data[3], t->v12_bus_a_voltage);
            break;
        case 0x0DU: /* 5V[B2-1], 5V_CURR[B4-3] */
            pack12(&data[0], &data[1], t->v5);
            pack12(&data[2], &data[3], t->v5_curr);
            break;
        case 0x0EU: /* 3.3V[B2-1], 3.3V_CURR[B4-3] */
            pack12(&data[0], &data[1], t->v3v3);
            pack12(&data[2], &data[3], t->v3v3_curr);
            break;
        case 0x0FU: /* AUX_24V, AUX_24V_CURR */
            pack12(&data[0], &data[1], t->aux_24v);
            pack12(&data[2], &data[3], t->aux_24v_curr);
            break;
        case 0x10U: /* AUX_12V, AUX_12V_CURR */
            pack12(&data[0], &data[1], t->aux_12v);
            pack12(&data[2], &data[3], t->aux_12v_curr);
            break;
        case 0x11U: /* ESTOP_REC_A_12V, ESTOP_REC_B_12V */
            pack12(&data[0], &data[1], t->estop_rec_a_12v);
            pack12(&data[2], &data[3], t->estop_rec_b_12v);
            break;
        case 0x12U: /* 12V_B_IMON3[B2-1], 12V_B_IMON4[B4-3] */
            pack12(&data[0], &data[1], t->v12_b_imon3);
            pack12(&data[2], &data[3], t->v12_b_imon4);
            break;
        case 0x13U: /* HV_OR[B2-1], MB_SP_IMON[B4-3] */
            pack12(&data[0], &data[1], t->hv_or);
            pack12(&data[2], &data[3], t->mb_sp_imon);
            break;
        case 0x14U: /* TEMP0, TEMP1, TEMP2 */
            pack12(&data[0], &data[1], t->temp0);
            pack12(&data[2], &data[3], t->temp1);
            pack12(&data[4], &data[5], t->temp2);
            break;
        case 0x15U: /* TEMP3, TEMP4, TEMP5 */
            pack12(&data[0], &data[1], t->temp3);
            pack12(&data[2], &data[3], t->temp4);
            pack12(&data[4], &data[5], t->temp5);
            break;
        case 0x16U: /* EX_TEMP0[B2-1], EX_TEMP1[B4-3] */
            pack12(&data[0], &data[1], t->ex_temp0);
            pack12(&data[2], &data[3], t->ex_temp1);
            break;
        case 0x17U: /* LIDAR_A_12V, LIDAR_B_12V */
            pack12(&data[0], &data[1], t->lidar_a_12v);
            pack12(&data[2], &data[3], t->lidar_b_12v);
            break;
        case 0x18U: /* STO_24V[B2-1], CHG_HVIL_OUT[B4-3] */
            pack12(&data[0], &data[1], t->sto_24v);
            pack12(&data[2], &data[3], t->chg_hvil_out);
            break;
        case 0x19U: /* ETH_2.5V[B2-1], ETH_1.2V[B4-3] */
            pack12(&data[0], &data[1], t->eth_2v5);
            pack12(&data[2], &data[3], t->eth_1v2);
            break;
        case 0x1AU: /* ECAT_3.3V[B2-1], ECAT_1.2V[B4-3] */
            pack12(&data[0], &data[1], t->ecat_3v3);
            pack12(&data[2], &data[3], t->ecat_1v2);
            break;

    /* ------------------------------------------------------------------
     * STATUS / COMMAND  0x1B-0x1E  (bit-mapped, Read Only)
     * ------------------------------------------------------------------ */
        case 0x1BU: /* COMP_PDG: B1={RL_J3..LL_J1}, B2={TR_J2J3..RL_J4}, B3={5b0,ROBOT_STO,MB_USER,MB_NK} */
            data[0] = st->comp_pdg[0];
            data[1] = st->comp_pdg[1];
            data[2] = st->comp_pdg[2];
            break;
        case 0x1CU: /* PDG_FAULT_GRP1: B1={3b0,CHG_FLTn..USER_HV_FLTn}, B2={MB_RA_FLTn..12V_B_PGD}, B3={6b0,IO0_P4.1,IO0_P4.0} */
            data[0] = st->pdg_fault_grp1[0];
            data[1] = st->pdg_fault_grp1[1];
            data[2] = st->pdg_fault_grp1[2];
            break;
        case 0x1DU: /* PDG_FAULT_GRP2: B1={PC_24V_FLTn..PC_B_12V_FLTn}, B2={6b0,AUX_12V_FLTn,AUX_12V_PG} */
            data[0] = st->pdg_fault_grp2[0];
            data[1] = st->pdg_fault_grp2[1];
            break;
        case 0x1EU: /* PDG_FAULT_GRP3: B1={24V_PGD..BATT2_OCn}, B2={6b0,IO1_P4.1,IO1_P4.0} */
            data[0] = st->pdg_fault_grp3[0];
            data[1] = st->pdg_fault_grp3[1];
            break;

    /* ------------------------------------------------------------------
     * POWER ENABLE  0x1F-0x23  (bit-mapped, R/W, parity at bit 0)
     * ------------------------------------------------------------------ */
        case 0x1FU: /* B1={ESC_EEPROM_WC1n,WC2n,WC3n,1b0,PC_24V_IGNITE,PC_A_12V_IGNITE,PC_B_12V_IGNITE,Parity} */
            data[0] = pe->grp1;
            break;
        case 0x20U: /* B1={24V_EN..USER_24V_ON,Par}, B2={STO_24V_EN..USER_HV_ON,Par}, B3={AUX_12V_EN..ESTOP_REC_B_12V_EN,Par} */
            data[0] = pe->grp2[0];
            data[1] = pe->grp2[1];
            data[2] = pe->grp2[2];
            break;
        case 0x21U: /* B1={4b0,MOT_BUS_EN,INP1,INP2,Par}, B2={BATT2..}, B3={BATT1..}, B4={CHG_REM..BATT1_RELAY,Par} */
            data[0] = pe->grp3[0];
            data[1] = pe->grp3[1];
            data[2] = pe->grp3[2];
            data[3] = pe->grp3[3];
            break;
        case 0x22U: /* B1={MCU_PB_OUT..PC_OUT0}  Read Only */
            data[0] = pe->periph_inputs;
            break;
        case 0x23U: /* B1={4b0,PC_IN2,PC_IN1,PC_IN0,Par}, B2={2b0,3.3V_ISO_CTRL4..0,Par}, B3={3b0,PANEL_LED3..0,Par} */
            data[0] = pe->grp4[0];
            data[1] = pe->grp4[1];
            data[2] = pe->grp4[2];
            break;

    /* ------------------------------------------------------------------
     * CALIBRATION OFFSETS  0x30-0x4A  (16-bit signed, LE)
     * Pattern: B1-2 = field_lo, B3-4 = field_mid, B5-6 = field_hi
     * ------------------------------------------------------------------ */
        case 0x30U: /* CHG_CURR_OFS[B2-1], BATT1_CURR_OFS[B4-3], BATT2_CURR_OFS[B6-5] */
            pack16(&data[0], &data[1], (uint16_t)co->chg_curr_ofs);
            pack16(&data[2], &data[3], (uint16_t)co->batt1_curr_ofs);
            pack16(&data[4], &data[5], (uint16_t)co->batt2_curr_ofs);
            break;
        case 0x31U: /* LL_IMON_OFS, RL_IMON_OFS, LA_IMON_OFS */
            pack16(&data[0], &data[1], (uint16_t)co->ll_imon_ofs);
            pack16(&data[2], &data[3], (uint16_t)co->rl_imon_ofs);
            pack16(&data[4], &data[5], (uint16_t)co->la_imon_ofs);
            break;
        case 0x32U: /* RA_IMON_OFS, TR_IMON_OFS, CHG_IN_OFS */
            pack16(&data[0], &data[1], (uint16_t)co->ra_imon_ofs);
            pack16(&data[2], &data[3], (uint16_t)co->tr_imon_ofs);
            pack16(&data[4], &data[5], (uint16_t)co->chg_in_ofs);
            break;
        case 0x33U: /* HV_OFS, BATT1_IN_OFS, BATT2_IN_OFS */
            pack16(&data[0], &data[1], (uint16_t)co->hv_ofs);
            pack16(&data[2], &data[3], (uint16_t)co->batt1_in_ofs);
            pack16(&data[4], &data[5], (uint16_t)co->batt2_in_ofs);
            break;
        case 0x34U: /* NK_IMON_OFS, MOT_BUS_OFS, CHG_HV_OFS */
            pack16(&data[0], &data[1], (uint16_t)co->nk_imon_ofs);
            pack16(&data[2], &data[3], (uint16_t)co->mot_bus_ofs);
            pack16(&data[4], &data[5], (uint16_t)co->chg_hv_ofs);
            break;
        case 0x35U: /* USER_HV_OFS[B2-1], USER_HV_CURR_OFS[B4-3] */
            pack16(&data[0], &data[1], (uint16_t)co->user_hv_ofs);
            pack16(&data[2], &data[3], (uint16_t)co->user_hv_curr_ofs);
            break;
        case 0x36U: /* PC_24V_OFS[B2-1], PC_24V_IMON_OFS[B4-3] */
            pack16(&data[0], &data[1], (uint16_t)co->pc_24v_ofs);
            pack16(&data[2], &data[3], (uint16_t)co->pc_24v_imon_ofs);
            break;
        case 0x37U: /* USER_24V_OFS, USER_24V_IMON_OFS */
            pack16(&data[0], &data[1], (uint16_t)co->user_24v_ofs);
            pack16(&data[2], &data[3], (uint16_t)co->user_24v_imon_ofs);
            break;
        case 0x38U: /* PC_A_12V_OFS, PC_A_12V_IMON_OFS */
            pack16(&data[0], &data[1], (uint16_t)co->pc_a_12v_ofs);
            pack16(&data[2], &data[3], (uint16_t)co->pc_a_12v_imon_ofs);
            break;
        case 0x39U: /* PC_B_12V_OFS, PC_B_12V_IMON_OFS */
            pack16(&data[0], &data[1], (uint16_t)co->pc_b_12v_ofs);
            pack16(&data[2], &data[3], (uint16_t)co->pc_b_12v_imon_ofs);
            break;
        case 0x3AU: /* 24V_IMON0_OFS, 24V_IMON1_OFS, 24V_IMON2_OFS */
            pack16(&data[0], &data[1], (uint16_t)co->v24_imon0_ofs);
            pack16(&data[2], &data[3], (uint16_t)co->v24_imon1_ofs);
            pack16(&data[4], &data[5], (uint16_t)co->v24_imon2_ofs);
            break;
        case 0x3BU: /* 12V_IMON0_OFS, 12V_IMON1_OFS, 12V_IMON2_OFS */
            pack16(&data[0], &data[1], (uint16_t)co->v12_imon0_ofs);
            pack16(&data[2], &data[3], (uint16_t)co->v12_imon1_ofs);
            pack16(&data[4], &data[5], (uint16_t)co->v12_imon2_ofs);
            break;
        case 0x3CU: /* 24V_OFS[B2-1], 12V_OFS[B4-3] */
            pack16(&data[0], &data[1], (uint16_t)co->v24_ofs);
            pack16(&data[2], &data[3], (uint16_t)co->v12_ofs);
            break;
        case 0x3DU: /* 5V_OFS[B2-1], 5V_CURR_OFS[B4-3] */
            pack16(&data[0], &data[1], (uint16_t)co->v5_ofs);
            pack16(&data[2], &data[3], (uint16_t)co->v5_curr_ofs);
            break;
        case 0x3EU: /* 3.3V_OFS[B2-1], 3.3V_CURR_OFS[B4-3] */
            pack16(&data[0], &data[1], (uint16_t)co->v3v3_ofs);
            pack16(&data[2], &data[3], (uint16_t)co->v3v3_curr_ofs);
            break;
        case 0x3FU: /* AUX_24V_OFS, AUX_24V_CURR_OFS */
            pack16(&data[0], &data[1], (uint16_t)co->aux_24v_ofs);
            pack16(&data[2], &data[3], (uint16_t)co->aux_24v_curr_ofs);
            break;
        case 0x40U: /* AUX_12V_OFS, AUX_12V_CURR_OFS */
            pack16(&data[0], &data[1], (uint16_t)co->aux_12v_ofs);
            pack16(&data[2], &data[3], (uint16_t)co->aux_12v_curr_ofs);
            break;
        case 0x41U: /* ESTOP_REC_A_12V_OFS, ESTOP_REC_B_12V_OFS */
            pack16(&data[0], &data[1], (uint16_t)co->estop_rec_a_12v_ofs);
            pack16(&data[2], &data[3], (uint16_t)co->estop_rec_b_12v_ofs);
            break;
        case 0x42U: /* 12V_B_IMON3_OFS, 12V_B_IMON4_OFS */
            pack16(&data[0], &data[1], (uint16_t)co->v12_b_imon3_ofs);
            pack16(&data[2], &data[3], (uint16_t)co->v12_b_imon4_ofs);
            break;
        case 0x43U: /* HV_OR_OFS, MB_SP_IMON_OFS */
            pack16(&data[0], &data[1], (uint16_t)co->hv_or_ofs);
            pack16(&data[2], &data[3], (uint16_t)co->mb_sp_imon_ofs);
            break;
        case 0x44U: /* TEMP0_OFS, TEMP1_OFS, TEMP2_OFS */
            pack16(&data[0], &data[1], (uint16_t)co->temp0_ofs);
            pack16(&data[2], &data[3], (uint16_t)co->temp1_ofs);
            pack16(&data[4], &data[5], (uint16_t)co->temp2_ofs);
            break;
        case 0x45U: /* TEMP3_OFS, TEMP4_OFS, TEMP5_OFS */
            pack16(&data[0], &data[1], (uint16_t)co->temp3_ofs);
            pack16(&data[2], &data[3], (uint16_t)co->temp4_ofs);
            pack16(&data[4], &data[5], (uint16_t)co->temp5_ofs);
            break;
        case 0x46U: /* EX_TEMP0_OFS, EX_TEMP1_OFS */
            pack16(&data[0], &data[1], (uint16_t)co->ex_temp0_ofs);
            pack16(&data[2], &data[3], (uint16_t)co->ex_temp1_ofs);
            break;
        case 0x47U: /* LIDAR_A_12V_OFS, LIDAR_B_12V_OFS */
            pack16(&data[0], &data[1], (uint16_t)co->lidar_a_12v_ofs);
            pack16(&data[2], &data[3], (uint16_t)co->lidar_b_12v_ofs);
            break;
        case 0x48U: /* STO_24V_OFS, CHG_HVIL_OUT_OFS */
            pack16(&data[0], &data[1], (uint16_t)co->sto_24v_ofs);
            pack16(&data[2], &data[3], (uint16_t)co->chg_hvil_out_ofs);
            break;
        case 0x49U: /* ETH_2.5V_OFS, ETH_1.2V_OFS */
            pack16(&data[0], &data[1], (uint16_t)co->eth_2v5_ofs);
            pack16(&data[2], &data[3], (uint16_t)co->eth_1v2_ofs);
            break;
        case 0x4AU: /* ECAT_3.3V_OFS, ECAT_1.2V_OFS */
            pack16(&data[0], &data[1], (uint16_t)co->ecat_3v3_ofs);
            pack16(&data[2], &data[3], (uint16_t)co->ecat_1v2_ofs);
            break;

    /* ------------------------------------------------------------------
     * CALIBRATION SCALES  0x50-0x6A  (16-bit signed, same layout as OFS)
     * ------------------------------------------------------------------ */
        case 0x50U: pack16(&data[0],&data[1],(uint16_t)cs->chg_curr_scl);   pack16(&data[2],&data[3],(uint16_t)cs->batt1_curr_scl); pack16(&data[4],&data[5],(uint16_t)cs->batt2_curr_scl); break;
        case 0x51U: pack16(&data[0],&data[1],(uint16_t)cs->ll_imon_scl);    pack16(&data[2],&data[3],(uint16_t)cs->rl_imon_scl);    pack16(&data[4],&data[5],(uint16_t)cs->la_imon_scl);    break;
        case 0x52U: pack16(&data[0],&data[1],(uint16_t)cs->ra_imon_scl);    pack16(&data[2],&data[3],(uint16_t)cs->tr_imon_scl);    pack16(&data[4],&data[5],(uint16_t)cs->chg_in_scl);     break;
        case 0x53U: pack16(&data[0],&data[1],(uint16_t)cs->hv_scl);         pack16(&data[2],&data[3],(uint16_t)cs->batt1_in_scl);   pack16(&data[4],&data[5],(uint16_t)cs->batt2_in_scl);   break;
        case 0x54U: pack16(&data[0],&data[1],(uint16_t)cs->nk_imon_scl);    pack16(&data[2],&data[3],(uint16_t)cs->mot_bus_scl);    pack16(&data[4],&data[5],(uint16_t)cs->chg_hv_scl);     break;
        case 0x55U: pack16(&data[0],&data[1],(uint16_t)cs->user_hv_scl);    pack16(&data[2],&data[3],(uint16_t)cs->user_hv_curr_scl); break;
        case 0x56U: pack16(&data[0],&data[1],(uint16_t)cs->pc_24v_scl);     pack16(&data[2],&data[3],(uint16_t)cs->pc_24v_imon_scl);  break;
        case 0x57U: pack16(&data[0],&data[1],(uint16_t)cs->user_24v_scl);   pack16(&data[2],&data[3],(uint16_t)cs->user_24v_imon_scl); break;
        case 0x58U: pack16(&data[0],&data[1],(uint16_t)cs->pc_a_12v_scl);   pack16(&data[2],&data[3],(uint16_t)cs->pc_a_12v_imon_scl); break;
        case 0x59U: pack16(&data[0],&data[1],(uint16_t)cs->pc_b_12v_scl);   pack16(&data[2],&data[3],(uint16_t)cs->pc_b_12v_imon_scl); break;
        case 0x5AU: pack16(&data[0],&data[1],(uint16_t)cs->v24_imon0_scl);  pack16(&data[2],&data[3],(uint16_t)cs->v24_imon1_scl);  pack16(&data[4],&data[5],(uint16_t)cs->v24_imon2_scl);  break;
        case 0x5BU: pack16(&data[0],&data[1],(uint16_t)cs->v12_imon0_scl);  pack16(&data[2],&data[3],(uint16_t)cs->v12_imon1_scl);  pack16(&data[4],&data[5],(uint16_t)cs->v12_imon2_scl);  break;
        case 0x5CU: pack16(&data[0],&data[1],(uint16_t)cs->v24_scl);        pack16(&data[2],&data[3],(uint16_t)cs->v12_scl);        break;
        case 0x5DU: pack16(&data[0],&data[1],(uint16_t)cs->v5_scl);         pack16(&data[2],&data[3],(uint16_t)cs->v5_curr_scl);    break;
        case 0x5EU: pack16(&data[0],&data[1],(uint16_t)cs->v3v3_scl);       pack16(&data[2],&data[3],(uint16_t)cs->v3v3_curr_scl);  break;
        case 0x5FU: pack16(&data[0],&data[1],(uint16_t)cs->aux_24v_scl);    pack16(&data[2],&data[3],(uint16_t)cs->aux_24v_curr_scl); break;
        case 0x60U: pack16(&data[0],&data[1],(uint16_t)cs->aux_12v_scl);    pack16(&data[2],&data[3],(uint16_t)cs->aux_12v_curr_scl); break;
        case 0x61U: pack16(&data[0],&data[1],(uint16_t)cs->estop_rec_a_12v_scl); pack16(&data[2],&data[3],(uint16_t)cs->estop_rec_b_12v_scl); break;
        case 0x62U: pack16(&data[0],&data[1],(uint16_t)cs->v12_b_imon3_scl); pack16(&data[2],&data[3],(uint16_t)cs->v12_b_imon4_scl); break;
        case 0x63U: pack16(&data[0],&data[1],(uint16_t)cs->hv_or_scl);      pack16(&data[2],&data[3],(uint16_t)cs->mb_sp_imon_scl); break;
        case 0x64U: pack16(&data[0],&data[1],(uint16_t)cs->temp0_scl);      pack16(&data[2],&data[3],(uint16_t)cs->temp1_scl);      pack16(&data[4],&data[5],(uint16_t)cs->temp2_scl);      break;
        case 0x65U: pack16(&data[0],&data[1],(uint16_t)cs->temp3_scl);      pack16(&data[2],&data[3],(uint16_t)cs->temp4_scl);      pack16(&data[4],&data[5],(uint16_t)cs->temp5_scl);      break;
        case 0x66U: pack16(&data[0],&data[1],(uint16_t)cs->ex_temp0_scl);   pack16(&data[2],&data[3],(uint16_t)cs->ex_temp1_scl);   break;
        case 0x67U: pack16(&data[0],&data[1],(uint16_t)cs->lidar_a_12v_scl); pack16(&data[2],&data[3],(uint16_t)cs->lidar_b_12v_scl); break;
        case 0x68U: pack16(&data[0],&data[1],(uint16_t)cs->sto_24v_scl);    pack16(&data[2],&data[3],(uint16_t)cs->chg_hvil_out_scl); break;
        case 0x69U: pack16(&data[0],&data[1],(uint16_t)cs->eth_2v5_scl);    pack16(&data[2],&data[3],(uint16_t)cs->eth_1v2_scl);    break;
        case 0x6AU: pack16(&data[0],&data[1],(uint16_t)cs->ecat_3v3_scl);   pack16(&data[2],&data[3],(uint16_t)cs->ecat_1v2_scl);   break;

    /* ------------------------------------------------------------------
     * ENGINEERING UNITS  0x70-0x8A  (16-bit signed, same layout as OFS)
     * EU = (raw - offset) * scale / 4095
     * Current: 1 count = 10mA, Voltage: 1 count = 10mV, Temp: 1 count = 100mC
     * ------------------------------------------------------------------ */
        case 0x70U: /* CHG_CURR_EU[B2-1], BATT1_CURR_EU[B4-3], BATT2_CURR_EU[B6-5] */
            pack16(&data[0],&data[1],(uint16_t)eu->chg_curr_eu);   pack16(&data[2],&data[3],(uint16_t)eu->batt1_curr_eu); pack16(&data[4],&data[5],(uint16_t)eu->batt2_curr_eu); break;
        case 0x71U: /* LL_IMON_EU, RL_IMON_EU, LA_IMON_EU */
            pack16(&data[0],&data[1],(uint16_t)eu->ll_imon_eu);    pack16(&data[2],&data[3],(uint16_t)eu->rl_imon_eu);    pack16(&data[4],&data[5],(uint16_t)eu->la_imon_eu);    break;
        case 0x72U: /* RA_IMON_EU, TR_IMON_EU, CHG_IN_EU */
            pack16(&data[0],&data[1],(uint16_t)eu->ra_imon_eu);    pack16(&data[2],&data[3],(uint16_t)eu->tr_imon_eu);    pack16(&data[4],&data[5],(uint16_t)eu->chg_in_eu);     break;
        case 0x73U: /* HV_EU, BATT1_IN_EU, BATT2_IN_EU */
            pack16(&data[0],&data[1],(uint16_t)eu->hv_eu);         pack16(&data[2],&data[3],(uint16_t)eu->batt1_in_eu);   pack16(&data[4],&data[5],(uint16_t)eu->batt2_in_eu);   break;
        case 0x74U: /* NK_IMON_EU, MOT_BUS_EU, CHG_HV_EU */
            pack16(&data[0],&data[1],(uint16_t)eu->nk_imon_eu);    pack16(&data[2],&data[3],(uint16_t)eu->mot_bus_eu);    pack16(&data[4],&data[5],(uint16_t)eu->chg_hv_eu);     break;
        case 0x75U: /* USER_HV_EU[B2-1], USER_HV_CURR_EU[B4-3] */
            pack16(&data[0],&data[1],(uint16_t)eu->user_hv_eu);    pack16(&data[2],&data[3],(uint16_t)eu->user_hv_curr_eu); break;
        case 0x76U: /* PC_24V_EU[B2-1], PC_24V_IMON_EU[B4-3] */
            pack16(&data[0],&data[1],(uint16_t)eu->pc_24v_eu);     pack16(&data[2],&data[3],(uint16_t)eu->pc_24v_imon_eu);  break;
        case 0x77U: /* USER_24V_EU, USER_24V_IMON_EU */
            pack16(&data[0],&data[1],(uint16_t)eu->user_24v_eu);   pack16(&data[2],&data[3],(uint16_t)eu->user_24v_imon_eu); break;
        case 0x78U: /* PC_A_12V_EU, PC_A_12V_IMON_EU */
            pack16(&data[0],&data[1],(uint16_t)eu->pc_a_12v_eu);   pack16(&data[2],&data[3],(uint16_t)eu->pc_a_12v_imon_eu); break;
        case 0x79U: /* PC_B_12V_EU, PC_B_12V_IMON_EU */
            pack16(&data[0],&data[1],(uint16_t)eu->pc_b_12v_eu);   pack16(&data[2],&data[3],(uint16_t)eu->pc_b_12v_imon_eu); break;
        case 0x7AU: /* 24V_IMON0_EU, 24V_IMON1_EU, 24V_IMON2_EU */
            pack16(&data[0],&data[1],(uint16_t)eu->v24_imon0_eu);  pack16(&data[2],&data[3],(uint16_t)eu->v24_imon1_eu);  pack16(&data[4],&data[5],(uint16_t)eu->v24_imon2_eu);  break;
        case 0x7BU: /* 12V_IMON0_EU, 12V_IMON1_EU, 12V_IMON2_EU */
            pack16(&data[0],&data[1],(uint16_t)eu->v12_imon0_eu);  pack16(&data[2],&data[3],(uint16_t)eu->v12_imon1_eu);  pack16(&data[4],&data[5],(uint16_t)eu->v12_imon2_eu);  break;
        case 0x7CU: /* 24V_EU[B2-1], 12V_EU[B4-3] */
            pack16(&data[0],&data[1],(uint16_t)eu->v24_eu);        pack16(&data[2],&data[3],(uint16_t)eu->v12_eu);        break;
        case 0x7DU: /* 5V_EU[B2-1], 5V_CURR_EU[B4-3] */
            pack16(&data[0],&data[1],(uint16_t)eu->v5_eu);         pack16(&data[2],&data[3],(uint16_t)eu->v5_curr_eu);    break;
        case 0x7EU: /* 3.3V_EU[B2-1], 3.3V_CURR_EU[B4-3] */
            pack16(&data[0],&data[1],(uint16_t)eu->v3v3_eu);       pack16(&data[2],&data[3],(uint16_t)eu->v3v3_curr_eu);  break;
        case 0x7FU: /* AUX_24V_EU, AUX_24V_CURR_EU */
            pack16(&data[0],&data[1],(uint16_t)eu->aux_24v_eu);    pack16(&data[2],&data[3],(uint16_t)eu->aux_24v_curr_eu); break;
        case 0x80U: /* AUX_12V_EU, AUX_12V_CURR_EU */
            pack16(&data[0],&data[1],(uint16_t)eu->aux_12v_eu);    pack16(&data[2],&data[3],(uint16_t)eu->aux_12v_curr_eu); break;
        case 0x81U: /* ESTOP_REC_A_12V_EU, ESTOP_REC_B_12V_EU */
            pack16(&data[0],&data[1],(uint16_t)eu->estop_rec_a_12v_eu); pack16(&data[2],&data[3],(uint16_t)eu->estop_rec_b_12v_eu); break;
        case 0x82U: /* 12V_B_IMON3_EU, 12V_B_IMON4_EU */
            pack16(&data[0],&data[1],(uint16_t)eu->v12_b_imon3_eu); pack16(&data[2],&data[3],(uint16_t)eu->v12_b_imon4_eu); break;
        case 0x83U: /* HV_OR_EU, MB_SP_IMON_EU */
            pack16(&data[0],&data[1],(uint16_t)eu->hv_or_eu);      pack16(&data[2],&data[3],(uint16_t)eu->mb_sp_imon_eu); break;
        case 0x84U: /* TEMP0_EU, TEMP1_EU, TEMP2_EU */
            pack16(&data[0],&data[1],(uint16_t)eu->temp0_eu);      pack16(&data[2],&data[3],(uint16_t)eu->temp1_eu);      pack16(&data[4],&data[5],(uint16_t)eu->temp2_eu);      break;
        case 0x85U: /* TEMP3_EU, TEMP4_EU, TEMP5_EU */
            pack16(&data[0],&data[1],(uint16_t)eu->temp3_eu);      pack16(&data[2],&data[3],(uint16_t)eu->temp4_eu);      pack16(&data[4],&data[5],(uint16_t)eu->temp5_eu);      break;
        case 0x86U: /* EX_TEMP0_EU, EX_TEMP1_EU */
            pack16(&data[0],&data[1],(uint16_t)eu->ex_temp0_eu);   pack16(&data[2],&data[3],(uint16_t)eu->ex_temp1_eu);   break;
        case 0x87U: /* LIDAR_A_12V_EU, LIDAR_B_12V_EU */
            pack16(&data[0],&data[1],(uint16_t)eu->lidar_a_12v_eu); pack16(&data[2],&data[3],(uint16_t)eu->lidar_b_12v_eu); break;
        case 0x88U: /* STO_24V_EU, CHG_HVIL_OUT_EU */
            pack16(&data[0],&data[1],(uint16_t)eu->sto_24v_eu);    pack16(&data[2],&data[3],(uint16_t)eu->chg_hvil_out_eu); break;
        case 0x89U: /* ETH_2.5V_EU, ETH_1.2V_EU */
            pack16(&data[0],&data[1],(uint16_t)eu->eth_2v5_eu);    pack16(&data[2],&data[3],(uint16_t)eu->eth_1v2_eu);    break;
        case 0x8AU: /* ECAT_3.3V_EU, ECAT_1.2V_EU */
            pack16(&data[0],&data[1],(uint16_t)eu->ecat_3v3_eu);   pack16(&data[2],&data[3],(uint16_t)eu->ecat_1v2_eu);   break;

    /* ------------------------------------------------------------------
     * DERIVED VALUES  0x90-0x91
     * ------------------------------------------------------------------ */
        case 0x90U: /* 12V_A_TOTAL_CURR_EU[B2-1], 12V_B_TOTAL_CURR_EU[B4-3], 24V_TOTAL[B6-5] */
            pack16(&data[0],&data[1],(uint16_t)eu->v12_a_total_curr_eu);
            pack16(&data[2],&data[3],(uint16_t)eu->v12_b_total_curr_eu);
            pack16(&data[4],&data[5],(uint16_t)eu->v24_total_curr_eu);
            break;
        case 0x91U: /* BATT_TOTAL_CURR_EU[B2-1] */
            pack16(&data[0],&data[1],(uint16_t)eu->batt_total_curr_eu);
            break;

    /* ------------------------------------------------------------------
     * ERROR CODE  0xA0
     * ------------------------------------------------------------------ */
        case 0xA0U: /* ERROR_CODE[31:0] in B4-1 */
            data[0] = (uint8_t)(g_regs.error_code        & 0xFFU);
            data[1] = (uint8_t)((g_regs.error_code >>  8U) & 0xFFU);
            data[2] = (uint8_t)((g_regs.error_code >> 16U) & 0xFFU);
            data[3] = (uint8_t)((g_regs.error_code >> 24U) & 0xFFU);
            break;

    /* ------------------------------------------------------------------
     * FAULT REGISTERS  0xA8-0xAA  (parity at bit0, clear-on-write-1)
     * ------------------------------------------------------------------ */
        case 0xA8U: /* B1={4b0,PC_24V_IGNITE,PC_A_12V_IGNITE,PC_B_12V_IGNITE,Parity} */
            data[0] = fr->reg1;
            break;
        case 0xA9U: /* B1-B3: same bit layout as PWR_EN_GRP2 */
            data[0] = fr->reg2[0];
            data[1] = fr->reg2[1];
            data[2] = fr->reg2[2];
            break;
        case 0xAAU: /* B1-B4: same bit layout as PWR_EN_GRP3 */
            data[0] = fr->reg3[0];
            data[1] = fr->reg3[1];
            data[2] = fr->reg3[2];
            data[3] = fr->reg3[3];
            break;

    /* ------------------------------------------------------------------
     * POSITIVE SOFT LIMITS  0xB0-0xCA  (16-bit signed, same layout as OFS)
     * ------------------------------------------------------------------ */
        case 0xB0U: pack16(&data[0],&data[1],(uint16_t)sl->chg_curr_lim);      pack16(&data[2],&data[3],(uint16_t)sl->batt1_curr_plim);  pack16(&data[4],&data[5],(uint16_t)sl->batt2_curr_plim); break;
        case 0xB1U: pack16(&data[0],&data[1],(uint16_t)sl->ll_imon_lim);       pack16(&data[2],&data[3],(uint16_t)sl->rl_imon_lim);       pack16(&data[4],&data[5],(uint16_t)sl->la_imon_lim);     break;
        case 0xB2U: pack16(&data[0],&data[1],(uint16_t)sl->ra_imon_lim);       pack16(&data[2],&data[3],(uint16_t)sl->tr_imon_lim);       pack16(&data[4],&data[5],(uint16_t)sl->chg_in_lim);      break;
        case 0xB3U: pack16(&data[0],&data[1],(uint16_t)sl->hv_lim);            pack16(&data[2],&data[3],(uint16_t)sl->batt1_in_lim);      pack16(&data[4],&data[5],(uint16_t)sl->batt2_in_lim);    break;
        case 0xB4U: pack16(&data[0],&data[1],(uint16_t)sl->nk_imon_lim);       pack16(&data[2],&data[3],(uint16_t)sl->mot_bus_lim);       pack16(&data[4],&data[5],(uint16_t)sl->chg_hv_lim);      break;
        case 0xB5U: pack16(&data[0],&data[1],(uint16_t)sl->user_hv_lim);       pack16(&data[2],&data[3],(uint16_t)sl->user_hv_curr_lim);  break;
        case 0xB6U: pack16(&data[0],&data[1],(uint16_t)sl->pc_24v_lim);        pack16(&data[2],&data[3],(uint16_t)sl->pc_24v_imon_lim);   break;
        case 0xB7U: pack16(&data[0],&data[1],(uint16_t)sl->user_24v_lim);      pack16(&data[2],&data[3],(uint16_t)sl->user_24v_imon_lim); break;
        case 0xB8U: pack16(&data[0],&data[1],(uint16_t)sl->pc_a_12v_lim);      pack16(&data[2],&data[3],(uint16_t)sl->pc_a_12v_imon_lim); break;
        case 0xB9U: pack16(&data[0],&data[1],(uint16_t)sl->pc_b_12v_lim);      pack16(&data[2],&data[3],(uint16_t)sl->pc_b_12v_imon_lim); break;
        case 0xBAU: pack16(&data[0],&data[1],(uint16_t)sl->v24_imon0_lim);     pack16(&data[2],&data[3],(uint16_t)sl->v24_imon1_lim);     pack16(&data[4],&data[5],(uint16_t)sl->v24_imon2_lim);   break;
        case 0xBBU: pack16(&data[0],&data[1],(uint16_t)sl->v12_imon0_lim);     pack16(&data[2],&data[3],(uint16_t)sl->v12_imon1_lim);     pack16(&data[4],&data[5],(uint16_t)sl->v12_imon2_lim);   break;
        case 0xBCU: pack16(&data[0],&data[1],(uint16_t)sl->v24_lim);           pack16(&data[2],&data[3],(uint16_t)sl->v12_lim);           break;
        case 0xBDU: pack16(&data[0],&data[1],(uint16_t)sl->v5_lim);            pack16(&data[2],&data[3],(uint16_t)sl->v5_curr_lim);       break;
        case 0xBEU: pack16(&data[0],&data[1],(uint16_t)sl->v3v3_lim);          pack16(&data[2],&data[3],(uint16_t)sl->v3v3_curr_lim);     break;
        case 0xBFU: pack16(&data[0],&data[1],(uint16_t)sl->aux_24v_lim);       pack16(&data[2],&data[3],(uint16_t)sl->aux_24v_curr_lim);  break;
        case 0xC0U: pack16(&data[0],&data[1],(uint16_t)sl->aux_12v_lim);       pack16(&data[2],&data[3],(uint16_t)sl->aux_12v_curr_lim);  break;
        case 0xC1U: pack16(&data[0],&data[1],(uint16_t)sl->estop_rec_a_12v_lim); pack16(&data[2],&data[3],(uint16_t)sl->estop_rec_b_12v_lim); break;
        case 0xC2U: pack16(&data[0],&data[1],(uint16_t)sl->v12_b_imon3_lim);   pack16(&data[2],&data[3],(uint16_t)sl->v12_b_imon4_lim);  break;
        case 0xC3U: pack16(&data[0],&data[1],(uint16_t)sl->hv_or_lim);         pack16(&data[2],&data[3],(uint16_t)sl->mb_sp_imon_lim);    break;
        case 0xC4U: pack16(&data[0],&data[1],(uint16_t)sl->temp0_plim);        pack16(&data[2],&data[3],(uint16_t)sl->temp1_plim);        pack16(&data[4],&data[5],(uint16_t)sl->temp2_plim);      break;
        case 0xC5U: pack16(&data[0],&data[1],(uint16_t)sl->temp3_plim);        pack16(&data[2],&data[3],(uint16_t)sl->temp4_plim);        pack16(&data[4],&data[5],(uint16_t)sl->temp5_plim);      break;
        case 0xC6U: pack16(&data[0],&data[1],(uint16_t)sl->ex_temp0_plim);     pack16(&data[2],&data[3],(uint16_t)sl->ex_temp1_plim);     break;
        case 0xC7U: pack16(&data[0],&data[1],(uint16_t)sl->lidar_a_12v_lim);   pack16(&data[2],&data[3],(uint16_t)sl->lidar_b_12v_lim);   break;
        case 0xC8U: pack16(&data[0],&data[1],(uint16_t)sl->sto_24v_lim);       pack16(&data[2],&data[3],(uint16_t)sl->chg_hvil_out_lim);  break;
        case 0xC9U: pack16(&data[0],&data[1],(uint16_t)sl->eth_2v5_lim);       pack16(&data[2],&data[3],(uint16_t)sl->eth_1v2_lim);       break;
        case 0xCAU: pack16(&data[0],&data[1],(uint16_t)sl->ecat_3v3_lim);      pack16(&data[2],&data[3],(uint16_t)sl->ecat_1v2_lim);      break;

    /* --- Negative Limits 0xCB-0xCF --- */
        case 0xCBU: /* 12V_A_TOTAL_CURR_LIM[B2-1], 12V_B_TOTAL_CURR_LIM[B4-3], 24V_TOTAL_CURR_LIM[B6-5] */
            pack16(&data[0],&data[1],(uint16_t)sl->v12_a_total_curr_lim);
            pack16(&data[2],&data[3],(uint16_t)sl->v12_b_total_curr_lim);
            pack16(&data[4],&data[5],(uint16_t)sl->v24_total_curr_lim);
            break;
        case 0xCCU: /* BATT1_CURR_NLIM[B4-3], BATT2_CURR_NLIM[B6-5]  (B2-1 = RSVD) */
            pack16(&data[2],&data[3],(uint16_t)sl->batt1_curr_nlim);
            pack16(&data[4],&data[5],(uint16_t)sl->batt2_curr_nlim);
            break;
        case 0xCDU: /* TEMP0_NLIM, TEMP1_NLIM, TEMP2_NLIM */
            pack16(&data[0],&data[1],(uint16_t)sl->temp0_nlim);
            pack16(&data[2],&data[3],(uint16_t)sl->temp1_nlim);
            pack16(&data[4],&data[5],(uint16_t)sl->temp2_nlim);
            break;
        case 0xCEU: /* TEMP3_NLIM, TEMP4_NLIM, TEMP5_NLIM */
            pack16(&data[0],&data[1],(uint16_t)sl->temp3_nlim);
            pack16(&data[2],&data[3],(uint16_t)sl->temp4_nlim);
            pack16(&data[4],&data[5],(uint16_t)sl->temp5_nlim);
            break;
        case 0xCFU: /* EX_TEMP0_NLIM[B2-1], EX_TEMP1_NLIM[B4-3] */
            pack16(&data[0],&data[1],(uint16_t)sl->ex_temp0_nlim);
            pack16(&data[2],&data[3],(uint16_t)sl->ex_temp1_nlim);
            break;

    /* ------------------------------------------------------------------
     * SOFT LIMIT FAULT FLAGS  0xD0-0xD1  (bit-mapped, clear-on-write-1)
     * ------------------------------------------------------------------ */
        case 0xD0U: /* B1-B6 (6 bytes of fault bits) */
            memcpy(&data[0], lf->soft_grp1, 6U);
            break;
        case 0xD1U: /* B1-B3 */
            data[0] = lf->soft_grp2[0];
            data[1] = lf->soft_grp2[1];
            data[2] = lf->soft_grp2[2];
            break;

    /* ------------------------------------------------------------------
     * HARD LIMIT FAULT FLAGS  0xD8-0xD9  (same layout as soft)
     * ------------------------------------------------------------------ */
        case 0xD8U:
            memcpy(&data[0], lf->hard_grp1, 6U);
            break;
        case 0xD9U:
            data[0] = lf->hard_grp2[0];
            data[1] = lf->hard_grp2[1];
            data[2] = lf->hard_grp2[2];
            break;

    /* ------------------------------------------------------------------
     * BUILD INFO  0x1F8
     * ------------------------------------------------------------------ */
        case 0x1F8U: /* HARDWARE_REVISION[B2-1], FIRMWARE_REVISION[B4-3] */
            pack16(&data[0], &data[1], g_regs.build_info.hw_rev);
            pack16(&data[2], &data[3], g_regs.build_info.fw_rev);
            break;

    /* ------------------------------------------------------------------
     * WRITE PROTECT  0x1FA
     * ------------------------------------------------------------------ */
        case 0x1FAU:
            data[0] = g_regs.write_protect.b1;
            data[1] = g_regs.write_protect.b2;
            data[2] = g_regs.write_protect.b3;
            data[3] = g_regs.write_protect.b4;
            data[4] = g_regs.write_protect.b5;
            break;

        default:
            /* Unknown or reserved register — return zeros */
            break;
    }
}

/* =========================================================================
 * PROCESS WRITE REQUEST
 * ========================================================================= */
static void process_write(uint16_t data_id, const uint8_t data[8])
{
    PMB_CalibOffset_t *co = &g_regs.calib_offset;
    PMB_CalibScale_t  *cs = &g_regs.calib_scale;
    PMB_SoftLimits_t  *sl = &g_regs.soft_limits;
    PMB_PowerEnable_t *pe = &g_regs.pwr_enable;
    PMB_FaultRegs_t   *fr = &g_regs.fault_regs;
    PMB_LimFaults_t   *lf = &g_regs.lim_faults;

    if (!write_allowed(data_id)) return;

    switch (data_id)
    {
    /* Power Enable */
        case 0x1FU: pe->grp1    = data[0]; break;
        case 0x20U: pe->grp2[0] = data[0]; pe->grp2[1] = data[1]; pe->grp2[2] = data[2]; break;
        case 0x21U: pe->grp3[0] = data[0]; pe->grp3[1] = data[1]; pe->grp3[2] = data[2]; pe->grp3[3] = data[3]; break;
        case 0x23U: pe->grp4[0] = data[0]; pe->grp4[1] = data[1]; pe->grp4[2] = data[2]; break;

    /* Calibration Offsets */
        case 0x30U: co->chg_curr_ofs=(int16_t)unpack16(data[0],data[1]); co->batt1_curr_ofs=(int16_t)unpack16(data[2],data[3]); co->batt2_curr_ofs=(int16_t)unpack16(data[4],data[5]); break;
        case 0x31U: co->ll_imon_ofs=(int16_t)unpack16(data[0],data[1]); co->rl_imon_ofs=(int16_t)unpack16(data[2],data[3]); co->la_imon_ofs=(int16_t)unpack16(data[4],data[5]); break;
        case 0x32U: co->ra_imon_ofs=(int16_t)unpack16(data[0],data[1]); co->tr_imon_ofs=(int16_t)unpack16(data[2],data[3]); co->chg_in_ofs=(int16_t)unpack16(data[4],data[5]); break;
        case 0x33U: co->hv_ofs=(int16_t)unpack16(data[0],data[1]); co->batt1_in_ofs=(int16_t)unpack16(data[2],data[3]); co->batt2_in_ofs=(int16_t)unpack16(data[4],data[5]); break;
        case 0x34U: co->nk_imon_ofs=(int16_t)unpack16(data[0],data[1]); co->mot_bus_ofs=(int16_t)unpack16(data[2],data[3]); co->chg_hv_ofs=(int16_t)unpack16(data[4],data[5]); break;
        case 0x35U: co->user_hv_ofs=(int16_t)unpack16(data[0],data[1]); co->user_hv_curr_ofs=(int16_t)unpack16(data[2],data[3]); break;
        case 0x36U: co->pc_24v_ofs=(int16_t)unpack16(data[0],data[1]); co->pc_24v_imon_ofs=(int16_t)unpack16(data[2],data[3]); break;
        case 0x37U: co->user_24v_ofs=(int16_t)unpack16(data[0],data[1]); co->user_24v_imon_ofs=(int16_t)unpack16(data[2],data[3]); break;
        case 0x38U: co->pc_a_12v_ofs=(int16_t)unpack16(data[0],data[1]); co->pc_a_12v_imon_ofs=(int16_t)unpack16(data[2],data[3]); break;
        case 0x39U: co->pc_b_12v_ofs=(int16_t)unpack16(data[0],data[1]); co->pc_b_12v_imon_ofs=(int16_t)unpack16(data[2],data[3]); break;
        case 0x3AU: co->v24_imon0_ofs=(int16_t)unpack16(data[0],data[1]); co->v24_imon1_ofs=(int16_t)unpack16(data[2],data[3]); co->v24_imon2_ofs=(int16_t)unpack16(data[4],data[5]); break;
        case 0x3BU: co->v12_imon0_ofs=(int16_t)unpack16(data[0],data[1]); co->v12_imon1_ofs=(int16_t)unpack16(data[2],data[3]); co->v12_imon2_ofs=(int16_t)unpack16(data[4],data[5]); break;
        case 0x3CU: co->v24_ofs=(int16_t)unpack16(data[0],data[1]); co->v12_ofs=(int16_t)unpack16(data[2],data[3]); break;
        case 0x3DU: co->v5_ofs=(int16_t)unpack16(data[0],data[1]); co->v5_curr_ofs=(int16_t)unpack16(data[2],data[3]); break;
        case 0x3EU: co->v3v3_ofs=(int16_t)unpack16(data[0],data[1]); co->v3v3_curr_ofs=(int16_t)unpack16(data[2],data[3]); break;
        case 0x3FU: co->aux_24v_ofs=(int16_t)unpack16(data[0],data[1]); co->aux_24v_curr_ofs=(int16_t)unpack16(data[2],data[3]); break;
        case 0x40U: co->aux_12v_ofs=(int16_t)unpack16(data[0],data[1]); co->aux_12v_curr_ofs=(int16_t)unpack16(data[2],data[3]); break;
        case 0x41U: co->estop_rec_a_12v_ofs=(int16_t)unpack16(data[0],data[1]); co->estop_rec_b_12v_ofs=(int16_t)unpack16(data[2],data[3]); break;
        case 0x42U: co->v12_b_imon3_ofs=(int16_t)unpack16(data[0],data[1]); co->v12_b_imon4_ofs=(int16_t)unpack16(data[2],data[3]); break;
        case 0x43U: co->hv_or_ofs=(int16_t)unpack16(data[0],data[1]); co->mb_sp_imon_ofs=(int16_t)unpack16(data[2],data[3]); break;
        case 0x44U: co->temp0_ofs=(int16_t)unpack16(data[0],data[1]); co->temp1_ofs=(int16_t)unpack16(data[2],data[3]); co->temp2_ofs=(int16_t)unpack16(data[4],data[5]); break;
        case 0x45U: co->temp3_ofs=(int16_t)unpack16(data[0],data[1]); co->temp4_ofs=(int16_t)unpack16(data[2],data[3]); co->temp5_ofs=(int16_t)unpack16(data[4],data[5]); break;
        case 0x46U: co->ex_temp0_ofs=(int16_t)unpack16(data[0],data[1]); co->ex_temp1_ofs=(int16_t)unpack16(data[2],data[3]); break;
        case 0x47U: co->lidar_a_12v_ofs=(int16_t)unpack16(data[0],data[1]); co->lidar_b_12v_ofs=(int16_t)unpack16(data[2],data[3]); break;
        case 0x48U: co->sto_24v_ofs=(int16_t)unpack16(data[0],data[1]); co->chg_hvil_out_ofs=(int16_t)unpack16(data[2],data[3]); break;
        case 0x49U: co->eth_2v5_ofs=(int16_t)unpack16(data[0],data[1]); co->eth_1v2_ofs=(int16_t)unpack16(data[2],data[3]); break;
        case 0x4AU: co->ecat_3v3_ofs=(int16_t)unpack16(data[0],data[1]); co->ecat_1v2_ofs=(int16_t)unpack16(data[2],data[3]); break;

    /* Calibration Scales */
        case 0x50U: cs->chg_curr_scl=(int16_t)unpack16(data[0],data[1]); cs->batt1_curr_scl=(int16_t)unpack16(data[2],data[3]); cs->batt2_curr_scl=(int16_t)unpack16(data[4],data[5]); break;
        case 0x51U: cs->ll_imon_scl=(int16_t)unpack16(data[0],data[1]); cs->rl_imon_scl=(int16_t)unpack16(data[2],data[3]); cs->la_imon_scl=(int16_t)unpack16(data[4],data[5]); break;
        case 0x52U: cs->ra_imon_scl=(int16_t)unpack16(data[0],data[1]); cs->tr_imon_scl=(int16_t)unpack16(data[2],data[3]); cs->chg_in_scl=(int16_t)unpack16(data[4],data[5]); break;
        case 0x53U: cs->hv_scl=(int16_t)unpack16(data[0],data[1]); cs->batt1_in_scl=(int16_t)unpack16(data[2],data[3]); cs->batt2_in_scl=(int16_t)unpack16(data[4],data[5]); break;
        case 0x54U: cs->nk_imon_scl=(int16_t)unpack16(data[0],data[1]); cs->mot_bus_scl=(int16_t)unpack16(data[2],data[3]); cs->chg_hv_scl=(int16_t)unpack16(data[4],data[5]); break;
        case 0x55U: cs->user_hv_scl=(int16_t)unpack16(data[0],data[1]); cs->user_hv_curr_scl=(int16_t)unpack16(data[2],data[3]); break;
        case 0x56U: cs->pc_24v_scl=(int16_t)unpack16(data[0],data[1]); cs->pc_24v_imon_scl=(int16_t)unpack16(data[2],data[3]); break;
        case 0x57U: cs->user_24v_scl=(int16_t)unpack16(data[0],data[1]); cs->user_24v_imon_scl=(int16_t)unpack16(data[2],data[3]); break;
        case 0x58U: cs->pc_a_12v_scl=(int16_t)unpack16(data[0],data[1]); cs->pc_a_12v_imon_scl=(int16_t)unpack16(data[2],data[3]); break;
        case 0x59U: cs->pc_b_12v_scl=(int16_t)unpack16(data[0],data[1]); cs->pc_b_12v_imon_scl=(int16_t)unpack16(data[2],data[3]); break;
        case 0x5AU: cs->v24_imon0_scl=(int16_t)unpack16(data[0],data[1]); cs->v24_imon1_scl=(int16_t)unpack16(data[2],data[3]); cs->v24_imon2_scl=(int16_t)unpack16(data[4],data[5]); break;
        case 0x5BU: cs->v12_imon0_scl=(int16_t)unpack16(data[0],data[1]); cs->v12_imon1_scl=(int16_t)unpack16(data[2],data[3]); cs->v12_imon2_scl=(int16_t)unpack16(data[4],data[5]); break;
        case 0x5CU: cs->v24_scl=(int16_t)unpack16(data[0],data[1]); cs->v12_scl=(int16_t)unpack16(data[2],data[3]); break;
        case 0x5DU: cs->v5_scl=(int16_t)unpack16(data[0],data[1]); cs->v5_curr_scl=(int16_t)unpack16(data[2],data[3]); break;
        case 0x5EU: cs->v3v3_scl=(int16_t)unpack16(data[0],data[1]); cs->v3v3_curr_scl=(int16_t)unpack16(data[2],data[3]); break;
        case 0x5FU: cs->aux_24v_scl=(int16_t)unpack16(data[0],data[1]); cs->aux_24v_curr_scl=(int16_t)unpack16(data[2],data[3]); break;
        case 0x60U: cs->aux_12v_scl=(int16_t)unpack16(data[0],data[1]); cs->aux_12v_curr_scl=(int16_t)unpack16(data[2],data[3]); break;
        case 0x61U: cs->estop_rec_a_12v_scl=(int16_t)unpack16(data[0],data[1]); cs->estop_rec_b_12v_scl=(int16_t)unpack16(data[2],data[3]); break;
        case 0x62U: cs->v12_b_imon3_scl=(int16_t)unpack16(data[0],data[1]); cs->v12_b_imon4_scl=(int16_t)unpack16(data[2],data[3]); break;
        case 0x63U: cs->hv_or_scl=(int16_t)unpack16(data[0],data[1]); cs->mb_sp_imon_scl=(int16_t)unpack16(data[2],data[3]); break;
        case 0x64U: cs->temp0_scl=(int16_t)unpack16(data[0],data[1]); cs->temp1_scl=(int16_t)unpack16(data[2],data[3]); cs->temp2_scl=(int16_t)unpack16(data[4],data[5]); break;
        case 0x65U: cs->temp3_scl=(int16_t)unpack16(data[0],data[1]); cs->temp4_scl=(int16_t)unpack16(data[2],data[3]); cs->temp5_scl=(int16_t)unpack16(data[4],data[5]); break;
        case 0x66U: cs->ex_temp0_scl=(int16_t)unpack16(data[0],data[1]); cs->ex_temp1_scl=(int16_t)unpack16(data[2],data[3]); break;
        case 0x67U: cs->lidar_a_12v_scl=(int16_t)unpack16(data[0],data[1]); cs->lidar_b_12v_scl=(int16_t)unpack16(data[2],data[3]); break;
        case 0x68U: cs->sto_24v_scl=(int16_t)unpack16(data[0],data[1]); cs->chg_hvil_out_scl=(int16_t)unpack16(data[2],data[3]); break;
        case 0x69U: cs->eth_2v5_scl=(int16_t)unpack16(data[0],data[1]); cs->eth_1v2_scl=(int16_t)unpack16(data[2],data[3]); break;
        case 0x6AU: cs->ecat_3v3_scl=(int16_t)unpack16(data[0],data[1]); cs->ecat_1v2_scl=(int16_t)unpack16(data[2],data[3]); break;

    /* Positive Soft Limits */
        case 0xB0U: sl->chg_curr_lim=(int16_t)unpack16(data[0],data[1]); sl->batt1_curr_plim=(int16_t)unpack16(data[2],data[3]); sl->batt2_curr_plim=(int16_t)unpack16(data[4],data[5]); break;
        case 0xB1U: sl->ll_imon_lim=(int16_t)unpack16(data[0],data[1]); sl->rl_imon_lim=(int16_t)unpack16(data[2],data[3]); sl->la_imon_lim=(int16_t)unpack16(data[4],data[5]); break;
        case 0xB2U: sl->ra_imon_lim=(int16_t)unpack16(data[0],data[1]); sl->tr_imon_lim=(int16_t)unpack16(data[2],data[3]); sl->chg_in_lim=(int16_t)unpack16(data[4],data[5]); break;
        case 0xB3U: sl->hv_lim=(int16_t)unpack16(data[0],data[1]); sl->batt1_in_lim=(int16_t)unpack16(data[2],data[3]); sl->batt2_in_lim=(int16_t)unpack16(data[4],data[5]); break;
        case 0xB4U: sl->nk_imon_lim=(int16_t)unpack16(data[0],data[1]); sl->mot_bus_lim=(int16_t)unpack16(data[2],data[3]); sl->chg_hv_lim=(int16_t)unpack16(data[4],data[5]); break;
        case 0xB5U: sl->user_hv_lim=(int16_t)unpack16(data[0],data[1]); sl->user_hv_curr_lim=(int16_t)unpack16(data[2],data[3]); break;
        case 0xB6U: sl->pc_24v_lim=(int16_t)unpack16(data[0],data[1]); sl->pc_24v_imon_lim=(int16_t)unpack16(data[2],data[3]); break;
        case 0xB7U: sl->user_24v_lim=(int16_t)unpack16(data[0],data[1]); sl->user_24v_imon_lim=(int16_t)unpack16(data[2],data[3]); break;
        case 0xB8U: sl->pc_a_12v_lim=(int16_t)unpack16(data[0],data[1]); sl->pc_a_12v_imon_lim=(int16_t)unpack16(data[2],data[3]); break;
        case 0xB9U: sl->pc_b_12v_lim=(int16_t)unpack16(data[0],data[1]); sl->pc_b_12v_imon_lim=(int16_t)unpack16(data[2],data[3]); break;
        case 0xBAU: sl->v24_imon0_lim=(int16_t)unpack16(data[0],data[1]); sl->v24_imon1_lim=(int16_t)unpack16(data[2],data[3]); sl->v24_imon2_lim=(int16_t)unpack16(data[4],data[5]); break;
        case 0xBBU: sl->v12_imon0_lim=(int16_t)unpack16(data[0],data[1]); sl->v12_imon1_lim=(int16_t)unpack16(data[2],data[3]); sl->v12_imon2_lim=(int16_t)unpack16(data[4],data[5]); break;
        case 0xBCU: sl->v24_lim=(int16_t)unpack16(data[0],data[1]); sl->v12_lim=(int16_t)unpack16(data[2],data[3]); break;
        case 0xBDU: sl->v5_lim=(int16_t)unpack16(data[0],data[1]); sl->v5_curr_lim=(int16_t)unpack16(data[2],data[3]); break;
        case 0xBEU: sl->v3v3_lim=(int16_t)unpack16(data[0],data[1]); sl->v3v3_curr_lim=(int16_t)unpack16(data[2],data[3]); break;
        case 0xBFU: sl->aux_24v_lim=(int16_t)unpack16(data[0],data[1]); sl->aux_24v_curr_lim=(int16_t)unpack16(data[2],data[3]); break;
        case 0xC0U: sl->aux_12v_lim=(int16_t)unpack16(data[0],data[1]); sl->aux_12v_curr_lim=(int16_t)unpack16(data[2],data[3]); break;
        case 0xC1U: sl->estop_rec_a_12v_lim=(int16_t)unpack16(data[0],data[1]); sl->estop_rec_b_12v_lim=(int16_t)unpack16(data[2],data[3]); break;
        case 0xC2U: sl->v12_b_imon3_lim=(int16_t)unpack16(data[0],data[1]); sl->v12_b_imon4_lim=(int16_t)unpack16(data[2],data[3]); break;
        case 0xC3U: sl->hv_or_lim=(int16_t)unpack16(data[0],data[1]); sl->mb_sp_imon_lim=(int16_t)unpack16(data[2],data[3]); break;
        case 0xC4U: sl->temp0_plim=(int16_t)unpack16(data[0],data[1]); sl->temp1_plim=(int16_t)unpack16(data[2],data[3]); sl->temp2_plim=(int16_t)unpack16(data[4],data[5]); break;
        case 0xC5U: sl->temp3_plim=(int16_t)unpack16(data[0],data[1]); sl->temp4_plim=(int16_t)unpack16(data[2],data[3]); sl->temp5_plim=(int16_t)unpack16(data[4],data[5]); break;
        case 0xC6U: sl->ex_temp0_plim=(int16_t)unpack16(data[0],data[1]); sl->ex_temp1_plim=(int16_t)unpack16(data[2],data[3]); break;
        case 0xC7U: sl->lidar_a_12v_lim=(int16_t)unpack16(data[0],data[1]); sl->lidar_b_12v_lim=(int16_t)unpack16(data[2],data[3]); break;
        case 0xC8U: sl->sto_24v_lim=(int16_t)unpack16(data[0],data[1]); sl->chg_hvil_out_lim=(int16_t)unpack16(data[2],data[3]); break;
        case 0xC9U: sl->eth_2v5_lim=(int16_t)unpack16(data[0],data[1]); sl->eth_1v2_lim=(int16_t)unpack16(data[2],data[3]); break;
        case 0xCAU: sl->ecat_3v3_lim=(int16_t)unpack16(data[0],data[1]); sl->ecat_1v2_lim=(int16_t)unpack16(data[2],data[3]); break;
        case 0xCBU: sl->v12_a_total_curr_lim=(int16_t)unpack16(data[0],data[1]); sl->v12_b_total_curr_lim=(int16_t)unpack16(data[2],data[3]); sl->v24_total_curr_lim=(int16_t)unpack16(data[4],data[5]); break;
        case 0xCCU: sl->batt1_curr_nlim=(int16_t)unpack16(data[2],data[3]); sl->batt2_curr_nlim=(int16_t)unpack16(data[4],data[5]); break;
        case 0xCDU: sl->temp0_nlim=(int16_t)unpack16(data[0],data[1]); sl->temp1_nlim=(int16_t)unpack16(data[2],data[3]); sl->temp2_nlim=(int16_t)unpack16(data[4],data[5]); break;
        case 0xCEU: sl->temp3_nlim=(int16_t)unpack16(data[0],data[1]); sl->temp4_nlim=(int16_t)unpack16(data[2],data[3]); sl->temp5_nlim=(int16_t)unpack16(data[4],data[5]); break;
        case 0xCFU: sl->ex_temp0_nlim=(int16_t)unpack16(data[0],data[1]); sl->ex_temp1_nlim=(int16_t)unpack16(data[2],data[3]); break;

    /* Fault registers: writing 1 clears the fault (rising-edge clear) */
        case 0xA8U: fr->reg1    &= ~data[0]; break;
        case 0xA9U: fr->reg2[0] &= ~data[0]; fr->reg2[1] &= ~data[1]; fr->reg2[2] &= ~data[2]; break;
        case 0xAAU: fr->reg3[0] &= ~data[0]; fr->reg3[1] &= ~data[1]; fr->reg3[2] &= ~data[2]; fr->reg3[3] &= ~data[3]; break;

    /* Limit fault flags: same clear-on-write-1 semantics */
        case 0xD0U: for (int i=0;i<6;i++) lf->soft_grp1[i] &= ~data[i]; break;
        case 0xD1U: lf->soft_grp2[0]&=~data[0]; lf->soft_grp2[1]&=~data[1]; lf->soft_grp2[2]&=~data[2]; break;
        case 0xD8U: for (int i=0;i<6;i++) lf->hard_grp1[i] &= ~data[i]; break;
        case 0xD9U: lf->hard_grp2[0]&=~data[0]; lf->hard_grp2[1]&=~data[1]; lf->hard_grp2[2]&=~data[2]; break;

    /* Build info */
        case 0x1F8U:
            g_regs.build_info.hw_rev = unpack16(data[0], data[1]);
            g_regs.build_info.fw_rev = unpack16(data[2], data[3]);
            break;

    /* Write protect */
        case 0x1FAU:
            g_regs.write_protect.b1 = data[0];
            g_regs.write_protect.b2 = data[1];
            g_regs.write_protect.b3 = data[2];
            g_regs.write_protect.b4 = data[3];
            g_regs.write_protect.b5 = data[4];
            break;

        default: break;
    }
}

/* =========================================================================
 * TRANSMIT RESPONSE
 * ========================================================================= */
static void transmit_response(uint16_t data_id, uint8_t priority,
                               uint8_t dst_addr, const uint8_t tx_data[8])
{
    uint32_t tx_id = PMB_CAN_BUILD_ID(priority, data_id, dst_addr, PMB_ADDR);

    XMC_CAN_MO_t *tx_mo = MULTICAN_CONFIG_0.lmobj_ptr[PMB_TX_MO]->mo_ptr;

    tx_mo->can_id_mode    = XMC_CAN_FRAME_TYPE_EXTENDED_29BITS;
    tx_mo->can_identifier = tx_id;
    tx_mo->can_id_mask    = 0x1FFFFFFFUL;
    tx_mo->can_ide_mask   = 1U;
    tx_mo->can_data_length = 8U;

    /* XMC MultiCAN stores CAN data as two uint32 words, little-endian within each word.
     * data[0] = Byte1 goes into the LSByte of can_data[0]. */
    tx_mo->can_data[0] = (uint32_t)tx_data[0]
                       | ((uint32_t)tx_data[1] << 8U)
                       | ((uint32_t)tx_data[2] << 16U)
                       | ((uint32_t)tx_data[3] << 24U);
    tx_mo->can_data[1] = (uint32_t)tx_data[4]
                       | ((uint32_t)tx_data[5] << 8U)
                       | ((uint32_t)tx_data[6] << 16U)
                       | ((uint32_t)tx_data[7] << 24U);

    XMC_CAN_MO_Config(tx_mo);
    XMC_CAN_MO_SetStatus(tx_mo, XMC_CAN_MO_SET_STATUS_TX_REQUEST);
}

/* =========================================================================
 * EU CALCULATION
 * Formula: EU = (raw - offset) * scale / 4095
 * ========================================================================= */
static inline int16_t calc_eu(uint16_t raw, int16_t ofs, int16_t scl)
{
    return (int16_t)(((int32_t)(raw - (uint16_t)ofs) * scl) / 4095);
}

void PMB_UpdateEU(void)
{
    PMB_Telemetry_t *t  = &g_regs.telemetry;
    PMB_CalibOffset_t *co = &g_regs.calib_offset;
    PMB_CalibScale_t  *cs = &g_regs.calib_scale;
    PMB_EU_t          *eu = &g_regs.eu;

    eu->chg_curr_eu         = calc_eu(t->chg_curr,        co->chg_curr_ofs,        cs->chg_curr_scl);
    eu->batt1_curr_eu       = calc_eu(t->batt1_curr,      co->batt1_curr_ofs,      cs->batt1_curr_scl);
    eu->batt2_curr_eu       = calc_eu(t->batt2_curr,      co->batt2_curr_ofs,      cs->batt2_curr_scl);
    eu->ll_imon_eu          = calc_eu(t->ll_imon,         co->ll_imon_ofs,         cs->ll_imon_scl);
    eu->rl_imon_eu          = calc_eu(t->rl_imon,         co->rl_imon_ofs,         cs->rl_imon_scl);
    eu->la_imon_eu          = calc_eu(t->la_imon,         co->la_imon_ofs,         cs->la_imon_scl);
    eu->ra_imon_eu          = calc_eu(t->ra_imon,         co->ra_imon_ofs,         cs->ra_imon_scl);
    eu->tr_imon_eu          = calc_eu(t->tr_imon,         co->tr_imon_ofs,         cs->tr_imon_scl);
    eu->chg_in_eu           = calc_eu(t->chg_in,          co->chg_in_ofs,          cs->chg_in_scl);
    eu->hv_eu               = calc_eu(t->hv,              co->hv_ofs,              cs->hv_scl);
    eu->batt1_in_eu         = calc_eu(t->batt1_in,        co->batt1_in_ofs,        cs->batt1_in_scl);
    eu->batt2_in_eu         = calc_eu(t->batt2_in,        co->batt2_in_ofs,        cs->batt2_in_scl);
    eu->nk_imon_eu          = calc_eu(t->nk_imon,         co->nk_imon_ofs,         cs->nk_imon_scl);
    eu->mot_bus_eu          = calc_eu(t->mot_bus,         co->mot_bus_ofs,         cs->mot_bus_scl);
    eu->chg_hv_eu           = calc_eu(t->chg_hv,          co->chg_hv_ofs,          cs->chg_hv_scl);
    eu->user_hv_eu          = calc_eu(t->user_hv,         co->user_hv_ofs,         cs->user_hv_scl);
    eu->user_hv_curr_eu     = calc_eu(t->user_hv_curr,    co->user_hv_curr_ofs,    cs->user_hv_curr_scl);
    eu->pc_24v_eu           = calc_eu(t->pc_24v,          co->pc_24v_ofs,          cs->pc_24v_scl);
    eu->pc_24v_imon_eu      = calc_eu(t->pc_24v_imon,     co->pc_24v_imon_ofs,     cs->pc_24v_imon_scl);
    eu->user_24v_eu         = calc_eu(t->user_24v,        co->user_24v_ofs,        cs->user_24v_scl);
    eu->user_24v_imon_eu    = calc_eu(t->user_24v_imon,   co->user_24v_imon_ofs,   cs->user_24v_imon_scl);
    eu->pc_a_12v_eu         = calc_eu(t->pc_a_12v,        co->pc_a_12v_ofs,        cs->pc_a_12v_scl);
    eu->pc_a_12v_imon_eu    = calc_eu(t->pc_a_12v_imon,   co->pc_a_12v_imon_ofs,   cs->pc_a_12v_imon_scl);
    eu->pc_b_12v_eu         = calc_eu(t->pc_b_12v,        co->pc_b_12v_ofs,        cs->pc_b_12v_scl);
    eu->pc_b_12v_imon_eu    = calc_eu(t->pc_b_12v_imon,   co->pc_b_12v_imon_ofs,   cs->pc_b_12v_imon_scl);
    eu->v24_imon0_eu        = calc_eu(t->v24_imon0,       co->v24_imon0_ofs,       cs->v24_imon0_scl);
    eu->v24_imon1_eu        = calc_eu(t->v24_imon1,       co->v24_imon1_ofs,       cs->v24_imon1_scl);
    eu->v24_imon2_eu        = calc_eu(t->v24_imon2,       co->v24_imon2_ofs,       cs->v24_imon2_scl);
    eu->v12_imon0_eu        = calc_eu(t->v12_imon0,       co->v12_imon0_ofs,       cs->v12_imon0_scl);
    eu->v12_imon1_eu        = calc_eu(t->v12_imon1,       co->v12_imon1_ofs,       cs->v12_imon1_scl);
    eu->v12_imon2_eu        = calc_eu(t->v12_imon2,       co->v12_imon2_ofs,       cs->v12_imon2_scl);
    eu->v24_eu              = calc_eu(t->v24_bus_voltage,  co->v24_ofs,             cs->v24_scl);
    eu->v12_eu              = calc_eu(t->v12_bus_a_voltage,co->v12_ofs,             cs->v12_scl);
    eu->v5_eu               = calc_eu(t->v5,              co->v5_ofs,              cs->v5_scl);
    eu->v5_curr_eu          = calc_eu(t->v5_curr,         co->v5_curr_ofs,         cs->v5_curr_scl);
    eu->v3v3_eu             = calc_eu(t->v3v3,            co->v3v3_ofs,            cs->v3v3_scl);
    eu->v3v3_curr_eu        = calc_eu(t->v3v3_curr,       co->v3v3_curr_ofs,       cs->v3v3_curr_scl);
    eu->aux_24v_eu          = calc_eu(t->aux_24v,         co->aux_24v_ofs,         cs->aux_24v_scl);
    eu->aux_24v_curr_eu     = calc_eu(t->aux_24v_curr,    co->aux_24v_curr_ofs,    cs->aux_24v_curr_scl);
    eu->aux_12v_eu          = calc_eu(t->aux_12v,         co->aux_12v_ofs,         cs->aux_12v_scl);
    eu->aux_12v_curr_eu     = calc_eu(t->aux_12v_curr,    co->aux_12v_curr_ofs,    cs->aux_12v_curr_scl);
    eu->estop_rec_a_12v_eu  = calc_eu(t->estop_rec_a_12v, co->estop_rec_a_12v_ofs, cs->estop_rec_a_12v_scl);
    eu->estop_rec_b_12v_eu  = calc_eu(t->estop_rec_b_12v, co->estop_rec_b_12v_ofs, cs->estop_rec_b_12v_scl);
    eu->v12_b_imon3_eu      = calc_eu(t->v12_b_imon3,    co->v12_b_imon3_ofs,     cs->v12_b_imon3_scl);
    eu->v12_b_imon4_eu      = calc_eu(t->v12_b_imon4,    co->v12_b_imon4_ofs,     cs->v12_b_imon4_scl);
    eu->hv_or_eu            = calc_eu(t->hv_or,           co->hv_or_ofs,           cs->hv_or_scl);
    eu->mb_sp_imon_eu       = calc_eu(t->mb_sp_imon,      co->mb_sp_imon_ofs,      cs->mb_sp_imon_scl);
    eu->temp0_eu            = calc_eu(t->temp0,           co->temp0_ofs,           cs->temp0_scl);
    eu->temp1_eu            = calc_eu(t->temp1,           co->temp1_ofs,           cs->temp1_scl);
    eu->temp2_eu            = calc_eu(t->temp2,           co->temp2_ofs,           cs->temp2_scl);
    eu->temp3_eu            = calc_eu(t->temp3,           co->temp3_ofs,           cs->temp3_scl);
    eu->temp4_eu            = calc_eu(t->temp4,           co->temp4_ofs,           cs->temp4_scl);
    eu->temp5_eu            = calc_eu(t->temp5,           co->temp5_ofs,           cs->temp5_scl);
    eu->ex_temp0_eu         = calc_eu(t->ex_temp0,        co->ex_temp0_ofs,        cs->ex_temp0_scl);
    eu->ex_temp1_eu         = calc_eu(t->ex_temp1,        co->ex_temp1_ofs,        cs->ex_temp1_scl);
    eu->lidar_a_12v_eu      = calc_eu(t->lidar_a_12v,    co->lidar_a_12v_ofs,     cs->lidar_a_12v_scl);
    eu->lidar_b_12v_eu      = calc_eu(t->lidar_b_12v,    co->lidar_b_12v_ofs,     cs->lidar_b_12v_scl);
    eu->sto_24v_eu          = calc_eu(t->sto_24v,         co->sto_24v_ofs,         cs->sto_24v_scl);
    eu->chg_hvil_out_eu     = calc_eu(t->chg_hvil_out,   co->chg_hvil_out_ofs,    cs->chg_hvil_out_scl);
    eu->eth_2v5_eu          = calc_eu(t->eth_2v5,         co->eth_2v5_ofs,         cs->eth_2v5_scl);
    eu->eth_1v2_eu          = calc_eu(t->eth_1v2,         co->eth_1v2_ofs,         cs->eth_1v2_scl);
    eu->ecat_3v3_eu         = calc_eu(t->ecat_3v3,        co->ecat_3v3_ofs,        cs->ecat_3v3_scl);
    eu->ecat_1v2_eu         = calc_eu(t->ecat_1v2,        co->ecat_1v2_ofs,        cs->ecat_1v2_scl);

    /* Derived: totals */
    eu->v12_a_total_curr_eu = (int16_t)((int32_t)eu->v12_imon0_eu + eu->v12_imon1_eu + eu->v12_imon2_eu);
    eu->v12_b_total_curr_eu = (int16_t)((int32_t)eu->v12_b_imon3_eu + eu->v12_b_imon4_eu);
    eu->v24_total_curr_eu   = (int16_t)((int32_t)eu->v24_imon0_eu + eu->v24_imon1_eu + eu->v24_imon2_eu);
    eu->batt_total_curr_eu  = (int16_t)((int32_t)eu->batt1_curr_eu + eu->batt2_curr_eu);
}

/* =========================================================================
 * PUBLIC API
 * ========================================================================= */

void PMB_Emulator_Init(void)
{
    memset(&g_regs, 0, sizeof(g_regs));

    g_regs.build_info.hw_rev = 0x0001U;
    g_regs.build_info.fw_rev = 0x0001U;

    /* IO test pins per spec:
     *   0x1C Byte3: IO0_P4.1=1 (3.3V supply), IO0_P4.0=0 (DGND)  → b3 = 0x02
     *   0x1E Byte2: IO1_P4.1=0 (DGND),        IO1_P4.0=1 (3.3V)  → b2 = 0x01  */
    g_regs.status.pdg_fault_grp1[2] = 0x02U;
    g_regs.status.pdg_fault_grp3[1] = 0x01U;

    /* Configure RX MO: accept all extended frames addressed to PMB (dst = 0xC0).
     * Mask checks only the destination field [15:8]. */
    XMC_CAN_MO_t *rx = MULTICAN_CONFIG_0.lmobj_ptr[PMB_RX_MO]->mo_ptr;
    rx->can_mo_type    = XMC_CAN_MO_TYPE_RECMSGOBJ;
    rx->can_id_mode    = XMC_CAN_FRAME_TYPE_EXTENDED_29BITS;
    rx->can_identifier = PMB_CAN_BUILD_ID(0U, 0U, PMB_ADDR, 0U);
    rx->can_id_mask    = (uint32_t)(0xFFUL << 8U);   /* check only dst field */
    rx->can_ide_mask   = 1U;
    rx->can_data_length = 8U;
    XMC_CAN_MO_Config(rx);

    /* Configure TX MO */
    XMC_CAN_MO_t *tx = MULTICAN_CONFIG_0.lmobj_ptr[PMB_TX_MO]->mo_ptr;
    tx->can_mo_type    = XMC_CAN_MO_TYPE_TRANSMSGOBJ;
    tx->can_id_mode    = XMC_CAN_FRAME_TYPE_EXTENDED_29BITS;
    tx->can_identifier = PMB_CAN_BUILD_ID(4U, 0U, FIXTURE_ADDR, PMB_ADDR);
    tx->can_id_mask    = 0x1FFFFFFFUL;
    tx->can_ide_mask   = 1U;
    tx->can_data_length = 8U;
    XMC_CAN_MO_Config(tx);
}

void PMB_Emulator_Run(void)
{
    XMC_CAN_MO_t *rx = MULTICAN_CONFIG_0.lmobj_ptr[PMB_RX_MO]->mo_ptr;

    /* Check NEWDAT/RXUPD flag */
    uint32_t status = XMC_CAN_MO_GetStatus(rx);
    bool new_frame  = ((status & CAN_MO_MOSTAT_NEWDAT_Msk) != 0U) ||
                      ((status & CAN_MO_MOSTAT_RXUPD_Msk)  != 0U);
    if (!new_frame) return;

    /* Extract received data (XMC stores bytes LE within each 32-bit word) */
    uint8_t rx_data[8];
    rx_data[0] = (uint8_t)(rx->can_data[0]        & 0xFFU);
    rx_data[1] = (uint8_t)((rx->can_data[0] >>  8U) & 0xFFU);
    rx_data[2] = (uint8_t)((rx->can_data[0] >> 16U) & 0xFFU);
    rx_data[3] = (uint8_t)((rx->can_data[0] >> 24U) & 0xFFU);
    rx_data[4] = (uint8_t)(rx->can_data[1]        & 0xFFU);
    rx_data[5] = (uint8_t)((rx->can_data[1] >>  8U) & 0xFFU);
    rx_data[6] = (uint8_t)((rx->can_data[1] >> 16U) & 0xFFU);
    rx_data[7] = (uint8_t)((rx->can_data[1] >> 24U) & 0xFFU);

    XMC_CAN_MO_ResetStatus(rx, XMC_CAN_MO_RESET_STATUS_RX_PENDING);

    uint32_t rx_id   = rx->can_identifier;
    uint16_t data_id = (uint16_t)PMB_GET_DATA_ID(rx_id);
    uint8_t  src     = (uint8_t)PMB_GET_SRC(rx_id);
    uint8_t  prio    = (uint8_t)PMB_GET_PRIORITY(rx_id);

    /* Distinguish read vs write:
     * A frame with all payload bytes zero is a read request.
     * Any non-zero payload byte triggers a write. */
    bool is_write = false;
    for (int i = 0; i < 6; i++) { /* only check bytes 1-6; bytes 7-8 are counter */
        if (rx_data[i] != 0U) { is_write = true; break; }
    }

    if (is_write) {
        process_write(data_id, rx_data);
        PMB_UpdateEU(); /* Recalculate EU after any offset/scale write */
    }

    uint8_t tx_data[8];
    build_response(data_id, tx_data);
    transmit_response(data_id, prio, src, tx_data);
}

PMB_Registers_t *PMB_GetRegs(void)
{
    return &g_regs;
}
