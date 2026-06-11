/* Rows parsed from CSV: 174 */

/* === #define REG_* CONSTANTS === */
#define REG_TELEMETRY_GROUP_1                              (0x00U)
#define REG_TELEMETRY_GROUP_2                              (0x01U)
#define REG_TELEMETRY_GROUP_3                              (0x02U)
#define REG_TELEMETRY_GROUP_4                              (0x03U)
#define REG_TELEMETRY_GROUP_5                              (0x04U)
#define REG_TELEMETRY_GROUP_6                              (0x05U)
#define REG_TELEMETRY_GROUP_7                              (0x06U)
#define REG_TELEMETRY_GROUP_8                              (0x07U)
#define REG_TELEMETRY_GROUP_9                              (0x08U)
#define REG_TELEMETRY_GROUP_10                             (0x09U)
#define REG_TELEMETRY_GROUP_11                             (0x0AU)
#define REG_TELEMETRY_GROUP_12                             (0x0BU)
#define REG_TELEMETRY_GROUP_13                             (0x0CU)
#define REG_TELEMETRY_GROUP_14                             (0x0DU)
#define REG_TELEMETRY_GROUP_15                             (0x0EU)
#define REG_TELEMETRY_GROUP_16                             (0x0FU)
#define REG_TELEMETRY_GROUP_17                             (0x10U)
#define REG_TELEMETRY_GROUP_18                             (0x11U)
#define REG_TELEMETRY_GROUP_19                             (0x12U)
#define REG_TELEMETRY_GROUP_20                             (0x13U)
#define REG_TELEMETRY_GROUP_21                             (0x14U)
#define REG_TELEMETRY_GROUP_22                             (0x15U)
#define REG_TELEMETRY_GROUP_23                             (0x16U)
#define REG_TELEMETRY_GROUP_24                             (0x17U)
#define REG_TELEMETRY_GROUP_25                             (0x18U)
#define REG_TELEMETRY_GROUP_26                             (0x19U)
#define REG_TELEMETRY_GROUP_27                             (0x1AU)
#define REG_POWER_GOOD_DETECTION_FROM_COMPARATOR_COMP_PDG  (0x1BU)
#define REG_POWER_GOOD_AND_FAULT_GROUP_1                   (0x1CU)
#define REG_POWER_GOOD_AND_FAULT_GROUP_2                   (0x1DU)
#define REG_POWER_GOOD_AND_FAULT_GROUP_3                   (0x1EU)
#define REG_POWER_ENABLE_GROUP_1                           (0x1FU)
#define REG_POWER_ENABLE_GROUP_2                           (0x20U)
#define REG_POWER_ENABLE_GROUP_3                           (0x21U)
#define REG_PERIPHERAL_INPUTS                              (0x22U)
#define REG_POWER_ENABLE_GROUP_4                           (0x23U)
#define REG_ADC_OFFSET_GROUP_1                             (0x30U)
#define REG_ADC_OFFSET_GROUP_2                             (0x31U)
#define REG_ADC_OFFSET_GROUP_3                             (0x32U)
#define REG_ADC_OFFSET_GROUP_4                             (0x33U)
#define REG_ADC_OFFSET_GROUP_5                             (0x34U)
#define REG_ADC_OFFSET_GROUP_6                             (0x35U)
#define REG_ADC_OFFSET_GROUP_7                             (0x36U)
#define REG_ADC_OFFSET_GROUP_8                             (0x37U)
#define REG_ADC_OFFSET_GROUP_9                             (0x38U)
#define REG_ADC_OFFSET_GROUP_10                            (0x39U)
#define REG_ADC_OFFSET_GROUP_11                            (0x3AU)
#define REG_ADC_OFFSET_GROUP_12                            (0x3BU)
#define REG_ADC_OFFSET_GROUP_13                            (0x3CU)
#define REG_ADC_OFFSET_GROUP_14                            (0x3DU)
#define REG_ADC_OFFSET_GROUP_15                            (0x3EU)
#define REG_ADC_OFFSET_GROUP_16                            (0x3FU)
#define REG_ADC_OFFSET_GROUP_17                            (0x40U)
#define REG_ADC_OFFSET_GROUP_18                            (0x41U)
#define REG_ADC_OFFSET_GROUP_19                            (0x42U)
#define REG_ADC_OFFSET_GROUP_20                            (0x43U)
#define REG_ADC_OFFSET_GROUP_21                            (0x44U)
#define REG_ADC_OFFSET_GROUP_22                            (0x45U)
#define REG_ADC_OFFSET_GROUP_23                            (0x46U)
#define REG_ADC_OFFSET_GROUP_24                            (0x47U)
#define REG_ADC_OFFSET_GROUP_25                            (0x48U)
#define REG_ADC_OFFSET_GROUP_26                            (0x49U)
#define REG_ADC_OFFSET_GROUP_27                            (0x4AU)
#define REG_ADC_SCALE_GROUP_1                              (0x50U)
#define REG_ADC_SCALE_GROUP_2                              (0x51U)
#define REG_ADC_SCALE_GROUP_3                              (0x52U)
#define REG_ADC_SCALE_GROUP_4                              (0x53U)
#define REG_ADC_SCALE_GROUP_5                              (0x54U)
#define REG_ADC_SCALE_GROUP_6                              (0x55U)
#define REG_ADC_SCALE_GROUP_7                              (0x56U)
#define REG_ADC_SCALE_GROUP_8                              (0x57U)
#define REG_ADC_SCALE_GROUP_9                              (0x58U)
#define REG_ADC_SCALE_GROUP_10                             (0x59U)
#define REG_ADC_SCALE_GROUP_11                             (0x5AU)
#define REG_ADC_SCALE_GROUP_12                             (0x5BU)
#define REG_ADC_SCALE_GROUP_13                             (0x5CU)
#define REG_ADC_SCALE_GROUP_14                             (0x5DU)
#define REG_ADC_SCALE_GROUP_15                             (0x5EU)
#define REG_ADC_SCALE_GROUP_16                             (0x5FU)
#define REG_ADC_SCALE_GROUP_17                             (0x60U)
#define REG_ADC_SCALE_GROUP_18                             (0x61U)
#define REG_ADC_SCALE_GROUP_19                             (0x62U)
#define REG_ADC_SCALE_GROUP_20                             (0x63U)
#define REG_ADC_SCALE_GROUP_21                             (0x64U)
#define REG_ADC_SCALE_GROUP_22                             (0x65U)
#define REG_ADC_SCALE_GROUP_23                             (0x66U)
#define REG_ADC_SCALE_GROUP_24                             (0x67U)
#define REG_ADC_SCALE_GROUP_25                             (0x68U)
#define REG_ADC_SCALE_GROUP_26                             (0x69U)
#define REG_ADC_SCALE_GROUP_27                             (0x6AU)
#define REG_EU_GROUP_1                                     (0x70U)
#define REG_EU_GROUP_2                                     (0x71U)
#define REG_EU_GROUP_2                                     (0x72U)
#define REG_EU_GROUP_2                                     (0x73U)
#define REG_EU_GROUP_3                                     (0x74U)
#define REG_EU_GROUP_2                                     (0x75U)
#define REG_EU_GROUP_4                                     (0x76U)
#define REG_EU_GROUP_2                                     (0x77U)
#define REG_EU_GROUP_5                                     (0x78U)
#define REG_EU_GROUP_2                                     (0x79U)
#define REG_EU_GROUP_6                                     (0x7AU)
#define REG_EU_GROUP_2                                     (0x7BU)
#define REG_EU_GROUP_7                                     (0x7CU)
#define REG_EU_GROUP_2                                     (0x7DU)
#define REG_EU_GROUP_8                                     (0x7EU)
#define REG_EU_GROUP_2                                     (0x7FU)
#define REG_EU_GROUP_9                                     (0x80U)
#define REG_EU_GROUP_2                                     (0x81U)
#define REG_EU_GROUP_10                                    (0x82U)
#define REG_EU_GROUP_2                                     (0x83U)
#define REG_EU_GROUP_11                                    (0x84U)
#define REG_EU_GROUP_2                                     (0x85U)
#define REG_EU_GROUP_12                                    (0x86U)
#define REG_EU_GROUP_2                                     (0x87U)
#define REG_EU_GROUP_13                                    (0x88U)
#define REG_EU_GROUP_2                                     (0x89U)
#define REG_EU_GROUP_14                                    (0x8AU)
#define REG_DERIVED_EU_GROUP_1                             (0x90U)
#define REG_DERIVED_EU_GROUP_2                             (0x91U)
#define REG_ERROR_REGISTER                                 (0xA0U)
#define REG_FAULT_REGISTER_1                               (0xA8U)
#define REG_FAULT_REGISTER_2                               (0xA9U)
#define REG_FAULT_REGISTER_3                               (0xAAU)
#define REG_UPPER_POSITIVE_LIMITS_GROUP_1                  (0xB0U)
#define REG_UPPER_POSITIVE_LIMITS_GROUP_2                  (0xB1U)
#define REG_UPPER_POSITIVE_LIMITS_GROUP_3                  (0xB2U)
#define REG_UPPER_POSITIVE_LIMITS_GROUP_4                  (0xB3U)
#define REG_UPPER_POSITIVE_LIMITS_GROUP_5                  (0xB4U)
#define REG_UPPER_POSITIVE_LIMITS_GROUP_6                  (0xB5U)
#define REG_UPPER_POSITIVE_LIMITS_GROUP_7                  (0xB6U)
#define REG_UPPER_POSITIVE_LIMITS_GROUP_8                  (0xB7U)
#define REG_UPPER_POSITIVE_LIMITS_GROUP_9                  (0xB8U)
#define REG_UPPER_POSITIVE_LIMITS_GROUP_10                 (0xB9U)
#define REG_UPPER_POSITIVE_LIMITS_GROUP_11                 (0xBAU)
#define REG_UPPER_POSITIVE_LIMITS_GROUP_12                 (0xBBU)
#define REG_UPPER_POSITIVE_LIMITS_GROUP_13                 (0xBCU)
#define REG_UPPER_POSITIVE_LIMITS_GROUP_14                 (0xBDU)
#define REG_UPPER_POSITIVE_LIMITS_GROUP_15                 (0xBEU)
#define REG_UPPER_POSITIVE_LIMITS_GROUP_16                 (0xBFU)
#define REG_UPPER_POSITIVE_LIMITS_GROUP_17                 (0xC0U)
#define REG_UPPER_POSITIVE_LIMITS_GROUP_18                 (0xC1U)
#define REG_UPPER_POSITIVE_LIMITS_GROUP_19                 (0xC2U)
#define REG_UPPER_POSITIVE_LIMITS_GROUP_20                 (0xC3U)
#define REG_UPPER_POSITIVE_LIMITS_GROUP_21                 (0xC4U)
#define REG_UPPER_POSITIVE_LIMITS_GROUP_22                 (0xC5U)
#define REG_UPPER_POSITIVE_LIMITS_GROUP_23                 (0xC6U)
#define REG_UPPER_POSITIVE_LIMITS_GROUP_24                 (0xC7U)
#define REG_UPPER_POSITIVE_LIMITS_GROUP_25                 (0xC8U)
#define REG_UPPER_POSITIVE_LIMITS_GROUP_26                 (0xC9U)
#define REG_UPPER_POSITIVE_LIMITS_GROUP_27                 (0xCAU)
#define REG_NEGATIVE_LIMIT_GROUP_1                         (0xCBU)
#define REG_NEGATIVE_LIMIT_GROUP_2                         (0xCCU)
#define REG_NEGATIVE_LIMIT_GROUP_3                         (0xCDU)
#define REG_NEGATIVE_LIMIT_GROUP_4                         (0xCEU)
#define REG_NEGATIVE_LIMIT_GROUP_5                         (0xCFU)
#define REG_SOFT_LIMIT_FAULT_GROUP_1                       (0xD0U)
#define REG_SOFT_LIMIT_FAULT_GROUP_2                       (0xD1U)
#define REG_HARD_LIMIT_FAULT_GROUP_1                       (0xD8U)
#define REG_HARD_LIMIT_FAULT_GROUP_2                       (0xD9U)
#define REG_HW_AND_FW_REVISION                             (0x1F8U)
#define REG_WRITE_PROTECT                                  (0x1FAU)

