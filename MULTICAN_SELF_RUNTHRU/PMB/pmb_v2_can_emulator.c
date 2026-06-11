/*******************************************************************************
 * pmb_v2_can_emulator.c
 * PMB CAN Protocol v2 emulator — implementation
 ******************************************************************************/
#include "pmb_v2_can_emulator.h"
#include <string.h>

/* ---- Single register file ---- */
static PMB_Registers_t g_regs;

/* ---- Packet counter (echoed back from incoming frame) ---- */
static uint8_t s_pkt_hi;
static uint8_t s_pkt_lo;

/* ============================================================
 * Bit-packing helpers
 * ============================================================ */

/* 12-bit unsigned: lo = val[7:0], hi = {4b0000, val[11:8]} */
static void pack12(uint8_t *lo, uint8_t *hi, uint16_t val)
{
    *lo = (uint8_t)(val & 0xFFU);
    *hi = (uint8_t)((val >> 8U) & 0x0FU);
}

// static uint16_t unpack12(uint8_t lo, uint8_t hi)
// {
//     return (uint16_t)((uint16_t)(hi & 0x0FU) << 8U) | lo;
// }

/* 16-bit signed little-endian */
static void pack16(uint8_t *lo, uint8_t *hi, uint16_t val)
{
    *lo = (uint8_t)(val & 0xFFU);
    *hi = (uint8_t)((val >> 8U) & 0xFFU);
}

static uint16_t unpack16(uint8_t lo, uint8_t hi)
{
    return (uint16_t)((uint16_t)hi << 8U) | lo;
}

/* Even parity over bits[7:1]; result placed in bit0 */
static uint8_t even_parity(uint8_t byte)
{
    uint8_t p = byte >> 1U;
    p ^= p >> 4U;
    p ^= p >> 2U;
    p ^= p >> 1U;
    return (byte & 0xFEU) | (p & 0x01U);
}

/* ============================================================
 * Write-protect check
 * bit0=LIM, bit1=OFS, bit2=SCL, bit3=REV (in write_protect.b1)
 * ============================================================ */
static int write_allowed(uint16_t data_id)
{
    uint8_t wp = g_regs.write_protect.b1;
    if (data_id >= 0x30U && data_id <= 0x4AU) return !(wp & 0x02U); /* OFS */
    if (data_id >= 0x50U && data_id <= 0x6AU) return !(wp & 0x04U); /* SCL */
    if (data_id >= 0xB0U && data_id <= 0xCFU) return !(wp & 0x01U); /* LIM */
    if (data_id == 0x1F8U)                    return !(wp & 0x08U); /* REV */
    return 1; /* not protected */
}

/* ============================================================
 * EU calculation:  EU = (raw - ofs) * scl / 4095
 * ============================================================ */
static int16_t calc_eu(uint16_t raw, int16_t ofs, int16_t scl)
{
    int32_t v = ((int32_t)raw - ofs) * scl;
    return (int16_t)(v / 4095);
}

/* ============================================================
 * build_response() — pack g_regs fields into data[0..5]
 * data[6] and data[7] are the echoed packet counter (set by caller)
 * ============================================================ */