/* ============================================================
 * READ: build_response() switch-case bodies
 * ============================================================ */

/* === READ 12-BIT TELEMETRY (0x00-0x1A) === */
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

/* === READ STATUS BIT-MAPPED (0x1B-0x1E) === */
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

/* === READ POWER ENABLE (0x1F-0x23) === */
        case 0x1FU: /* PWR_EN_GRP1 */
            { PMB_PowerEnable_t *pe = &g_regs.pwr_enable;
              data[0] = pe->grp1;
            } break;
        case 0x20U: /* PWR_EN_GRP2 */
            { PMB_PowerEnable_t *pe = &g_regs.pwr_enable;
              data[0] = pe->grp2[0];
              data[1] = pe->grp2[1];
              data[2] = pe->grp2[2];
            } break;
        case 0x21U: /* PWR_EN_GRP3 */
            { PMB_PowerEnable_t *pe = &g_regs.pwr_enable;
              data[0] = pe->grp3[0];
              data[1] = pe->grp3[1];
              data[2] = pe->grp3[2];
              data[3] = pe->grp3[3];
            } break;
        case 0x22U: /* PERIPH_INPUTS */
            { PMB_PowerEnable_t *pe = &g_regs.pwr_enable;
              data[0] = pe->periph_inputs;
            } break;
        case 0x23U: /* PWR_EN_GRP4 */
            { PMB_PowerEnable_t *pe = &g_regs.pwr_enable;
              data[0] = pe->grp4[0];
              data[1] = pe->grp4[1];
              data[2] = pe->grp4[2];
            } break;

/* === READ 16-BIT OFFSETS (0x30-0x4A) === */
        case 0x30U: /* OFS 0x30 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_offset.chg_curr_ofs);
            pack16(&data[2], &data[3], (uint16_t)r->calib_offset.batt1_curr_ofs);
            pack16(&data[4], &data[5], (uint16_t)r->calib_offset.batt2_curr_ofs);
            break;
        case 0x31U: /* OFS 0x31 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_offset.ll_imon_ofs);
            pack16(&data[2], &data[3], (uint16_t)r->calib_offset.rl_imon_ofs);
            pack16(&data[4], &data[5], (uint16_t)r->calib_offset.la_imon_ofs);
            break;
        case 0x32U: /* OFS 0x32 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_offset.ra_imon_ofs);
            pack16(&data[2], &data[3], (uint16_t)r->calib_offset.tr_imon_ofs);
            pack16(&data[4], &data[5], (uint16_t)r->calib_offset.chg_in_ofs);
            break;
        case 0x33U: /* OFS 0x33 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_offset.hv_ofs);
            pack16(&data[2], &data[3], (uint16_t)r->calib_offset.batt1_in_ofs);
            pack16(&data[4], &data[5], (uint16_t)r->calib_offset.batt2_in_ofs);
            break;
        case 0x34U: /* OFS 0x34 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_offset.nk_imon_ofs);
            pack16(&data[2], &data[3], (uint16_t)r->calib_offset.mot_bus_ofs);
            pack16(&data[4], &data[5], (uint16_t)r->calib_offset.chg_hv_ofs);
            break;
        case 0x35U: /* OFS 0x35 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_offset.user_hv_ofs);
            pack16(&data[2], &data[3], (uint16_t)r->calib_offset.user_hv_curr_ofs);
            break;
        case 0x36U: /* OFS 0x36 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_offset.pc_24v_ofs);
            pack16(&data[2], &data[3], (uint16_t)r->calib_offset.pc_24v_imon_ofs);
            break;
        case 0x37U: /* OFS 0x37 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_offset.user_24v_ofs);
            pack16(&data[2], &data[3], (uint16_t)r->calib_offset.user_24v_imon_ofs);
            break;
        case 0x38U: /* OFS 0x38 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_offset.pc_a_12v_ofs);
            pack16(&data[2], &data[3], (uint16_t)r->calib_offset.pc_a_12v_imon_ofs);
            break;
        case 0x39U: /* OFS 0x39 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_offset.pc_b_12v_ofs);
            pack16(&data[2], &data[3], (uint16_t)r->calib_offset.pc_b_12v_imon_ofs);
            break;
        case 0x3AU: /* OFS 0x3A */
            pack16(&data[0], &data[1], (uint16_t)r->calib_offset.v24_imon0_ofs);
            pack16(&data[2], &data[3], (uint16_t)r->calib_offset.v24_imon1_ofs);
            pack16(&data[4], &data[5], (uint16_t)r->calib_offset.v24_imon2_ofs);
            break;
        case 0x3BU: /* OFS 0x3B */
            pack16(&data[0], &data[1], (uint16_t)r->calib_offset.v12_imon0_ofs);
            pack16(&data[2], &data[3], (uint16_t)r->calib_offset.v12_imon1_ofs);
            pack16(&data[4], &data[5], (uint16_t)r->calib_offset.v12_imon2_ofs);
            break;
        case 0x3CU: /* OFS 0x3C */
            pack16(&data[0], &data[1], (uint16_t)r->calib_offset.v24_ofs);
            pack16(&data[2], &data[3], (uint16_t)r->calib_offset.v12_ofs);
            break;
        case 0x3DU: /* OFS 0x3D */
            pack16(&data[0], &data[1], (uint16_t)r->calib_offset.v5_ofs);
            pack16(&data[2], &data[3], (uint16_t)r->calib_offset.v5_curr_ofs);
            break;
        case 0x3EU: /* OFS 0x3E */
            pack16(&data[0], &data[1], (uint16_t)r->calib_offset.v3v3_ofs);
            pack16(&data[2], &data[3], (uint16_t)r->calib_offset.v3v3_curr_ofs);
            break;
        case 0x3FU: /* OFS 0x3F */
            pack16(&data[0], &data[1], (uint16_t)r->calib_offset.aux_24v_ofs);
            pack16(&data[2], &data[3], (uint16_t)r->calib_offset.aux_24v_curr_ofs);
            break;
        case 0x40U: /* OFS 0x40 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_offset.aux_12v_ofs);
            pack16(&data[2], &data[3], (uint16_t)r->calib_offset.aux_12v_curr_ofs);
            break;
        case 0x41U: /* OFS 0x41 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_offset.estop_rec_a_12v_ofs);
            pack16(&data[2], &data[3], (uint16_t)r->calib_offset.estop_rec_b_12v_ofs);
            break;
        case 0x42U: /* OFS 0x42 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_offset.v12_b_imon3_ofs);
            pack16(&data[2], &data[3], (uint16_t)r->calib_offset.v12_b_imon4_ofs);
            break;
        case 0x43U: /* OFS 0x43 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_offset.hv_or_ofs);
            pack16(&data[2], &data[3], (uint16_t)r->calib_offset.mb_sp_imon_ofs);
            break;
        case 0x44U: /* OFS 0x44 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_offset.temp0_ofs);
            pack16(&data[2], &data[3], (uint16_t)r->calib_offset.temp1_ofs);
            pack16(&data[4], &data[5], (uint16_t)r->calib_offset.temp2_ofs);
            break;
        case 0x45U: /* OFS 0x45 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_offset.temp3_ofs);
            pack16(&data[2], &data[3], (uint16_t)r->calib_offset.temp4_ofs);
            pack16(&data[4], &data[5], (uint16_t)r->calib_offset.temp5_ofs);
            break;
        case 0x46U: /* OFS 0x46 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_offset.ex_temp0_ofs);
            pack16(&data[2], &data[3], (uint16_t)r->calib_offset.ex_temp1_ofs);
            break;
        case 0x47U: /* OFS 0x47 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_offset.lidar_a_12v_ofs);
            pack16(&data[2], &data[3], (uint16_t)r->calib_offset.lidar_b_12v_ofs);
            break;
        case 0x48U: /* OFS 0x48 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_offset.sto_24v_ofs);
            pack16(&data[2], &data[3], (uint16_t)r->calib_offset.chg_hvil_out_ofs);
            break;
        case 0x49U: /* OFS 0x49 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_offset.eth_2v5_ofs);
            pack16(&data[2], &data[3], (uint16_t)r->calib_offset.eth_1v2_ofs);
            break;
        case 0x4AU: /* OFS 0x4A */
            pack16(&data[0], &data[1], (uint16_t)r->calib_offset.ecat_3v3_ofs);
            pack16(&data[2], &data[3], (uint16_t)r->calib_offset.ecat_1v2_ofs);
            break;

/* === READ 16-BIT SCALES (0x50-0x6A) === */
        case 0x50U: /* SCL 0x50 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_scale.chg_curr_scl);
            pack16(&data[2], &data[3], (uint16_t)r->calib_scale.batt1_curr_scl);
            pack16(&data[4], &data[5], (uint16_t)r->calib_scale.batt2_curr_scl);
            break;
        case 0x51U: /* SCL 0x51 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_scale.ll_imon_scl);
            pack16(&data[2], &data[3], (uint16_t)r->calib_scale.rl_imon_scl);
            pack16(&data[4], &data[5], (uint16_t)r->calib_scale.la_imon_scl);
            break;
        case 0x52U: /* SCL 0x52 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_scale.ra_imon_scl);
            pack16(&data[2], &data[3], (uint16_t)r->calib_scale.tr_imon_scl);
            pack16(&data[4], &data[5], (uint16_t)r->calib_scale.chg_in_scl);
            break;
        case 0x53U: /* SCL 0x53 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_scale.hv_scl);
            pack16(&data[2], &data[3], (uint16_t)r->calib_scale.batt1_in_scl);
            pack16(&data[4], &data[5], (uint16_t)r->calib_scale.batt2_in_scl);
            break;
        case 0x54U: /* SCL 0x54 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_scale.nk_imon_scl);
            pack16(&data[2], &data[3], (uint16_t)r->calib_scale.mot_bus_scl);
            pack16(&data[4], &data[5], (uint16_t)r->calib_scale.chg_hv_scl);
            break;
        case 0x55U: /* SCL 0x55 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_scale.user_hv_scl);
            pack16(&data[2], &data[3], (uint16_t)r->calib_scale.user_hv_curr_scl);
            break;
        case 0x56U: /* SCL 0x56 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_scale.pc_24v_scl);
            pack16(&data[2], &data[3], (uint16_t)r->calib_scale.pc_24v_imon_scl);
            break;
        case 0x57U: /* SCL 0x57 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_scale.user_24v_scl);
            pack16(&data[2], &data[3], (uint16_t)r->calib_scale.user_24v_imon_scl);
            break;
        case 0x58U: /* SCL 0x58 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_scale.pc_a_12v_scl);
            pack16(&data[2], &data[3], (uint16_t)r->calib_scale.pc_a_12v_imon_scl);
            break;
        case 0x59U: /* SCL 0x59 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_scale.pc_b_12v_scl);
            pack16(&data[2], &data[3], (uint16_t)r->calib_scale.pc_b_12v_imon_scl);
            break;
        case 0x5AU: /* SCL 0x5A */
            pack16(&data[0], &data[1], (uint16_t)r->calib_scale.v24_imon0_scl);
            pack16(&data[2], &data[3], (uint16_t)r->calib_scale.v24_imon1_scl);
            pack16(&data[4], &data[5], (uint16_t)r->calib_scale.v24_imon2_scl);
            break;
        case 0x5BU: /* SCL 0x5B */
            pack16(&data[0], &data[1], (uint16_t)r->calib_scale.v12_imon0_scl);
            pack16(&data[2], &data[3], (uint16_t)r->calib_scale.v12_imon1_scl);
            pack16(&data[4], &data[5], (uint16_t)r->calib_scale.v12_imon2_scl);
            break;
        case 0x5CU: /* SCL 0x5C */
            pack16(&data[0], &data[1], (uint16_t)r->calib_scale.v24_scl);
            pack16(&data[2], &data[3], (uint16_t)r->calib_scale.v12_scl);
            break;
        case 0x5DU: /* SCL 0x5D */
            pack16(&data[0], &data[1], (uint16_t)r->calib_scale.v5_scl);
            pack16(&data[2], &data[3], (uint16_t)r->calib_scale.v5_curr_scl);
            break;
        case 0x5EU: /* SCL 0x5E */
            pack16(&data[0], &data[1], (uint16_t)r->calib_scale.v3v3_scl);
            pack16(&data[2], &data[3], (uint16_t)r->calib_scale.v3v3_curr_scl);
            break;
        case 0x5FU: /* SCL 0x5F */
            pack16(&data[0], &data[1], (uint16_t)r->calib_scale.aux_24v_scl);
            pack16(&data[2], &data[3], (uint16_t)r->calib_scale.aux_24v_curr_scl);
            break;
        case 0x60U: /* SCL 0x60 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_scale.aux_12v_scl);
            pack16(&data[2], &data[3], (uint16_t)r->calib_scale.aux_12v_curr_scl);
            break;
        case 0x61U: /* SCL 0x61 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_scale.estop_rec_a_12v_scl);
            pack16(&data[2], &data[3], (uint16_t)r->calib_scale.estop_rec_b_12v_scl);
            break;
        case 0x62U: /* SCL 0x62 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_scale.v12_b_imon3_scl);
            pack16(&data[2], &data[3], (uint16_t)r->calib_scale.v12_b_imon4_scl);
            break;
        case 0x63U: /* SCL 0x63 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_scale.hv_or_scl);
            pack16(&data[2], &data[3], (uint16_t)r->calib_scale.mb_sp_imon_scl);
            break;
        case 0x64U: /* SCL 0x64 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_scale.temp0_scl);
            pack16(&data[2], &data[3], (uint16_t)r->calib_scale.temp1_scl);
            pack16(&data[4], &data[5], (uint16_t)r->calib_scale.temp2_scl);
            break;
        case 0x65U: /* SCL 0x65 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_scale.temp3_scl);
            pack16(&data[2], &data[3], (uint16_t)r->calib_scale.temp4_scl);
            pack16(&data[4], &data[5], (uint16_t)r->calib_scale.temp5_scl);
            break;
        case 0x66U: /* SCL 0x66 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_scale.ex_temp0_scl);
            pack16(&data[2], &data[3], (uint16_t)r->calib_scale.ex_temp1_scl);
            break;
        case 0x67U: /* SCL 0x67 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_scale.lidar_a_12v_scl);
            pack16(&data[2], &data[3], (uint16_t)r->calib_scale.lidar_b_12v_scl);
            break;
        case 0x68U: /* SCL 0x68 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_scale.sto_24v_scl);
            pack16(&data[2], &data[3], (uint16_t)r->calib_scale.chg_hvil_out_scl);
            break;
        case 0x69U: /* SCL 0x69 */
            pack16(&data[0], &data[1], (uint16_t)r->calib_scale.eth_2v5_scl);
            pack16(&data[2], &data[3], (uint16_t)r->calib_scale.eth_1v2_scl);
            break;
        case 0x6AU: /* SCL 0x6A */
            pack16(&data[0], &data[1], (uint16_t)r->calib_scale.ecat_3v3_scl);
            pack16(&data[2], &data[3], (uint16_t)r->calib_scale.ecat_1v2_scl);
            break;