static void build_response(uint16_t data_id, uint8_t data[8])
{
    PMB_Telemetry_t  *t = &g_regs.telem;
    PMB_Registers_t  *r = &g_regs;

    switch (data_id) {

        /* === 12-BIT TELEMETRY (0x00-0x1A) === */
        case 0x00U: /* Telemetry Group 1 */
            pack12(&data[0], &data[1], t->chg_curr);
            pack12(&data[2], &data[3], t->batt1_curr);
            pack12(&data[4], &data[5], t->batt2_curr);
            break;
        case 0x01U: /* Telemetry Group 2 */
            pack12(&data[0], &data[1], t->ll_imon);
            pack12(&data[2], &data[3], t->rl_imon);
            pack12(&data[4], &data[5], t->la_imon);
            break;
        case 0x02U: /* Telemetry Group 3 */
            pack12(&data[0], &data[1], t->ra_imon);
            pack12(&data[2], &data[3], t->tr_imon);
            pack12(&data[4], &data[5], t->chg_in);
            break;
        case 0x03U: /* Telemetry Group 4 */
            pack12(&data[0], &data[1], t->hv);
            pack12(&data[2], &data[3], t->batt1_in);
            pack12(&data[4], &data[5], t->batt2_in);
            break;
        case 0x04U: /* Telemetry Group 5 */
            pack12(&data[0], &data[1], t->nk_imon);
            pack12(&data[2], &data[3], t->mot_bus);
            pack12(&data[4], &data[5], t->chg_hv);
            break;
        case 0x05U: /* Telemetry Group 6 */
            pack12(&data[0], &data[1], t->user_hv);
            pack12(&data[2], &data[3], t->user_hv_curr);
            break;
        case 0x06U: /* Telemetry Group 7 */
            pack12(&data[0], &data[1], t->pc_24v);
            pack12(&data[2], &data[3], t->pc_24v_imon);
            break;
        case 0x07U: /* Telemetry Group 8 */
            pack12(&data[0], &data[1], t->user_24v);
            pack12(&data[2], &data[3], t->user_24v_imon);
            break;
        case 0x08U: /* Telemetry Group 9 */
            pack12(&data[0], &data[1], t->pc_a_12v);
            pack12(&data[2], &data[3], t->pc_a_12v_imon);
            break;
        case 0x09U: /* Telemetry Group 10 */
            pack12(&data[0], &data[1], t->pc_b_12v);
            pack12(&data[2], &data[3], t->pc_b_12v_imon);
            break;
        case 0x0AU: /* Telemetry Group 11 */
            pack12(&data[0], &data[1], t->v24_imon0);
            pack12(&data[2], &data[3], t->v24_imon1);
            pack12(&data[4], &data[5], t->v24_imon2);
            break;
        case 0x0BU: /* Telemetry Group 12 */
            pack12(&data[0], &data[1], t->v12_imon0);
            pack12(&data[2], &data[3], t->v12_imon1);
            pack12(&data[4], &data[5], t->v12_imon2);
            break;
        case 0x0CU: /* Telemetry Group 13 */
            pack12(&data[0], &data[1], t->v24_bus_voltage);
            pack12(&data[2], &data[3], t->v12_bus_a_voltage);
            break;
        case 0x0DU: /* Telemetry Group 14 */
            pack12(&data[0], &data[1], t->v5);
            pack12(&data[2], &data[3], t->v5_curr);
            break;
        case 0x0EU: /* Telemetry Group 15 */
            pack12(&data[0], &data[1], t->v3v3);
            pack12(&data[2], &data[3], t->v3v3_curr);
            break;
        case 0x0FU: /* Telemetry Group 16 */
            pack12(&data[0], &data[1], t->aux_24v);
            pack12(&data[2], &data[3], t->aux_24v_curr);
            break;
        case 0x10U: /* Telemetry Group 17 */
            pack12(&data[0], &data[1], t->aux_12v);
            pack12(&data[2], &data[3], t->aux_12v_curr);
            break;
        case 0x11U: /* Telemetry Group 18 */
            pack12(&data[0], &data[1], t->estop_rec_a_12v);
            pack12(&data[2], &data[3], t->estop_rec_b_12v);
            break;
        case 0x12U: /* Telemetry Group 19 */
            pack12(&data[0], &data[1], t->v12_b_imon3);
            pack12(&data[2], &data[3], t->v12_b_imon4);
            break;
        case 0x13U: /* Telemetry Group 20 */
            pack12(&data[0], &data[1], t->hv_or);
            pack12(&data[2], &data[3], t->mb_sp_imon);
            break;
        case 0x14U: /* Telemetry Group 21 */
            pack12(&data[0], &data[1], t->temp0);
            pack12(&data[2], &data[3], t->temp1);
            pack12(&data[4], &data[5], t->temp2);
            break;
        case 0x15U: /* Telemetry Group 22 */
            pack12(&data[0], &data[1], t->temp3);
            pack12(&data[2], &data[3], t->temp4);
            pack12(&data[4], &data[5], t->temp5);
            break;
        case 0x16U: /* Telemetry Group 23 */
            pack12(&data[0], &data[1], t->ex_temp0);
            pack12(&data[2], &data[3], t->ex_temp1);
            break;
        case 0x17U: /* Telemetry Group 24 */
            pack12(&data[0], &data[1], t->lidar_a_12v);
            pack12(&data[2], &data[3], t->lidar_b_12v);
            break;
        case 0x18U: /* Telemetry Group 25 */
            pack12(&data[0], &data[1], t->sto_24v);
            pack12(&data[2], &data[3], t->chg_hvil_out);
            break;
        case 0x19U: /* Telemetry Group 26 */
            pack12(&data[0], &data[1], t->eth_2v5);
            pack12(&data[2], &data[3], t->eth_1v2);
            break;
        case 0x1AU: /* Telemetry Group 27 */
            pack12(&data[0], &data[1], t->ecat_3v3);
            pack12(&data[2], &data[3], t->ecat_1v2);
            break;

        /* === STATUS BIT-MAPPED (0x1B-0x1E) === */
        case 0x1BU: /* COMP_PDG */
            { PMB_Status_t *st = &g_regs.status;
              data[0] = st->comp_pdg[0];
              data[1] = st->comp_pdg[1];
              data[2] = st->comp_pdg[2];
            } break;
        case 0x1CU: /* PDG_FAULT_GRP1 */
            { PMB_Status_t *st = &g_regs.status;
              data[0] = st->pdg_fault_grp1[0];
              data[1] = st->pdg_fault_grp1[1];
              data[2] = st->pdg_fault_grp1[2];
            } break;
        case 0x1DU: /* PDG_FAULT_GRP2 */
            { PMB_Status_t *st = &g_regs.status;
              data[0] = st->pdg_fault_grp2[0];
              data[1] = st->pdg_fault_grp2[1];
            } break;
        case 0x1EU: /* PDG_FAULT_GRP3 */
            { PMB_Status_t *st = &g_regs.status;
              data[0] = st->pdg_fault_grp3[0];
              data[1] = st->pdg_fault_grp3[1];
            } break;

        /* === POWER ENABLE (0x1F-0x23) === */
        case 0x1FU:
            { PMB_PowerEnable_t *pe = &g_regs.pwr_enable;
              data[0] = pe->grp1;
            } break;
        case 0x20U:
            { PMB_PowerEnable_t *pe = &g_regs.pwr_enable;
              data[0] = pe->grp2[0];
              data[1] = pe->grp2[1];
              data[2] = pe->grp2[2];
            } break;
        case 0x21U:
            { PMB_PowerEnable_t *pe = &g_regs.pwr_enable;
              data[0] = pe->grp3[0];
              data[1] = pe->grp3[1];
              data[2] = pe->grp3[2];
              data[3] = pe->grp3[3];
            } break;
        case 0x22U:
            { PMB_PowerEnable_t *pe = &g_regs.pwr_enable;
              data[0] = pe->periph_inputs;
            } break;
        case 0x23U:
            { PMB_PowerEnable_t *pe = &g_regs.pwr_enable;
              data[0] = pe->grp4[0];
              data[1] = pe->grp4[1];
              data[2] = pe->grp4[2];
            } break;

        /* === 16-BIT OFFSETS (0x30-0x4A) === */
        case 0x30U: pack16(&data[0],&data[1],(uint16_t)r->calib_offset.chg_curr_ofs);   pack16(&data[2],&data[3],(uint16_t)r->calib_offset.batt1_curr_ofs);   pack16(&data[4],&data[5],(uint16_t)r->calib_offset.batt2_curr_ofs);   break;
        case 0x31U: pack16(&data[0],&data[1],(uint16_t)r->calib_offset.ll_imon_ofs);    pack16(&data[2],&data[3],(uint16_t)r->calib_offset.rl_imon_ofs);    pack16(&data[4],&data[5],(uint16_t)r->calib_offset.la_imon_ofs);    break;
        case 0x32U: pack16(&data[0],&data[1],(uint16_t)r->calib_offset.ra_imon_ofs);    pack16(&data[2],&data[3],(uint16_t)r->calib_offset.tr_imon_ofs);    pack16(&data[4],&data[5],(uint16_t)r->calib_offset.chg_in_ofs);    break;
        case 0x33U: pack16(&data[0],&data[1],(uint16_t)r->calib_offset.hv_ofs);         pack16(&data[2],&data[3],(uint16_t)r->calib_offset.batt1_in_ofs);   pack16(&data[4],&data[5],(uint16_t)r->calib_offset.batt2_in_ofs);   break;
        case 0x34U: pack16(&data[0],&data[1],(uint16_t)r->calib_offset.nk_imon_ofs);    pack16(&data[2],&data[3],(uint16_t)r->calib_offset.mot_bus_ofs);    pack16(&data[4],&data[5],(uint16_t)r->calib_offset.chg_hv_ofs);    break;
        case 0x35U: pack16(&data[0],&data[1],(uint16_t)r->calib_offset.user_hv_ofs);    pack16(&data[2],&data[3],(uint16_t)r->calib_offset.user_hv_curr_ofs); break;
        case 0x36U: pack16(&data[0],&data[1],(uint16_t)r->calib_offset.pc_24v_ofs);     pack16(&data[2],&data[3],(uint16_t)r->calib_offset.pc_24v_imon_ofs); break;
        case 0x37U: pack16(&data[0],&data[1],(uint16_t)r->calib_offset.user_24v_ofs);   pack16(&data[2],&data[3],(uint16_t)r->calib_offset.user_24v_imon_ofs); break;
        case 0x38U: pack16(&data[0],&data[1],(uint16_t)r->calib_offset.pc_a_12v_ofs);   pack16(&data[2],&data[3],(uint16_t)r->calib_offset.pc_a_12v_imon_ofs); break;
        case 0x39U: pack16(&data[0],&data[1],(uint16_t)r->calib_offset.pc_b_12v_ofs);   pack16(&data[2],&data[3],(uint16_t)r->calib_offset.pc_b_12v_imon_ofs); break;
        case 0x3AU: pack16(&data[0],&data[1],(uint16_t)r->calib_offset.v24_imon0_ofs);  pack16(&data[2],&data[3],(uint16_t)r->calib_offset.v24_imon1_ofs);  pack16(&data[4],&data[5],(uint16_t)r->calib_offset.v24_imon2_ofs);  break;
        case 0x3BU: pack16(&data[0],&data[1],(uint16_t)r->calib_offset.v12_imon0_ofs);  pack16(&data[2],&data[3],(uint16_t)r->calib_offset.v12_imon1_ofs);  pack16(&data[4],&data[5],(uint16_t)r->calib_offset.v12_imon2_ofs);  break;
        case 0x3CU: pack16(&data[0],&data[1],(uint16_t)r->calib_offset.v24_ofs);        pack16(&data[2],&data[3],(uint16_t)r->calib_offset.v12_ofs);        break;
        case 0x3DU: pack16(&data[0],&data[1],(uint16_t)r->calib_offset.v5_ofs);         pack16(&data[2],&data[3],(uint16_t)r->calib_offset.v5_curr_ofs);    break;
        case 0x3EU: pack16(&data[0],&data[1],(uint16_t)r->calib_offset.v3v3_ofs);       pack16(&data[2],&data[3],(uint16_t)r->calib_offset.v3v3_curr_ofs);  break;
        case 0x3FU: pack16(&data[0],&data[1],(uint16_t)r->calib_offset.aux_24v_ofs);    pack16(&data[2],&data[3],(uint16_t)r->calib_offset.aux_24v_curr_ofs); break;
        case 0x40U: pack16(&data[0],&data[1],(uint16_t)r->calib_offset.aux_12v_ofs);    pack16(&data[2],&data[3],(uint16_t)r->calib_offset.aux_12v_curr_ofs); break;
        case 0x41U: pack16(&data[0],&data[1],(uint16_t)r->calib_offset.estop_rec_a_12v_ofs); pack16(&data[2],&data[3],(uint16_t)r->calib_offset.estop_rec_b_12v_ofs); break;
        case 0x42U: pack16(&data[0],&data[1],(uint16_t)r->calib_offset.v12_b_imon3_ofs); pack16(&data[2],&data[3],(uint16_t)r->calib_offset.v12_b_imon4_ofs); break;
        case 0x43U: pack16(&data[0],&data[1],(uint16_t)r->calib_offset.hv_or_ofs);      pack16(&data[2],&data[3],(uint16_t)r->calib_offset.mb_sp_imon_ofs); break;
        case 0x44U: pack16(&data[0],&data[1],(uint16_t)r->calib_offset.temp0_ofs);      pack16(&data[2],&data[3],(uint16_t)r->calib_offset.temp1_ofs);      pack16(&data[4],&data[5],(uint16_t)r->calib_offset.temp2_ofs);      break;
        case 0x45U: pack16(&data[0],&data[1],(uint16_t)r->calib_offset.temp3_ofs);      pack16(&data[2],&data[3],(uint16_t)r->calib_offset.temp4_ofs);      pack16(&data[4],&data[5],(uint16_t)r->calib_offset.temp5_ofs);      break;
        case 0x46U: pack16(&data[0],&data[1],(uint16_t)r->calib_offset.ex_temp0_ofs);   pack16(&data[2],&data[3],(uint16_t)r->calib_offset.ex_temp1_ofs);   break;
        case 0x47U: pack16(&data[0],&data[1],(uint16_t)r->calib_offset.lidar_a_12v_ofs); pack16(&data[2],&data[3],(uint16_t)r->calib_offset.lidar_b_12v_ofs); break;
        case 0x48U: pack16(&data[0],&data[1],(uint16_t)r->calib_offset.sto_24v_ofs);    pack16(&data[2],&data[3],(uint16_t)r->calib_offset.chg_hvil_out_ofs); break;
        case 0x49U: pack16(&data[0],&data[1],(uint16_t)r->calib_offset.eth_2v5_ofs);    pack16(&data[2],&data[3],(uint16_t)r->calib_offset.eth_1v2_ofs);    break;
        case 0x4AU: pack16(&data[0],&data[1],(uint16_t)r->calib_offset.ecat_3v3_ofs);   pack16(&data[2],&data[3],(uint16_t)r->calib_offset.ecat_1v2_ofs);   break;

        /* === 16-BIT SCALES (0x50-0x6A) === */
        case 0x50U: pack16(&data[0],&data[1],(uint16_t)r->calib_scale.chg_curr_scl);   pack16(&data[2],&data[3],(uint16_t)r->calib_scale.batt1_curr_scl);   pack16(&data[4],&data[5],(uint16_t)r->calib_scale.batt2_curr_scl);   break;
        case 0x51U: pack16(&data[0],&data[1],(uint16_t)r->calib_scale.ll_imon_scl);    pack16(&data[2],&data[3],(uint16_t)r->calib_scale.rl_imon_scl);    pack16(&data[4],&data[5],(uint16_t)r->calib_scale.la_imon_scl);    break;
        case 0x52U: pack16(&data[0],&data[1],(uint16_t)r->calib_scale.ra_imon_scl);    pack16(&data[2],&data[3],(uint16_t)r->calib_scale.tr_imon_scl);    pack16(&data[4],&data[5],(uint16_t)r->calib_scale.chg_in_scl);    break;
        case 0x53U: pack16(&data[0],&data[1],(uint16_t)r->calib_scale.hv_scl);         pack16(&data[2],&data[3],(uint16_t)r->calib_scale.batt1_in_scl);   pack16(&data[4],&data[5],(uint16_t)r->calib_scale.batt2_in_scl);   break;
        case 0x54U: pack16(&data[0],&data[1],(uint16_t)r->calib_scale.nk_imon_scl);    pack16(&data[2],&data[3],(uint16_t)r->calib_scale.mot_bus_scl);    pack16(&data[4],&data[5],(uint16_t)r->calib_scale.chg_hv_scl);    break;
        case 0x55U: pack16(&data[0],&data[1],(uint16_t)r->calib_scale.user_hv_scl);    pack16(&data[2],&data[3],(uint16_t)r->calib_scale.user_hv_curr_scl); break;
        case 0x56U: pack16(&data[0],&data[1],(uint16_t)r->calib_scale.pc_24v_scl);     pack16(&data[2],&data[3],(uint16_t)r->calib_scale.pc_24v_imon_scl); break;
        case 0x57U: pack16(&data[0],&data[1],(uint16_t)r->calib_scale.user_24v_scl);   pack16(&data[2],&data[3],(uint16_t)r->calib_scale.user_24v_imon_scl); break;
        case 0x58U: pack16(&data[0],&data[1],(uint16_t)r->calib_scale.pc_a_12v_scl);   pack16(&data[2],&data[3],(uint16_t)r->calib_scale.pc_a_12v_imon_scl); break;
        case 0x59U: pack16(&data[0],&data[1],(uint16_t)r->calib_scale.pc_b_12v_scl);   pack16(&data[2],&data[3],(uint16_t)r->calib_scale.pc_b_12v_imon_scl); break;
        case 0x5AU: pack16(&data[0],&data[1],(uint16_t)r->calib_scale.v24_imon0_scl);  pack16(&data[2],&data[3],(uint16_t)r->calib_scale.v24_imon1_scl);  pack16(&data[4],&data[5],(uint16_t)r->calib_scale.v24_imon2_scl);  break;
        case 0x5BU: pack16(&data[0],&data[1],(uint16_t)r->calib_scale.v12_imon0_scl);  pack16(&data[2],&data[3],(uint16_t)r->calib_scale.v12_imon1_scl);  pack16(&data[4],&data[5],(uint16_t)r->calib_scale.v12_imon2_scl);  break;
        case 0x5CU: pack16(&data[0],&data[1],(uint16_t)r->calib_scale.v24_scl);        pack16(&data[2],&data[3],(uint16_t)r->calib_scale.v12_scl);        break;
        case 0x5DU: pack16(&data[0],&data[1],(uint16_t)r->calib_scale.v5_scl);         pack16(&data[2],&data[3],(uint16_t)r->calib_scale.v5_curr_scl);    break;
        case 0x5EU: pack16(&data[0],&data[1],(uint16_t)r->calib_scale.v3v3_scl);       pack16(&data[2],&data[3],(uint16_t)r->calib_scale.v3v3_curr_scl);  break;
        case 0x5FU: pack16(&data[0],&data[1],(uint16_t)r->calib_scale.aux_24v_scl);    pack16(&data[2],&data[3],(uint16_t)r->calib_scale.aux_24v_curr_scl); break;
        case 0x60U: pack16(&data[0],&data[1],(uint16_t)r->calib_scale.aux_12v_scl);    pack16(&data[2],&data[3],(uint16_t)r->calib_scale.aux_12v_curr_scl); break;
        case 0x61U: pack16(&data[0],&data[1],(uint16_t)r->calib_scale.estop_rec_a_12v_scl); pack16(&data[2],&data[3],(uint16_t)r->calib_scale.estop_rec_b_12v_scl); break;
        case 0x62U: pack16(&data[0],&data[1],(uint16_t)r->calib_scale.v12_b_imon3_scl); pack16(&data[2],&data[3],(uint16_t)r->calib_scale.v12_b_imon4_scl); break;
        case 0x63U: pack16(&data[0],&data[1],(uint16_t)r->calib_scale.hv_or_scl);      pack16(&data[2],&data[3],(uint16_t)r->calib_scale.mb_sp_imon_scl); break;
        case 0x64U: pack16(&data[0],&data[1],(uint16_t)r->calib_scale.temp0_scl);      pack16(&data[2],&data[3],(uint16_t)r->calib_scale.temp1_scl);      pack16(&data[4],&data[5],(uint16_t)r->calib_scale.temp2_scl);      break;
        case 0x65U: pack16(&data[0],&data[1],(uint16_t)r->calib_scale.temp3_scl);      pack16(&data[2],&data[3],(uint16_t)r->calib_scale.temp4_scl);      pack16(&data[4],&data[5],(uint16_t)r->calib_scale.temp5_scl);      break;
        case 0x66U: pack16(&data[0],&data[1],(uint16_t)r->calib_scale.ex_temp0_scl);   pack16(&data[2],&data[3],(uint16_t)r->calib_scale.ex_temp1_scl);   break;
        case 0x67U: pack16(&data[0],&data[1],(uint16_t)r->calib_scale.lidar_a_12v_scl); pack16(&data[2],&data[3],(uint16_t)r->calib_scale.lidar_b_12v_scl); break;
        case 0x68U: pack16(&data[0],&data[1],(uint16_t)r->calib_scale.sto_24v_scl);    pack16(&data[2],&data[3],(uint16_t)r->calib_scale.chg_hvil_out_scl); break;
        case 0x69U: pack16(&data[0],&data[1],(uint16_t)r->calib_scale.eth_2v5_scl);    pack16(&data[2],&data[3],(uint16_t)r->calib_scale.eth_1v2_scl);    break;
        case 0x6AU: pack16(&data[0],&data[1],(uint16_t)r->calib_scale.ecat_3v3_scl);   pack16(&data[2],&data[3],(uint16_t)r->calib_scale.ecat_1v2_scl);   break;

        /* === 16-BIT EU (0x70-0x8A) === */
        case 0x70U: pack16(&data[0],&data[1],(uint16_t)r->eu.chg_curr_eu);   pack16(&data[2],&data[3],(uint16_t)r->eu.batt1_curr_eu);   pack16(&data[4],&data[5],(uint16_t)r->eu.batt2_curr_eu);   break;
        case 0x71U: pack16(&data[0],&data[1],(uint16_t)r->eu.ll_imon_eu);    pack16(&data[2],&data[3],(uint16_t)r->eu.rl_imon_eu);    pack16(&data[4],&data[5],(uint16_t)r->eu.la_imon_eu);    break;
        case 0x72U: pack16(&data[0],&data[1],(uint16_t)r->eu.ra_imon_eu);    pack16(&data[2],&data[3],(uint16_t)r->eu.tr_imon_eu);    pack16(&data[4],&data[5],(uint16_t)r->eu.chg_in_eu);    break;
        case 0x73U: pack16(&data[0],&data[1],(uint16_t)r->eu.hv_eu);         pack16(&data[2],&data[3],(uint16_t)r->eu.batt1_in_eu);   pack16(&data[4],&data[5],(uint16_t)r->eu.batt2_in_eu);   break;
        case 0x74U: pack16(&data[0],&data[1],(uint16_t)r->eu.nk_imon_eu);    pack16(&data[2],&data[3],(uint16_t)r->eu.mot_bus_eu);    pack16(&data[4],&data[5],(uint16_t)r->eu.chg_hv_eu);    break;
        case 0x75U: pack16(&data[0],&data[1],(uint16_t)r->eu.user_hv_eu);    pack16(&data[2],&data[3],(uint16_t)r->eu.user_hv_curr_eu); break;
        case 0x76U: pack16(&data[0],&data[1],(uint16_t)r->eu.pc_24v_eu);     pack16(&data[2],&data[3],(uint16_t)r->eu.pc_24v_imon_eu); break;
        case 0x77U: pack16(&data[0],&data[1],(uint16_t)r->eu.user_24v_eu);   pack16(&data[2],&data[3],(uint16_t)r->eu.user_24v_imon_eu); break;
        case 0x78U: pack16(&data[0],&data[1],(uint16_t)r->eu.pc_a_12v_eu);   pack16(&data[2],&data[3],(uint16_t)r->eu.pc_a_12v_imon_eu); break;
        case 0x79U: pack16(&data[0],&data[1],(uint16_t)r->eu.pc_b_12v_eu);   pack16(&data[2],&data[3],(uint16_t)r->eu.pc_b_12v_imon_eu); break;
        case 0x7AU: pack16(&data[0],&data[1],(uint16_t)r->eu.v24_imon0_eu);  pack16(&data[2],&data[3],(uint16_t)r->eu.v24_imon1_eu);  pack16(&data[4],&data[5],(uint16_t)r->eu.v24_imon2_eu);  break;
        case 0x7BU: pack16(&data[0],&data[1],(uint16_t)r->eu.v12_imon0_eu);  pack16(&data[2],&data[3],(uint16_t)r->eu.v12_imon1_eu);  pack16(&data[4],&data[5],(uint16_t)r->eu.v12_imon2_eu);  break;
        case 0x7CU: pack16(&data[0],&data[1],(uint16_t)r->eu.v24_eu);        pack16(&data[2],&data[3],(uint16_t)r->eu.v12_eu);        break;
        case 0x7DU: pack16(&data[0],&data[1],(uint16_t)r->eu.v5_eu);         pack16(&data[2],&data[3],(uint16_t)r->eu.v5_curr_eu);    break;
        case 0x7EU: pack16(&data[0],&data[1],(uint16_t)r->eu.v3v3_eu);       pack16(&data[2],&data[3],(uint16_t)r->eu.v3v3_curr_eu);  break;
        case 0x7FU: pack16(&data[0],&data[1],(uint16_t)r->eu.aux_24v_eu);    pack16(&data[2],&data[3],(uint16_t)r->eu.aux_24v_curr_eu); break;
        case 0x80U: pack16(&data[0],&data[1],(uint16_t)r->eu.aux_12v_eu);    pack16(&data[2],&data[3],(uint16_t)r->eu.aux_12v_curr_eu); break;
        case 0x81U: pack16(&data[0],&data[1],(uint16_t)r->eu.estop_rec_a_12v_eu); pack16(&data[2],&data[3],(uint16_t)r->eu.estop_rec_b_12v_eu); break;
        case 0x82U: pack16(&data[0],&data[1],(uint16_t)r->eu.v12_b_imon3_eu); pack16(&data[2],&data[3],(uint16_t)r->eu.v12_b_imon4_eu); break;
        case 0x83U: pack16(&data[0],&data[1],(uint16_t)r->eu.hv_or_eu);      pack16(&data[2],&data[3],(uint16_t)r->eu.mb_sp_imon_eu); break;
        case 0x84U: pack16(&data[0],&data[1],(uint16_t)r->eu.temp0_eu);      pack16(&data[2],&data[3],(uint16_t)r->eu.temp1_eu);      pack16(&data[4],&data[5],(uint16_t)r->eu.temp2_eu);      break;
        case 0x85U: pack16(&data[0],&data[1],(uint16_t)r->eu.temp3_eu);      pack16(&data[2],&data[3],(uint16_t)r->eu.temp4_eu);      pack16(&data[4],&data[5],(uint16_t)r->eu.temp5_eu);      break;
        case 0x86U: pack16(&data[0],&data[1],(uint16_t)r->eu.ex_temp0_eu);   pack16(&data[2],&data[3],(uint16_t)r->eu.ex_temp1_eu);   break;
        case 0x87U: pack16(&data[0],&data[1],(uint16_t)r->eu.lidar_a_12v_eu); pack16(&data[2],&data[3],(uint16_t)r->eu.lidar_b_12v_eu); break;
        case 0x88U: pack16(&data[0],&data[1],(uint16_t)r->eu.sto_24v_eu);    pack16(&data[2],&data[3],(uint16_t)r->eu.chg_hvil_out_eu); break;
        case 0x89U: pack16(&data[0],&data[1],(uint16_t)r->eu.eth_2v5_eu);    pack16(&data[2],&data[3],(uint16_t)r->eu.eth_1v2_eu);    break;
        case 0x8AU: pack16(&data[0],&data[1],(uint16_t)r->eu.ecat_3v3_eu);   pack16(&data[2],&data[3],(uint16_t)r->eu.ecat_1v2_eu);   break;

        /* === DERIVED EU (0x90-0x91) === */
        case 0x90U: pack16(&data[0],&data[1],(uint16_t)r->eu.v12_a_total_curr_eu); pack16(&data[2],&data[3],(uint16_t)r->eu.v12_b_total_curr_eu); pack16(&data[4],&data[5],(uint16_t)r->eu.v24_total_curr_eu); break;
        case 0x91U: pack16(&data[0],&data[1],(uint16_t)r->eu.batt_total_curr_eu); break;

        /* === ERROR CODE (0xA0) === */
        case 0xA0U:
            data[0] = (uint8_t)( g_regs.error_code         & 0xFFU);
            data[1] = (uint8_t)((g_regs.error_code >>  8U) & 0xFFU);
            data[2] = (uint8_t)((g_regs.error_code >> 16U) & 0xFFU);
            data[3] = (uint8_t)((g_regs.error_code >> 24U) & 0xFFU);
            break;

        /* === FAULT REGISTERS (0xA8-0xAA) === */
        case 0xA8U:
            data[0] = g_regs.fault_regs.reg1;
            break;
        case 0xA9U:
            data[0] = g_regs.fault_regs.reg2[0];
            data[1] = g_regs.fault_regs.reg2[1];
            data[2] = g_regs.fault_regs.reg2[2];
            break;
        case 0xAAU:
            data[0] = g_regs.fault_regs.reg3[0];
            data[1] = g_regs.fault_regs.reg3[1];
            data[2] = g_regs.fault_regs.reg3[2];
            data[3] = g_regs.fault_regs.reg3[3];
            break;

        /* === 16-BIT LIMITS (0xB0-0xCF) === */
        case 0xB0U: pack16(&data[0],&data[1],(uint16_t)r->soft_limits.chg_curr_lim);        pack16(&data[2],&data[3],(uint16_t)r->soft_limits.batt1_curr_plim);     pack16(&data[4],&data[5],(uint16_t)r->soft_limits.batt2_curr_plim);     break;
        case 0xB1U: pack16(&data[0],&data[1],(uint16_t)r->soft_limits.ll_imon_lim);         pack16(&data[2],&data[3],(uint16_t)r->soft_limits.rl_imon_lim);         pack16(&data[4],&data[5],(uint16_t)r->soft_limits.la_imon_lim);         break;
        case 0xB2U: pack16(&data[0],&data[1],(uint16_t)r->soft_limits.ra_imon_lim);         pack16(&data[2],&data[3],(uint16_t)r->soft_limits.tr_imon_lim);         pack16(&data[4],&data[5],(uint16_t)r->soft_limits.chg_in_lim);         break;
        case 0xB3U: pack16(&data[0],&data[1],(uint16_t)r->soft_limits.hv_lim);              pack16(&data[2],&data[3],(uint16_t)r->soft_limits.batt1_in_lim);        pack16(&data[4],&data[5],(uint16_t)r->soft_limits.batt2_in_lim);        break;
        case 0xB4U: pack16(&data[0],&data[1],(uint16_t)r->soft_limits.nk_imon_lim);         pack16(&data[2],&data[3],(uint16_t)r->soft_limits.mot_bus_lim);         pack16(&data[4],&data[5],(uint16_t)r->soft_limits.chg_hv_lim);         break;
        case 0xB5U: pack16(&data[0],&data[1],(uint16_t)r->soft_limits.user_hv_lim);         pack16(&data[2],&data[3],(uint16_t)r->soft_limits.user_hv_curr_lim);    break;
        case 0xB6U: pack16(&data[0],&data[1],(uint16_t)r->soft_limits.pc_24v_lim);          pack16(&data[2],&data[3],(uint16_t)r->soft_limits.pc_24v_imon_lim);     break;
        case 0xB7U: pack16(&data[0],&data[1],(uint16_t)r->soft_limits.user_24v_lim);        pack16(&data[2],&data[3],(uint16_t)r->soft_limits.user_24v_imon_lim);   break;
        case 0xB8U: pack16(&data[0],&data[1],(uint16_t)r->soft_limits.pc_a_12v_lim);        pack16(&data[2],&data[3],(uint16_t)r->soft_limits.pc_a_12v_imon_lim);   break;
        case 0xB9U: pack16(&data[0],&data[1],(uint16_t)r->soft_limits.pc_b_12v_lim);        pack16(&data[2],&data[3],(uint16_t)r->soft_limits.pc_b_12v_imon_lim);   break;
        case 0xBAU: pack16(&data[0],&data[1],(uint16_t)r->soft_limits.v24_imon0_lim);       pack16(&data[2],&data[3],(uint16_t)r->soft_limits.v24_imon1_lim);       pack16(&data[4],&data[5],(uint16_t)r->soft_limits.v24_imon2_lim);       break;
        case 0xBBU: pack16(&data[0],&data[1],(uint16_t)r->soft_limits.v12_imon0_lim);       pack16(&data[2],&data[3],(uint16_t)r->soft_limits.v12_imon1_lim);       pack16(&data[4],&data[5],(uint16_t)r->soft_limits.v12_imon2_lim);       break;
        case 0xBCU: pack16(&data[0],&data[1],(uint16_t)r->soft_limits.v24_lim);             pack16(&data[2],&data[3],(uint16_t)r->soft_limits.v12_lim);             break;
        case 0xBDU: pack16(&data[0],&data[1],(uint16_t)r->soft_limits.v5_lim);              pack16(&data[2],&data[3],(uint16_t)r->soft_limits.v5_curr_lim);         break;
        case 0xBEU: pack16(&data[0],&data[1],(uint16_t)r->soft_limits.v3v3_lim);            pack16(&data[2],&data[3],(uint16_t)r->soft_limits.v3v3_curr_lim);       break;
        case 0xBFU: pack16(&data[0],&data[1],(uint16_t)r->soft_limits.aux_24v_lim);         pack16(&data[2],&data[3],(uint16_t)r->soft_limits.aux_24v_curr_lim);    break;
        case 0xC0U: pack16(&data[0],&data[1],(uint16_t)r->soft_limits.aux_12v_lim);         pack16(&data[2],&data[3],(uint16_t)r->soft_limits.aux_12v_curr_lim);    break;
        case 0xC1U: pack16(&data[0],&data[1],(uint16_t)r->soft_limits.estop_rec_a_12v_lim); pack16(&data[2],&data[3],(uint16_t)r->soft_limits.estop_rec_b_12v_lim); break;
        case 0xC2U: pack16(&data[0],&data[1],(uint16_t)r->soft_limits.v12_b_imon3_lim);     pack16(&data[2],&data[3],(uint16_t)r->soft_limits.v12_b_imon4_lim);     break;
        case 0xC3U: pack16(&data[0],&data[1],(uint16_t)r->soft_limits.hv_or_lim);           pack16(&data[2],&data[3],(uint16_t)r->soft_limits.mb_sp_imon_lim);      break;
        case 0xC4U: pack16(&data[0],&data[1],(uint16_t)r->soft_limits.temp0_plim);          pack16(&data[2],&data[3],(uint16_t)r->soft_limits.temp1_plim);          pack16(&data[4],&data[5],(uint16_t)r->soft_limits.temp2_plim);          break;
        case 0xC5U: pack16(&data[0],&data[1],(uint16_t)r->soft_limits.temp3_plim);          pack16(&data[2],&data[3],(uint16_t)r->soft_limits.temp4_plim);          pack16(&data[4],&data[5],(uint16_t)r->soft_limits.temp5_plim);          break;
        case 0xC6U: pack16(&data[0],&data[1],(uint16_t)r->soft_limits.ex_temp0_plim);       pack16(&data[2],&data[3],(uint16_t)r->soft_limits.ex_temp1_plim);       break;
        case 0xC7U: pack16(&data[0],&data[1],(uint16_t)r->soft_limits.lidar_a_12v_lim);     pack16(&data[2],&data[3],(uint16_t)r->soft_limits.lidar_b_12v_lim);     break;
        case 0xC8U: pack16(&data[0],&data[1],(uint16_t)r->soft_limits.sto_24v_lim);         pack16(&data[2],&data[3],(uint16_t)r->soft_limits.chg_hvil_out_lim);    break;
        case 0xC9U: pack16(&data[0],&data[1],(uint16_t)r->soft_limits.eth_2v5_lim);         pack16(&data[2],&data[3],(uint16_t)r->soft_limits.eth_1v2_lim);         break;
        case 0xCAU: pack16(&data[0],&data[1],(uint16_t)r->soft_limits.ecat_3v3_lim);        pack16(&data[2],&data[3],(uint16_t)r->soft_limits.ecat_1v2_lim);        break;
        case 0xCBU: pack16(&data[0],&data[1],(uint16_t)r->soft_limits.v12_a_total_curr_lim); pack16(&data[2],&data[3],(uint16_t)r->soft_limits.v12_b_total_curr_lim); pack16(&data[4],&data[5],(uint16_t)r->soft_limits.v24_total_curr_lim); break;
        case 0xCCU: pack16(&data[2],&data[3],(uint16_t)r->soft_limits.batt1_curr_nlim);     pack16(&data[4],&data[5],(uint16_t)r->soft_limits.batt2_curr_nlim);     break;
        case 0xCDU: pack16(&data[0],&data[1],(uint16_t)r->soft_limits.temp0_nlim);          pack16(&data[2],&data[3],(uint16_t)r->soft_limits.temp1_nlim);          pack16(&data[4],&data[5],(uint16_t)r->soft_limits.temp2_nlim);          break;
        case 0xCEU: pack16(&data[0],&data[1],(uint16_t)r->soft_limits.temp3_nlim);          pack16(&data[2],&data[3],(uint16_t)r->soft_limits.temp4_nlim);          pack16(&data[4],&data[5],(uint16_t)r->soft_limits.temp5_nlim);          break;
        case 0xCFU: pack16(&data[0],&data[1],(uint16_t)r->soft_limits.ex_temp0_nlim);       pack16(&data[2],&data[3],(uint16_t)r->soft_limits.ex_temp1_nlim);       break;

        /* === LIMIT FAULT FLAGS (0xD0-0xD1, 0xD8-0xD9) === */
        case 0xD0U: memcpy(&data[0], g_regs.lim_faults.soft_grp1, 6U); break;
        case 0xD1U:
            data[0] = g_regs.lim_faults.soft_grp2[0];
            data[1] = g_regs.lim_faults.soft_grp2[1];
            data[2] = g_regs.lim_faults.soft_grp2[2];
            break;
        case 0xD8U: memcpy(&data[0], g_regs.lim_faults.hard_grp1, 6U); break;
        case 0xD9U:
            data[0] = g_regs.lim_faults.hard_grp2[0];
            data[1] = g_regs.lim_faults.hard_grp2[1];
            data[2] = g_regs.lim_faults.hard_grp2[2];
            break;

        /* === BUILD INFO + WRITE PROTECT (0x1F8, 0x1FA) === */
        case 0x1F8U:
            pack16(&data[0], &data[1], g_regs.build_info.hw_rev);
            pack16(&data[2], &data[3], g_regs.build_info.fw_rev);
            break;
        case 0x1FAU:
            data[0] = g_regs.write_protect.b1;
            data[1] = g_regs.write_protect.b2;
            data[2] = g_regs.write_protect.b3;
            data[3] = g_regs.write_protect.b4;
            data[4] = g_regs.write_protect.b5;
            break;

        default:
            break;
    }
}