/* === READ 16-BIT EU (0x70-0x8A) === */
        case 0x70U: /* EU 0x70 */
            pack16(&data[0], &data[1], (uint16_t)r->eu.chg_curr_eu);
            pack16(&data[2], &data[3], (uint16_t)r->eu.batt1_curr_eu);
            pack16(&data[4], &data[5], (uint16_t)r->eu.batt2_curr_eu);
            break;
        case 0x71U: /* EU 0x71 */
            pack16(&data[0], &data[1], (uint16_t)r->eu.ll_imon_eu);
            pack16(&data[2], &data[3], (uint16_t)r->eu.rl_imon_eu);
            pack16(&data[4], &data[5], (uint16_t)r->eu.la_imon_eu);
            break;
        case 0x72U: /* EU 0x72 */
            pack16(&data[0], &data[1], (uint16_t)r->eu.ra_imon_eu);
            pack16(&data[2], &data[3], (uint16_t)r->eu.tr_imon_eu);
            pack16(&data[4], &data[5], (uint16_t)r->eu.chg_in_eu);
            break;
        case 0x73U: /* EU 0x73 */
            pack16(&data[0], &data[1], (uint16_t)r->eu.hv_eu);
            pack16(&data[2], &data[3], (uint16_t)r->eu.batt1_in_eu);
            pack16(&data[4], &data[5], (uint16_t)r->eu.batt2_in_eu);
            break;
        case 0x74U: /* EU 0x74 */
            pack16(&data[0], &data[1], (uint16_t)r->eu.nk_imon_eu);
            pack16(&data[2], &data[3], (uint16_t)r->eu.mot_bus_eu);
            pack16(&data[4], &data[5], (uint16_t)r->eu.chg_hv_eu);
            break;
        case 0x75U: /* EU 0x75 */
            pack16(&data[0], &data[1], (uint16_t)r->eu.user_hv_eu);
            pack16(&data[2], &data[3], (uint16_t)r->eu.user_hv_curr_eu);
            break;
        case 0x76U: /* EU 0x76 */
            pack16(&data[0], &data[1], (uint16_t)r->eu.pc_24v_eu);
            pack16(&data[2], &data[3], (uint16_t)r->eu.pc_24v_imon_eu);
            break;
        case 0x77U: /* EU 0x77 */
            pack16(&data[0], &data[1], (uint16_t)r->eu.user_24v_eu);
            pack16(&data[2], &data[3], (uint16_t)r->eu.user_24v_imon_eu);
            break;
        case 0x78U: /* EU 0x78 */
            pack16(&data[0], &data[1], (uint16_t)r->eu.pc_a_12v_eu);
            pack16(&data[2], &data[3], (uint16_t)r->eu.pc_a_12v_imon_eu);
            break;
        case 0x79U: /* EU 0x79 */
            pack16(&data[0], &data[1], (uint16_t)r->eu.pc_b_12v_eu);
            pack16(&data[2], &data[3], (uint16_t)r->eu.pc_b_12v_imon_eu);
            break;
        case 0x7AU: /* EU 0x7A */
            pack16(&data[0], &data[1], (uint16_t)r->eu.v24_imon0_eu);
            pack16(&data[2], &data[3], (uint16_t)r->eu.v24_imon1_eu);
            pack16(&data[4], &data[5], (uint16_t)r->eu.v24_imon2_eu);
            break;
        case 0x7BU: /* EU 0x7B */
            pack16(&data[0], &data[1], (uint16_t)r->eu.v12_imon0_eu);
            pack16(&data[2], &data[3], (uint16_t)r->eu.v12_imon1_eu);
            pack16(&data[4], &data[5], (uint16_t)r->eu.v12_imon2_eu);
            break;
        case 0x7CU: /* EU 0x7C */
            pack16(&data[0], &data[1], (uint16_t)r->eu.v24_eu);
            pack16(&data[2], &data[3], (uint16_t)r->eu.v12_eu);
            break;
        case 0x7DU: /* EU 0x7D */
            pack16(&data[0], &data[1], (uint16_t)r->eu.v5_eu);
            pack16(&data[2], &data[3], (uint16_t)r->eu.v5_curr_eu);
            break;
        case 0x7EU: /* EU 0x7E */
            pack16(&data[0], &data[1], (uint16_t)r->eu.v3v3_eu);
            pack16(&data[2], &data[3], (uint16_t)r->eu.v3v3_curr_eu);
            break;
        case 0x7FU: /* EU 0x7F */
            pack16(&data[0], &data[1], (uint16_t)r->eu.aux_24v_eu);
            pack16(&data[2], &data[3], (uint16_t)r->eu.aux_24v_curr_eu);
            break;
        case 0x80U: /* EU 0x80 */
            pack16(&data[0], &data[1], (uint16_t)r->eu.aux_12v_eu);
            pack16(&data[2], &data[3], (uint16_t)r->eu.aux_12v_curr_eu);
            break;
        case 0x81U: /* EU 0x81 */
            pack16(&data[0], &data[1], (uint16_t)r->eu.estop_rec_a_12v_eu);
            pack16(&data[2], &data[3], (uint16_t)r->eu.estop_rec_b_12v_eu);
            break;
        case 0x82U: /* EU 0x82 */
            pack16(&data[0], &data[1], (uint16_t)r->eu.v12_b_imon3_eu);
            pack16(&data[2], &data[3], (uint16_t)r->eu.v12_b_imon4_eu);
            break;
        case 0x83U: /* EU 0x83 */
            pack16(&data[0], &data[1], (uint16_t)r->eu.hv_or_eu);
            pack16(&data[2], &data[3], (uint16_t)r->eu.mb_sp_imon_eu);
            break;
        case 0x84U: /* EU 0x84 */
            pack16(&data[0], &data[1], (uint16_t)r->eu.temp0_eu);
            pack16(&data[2], &data[3], (uint16_t)r->eu.temp1_eu);
            pack16(&data[4], &data[5], (uint16_t)r->eu.temp2_eu);
            break;
        case 0x85U: /* EU 0x85 */
            pack16(&data[0], &data[1], (uint16_t)r->eu.temp3_eu);
            pack16(&data[2], &data[3], (uint16_t)r->eu.temp4_eu);
            pack16(&data[4], &data[5], (uint16_t)r->eu.temp5_eu);
            break;
        case 0x86U: /* EU 0x86 */
            pack16(&data[0], &data[1], (uint16_t)r->eu.ex_temp0_eu);
            pack16(&data[2], &data[3], (uint16_t)r->eu.ex_temp1_eu);
            break;
        case 0x87U: /* EU 0x87 */
            pack16(&data[0], &data[1], (uint16_t)r->eu.lidar_a_12v_eu);
            pack16(&data[2], &data[3], (uint16_t)r->eu.lidar_b_12v_eu);
            break;
        case 0x88U: /* EU 0x88 */
            pack16(&data[0], &data[1], (uint16_t)r->eu.sto_24v_eu);
            pack16(&data[2], &data[3], (uint16_t)r->eu.chg_hvil_out_eu);
            break;
        case 0x89U: /* EU 0x89 */
            pack16(&data[0], &data[1], (uint16_t)r->eu.eth_2v5_eu);
            pack16(&data[2], &data[3], (uint16_t)r->eu.eth_1v2_eu);
            break;
        case 0x8AU: /* EU 0x8A */
            pack16(&data[0], &data[1], (uint16_t)r->eu.ecat_3v3_eu);
            pack16(&data[2], &data[3], (uint16_t)r->eu.ecat_1v2_eu);
            break;

/* === READ DERIVED EU (0x90-0x91) === */
        case 0x90U: /* DERIVED 0x90 */
            pack16(&data[0], &data[1], (uint16_t)r->eu.v12_a_total_curr_eu);
            pack16(&data[2], &data[3], (uint16_t)r->eu.v12_b_total_curr_eu);
            pack16(&data[4], &data[5], (uint16_t)r->eu.v24_total_curr_eu);
            break;
        case 0x91U: /* DERIVED 0x91 */
            pack16(&data[0], &data[1], (uint16_t)r->eu.batt_total_curr_eu);
            break;

/* === READ ERROR CODE (0xA0) === */
        case 0xA0U: /* ERROR_CODE[31:0] in bytes 4-1 (32-bit unsigned LE) */
            data[0] = (uint8_t)(g_regs.error_code         & 0xFFU);
            data[1] = (uint8_t)((g_regs.error_code >>  8U) & 0xFFU);
            data[2] = (uint8_t)((g_regs.error_code >> 16U) & 0xFFU);
            data[3] = (uint8_t)((g_regs.error_code >> 24U) & 0xFFU);
            break;

/* === READ FAULT REGISTERS (0xA8-0xAA) === */
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
            break;

/* === READ 16-BIT LIMITS (0xB0-0xCF) === */
        case 0xB0U: /* LIM 0xB0 */
            pack16(&data[0], &data[1], (uint16_t)r->soft_limits.chg_curr_lim);
            pack16(&data[2], &data[3], (uint16_t)r->soft_limits.batt1_curr_plim);
            pack16(&data[4], &data[5], (uint16_t)r->soft_limits.batt2_curr_plim);
            break;
        case 0xB1U: /* LIM 0xB1 */
            pack16(&data[0], &data[1], (uint16_t)r->soft_limits.ll_imon_lim);
            pack16(&data[2], &data[3], (uint16_t)r->soft_limits.rl_imon_lim);
            pack16(&data[4], &data[5], (uint16_t)r->soft_limits.la_imon_lim);
            break;
        case 0xB2U: /* LIM 0xB2 */
            pack16(&data[0], &data[1], (uint16_t)r->soft_limits.ra_imon_lim);
            pack16(&data[2], &data[3], (uint16_t)r->soft_limits.tr_imon_lim);
            pack16(&data[4], &data[5], (uint16_t)r->soft_limits.chg_in_lim);
            break;
        case 0xB3U: /* LIM 0xB3 */
            pack16(&data[0], &data[1], (uint16_t)r->soft_limits.hv_lim);
            pack16(&data[2], &data[3], (uint16_t)r->soft_limits.batt1_in_lim);
            pack16(&data[4], &data[5], (uint16_t)r->soft_limits.batt2_in_lim);
            break;
        case 0xB4U: /* LIM 0xB4 */
            pack16(&data[0], &data[1], (uint16_t)r->soft_limits.nk_imon_lim);
            pack16(&data[2], &data[3], (uint16_t)r->soft_limits.mot_bus_lim);
            pack16(&data[4], &data[5], (uint16_t)r->soft_limits.chg_hv_lim);
            break;
        case 0xB5U: /* LIM 0xB5 */
            pack16(&data[0], &data[1], (uint16_t)r->soft_limits.user_hv_lim);
            pack16(&data[2], &data[3], (uint16_t)r->soft_limits.user_hv_curr_lim);
            break;
        case 0xB6U: /* LIM 0xB6 */
            pack16(&data[0], &data[1], (uint16_t)r->soft_limits.pc_24v_lim);
            pack16(&data[2], &data[3], (uint16_t)r->soft_limits.pc_24v_imon_lim);
            break;
        case 0xB7U: /* LIM 0xB7 */
            pack16(&data[0], &data[1], (uint16_t)r->soft_limits.user_24v_lim);
            pack16(&data[2], &data[3], (uint16_t)r->soft_limits.user_24v_imon_lim);
            break;
        case 0xB8U: /* LIM 0xB8 */
            pack16(&data[0], &data[1], (uint16_t)r->soft_limits.pc_a_12v_lim);
            pack16(&data[2], &data[3], (uint16_t)r->soft_limits.pc_a_12v_imon_lim);
            break;
        case 0xB9U: /* LIM 0xB9 */
            pack16(&data[0], &data[1], (uint16_t)r->soft_limits.pc_b_12v_lim);
            pack16(&data[2], &data[3], (uint16_t)r->soft_limits.pc_b_12v_imon_lim);
            break;
        case 0xBAU: /* LIM 0xBA */
            pack16(&data[0], &data[1], (uint16_t)r->soft_limits.v24_imon0_lim);
            pack16(&data[2], &data[3], (uint16_t)r->soft_limits.v24_imon1_lim);
            pack16(&data[4], &data[5], (uint16_t)r->soft_limits.v24_imon2_lim);
            break;
        case 0xBBU: /* LIM 0xBB */
            pack16(&data[0], &data[1], (uint16_t)r->soft_limits.v12_imon0_lim);
            pack16(&data[2], &data[3], (uint16_t)r->soft_limits.v12_imon1_lim);
            pack16(&data[4], &data[5], (uint16_t)r->soft_limits.v12_imon2_lim);
            break;
        case 0xBCU: /* LIM 0xBC */
            pack16(&data[0], &data[1], (uint16_t)r->soft_limits.v24_lim);
            pack16(&data[2], &data[3], (uint16_t)r->soft_limits.v12_lim);
            break;
        case 0xBDU: /* LIM 0xBD */
            pack16(&data[0], &data[1], (uint16_t)r->soft_limits.v5_lim);
            pack16(&data[2], &data[3], (uint16_t)r->soft_limits.v5_curr_lim);
            break;
        case 0xBEU: /* LIM 0xBE */
            pack16(&data[0], &data[1], (uint16_t)r->soft_limits.v3v3_lim);
            pack16(&data[2], &data[3], (uint16_t)r->soft_limits.v3v3_curr_lim);
            break;
        case 0xBFU: /* LIM 0xBF */
            pack16(&data[0], &data[1], (uint16_t)r->soft_limits.aux_24v_lim);
            pack16(&data[2], &data[3], (uint16_t)r->soft_limits.aux_24v_curr_lim);
            break;
        case 0xC0U: /* LIM 0xC0 */
            pack16(&data[0], &data[1], (uint16_t)r->soft_limits.aux_12v_lim);
            pack16(&data[2], &data[3], (uint16_t)r->soft_limits.aux_12v_curr_lim);
            break;
        case 0xC1U: /* LIM 0xC1 */
            pack16(&data[0], &data[1], (uint16_t)r->soft_limits.estop_rec_a_12v_lim);
            pack16(&data[2], &data[3], (uint16_t)r->soft_limits.estop_rec_b_12v_lim);
            break;
        case 0xC2U: /* LIM 0xC2 */
            pack16(&data[0], &data[1], (uint16_t)r->soft_limits.v12_b_imon3_lim);
            pack16(&data[2], &data[3], (uint16_t)r->soft_limits.v12_b_imon4_lim);
            break;
        case 0xC3U: /* LIM 0xC3 */
            pack16(&data[0], &data[1], (uint16_t)r->soft_limits.hv_or_lim);
            pack16(&data[2], &data[3], (uint16_t)r->soft_limits.mb_sp_imon_lim);
            break;
        case 0xC4U: /* LIM 0xC4 */
            pack16(&data[0], &data[1], (uint16_t)r->soft_limits.temp0_plim);
            pack16(&data[2], &data[3], (uint16_t)r->soft_limits.temp1_plim);
            pack16(&data[4], &data[5], (uint16_t)r->soft_limits.temp2_plim);
            break;
        case 0xC5U: /* LIM 0xC5 */
            pack16(&data[0], &data[1], (uint16_t)r->soft_limits.temp3_plim);
            pack16(&data[2], &data[3], (uint16_t)r->soft_limits.temp4_plim);
            pack16(&data[4], &data[5], (uint16_t)r->soft_limits.temp5_plim);
            break;
        case 0xC6U: /* LIM 0xC6 */
            pack16(&data[0], &data[1], (uint16_t)r->soft_limits.ex_temp0_plim);
            pack16(&data[2], &data[3], (uint16_t)r->soft_limits.ex_temp1_plim);
            break;
        case 0xC7U: /* LIM 0xC7 */
            pack16(&data[0], &data[1], (uint16_t)r->soft_limits.lidar_a_12v_lim);
            pack16(&data[2], &data[3], (uint16_t)r->soft_limits.lidar_b_12v_lim);
            break;
        case 0xC8U: /* LIM 0xC8 */
            pack16(&data[0], &data[1], (uint16_t)r->soft_limits.sto_24v_lim);
            pack16(&data[2], &data[3], (uint16_t)r->soft_limits.chg_hvil_out_lim);
            break;
        case 0xC9U: /* LIM 0xC9 */
            pack16(&data[0], &data[1], (uint16_t)r->soft_limits.eth_2v5_lim);
            pack16(&data[2], &data[3], (uint16_t)r->soft_limits.eth_1v2_lim);
            break;
        case 0xCAU: /* LIM 0xCA */
            pack16(&data[0], &data[1], (uint16_t)r->soft_limits.ecat_3v3_lim);
            pack16(&data[2], &data[3], (uint16_t)r->soft_limits.ecat_1v2_lim);
            break;
        case 0xCBU: /* LIM 0xCB */
            pack16(&data[0], &data[1], (uint16_t)r->soft_limits.v12_a_total_curr_lim);
            pack16(&data[2], &data[3], (uint16_t)r->soft_limits.v12_b_total_curr_lim);
            pack16(&data[4], &data[5], (uint16_t)r->soft_limits.v24_total_curr_lim);
            break;
        case 0xCCU: /* LIM 0xCC */
            pack16(&data[2], &data[3], (uint16_t)r->soft_limits.batt1_curr_nlim);
            pack16(&data[4], &data[5], (uint16_t)r->soft_limits.batt2_curr_nlim);
            break;
        case 0xCDU: /* LIM 0xCD */
            pack16(&data[0], &data[1], (uint16_t)r->soft_limits.temp0_nlim);
            pack16(&data[2], &data[3], (uint16_t)r->soft_limits.temp1_nlim);
            pack16(&data[4], &data[5], (uint16_t)r->soft_limits.temp2_nlim);
            break;
        case 0xCEU: /* LIM 0xCE */
            pack16(&data[0], &data[1], (uint16_t)r->soft_limits.temp3_nlim);
            pack16(&data[2], &data[3], (uint16_t)r->soft_limits.temp4_nlim);
            pack16(&data[4], &data[5], (uint16_t)r->soft_limits.temp5_nlim);
            break;
        case 0xCFU: /* LIM 0xCF */
            pack16(&data[0], &data[1], (uint16_t)r->soft_limits.ex_temp0_nlim);
            pack16(&data[2], &data[3], (uint16_t)r->soft_limits.ex_temp1_nlim);
            break;