/* ============================================================
 * process_write() — unpack data[0..5] into g_regs fields
 * ============================================================ */
static void process_write(uint16_t data_id, const uint8_t data[8])
{
    PMB_Registers_t *r = &g_regs;

    if (!write_allowed(data_id)) { return; }

    switch (data_id) {

        /* === POWER ENABLE (0x1F-0x23, 0x22 is read-only) === */
        case 0x1FU:
            { PMB_PowerEnable_t *pe = &g_regs.pwr_enable;
              pe->grp1 = even_parity(data[0]);
            } break;
        case 0x20U:
            { PMB_PowerEnable_t *pe = &g_regs.pwr_enable;
              pe->grp2[0] = even_parity(data[0]);
              pe->grp2[1] = even_parity(data[1]);
              pe->grp2[2] = even_parity(data[2]);
            } break;
        case 0x21U:
            { PMB_PowerEnable_t *pe = &g_regs.pwr_enable;
              pe->grp3[0] = even_parity(data[0]);
              pe->grp3[1] = even_parity(data[1]);
              pe->grp3[2] = even_parity(data[2]);
              pe->grp3[3] = even_parity(data[3]);
            } break;
        case 0x23U:
            { PMB_PowerEnable_t *pe = &g_regs.pwr_enable;
              pe->grp4[0] = even_parity(data[0]);
              pe->grp4[1] = even_parity(data[1]);
              pe->grp4[2] = even_parity(data[2]);
            } break;

        /* === 16-BIT OFFSETS (0x30-0x4A) === */
        case 0x30U: r->calib_offset.chg_curr_ofs=(int16_t)unpack16(data[0],data[1]); r->calib_offset.batt1_curr_ofs=(int16_t)unpack16(data[2],data[3]); r->calib_offset.batt2_curr_ofs=(int16_t)unpack16(data[4],data[5]); break;
        case 0x31U: r->calib_offset.ll_imon_ofs=(int16_t)unpack16(data[0],data[1]); r->calib_offset.rl_imon_ofs=(int16_t)unpack16(data[2],data[3]); r->calib_offset.la_imon_ofs=(int16_t)unpack16(data[4],data[5]); break;
        case 0x32U: r->calib_offset.ra_imon_ofs=(int16_t)unpack16(data[0],data[1]); r->calib_offset.tr_imon_ofs=(int16_t)unpack16(data[2],data[3]); r->calib_offset.chg_in_ofs=(int16_t)unpack16(data[4],data[5]); break;
        case 0x33U: r->calib_offset.hv_ofs=(int16_t)unpack16(data[0],data[1]); r->calib_offset.batt1_in_ofs=(int16_t)unpack16(data[2],data[3]); r->calib_offset.batt2_in_ofs=(int16_t)unpack16(data[4],data[5]); break;
        case 0x34U: r->calib_offset.nk_imon_ofs=(int16_t)unpack16(data[0],data[1]); r->calib_offset.mot_bus_ofs=(int16_t)unpack16(data[2],data[3]); r->calib_offset.chg_hv_ofs=(int16_t)unpack16(data[4],data[5]); break;
        case 0x35U: r->calib_offset.user_hv_ofs=(int16_t)unpack16(data[0],data[1]); r->calib_offset.user_hv_curr_ofs=(int16_t)unpack16(data[2],data[3]); break;
        case 0x36U: r->calib_offset.pc_24v_ofs=(int16_t)unpack16(data[0],data[1]); r->calib_offset.pc_24v_imon_ofs=(int16_t)unpack16(data[2],data[3]); break;
        case 0x37U: r->calib_offset.user_24v_ofs=(int16_t)unpack16(data[0],data[1]); r->calib_offset.user_24v_imon_ofs=(int16_t)unpack16(data[2],data[3]); break;
        case 0x38U: r->calib_offset.pc_a_12v_ofs=(int16_t)unpack16(data[0],data[1]); r->calib_offset.pc_a_12v_imon_ofs=(int16_t)unpack16(data[2],data[3]); break;
        case 0x39U: r->calib_offset.pc_b_12v_ofs=(int16_t)unpack16(data[0],data[1]); r->calib_offset.pc_b_12v_imon_ofs=(int16_t)unpack16(data[2],data[3]); break;
        case 0x3AU: r->calib_offset.v24_imon0_ofs=(int16_t)unpack16(data[0],data[1]); r->calib_offset.v24_imon1_ofs=(int16_t)unpack16(data[2],data[3]); r->calib_offset.v24_imon2_ofs=(int16_t)unpack16(data[4],data[5]); break;
        case 0x3BU: r->calib_offset.v12_imon0_ofs=(int16_t)unpack16(data[0],data[1]); r->calib_offset.v12_imon1_ofs=(int16_t)unpack16(data[2],data[3]); r->calib_offset.v12_imon2_ofs=(int16_t)unpack16(data[4],data[5]); break;
        case 0x3CU: r->calib_offset.v24_ofs=(int16_t)unpack16(data[0],data[1]); r->calib_offset.v12_ofs=(int16_t)unpack16(data[2],data[3]); break;
        case 0x3DU: r->calib_offset.v5_ofs=(int16_t)unpack16(data[0],data[1]); r->calib_offset.v5_curr_ofs=(int16_t)unpack16(data[2],data[3]); break;
        case 0x3EU: r->calib_offset.v3v3_ofs=(int16_t)unpack16(data[0],data[1]); r->calib_offset.v3v3_curr_ofs=(int16_t)unpack16(data[2],data[3]); break;
        case 0x3FU: r->calib_offset.aux_24v_ofs=(int16_t)unpack16(data[0],data[1]); r->calib_offset.aux_24v_curr_ofs=(int16_t)unpack16(data[2],data[3]); break;
        case 0x40U: r->calib_offset.aux_12v_ofs=(int16_t)unpack16(data[0],data[1]); r->calib_offset.aux_12v_curr_ofs=(int16_t)unpack16(data[2],data[3]); break;
        case 0x41U: r->calib_offset.estop_rec_a_12v_ofs=(int16_t)unpack16(data[0],data[1]); r->calib_offset.estop_rec_b_12v_ofs=(int16_t)unpack16(data[2],data[3]); break;
        case 0x42U: r->calib_offset.v12_b_imon3_ofs=(int16_t)unpack16(data[0],data[1]); r->calib_offset.v12_b_imon4_ofs=(int16_t)unpack16(data[2],data[3]); break;
        case 0x43U: r->calib_offset.hv_or_ofs=(int16_t)unpack16(data[0],data[1]); r->calib_offset.mb_sp_imon_ofs=(int16_t)unpack16(data[2],data[3]); break;
        case 0x44U: r->calib_offset.temp0_ofs=(int16_t)unpack16(data[0],data[1]); r->calib_offset.temp1_ofs=(int16_t)unpack16(data[2],data[3]); r->calib_offset.temp2_ofs=(int16_t)unpack16(data[4],data[5]); break;
        case 0x45U: r->calib_offset.temp3_ofs=(int16_t)unpack16(data[0],data[1]); r->calib_offset.temp4_ofs=(int16_t)unpack16(data[2],data[3]); r->calib_offset.temp5_ofs=(int16_t)unpack16(data[4],data[5]); break;
        case 0x46U: r->calib_offset.ex_temp0_ofs=(int16_t)unpack16(data[0],data[1]); r->calib_offset.ex_temp1_ofs=(int16_t)unpack16(data[2],data[3]); break;
        case 0x47U: r->calib_offset.lidar_a_12v_ofs=(int16_t)unpack16(data[0],data[1]); r->calib_offset.lidar_b_12v_ofs=(int16_t)unpack16(data[2],data[3]); break;
        case 0x48U: r->calib_offset.sto_24v_ofs=(int16_t)unpack16(data[0],data[1]); r->calib_offset.chg_hvil_out_ofs=(int16_t)unpack16(data[2],data[3]); break;
        case 0x49U: r->calib_offset.eth_2v5_ofs=(int16_t)unpack16(data[0],data[1]); r->calib_offset.eth_1v2_ofs=(int16_t)unpack16(data[2],data[3]); break;
        case 0x4AU: r->calib_offset.ecat_3v3_ofs=(int16_t)unpack16(data[0],data[1]); r->calib_offset.ecat_1v2_ofs=(int16_t)unpack16(data[2],data[3]); break;

        /* === 16-BIT SCALES (0x50-0x6A) === */
        case 0x50U: r->calib_scale.chg_curr_scl=(int16_t)unpack16(data[0],data[1]); r->calib_scale.batt1_curr_scl=(int16_t)unpack16(data[2],data[3]); r->calib_scale.batt2_curr_scl=(int16_t)unpack16(data[4],data[5]); break;
        case 0x51U: r->calib_scale.ll_imon_scl=(int16_t)unpack16(data[0],data[1]); r->calib_scale.rl_imon_scl=(int16_t)unpack16(data[2],data[3]); r->calib_scale.la_imon_scl=(int16_t)unpack16(data[4],data[5]); break;
        case 0x52U: r->calib_scale.ra_imon_scl=(int16_t)unpack16(data[0],data[1]); r->calib_scale.tr_imon_scl=(int16_t)unpack16(data[2],data[3]); r->calib_scale.chg_in_scl=(int16_t)unpack16(data[4],data[5]); break;
        case 0x53U: r->calib_scale.hv_scl=(int16_t)unpack16(data[0],data[1]); r->calib_scale.batt1_in_scl=(int16_t)unpack16(data[2],data[3]); r->calib_scale.batt2_in_scl=(int16_t)unpack16(data[4],data[5]); break;
        case 0x54U: r->calib_scale.nk_imon_scl=(int16_t)unpack16(data[0],data[1]); r->calib_scale.mot_bus_scl=(int16_t)unpack16(data[2],data[3]); r->calib_scale.chg_hv_scl=(int16_t)unpack16(data[4],data[5]); break;
        case 0x55U: r->calib_scale.user_hv_scl=(int16_t)unpack16(data[0],data[1]); r->calib_scale.user_hv_curr_scl=(int16_t)unpack16(data[2],data[3]); break;
        case 0x56U: r->calib_scale.pc_24v_scl=(int16_t)unpack16(data[0],data[1]); r->calib_scale.pc_24v_imon_scl=(int16_t)unpack16(data[2],data[3]); break;
        case 0x57U: r->calib_scale.user_24v_scl=(int16_t)unpack16(data[0],data[1]); r->calib_scale.user_24v_imon_scl=(int16_t)unpack16(data[2],data[3]); break;
        case 0x58U: r->calib_scale.pc_a_12v_scl=(int16_t)unpack16(data[0],data[1]); r->calib_scale.pc_a_12v_imon_scl=(int16_t)unpack16(data[2],data[3]); break;
        case 0x59U: r->calib_scale.pc_b_12v_scl=(int16_t)unpack16(data[0],data[1]); r->calib_scale.pc_b_12v_imon_scl=(int16_t)unpack16(data[2],data[3]); break;
        case 0x5AU: r->calib_scale.v24_imon0_scl=(int16_t)unpack16(data[0],data[1]); r->calib_scale.v24_imon1_scl=(int16_t)unpack16(data[2],data[3]); r->calib_scale.v24_imon2_scl=(int16_t)unpack16(data[4],data[5]); break;
        case 0x5BU: r->calib_scale.v12_imon0_scl=(int16_t)unpack16(data[0],data[1]); r->calib_scale.v12_imon1_scl=(int16_t)unpack16(data[2],data[3]); r->calib_scale.v12_imon2_scl=(int16_t)unpack16(data[4],data[5]); break;
        case 0x5CU: r->calib_scale.v24_scl=(int16_t)unpack16(data[0],data[1]); r->calib_scale.v12_scl=(int16_t)unpack16(data[2],data[3]); break;
        case 0x5DU: r->calib_scale.v5_scl=(int16_t)unpack16(data[0],data[1]); r->calib_scale.v5_curr_scl=(int16_t)unpack16(data[2],data[3]); break;
        case 0x5EU: r->calib_scale.v3v3_scl=(int16_t)unpack16(data[0],data[1]); r->calib_scale.v3v3_curr_scl=(int16_t)unpack16(data[2],data[3]); break;
        case 0x5FU: r->calib_scale.aux_24v_scl=(int16_t)unpack16(data[0],data[1]); r->calib_scale.aux_24v_curr_scl=(int16_t)unpack16(data[2],data[3]); break;
        case 0x60U: r->calib_scale.aux_12v_scl=(int16_t)unpack16(data[0],data[1]); r->calib_scale.aux_12v_curr_scl=(int16_t)unpack16(data[2],data[3]); break;
        case 0x61U: r->calib_scale.estop_rec_a_12v_scl=(int16_t)unpack16(data[0],data[1]); r->calib_scale.estop_rec_b_12v_scl=(int16_t)unpack16(data[2],data[3]); break;
        case 0x62U: r->calib_scale.v12_b_imon3_scl=(int16_t)unpack16(data[0],data[1]); r->calib_scale.v12_b_imon4_scl=(int16_t)unpack16(data[2],data[3]); break;
        case 0x63U: r->calib_scale.hv_or_scl=(int16_t)unpack16(data[0],data[1]); r->calib_scale.mb_sp_imon_scl=(int16_t)unpack16(data[2],data[3]); break;
        case 0x64U: r->calib_scale.temp0_scl=(int16_t)unpack16(data[0],data[1]); r->calib_scale.temp1_scl=(int16_t)unpack16(data[2],data[3]); r->calib_scale.temp2_scl=(int16_t)unpack16(data[4],data[5]); break;
        case 0x65U: r->calib_scale.temp3_scl=(int16_t)unpack16(data[0],data[1]); r->calib_scale.temp4_scl=(int16_t)unpack16(data[2],data[3]); r->calib_scale.temp5_scl=(int16_t)unpack16(data[4],data[5]); break;
        case 0x66U: r->calib_scale.ex_temp0_scl=(int16_t)unpack16(data[0],data[1]); r->calib_scale.ex_temp1_scl=(int16_t)unpack16(data[2],data[3]); break;
        case 0x67U: r->calib_scale.lidar_a_12v_scl=(int16_t)unpack16(data[0],data[1]); r->calib_scale.lidar_b_12v_scl=(int16_t)unpack16(data[2],data[3]); break;
        case 0x68U: r->calib_scale.sto_24v_scl=(int16_t)unpack16(data[0],data[1]); r->calib_scale.chg_hvil_out_scl=(int16_t)unpack16(data[2],data[3]); break;
        case 0x69U: r->calib_scale.eth_2v5_scl=(int16_t)unpack16(data[0],data[1]); r->calib_scale.eth_1v2_scl=(int16_t)unpack16(data[2],data[3]); break;
        case 0x6AU: r->calib_scale.ecat_3v3_scl=(int16_t)unpack16(data[0],data[1]); r->calib_scale.ecat_1v2_scl=(int16_t)unpack16(data[2],data[3]); break;

        /* === 16-BIT LIMITS (0xB0-0xCF) === */
        case 0xB0U: r->soft_limits.chg_curr_lim=(int16_t)unpack16(data[0],data[1]); r->soft_limits.batt1_curr_plim=(int16_t)unpack16(data[2],data[3]); r->soft_limits.batt2_curr_plim=(int16_t)unpack16(data[4],data[5]); break;
        case 0xB1U: r->soft_limits.ll_imon_lim=(int16_t)unpack16(data[0],data[1]); r->soft_limits.rl_imon_lim=(int16_t)unpack16(data[2],data[3]); r->soft_limits.la_imon_lim=(int16_t)unpack16(data[4],data[5]); break;
        case 0xB2U: r->soft_limits.ra_imon_lim=(int16_t)unpack16(data[0],data[1]); r->soft_limits.tr_imon_lim=(int16_t)unpack16(data[2],data[3]); r->soft_limits.chg_in_lim=(int16_t)unpack16(data[4],data[5]); break;
        case 0xB3U: r->soft_limits.hv_lim=(int16_t)unpack16(data[0],data[1]); r->soft_limits.batt1_in_lim=(int16_t)unpack16(data[2],data[3]); r->soft_limits.batt2_in_lim=(int16_t)unpack16(data[4],data[5]); break;
        case 0xB4U: r->soft_limits.nk_imon_lim=(int16_t)unpack16(data[0],data[1]); r->soft_limits.mot_bus_lim=(int16_t)unpack16(data[2],data[3]); r->soft_limits.chg_hv_lim=(int16_t)unpack16(data[4],data[5]); break;
        case 0xB5U: r->soft_limits.user_hv_lim=(int16_t)unpack16(data[0],data[1]); r->soft_limits.user_hv_curr_lim=(int16_t)unpack16(data[2],data[3]); break;
        case 0xB6U: r->soft_limits.pc_24v_lim=(int16_t)unpack16(data[0],data[1]); r->soft_limits.pc_24v_imon_lim=(int16_t)unpack16(data[2],data[3]); break;
        case 0xB7U: r->soft_limits.user_24v_lim=(int16_t)unpack16(data[0],data[1]); r->soft_limits.user_24v_imon_lim=(int16_t)unpack16(data[2],data[3]); break;
        case 0xB8U: r->soft_limits.pc_a_12v_lim=(int16_t)unpack16(data[0],data[1]); r->soft_limits.pc_a_12v_imon_lim=(int16_t)unpack16(data[2],data[3]); break;
        case 0xB9U: r->soft_limits.pc_b_12v_lim=(int16_t)unpack16(data[0],data[1]); r->soft_limits.pc_b_12v_imon_lim=(int16_t)unpack16(data[2],data[3]); break;
        case 0xBAU: r->soft_limits.v24_imon0_lim=(int16_t)unpack16(data[0],data[1]); r->soft_limits.v24_imon1_lim=(int16_t)unpack16(data[2],data[3]); r->soft_limits.v24_imon2_lim=(int16_t)unpack16(data[4],data[5]); break;
        case 0xBBU: r->soft_limits.v12_imon0_lim=(int16_t)unpack16(data[0],data[1]); r->soft_limits.v12_imon1_lim=(int16_t)unpack16(data[2],data[3]); r->soft_limits.v12_imon2_lim=(int16_t)unpack16(data[4],data[5]); break;
        case 0xBCU: r->soft_limits.v24_lim=(int16_t)unpack16(data[0],data[1]); r->soft_limits.v12_lim=(int16_t)unpack16(data[2],data[3]); break;
        case 0xBDU: r->soft_limits.v5_lim=(int16_t)unpack16(data[0],data[1]); r->soft_limits.v5_curr_lim=(int16_t)unpack16(data[2],data[3]); break;
        case 0xBEU: r->soft_limits.v3v3_lim=(int16_t)unpack16(data[0],data[1]); r->soft_limits.v3v3_curr_lim=(int16_t)unpack16(data[2],data[3]); break;
        case 0xBFU: r->soft_limits.aux_24v_lim=(int16_t)unpack16(data[0],data[1]); r->soft_limits.aux_24v_curr_lim=(int16_t)unpack16(data[2],data[3]); break;
        case 0xC0U: r->soft_limits.aux_12v_lim=(int16_t)unpack16(data[0],data[1]); r->soft_limits.aux_12v_curr_lim=(int16_t)unpack16(data[2],data[3]); break;
        case 0xC1U: r->soft_limits.estop_rec_a_12v_lim=(int16_t)unpack16(data[0],data[1]); r->soft_limits.estop_rec_b_12v_lim=(int16_t)unpack16(data[2],data[3]); break;
        case 0xC2U: r->soft_limits.v12_b_imon3_lim=(int16_t)unpack16(data[0],data[1]); r->soft_limits.v12_b_imon4_lim=(int16_t)unpack16(data[2],data[3]); break;
        case 0xC3U: r->soft_limits.hv_or_lim=(int16_t)unpack16(data[0],data[1]); r->soft_limits.mb_sp_imon_lim=(int16_t)unpack16(data[2],data[3]); break;
        case 0xC4U: r->soft_limits.temp0_plim=(int16_t)unpack16(data[0],data[1]); r->soft_limits.temp1_plim=(int16_t)unpack16(data[2],data[3]); r->soft_limits.temp2_plim=(int16_t)unpack16(data[4],data[5]); break;
        case 0xC5U: r->soft_limits.temp3_plim=(int16_t)unpack16(data[0],data[1]); r->soft_limits.temp4_plim=(int16_t)unpack16(data[2],data[3]); r->soft_limits.temp5_plim=(int16_t)unpack16(data[4],data[5]); break;
        case 0xC6U: r->soft_limits.ex_temp0_plim=(int16_t)unpack16(data[0],data[1]); r->soft_limits.ex_temp1_plim=(int16_t)unpack16(data[2],data[3]); break;
        case 0xC7U: r->soft_limits.lidar_a_12v_lim=(int16_t)unpack16(data[0],data[1]); r->soft_limits.lidar_b_12v_lim=(int16_t)unpack16(data[2],data[3]); break;
        case 0xC8U: r->soft_limits.sto_24v_lim=(int16_t)unpack16(data[0],data[1]); r->soft_limits.chg_hvil_out_lim=(int16_t)unpack16(data[2],data[3]); break;
        case 0xC9U: r->soft_limits.eth_2v5_lim=(int16_t)unpack16(data[0],data[1]); r->soft_limits.eth_1v2_lim=(int16_t)unpack16(data[2],data[3]); break;
        case 0xCAU: r->soft_limits.ecat_3v3_lim=(int16_t)unpack16(data[0],data[1]); r->soft_limits.ecat_1v2_lim=(int16_t)unpack16(data[2],data[3]); break;
        case 0xCBU: r->soft_limits.v12_a_total_curr_lim=(int16_t)unpack16(data[0],data[1]); r->soft_limits.v12_b_total_curr_lim=(int16_t)unpack16(data[2],data[3]); r->soft_limits.v24_total_curr_lim=(int16_t)unpack16(data[4],data[5]); break;
        case 0xCCU: r->soft_limits.batt1_curr_nlim=(int16_t)unpack16(data[2],data[3]); r->soft_limits.batt2_curr_nlim=(int16_t)unpack16(data[4],data[5]); break;
        case 0xCDU: r->soft_limits.temp0_nlim=(int16_t)unpack16(data[0],data[1]); r->soft_limits.temp1_nlim=(int16_t)unpack16(data[2],data[3]); r->soft_limits.temp2_nlim=(int16_t)unpack16(data[4],data[5]); break;
        case 0xCEU: r->soft_limits.temp3_nlim=(int16_t)unpack16(data[0],data[1]); r->soft_limits.temp4_nlim=(int16_t)unpack16(data[2],data[3]); r->soft_limits.temp5_nlim=(int16_t)unpack16(data[4],data[5]); break;
        case 0xCFU: r->soft_limits.ex_temp0_nlim=(int16_t)unpack16(data[0],data[1]); r->soft_limits.ex_temp1_nlim=(int16_t)unpack16(data[2],data[3]); break;

        /* === FAULT REGISTERS: writing 1 clears the fault === */
        case 0xA8U: g_regs.fault_regs.reg1    &= ~data[0]; break;
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
            break;

        /* === LIMIT FAULT FLAGS: clear-on-write-1 === */
        case 0xD0U:
            { int _i; for (_i = 0; _i < 6; _i++) g_regs.lim_faults.soft_grp1[_i] &= ~data[_i]; }
            break;
        case 0xD1U:
            g_regs.lim_faults.soft_grp2[0] &= ~data[0];
            g_regs.lim_faults.soft_grp2[1] &= ~data[1];
            g_regs.lim_faults.soft_grp2[2] &= ~data[2];
            break;
        case 0xD8U:
            { int _i; for (_i = 0; _i < 6; _i++) g_regs.lim_faults.hard_grp1[_i] &= ~data[_i]; }
            break;
        case 0xD9U:
            g_regs.lim_faults.hard_grp2[0] &= ~data[0];
            g_regs.lim_faults.hard_grp2[1] &= ~data[1];
            g_regs.lim_faults.hard_grp2[2] &= ~data[2];
            break;

        /* === BUILD INFO + WRITE PROTECT === */
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
            break;

        default:
            break;
    }
}