/* === READ LIMIT FAULT FLAGS (0xD0-0xD1, 0xD8-0xD9) === */
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
            break;

/* === READ BUILD INFO + WRITE PROTECT (0x1F8, 0x1FA) === */
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
            break;

/* ============================================================
 * WRITE: process_write() switch-case bodies
 * ============================================================ */

/* === WRITE POWER ENABLE (0x1F-0x23, except 0x22 read-only) === */
        case 0x1FU: /* PWR_EN_GRP1 */
            { PMB_PowerEnable_t *pe = &g_regs.pwr_enable;
              pe->grp1 = data[0];
            } break;
        case 0x20U: /* PWR_EN_GRP2 */
            { PMB_PowerEnable_t *pe = &g_regs.pwr_enable;
              pe->grp2[0] = data[0];
              pe->grp2[1] = data[1];
              pe->grp2[2] = data[2];
            } break;
        case 0x21U: /* PWR_EN_GRP3 */
            { PMB_PowerEnable_t *pe = &g_regs.pwr_enable;
              pe->grp3[0] = data[0];
              pe->grp3[1] = data[1];
              pe->grp3[2] = data[2];
              pe->grp3[3] = data[3];
            } break;
        case 0x23U: /* PWR_EN_GRP4 */
            { PMB_PowerEnable_t *pe = &g_regs.pwr_enable;
              pe->grp4[0] = data[0];
              pe->grp4[1] = data[1];
              pe->grp4[2] = data[2];
            } break;

/* === WRITE 16-BIT OFFSETS (0x30-0x4A) === */
        case 0x30U:
            r->calib_offset.chg_curr_ofs = (int16_t)unpack16(data[0], data[1]);
            r->calib_offset.batt1_curr_ofs = (int16_t)unpack16(data[2], data[3]);
            r->calib_offset.batt2_curr_ofs = (int16_t)unpack16(data[4], data[5]);
            break;
        case 0x31U:
            r->calib_offset.ll_imon_ofs = (int16_t)unpack16(data[0], data[1]);
            r->calib_offset.rl_imon_ofs = (int16_t)unpack16(data[2], data[3]);
            r->calib_offset.la_imon_ofs = (int16_t)unpack16(data[4], data[5]);
            break;
        case 0x32U:
            r->calib_offset.ra_imon_ofs = (int16_t)unpack16(data[0], data[1]);
            r->calib_offset.tr_imon_ofs = (int16_t)unpack16(data[2], data[3]);
            r->calib_offset.chg_in_ofs = (int16_t)unpack16(data[4], data[5]);
            break;
        case 0x33U:
            r->calib_offset.hv_ofs = (int16_t)unpack16(data[0], data[1]);
            r->calib_offset.batt1_in_ofs = (int16_t)unpack16(data[2], data[3]);
            r->calib_offset.batt2_in_ofs = (int16_t)unpack16(data[4], data[5]);
            break;
        case 0x34U:
            r->calib_offset.nk_imon_ofs = (int16_t)unpack16(data[0], data[1]);
            r->calib_offset.mot_bus_ofs = (int16_t)unpack16(data[2], data[3]);
            r->calib_offset.chg_hv_ofs = (int16_t)unpack16(data[4], data[5]);
            break;
        case 0x35U:
            r->calib_offset.user_hv_ofs = (int16_t)unpack16(data[0], data[1]);
            r->calib_offset.user_hv_curr_ofs = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0x36U:
            r->calib_offset.pc_24v_ofs = (int16_t)unpack16(data[0], data[1]);
            r->calib_offset.pc_24v_imon_ofs = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0x37U:
            r->calib_offset.user_24v_ofs = (int16_t)unpack16(data[0], data[1]);
            r->calib_offset.user_24v_imon_ofs = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0x38U:
            r->calib_offset.pc_a_12v_ofs = (int16_t)unpack16(data[0], data[1]);
            r->calib_offset.pc_a_12v_imon_ofs = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0x39U:
            r->calib_offset.pc_b_12v_ofs = (int16_t)unpack16(data[0], data[1]);
            r->calib_offset.pc_b_12v_imon_ofs = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0x3AU:
            r->calib_offset.v24_imon0_ofs = (int16_t)unpack16(data[0], data[1]);
            r->calib_offset.v24_imon1_ofs = (int16_t)unpack16(data[2], data[3]);
            r->calib_offset.v24_imon2_ofs = (int16_t)unpack16(data[4], data[5]);
            break;
        case 0x3BU:
            r->calib_offset.v12_imon0_ofs = (int16_t)unpack16(data[0], data[1]);
            r->calib_offset.v12_imon1_ofs = (int16_t)unpack16(data[2], data[3]);
            r->calib_offset.v12_imon2_ofs = (int16_t)unpack16(data[4], data[5]);
            break;
        case 0x3CU:
            r->calib_offset.v24_ofs = (int16_t)unpack16(data[0], data[1]);
            r->calib_offset.v12_ofs = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0x3DU:
            r->calib_offset.v5_ofs = (int16_t)unpack16(data[0], data[1]);
            r->calib_offset.v5_curr_ofs = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0x3EU:
            r->calib_offset.v3v3_ofs = (int16_t)unpack16(data[0], data[1]);
            r->calib_offset.v3v3_curr_ofs = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0x3FU:
            r->calib_offset.aux_24v_ofs = (int16_t)unpack16(data[0], data[1]);
            r->calib_offset.aux_24v_curr_ofs = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0x40U:
            r->calib_offset.aux_12v_ofs = (int16_t)unpack16(data[0], data[1]);
            r->calib_offset.aux_12v_curr_ofs = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0x41U:
            r->calib_offset.estop_rec_a_12v_ofs = (int16_t)unpack16(data[0], data[1]);
            r->calib_offset.estop_rec_b_12v_ofs = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0x42U:
            r->calib_offset.v12_b_imon3_ofs = (int16_t)unpack16(data[0], data[1]);
            r->calib_offset.v12_b_imon4_ofs = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0x43U:
            r->calib_offset.hv_or_ofs = (int16_t)unpack16(data[0], data[1]);
            r->calib_offset.mb_sp_imon_ofs = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0x44U:
            r->calib_offset.temp0_ofs = (int16_t)unpack16(data[0], data[1]);
            r->calib_offset.temp1_ofs = (int16_t)unpack16(data[2], data[3]);
            r->calib_offset.temp2_ofs = (int16_t)unpack16(data[4], data[5]);
            break;
        case 0x45U:
            r->calib_offset.temp3_ofs = (int16_t)unpack16(data[0], data[1]);
            r->calib_offset.temp4_ofs = (int16_t)unpack16(data[2], data[3]);
            r->calib_offset.temp5_ofs = (int16_t)unpack16(data[4], data[5]);
            break;
        case 0x46U:
            r->calib_offset.ex_temp0_ofs = (int16_t)unpack16(data[0], data[1]);
            r->calib_offset.ex_temp1_ofs = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0x47U:
            r->calib_offset.lidar_a_12v_ofs = (int16_t)unpack16(data[0], data[1]);
            r->calib_offset.lidar_b_12v_ofs = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0x48U:
            r->calib_offset.sto_24v_ofs = (int16_t)unpack16(data[0], data[1]);
            r->calib_offset.chg_hvil_out_ofs = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0x49U:
            r->calib_offset.eth_2v5_ofs = (int16_t)unpack16(data[0], data[1]);
            r->calib_offset.eth_1v2_ofs = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0x4AU:
            r->calib_offset.ecat_3v3_ofs = (int16_t)unpack16(data[0], data[1]);
            r->calib_offset.ecat_1v2_ofs = (int16_t)unpack16(data[2], data[3]);
            break;

/* === WRITE 16-BIT SCALES (0x50-0x6A) === */
        case 0x50U:
            r->calib_scale.chg_curr_scl = (int16_t)unpack16(data[0], data[1]);
            r->calib_scale.batt1_curr_scl = (int16_t)unpack16(data[2], data[3]);
            r->calib_scale.batt2_curr_scl = (int16_t)unpack16(data[4], data[5]);
            break;
        case 0x51U:
            r->calib_scale.ll_imon_scl = (int16_t)unpack16(data[0], data[1]);
            r->calib_scale.rl_imon_scl = (int16_t)unpack16(data[2], data[3]);
            r->calib_scale.la_imon_scl = (int16_t)unpack16(data[4], data[5]);
            break;
        case 0x52U:
            r->calib_scale.ra_imon_scl = (int16_t)unpack16(data[0], data[1]);
            r->calib_scale.tr_imon_scl = (int16_t)unpack16(data[2], data[3]);
            r->calib_scale.chg_in_scl = (int16_t)unpack16(data[4], data[5]);
            break;
        case 0x53U:
            r->calib_scale.hv_scl = (int16_t)unpack16(data[0], data[1]);
            r->calib_scale.batt1_in_scl = (int16_t)unpack16(data[2], data[3]);
            r->calib_scale.batt2_in_scl = (int16_t)unpack16(data[4], data[5]);
            break;
        case 0x54U:
            r->calib_scale.nk_imon_scl = (int16_t)unpack16(data[0], data[1]);
            r->calib_scale.mot_bus_scl = (int16_t)unpack16(data[2], data[3]);
            r->calib_scale.chg_hv_scl = (int16_t)unpack16(data[4], data[5]);
            break;
        case 0x55U:
            r->calib_scale.user_hv_scl = (int16_t)unpack16(data[0], data[1]);
            r->calib_scale.user_hv_curr_scl = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0x56U:
            r->calib_scale.pc_24v_scl = (int16_t)unpack16(data[0], data[1]);
            r->calib_scale.pc_24v_imon_scl = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0x57U:
            r->calib_scale.user_24v_scl = (int16_t)unpack16(data[0], data[1]);
            r->calib_scale.user_24v_imon_scl = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0x58U:
            r->calib_scale.pc_a_12v_scl = (int16_t)unpack16(data[0], data[1]);
            r->calib_scale.pc_a_12v_imon_scl = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0x59U:
            r->calib_scale.pc_b_12v_scl = (int16_t)unpack16(data[0], data[1]);
            r->calib_scale.pc_b_12v_imon_scl = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0x5AU:
            r->calib_scale.v24_imon0_scl = (int16_t)unpack16(data[0], data[1]);
            r->calib_scale.v24_imon1_scl = (int16_t)unpack16(data[2], data[3]);
            r->calib_scale.v24_imon2_scl = (int16_t)unpack16(data[4], data[5]);
            break;
        case 0x5BU:
            r->calib_scale.v12_imon0_scl = (int16_t)unpack16(data[0], data[1]);
            r->calib_scale.v12_imon1_scl = (int16_t)unpack16(data[2], data[3]);
            r->calib_scale.v12_imon2_scl = (int16_t)unpack16(data[4], data[5]);
            break;
        case 0x5CU:
            r->calib_scale.v24_scl = (int16_t)unpack16(data[0], data[1]);
            r->calib_scale.v12_scl = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0x5DU:
            r->calib_scale.v5_scl = (int16_t)unpack16(data[0], data[1]);
            r->calib_scale.v5_curr_scl = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0x5EU:
            r->calib_scale.v3v3_scl = (int16_t)unpack16(data[0], data[1]);
            r->calib_scale.v3v3_curr_scl = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0x5FU:
            r->calib_scale.aux_24v_scl = (int16_t)unpack16(data[0], data[1]);
            r->calib_scale.aux_24v_curr_scl = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0x60U:
            r->calib_scale.aux_12v_scl = (int16_t)unpack16(data[0], data[1]);
            r->calib_scale.aux_12v_curr_scl = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0x61U:
            r->calib_scale.estop_rec_a_12v_scl = (int16_t)unpack16(data[0], data[1]);
            r->calib_scale.estop_rec_b_12v_scl = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0x62U:
            r->calib_scale.v12_b_imon3_scl = (int16_t)unpack16(data[0], data[1]);
            r->calib_scale.v12_b_imon4_scl = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0x63U:
            r->calib_scale.hv_or_scl = (int16_t)unpack16(data[0], data[1]);
            r->calib_scale.mb_sp_imon_scl = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0x64U:
            r->calib_scale.temp0_scl = (int16_t)unpack16(data[0], data[1]);
            r->calib_scale.temp1_scl = (int16_t)unpack16(data[2], data[3]);
            r->calib_scale.temp2_scl = (int16_t)unpack16(data[4], data[5]);
            break;
        case 0x65U:
            r->calib_scale.temp3_scl = (int16_t)unpack16(data[0], data[1]);
            r->calib_scale.temp4_scl = (int16_t)unpack16(data[2], data[3]);
            r->calib_scale.temp5_scl = (int16_t)unpack16(data[4], data[5]);
            break;
        case 0x66U:
            r->calib_scale.ex_temp0_scl = (int16_t)unpack16(data[0], data[1]);
            r->calib_scale.ex_temp1_scl = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0x67U:
            r->calib_scale.lidar_a_12v_scl = (int16_t)unpack16(data[0], data[1]);
            r->calib_scale.lidar_b_12v_scl = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0x68U:
            r->calib_scale.sto_24v_scl = (int16_t)unpack16(data[0], data[1]);
            r->calib_scale.chg_hvil_out_scl = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0x69U:
            r->calib_scale.eth_2v5_scl = (int16_t)unpack16(data[0], data[1]);
            r->calib_scale.eth_1v2_scl = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0x6AU:
            r->calib_scale.ecat_3v3_scl = (int16_t)unpack16(data[0], data[1]);
            r->calib_scale.ecat_1v2_scl = (int16_t)unpack16(data[2], data[3]);
            break;