/* ============================================================
 * transmit_response() — build and send the response frame
 * ============================================================ */
static void transmit_response(uint8_t src, uint16_t data_id, uint8_t data[8])
{
    XMC_CAN_MO_t *mo = MULTICAN_CONFIG_0.lmobj_ptr[PMB_TX_MO]->mo_ptr;

    /* Echo the packet counter from the request */
    data[6] = s_pkt_lo;
    data[7] = s_pkt_hi;

    /* 29-bit extended CAN ID: priority=4, data_id, dst=src, src=PMB_ADDR */
    mo->can_identifier = PMB_CAN_BUILD_ID(4U, data_id, src, PMB_ADDR);
    mo->can_id_mode    = XMC_CAN_FRAME_TYPE_EXTENDED_29BITS;
    mo->can_data_length = 8U;

    uint8_t i;
    for (i = 0U; i < 8U; i++) {
        mo->can_data_byte[i] = data[i];
    }

    XMC_CAN_MO_UpdateData(mo);
    XMC_CAN_MO_Transmit(mo);
}

/* ============================================================
 * Public API
 * ============================================================ */

void PMB_Emulator_Init(void)
{
    XMC_CAN_MO_t *rx_mo = MULTICAN_CONFIG_0.lmobj_ptr[PMB_RX_MO]->mo_ptr;
    XMC_CAN_MO_t *tx_mo = MULTICAN_CONFIG_0.lmobj_ptr[PMB_TX_MO]->mo_ptr;

    memset(&g_regs, 0, sizeof(g_regs));

    /* Configure RX MO to accept any 29-bit extended frame where dst byte = PMB_ADDR.
     * Mask checks bits[15:8] (the dst field) only. */
    rx_mo->can_identifier         = PMB_CAN_BUILD_ID(0U, 0U, PMB_ADDR, 0U);
    rx_mo->can_id_mask            = 0x0000FF00UL;   /* mask dst field only */
    rx_mo->can_id_mode            = XMC_CAN_FRAME_TYPE_EXTENDED_29BITS;
    rx_mo->can_mo_type            = XMC_CAN_MO_TYPE_RECMSGOBJ;
    rx_mo->can_data_length        = 8U;
    XMC_CAN_MO_Config(rx_mo);

    /* TX MO: extended frame, DLC=8; ID is set per-message in transmit_response */
    tx_mo->can_id_mode     = XMC_CAN_FRAME_TYPE_EXTENDED_29BITS;
    tx_mo->can_mo_type     = XMC_CAN_MO_TYPE_TRANSMSGOBJ;
    tx_mo->can_data_length = 8U;
    XMC_CAN_MO_Config(tx_mo);
}

void PMB_Emulator_Run(void)
{
    XMC_CAN_MO_t *rx_mo = MULTICAN_CONFIG_0.lmobj_ptr[PMB_RX_MO]->mo_ptr;

    if (!(XMC_CAN_MO_GetStatus(rx_mo) & (uint32_t)XMC_CAN_MO_STATUS_RX_PENDING)) {
        return;
    }

    XMC_CAN_MO_Receive(rx_mo);

    uint32_t can_id  = rx_mo->can_identifier;
    uint16_t data_id = (uint16_t)PMB_GET_DATA_ID(can_id);
    uint8_t  src     = (uint8_t) PMB_GET_SRC(can_id);

    /* Save packet counter from bytes 7-8 (data[6] and data[7]) */
    s_pkt_lo = rx_mo->can_data_byte[6];
    s_pkt_hi = rx_mo->can_data_byte[7];

    uint8_t resp[8];
    memset(resp, 0, sizeof(resp));

    /* Determine read vs. write: all-zero payload = read request */
    uint8_t is_write = 0U;
    uint8_t j;
    for (j = 0U; j < 6U; j++) {
        if (rx_mo->can_data_byte[j] != 0U) { is_write = 1U; break; }
    }

    if (is_write) {
        process_write(data_id, rx_mo->can_data_byte);
    }

    build_response(data_id, resp);
    transmit_response(src, data_id, resp);
}