/* === WRITE 16-BIT LIMITS (0xB0-0xCF) === */
        case 0xB0U:
            r->soft_limits.chg_curr_lim = (int16_t)unpack16(data[0], data[1]);
            r->soft_limits.batt1_curr_plim = (int16_t)unpack16(data[2], data[3]);
            r->soft_limits.batt2_curr_plim = (int16_t)unpack16(data[4], data[5]);
            break;
        case 0xB1U:
            r->soft_limits.ll_imon_lim = (int16_t)unpack16(data[0], data[1]);
            r->soft_limits.rl_imon_lim = (int16_t)unpack16(data[2], data[3]);
            r->soft_limits.la_imon_lim = (int16_t)unpack16(data[4], data[5]);
            break;
        case 0xB2U:
            r->soft_limits.ra_imon_lim = (int16_t)unpack16(data[0], data[1]);
            r->soft_limits.tr_imon_lim = (int16_t)unpack16(data[2], data[3]);
            r->soft_limits.chg_in_lim = (int16_t)unpack16(data[4], data[5]);
            break;
        case 0xB3U:
            r->soft_limits.hv_lim = (int16_t)unpack16(data[0], data[1]);
            r->soft_limits.batt1_in_lim = (int16_t)unpack16(data[2], data[3]);
            r->soft_limits.batt2_in_lim = (int16_t)unpack16(data[4], data[5]);
            break;
        case 0xB4U:
            r->soft_limits.nk_imon_lim = (int16_t)unpack16(data[0], data[1]);
            r->soft_limits.mot_bus_lim = (int16_t)unpack16(data[2], data[3]);
            r->soft_limits.chg_hv_lim = (int16_t)unpack16(data[4], data[5]);
            break;
        case 0xB5U:
            r->soft_limits.user_hv_lim = (int16_t)unpack16(data[0], data[1]);
            r->soft_limits.user_hv_curr_lim = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0xB6U:
            r->soft_limits.pc_24v_lim = (int16_t)unpack16(data[0], data[1]);
            r->soft_limits.pc_24v_imon_lim = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0xB7U:
            r->soft_limits.user_24v_lim = (int16_t)unpack16(data[0], data[1]);
            r->soft_limits.user_24v_imon_lim = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0xB8U:
            r->soft_limits.pc_a_12v_lim = (int16_t)unpack16(data[0], data[1]);
            r->soft_limits.pc_a_12v_imon_lim = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0xB9U:
            r->soft_limits.pc_b_12v_lim = (int16_t)unpack16(data[0], data[1]);
            r->soft_limits.pc_b_12v_imon_lim = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0xBAU:
            r->soft_limits.v24_imon0_lim = (int16_t)unpack16(data[0], data[1]);
            r->soft_limits.v24_imon1_lim = (int16_t)unpack16(data[2], data[3]);
            r->soft_limits.v24_imon2_lim = (int16_t)unpack16(data[4], data[5]);
            break;
        case 0xBBU:
            r->soft_limits.v12_imon0_lim = (int16_t)unpack16(data[0], data[1]);
            r->soft_limits.v12_imon1_lim = (int16_t)unpack16(data[2], data[3]);
            r->soft_limits.v12_imon2_lim = (int16_t)unpack16(data[4], data[5]);
            break;
        case 0xBCU:
            r->soft_limits.v24_lim = (int16_t)unpack16(data[0], data[1]);
            r->soft_limits.v12_lim = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0xBDU:
            r->soft_limits.v5_lim = (int16_t)unpack16(data[0], data[1]);
            r->soft_limits.v5_curr_lim = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0xBEU:
            r->soft_limits.v3v3_lim = (int16_t)unpack16(data[0], data[1]);
            r->soft_limits.v3v3_curr_lim = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0xBFU:
            r->soft_limits.aux_24v_lim = (int16_t)unpack16(data[0], data[1]);
            r->soft_limits.aux_24v_curr_lim = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0xC0U:
            r->soft_limits.aux_12v_lim = (int16_t)unpack16(data[0], data[1]);
            r->soft_limits.aux_12v_curr_lim = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0xC1U:
            r->soft_limits.estop_rec_a_12v_lim = (int16_t)unpack16(data[0], data[1]);
            r->soft_limits.estop_rec_b_12v_lim = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0xC2U:
            r->soft_limits.v12_b_imon3_lim = (int16_t)unpack16(data[0], data[1]);
            r->soft_limits.v12_b_imon4_lim = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0xC3U:
            r->soft_limits.hv_or_lim = (int16_t)unpack16(data[0], data[1]);
            r->soft_limits.mb_sp_imon_lim = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0xC4U:
            r->soft_limits.temp0_plim = (int16_t)unpack16(data[0], data[1]);
            r->soft_limits.temp1_plim = (int16_t)unpack16(data[2], data[3]);
            r->soft_limits.temp2_plim = (int16_t)unpack16(data[4], data[5]);
            break;
        case 0xC5U:
            r->soft_limits.temp3_plim = (int16_t)unpack16(data[0], data[1]);
            r->soft_limits.temp4_plim = (int16_t)unpack16(data[2], data[3]);
            r->soft_limits.temp5_plim = (int16_t)unpack16(data[4], data[5]);
            break;
        case 0xC6U:
            r->soft_limits.ex_temp0_plim = (int16_t)unpack16(data[0], data[1]);
            r->soft_limits.ex_temp1_plim = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0xC7U:
            r->soft_limits.lidar_a_12v_lim = (int16_t)unpack16(data[0], data[1]);
            r->soft_limits.lidar_b_12v_lim = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0xC8U:
            r->soft_limits.sto_24v_lim = (int16_t)unpack16(data[0], data[1]);
            r->soft_limits.chg_hvil_out_lim = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0xC9U:
            r->soft_limits.eth_2v5_lim = (int16_t)unpack16(data[0], data[1]);
            r->soft_limits.eth_1v2_lim = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0xCAU:
            r->soft_limits.ecat_3v3_lim = (int16_t)unpack16(data[0], data[1]);
            r->soft_limits.ecat_1v2_lim = (int16_t)unpack16(data[2], data[3]);
            break;
        case 0xCBU:
            r->soft_limits.v12_a_total_curr_lim = (int16_t)unpack16(data[0], data[1]);
            r->soft_limits.v12_b_total_curr_lim = (int16_t)unpack16(data[2], data[3]);
            r->soft_limits.v24_total_curr_lim = (int16_t)unpack16(data[4], data[5]);
            break;
        case 0xCCU:
            r->soft_limits.batt1_curr_nlim = (int16_t)unpack16(data[2], data[3]);
            r->soft_limits.batt2_curr_nlim = (int16_t)unpack16(data[4], data[5]);
            break;
        case 0xCDU:
            r->soft_limits.temp0_nlim = (int16_t)unpack16(data[0], data[1]);
            r->soft_limits.temp1_nlim = (int16_t)unpack16(data[2], data[3]);
            r->soft_limits.temp2_nlim = (int16_t)unpack16(data[4], data[5]);
            break;
        case 0xCEU:
            r->soft_limits.temp3_nlim = (int16_t)unpack16(data[0], data[1]);
            r->soft_limits.temp4_nlim = (int16_t)unpack16(data[2], data[3]);
            r->soft_limits.temp5_nlim = (int16_t)unpack16(data[4], data[5]);
            break;
        case 0xCFU:
            r->soft_limits.ex_temp0_nlim = (int16_t)unpack16(data[0], data[1]);
            r->soft_limits.ex_temp1_nlim = (int16_t)unpack16(data[2], data[3]);
            break;

/* === WRITE FAULT REGISTERS (0xA8-0xAA): writing 1 clears === */
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
            break;

/* === WRITE LIMIT FAULT FLAGS (0xD0-0xD1, 0xD8-0xD9): clear-on-write-1 === */
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
            break;

/* === WRITE BUILD INFO + WRITE PROTECT (0x1F8, 0x1FA) === */
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

/* Summary: 161 read cases, 99 write cases, 161 active registers parsed from CSV */