void PMB_UpdateEU(void)
{
    PMB_Telemetry_t  *t = &g_regs.telem;
    PMB_CalibOffset_t *o = &g_regs.calib_offset;
    PMB_CalibScale_t  *s = &g_regs.calib_scale;
    PMB_EU_t          *e = &g_regs.eu;

    e->chg_curr_eu          = calc_eu(t->chg_curr,          o->chg_curr_ofs,          s->chg_curr_scl);
    e->batt1_curr_eu        = calc_eu(t->batt1_curr,        o->batt1_curr_ofs,        s->batt1_curr_scl);
    e->batt2_curr_eu        = calc_eu(t->batt2_curr,        o->batt2_curr_ofs,        s->batt2_curr_scl);
    e->ll_imon_eu           = calc_eu(t->ll_imon,           o->ll_imon_ofs,           s->ll_imon_scl);
    e->rl_imon_eu           = calc_eu(t->rl_imon,           o->rl_imon_ofs,           s->rl_imon_scl);
    e->la_imon_eu           = calc_eu(t->la_imon,           o->la_imon_ofs,           s->la_imon_scl);
    e->ra_imon_eu           = calc_eu(t->ra_imon,           o->ra_imon_ofs,           s->ra_imon_scl);
    e->tr_imon_eu           = calc_eu(t->tr_imon,           o->tr_imon_ofs,           s->tr_imon_scl);
    e->chg_in_eu            = calc_eu(t->chg_in,            o->chg_in_ofs,            s->chg_in_scl);
    e->hv_eu                = calc_eu(t->hv,                o->hv_ofs,                s->hv_scl);
    e->batt1_in_eu          = calc_eu(t->batt1_in,          o->batt1_in_ofs,          s->batt1_in_scl);
    e->batt2_in_eu          = calc_eu(t->batt2_in,          o->batt2_in_ofs,          s->batt2_in_scl);
    e->nk_imon_eu           = calc_eu(t->nk_imon,           o->nk_imon_ofs,           s->nk_imon_scl);
    e->mot_bus_eu           = calc_eu(t->mot_bus,           o->mot_bus_ofs,           s->mot_bus_scl);
    e->chg_hv_eu            = calc_eu(t->chg_hv,            o->chg_hv_ofs,            s->chg_hv_scl);
    e->user_hv_eu           = calc_eu(t->user_hv,           o->user_hv_ofs,           s->user_hv_scl);
    e->user_hv_curr_eu      = calc_eu(t->user_hv_curr,      o->user_hv_curr_ofs,      s->user_hv_curr_scl);
    e->pc_24v_eu            = calc_eu(t->pc_24v,            o->pc_24v_ofs,            s->pc_24v_scl);
    e->pc_24v_imon_eu       = calc_eu(t->pc_24v_imon,       o->pc_24v_imon_ofs,       s->pc_24v_imon_scl);
    e->user_24v_eu          = calc_eu(t->user_24v,          o->user_24v_ofs,          s->user_24v_scl);
    e->user_24v_imon_eu     = calc_eu(t->user_24v_imon,     o->user_24v_imon_ofs,     s->user_24v_imon_scl);
    e->pc_a_12v_eu          = calc_eu(t->pc_a_12v,          o->pc_a_12v_ofs,          s->pc_a_12v_scl);
    e->pc_a_12v_imon_eu     = calc_eu(t->pc_a_12v_imon,     o->pc_a_12v_imon_ofs,     s->pc_a_12v_imon_scl);
    e->pc_b_12v_eu          = calc_eu(t->pc_b_12v,          o->pc_b_12v_ofs,          s->pc_b_12v_scl);
    e->pc_b_12v_imon_eu     = calc_eu(t->pc_b_12v_imon,     o->pc_b_12v_imon_ofs,     s->pc_b_12v_imon_scl);
    e->v24_imon0_eu         = calc_eu(t->v24_imon0,         o->v24_imon0_ofs,         s->v24_imon0_scl);
    e->v24_imon1_eu         = calc_eu(t->v24_imon1,         o->v24_imon1_ofs,         s->v24_imon1_scl);
    e->v24_imon2_eu         = calc_eu(t->v24_imon2,         o->v24_imon2_ofs,         s->v24_imon2_scl);
    e->v12_imon0_eu         = calc_eu(t->v12_imon0,         o->v12_imon0_ofs,         s->v12_imon0_scl);
    e->v12_imon1_eu         = calc_eu(t->v12_imon1,         o->v12_imon1_ofs,         s->v12_imon1_scl);
    e->v12_imon2_eu         = calc_eu(t->v12_imon2,         o->v12_imon2_ofs,         s->v12_imon2_scl);
    e->v24_eu               = calc_eu(t->v24_bus_voltage,   o->v24_ofs,               s->v24_scl);
    e->v12_eu               = calc_eu(t->v12_bus_a_voltage, o->v12_ofs,               s->v12_scl);
    e->v5_eu                = calc_eu(t->v5,                o->v5_ofs,                s->v5_scl);
    e->v5_curr_eu           = calc_eu(t->v5_curr,           o->v5_curr_ofs,           s->v5_curr_scl);
    e->v3v3_eu              = calc_eu(t->v3v3,              o->v3v3_ofs,              s->v3v3_scl);
    e->v3v3_curr_eu         = calc_eu(t->v3v3_curr,         o->v3v3_curr_ofs,         s->v3v3_curr_scl);
    e->aux_24v_eu           = calc_eu(t->aux_24v,           o->aux_24v_ofs,           s->aux_24v_scl);
    e->aux_24v_curr_eu      = calc_eu(t->aux_24v_curr,      o->aux_24v_curr_ofs,      s->aux_24v_curr_scl);
    e->aux_12v_eu           = calc_eu(t->aux_12v,           o->aux_12v_ofs,           s->aux_12v_scl);
    e->aux_12v_curr_eu      = calc_eu(t->aux_12v_curr,      o->aux_12v_curr_ofs,      s->aux_12v_curr_scl);
    e->estop_rec_a_12v_eu   = calc_eu(t->estop_rec_a_12v,   o->estop_rec_a_12v_ofs,   s->estop_rec_a_12v_scl);
    e->estop_rec_b_12v_eu   = calc_eu(t->estop_rec_b_12v,   o->estop_rec_b_12v_ofs,   s->estop_rec_b_12v_scl);
    e->v12_b_imon3_eu       = calc_eu(t->v12_b_imon3,       o->v12_b_imon3_ofs,       s->v12_b_imon3_scl);
    e->v12_b_imon4_eu       = calc_eu(t->v12_b_imon4,       o->v12_b_imon4_ofs,       s->v12_b_imon4_scl);
    e->hv_or_eu             = calc_eu(t->hv_or,             o->hv_or_ofs,             s->hv_or_scl);
    e->mb_sp_imon_eu        = calc_eu(t->mb_sp_imon,        o->mb_sp_imon_ofs,        s->mb_sp_imon_scl);
    e->temp0_eu             = calc_eu(t->temp0,             o->temp0_ofs,             s->temp0_scl);
    e->temp1_eu             = calc_eu(t->temp1,             o->temp1_ofs,             s->temp1_scl);
    e->temp2_eu             = calc_eu(t->temp2,             o->temp2_ofs,             s->temp2_scl);
    e->temp3_eu             = calc_eu(t->temp3,             o->temp3_ofs,             s->temp3_scl);
    e->temp4_eu             = calc_eu(t->temp4,             o->temp4_ofs,             s->temp4_scl);
    e->temp5_eu             = calc_eu(t->temp5,             o->temp5_ofs,             s->temp5_scl);
    e->ex_temp0_eu          = calc_eu(t->ex_temp0,          o->ex_temp0_ofs,          s->ex_temp0_scl);
    e->ex_temp1_eu          = calc_eu(t->ex_temp1,          o->ex_temp1_ofs,          s->ex_temp1_scl);
    e->lidar_a_12v_eu       = calc_eu(t->lidar_a_12v,       o->lidar_a_12v_ofs,       s->lidar_a_12v_scl);
    e->lidar_b_12v_eu       = calc_eu(t->lidar_b_12v,       o->lidar_b_12v_ofs,       s->lidar_b_12v_scl);
    e->sto_24v_eu           = calc_eu(t->sto_24v,           o->sto_24v_ofs,           s->sto_24v_scl);
    e->chg_hvil_out_eu      = calc_eu(t->chg_hvil_out,      o->chg_hvil_out_ofs,      s->chg_hvil_out_scl);
    e->eth_2v5_eu           = calc_eu(t->eth_2v5,           o->eth_2v5_ofs,           s->eth_2v5_scl);
    e->eth_1v2_eu           = calc_eu(t->eth_1v2,           o->eth_1v2_ofs,           s->eth_1v2_scl);
    e->ecat_3v3_eu          = calc_eu(t->ecat_3v3,          o->ecat_3v3_ofs,          s->ecat_3v3_scl);
    e->ecat_1v2_eu          = calc_eu(t->ecat_1v2,          o->ecat_1v2_ofs,          s->ecat_1v2_scl);

    /* Derived totals — application fills these by summing EU values */
    e->v12_a_total_curr_eu  = (int16_t)(e->v12_imon0_eu + e->v12_imon1_eu + e->v12_imon2_eu);
    e->v12_b_total_curr_eu  = (int16_t)(e->v12_b_imon3_eu + e->v12_b_imon4_eu);
    e->v24_total_curr_eu    = (int16_t)(e->v24_imon0_eu + e->v24_imon1_eu + e->v24_imon2_eu);
    e->batt_total_curr_eu   = (int16_t)(e->batt1_curr_eu + e->batt2_curr_eu);
}

const PMB_Registers_t *PMB_GetRegs(void)
{
    return &g_regs;
}

PMB_Registers_t *PMB_GetRegsMutable(void)
{
    return &g_regs;
}
