/**
 * @file pmb_loopback_test.c
 * @brief PMB CAN emulator loopback self-test — covers ALL 161 Data IDs
 *
 * Each test injects a read request directly into MO0's registers (simulating
 * what the ULEAM fixture sends), calls PMB_Emulator_Run() to process it, then
 * reads the response from MO1 and compares bytes 0-5 against expected values
 * derived directly from the current PMB_Registers_t state.
 *
 * Bytes 6-7 are the packet counter and are intentionally skipped in comparison.
 *
 * Coverage: 27 telemetry, 4 status, 5 power enable, 27 offset, 27 scale,
 *           27 EU, 2 derived EU, 1 error, 3 fault, 27 pos limits, 5 neg limits,
 *           4 limit fault flags, 1 build info, 1 write protect = 161 total.
 */

#include "pmb_loopback_test.h"
#include "pmb_can_emulator.h"
#include <string.h>

/* =========================================================================
 * INTERNAL HELPERS
 * ========================================================================= */
static void delay_cycles(uint32_t n) { volatile uint32_t i = n; while (i--) { __NOP(); } }

static void inject_read_request(uint16_t data_id)
{
    XMC_CAN_MO_t *rx = MULTICAN_CONFIG_0.lmobj_ptr[PMB_RX_MO]->mo_ptr;
    rx->can_identifier = PMB_CAN_BUILD_ID(4U, data_id, PMB_ADDR, FIXTURE_ADDR);
    rx->can_data[0] = 0U;
    rx->can_data[1] = 0U;
    XMC_CAN_MO_SetStatus(rx, XMC_CAN_MO_SET_STATUS_RX_PENDING);
}

static void read_tx_mo(uint8_t out[8])
{
    XMC_CAN_MO_t *tx = MULTICAN_CONFIG_0.lmobj_ptr[PMB_TX_MO]->mo_ptr;
    out[0]=(uint8_t)(tx->can_data[0]        &0xFFU);
    out[1]=(uint8_t)((tx->can_data[0]>> 8U)&0xFFU);
    out[2]=(uint8_t)((tx->can_data[0]>>16U)&0xFFU);
    out[3]=(uint8_t)((tx->can_data[0]>>24U)&0xFFU);
    out[4]=(uint8_t)(tx->can_data[1]        &0xFFU);
    out[5]=(uint8_t)((tx->can_data[1]>> 8U)&0xFFU);
    out[6]=(uint8_t)((tx->can_data[1]>>16U)&0xFFU);
    out[7]=(uint8_t)((tx->can_data[1]>>24U)&0xFFU);
}

static void p12(uint8_t *lo, uint8_t *hi, uint16_t v)
    { *lo=(uint8_t)(v&0xFFU); *hi=(uint8_t)((v>>8U)&0x0FU); }
static void p16(uint8_t *lo, uint8_t *hi, int16_t v)
    { *lo=(uint8_t)((uint16_t)v&0xFFU); *hi=(uint8_t)((uint16_t)v>>8U); }

/* =========================================================================
 * PRESET: non-zero values so responses are not trivially all-zeros
 * ========================================================================= */
static void preset_registers(void)
{
    PMB_Registers_t *r = PMB_GetRegs();

    /* Telemetry — distinctive non-zero raw ADC values */
    r->telemetry.chg_curr          = 0x200U;
    r->telemetry.batt1_curr        = 0x300U;
    r->telemetry.batt2_curr        = 0x310U;
    r->telemetry.ll_imon           = 0x050U;
    r->telemetry.rl_imon           = 0x051U;
    r->telemetry.la_imon           = 0x052U;
    r->telemetry.ra_imon           = 0x053U;
    r->telemetry.tr_imon           = 0x054U;
    r->telemetry.chg_in            = 0x640U;
    r->telemetry.hv                = 0x960U;
    r->telemetry.batt1_in          = 0x961U;
    r->telemetry.batt2_in          = 0x962U;
    r->telemetry.nk_imon           = 0x020U;
    r->telemetry.mot_bus           = 0x963U;
    r->telemetry.chg_hv            = 0x964U;
    r->telemetry.user_hv           = 0x965U;
    r->telemetry.user_hv_curr      = 0x030U;
    r->telemetry.pc_24v            = 0x5DCU;
    r->telemetry.pc_24v_imon       = 0x040U;
    r->telemetry.user_24v          = 0x5DDU;
    r->telemetry.user_24v_imon     = 0x041U;
    r->telemetry.pc_a_12v          = 0x2EEU;
    r->telemetry.pc_a_12v_imon     = 0x042U;
    r->telemetry.pc_b_12v          = 0x2EFU;
    r->telemetry.pc_b_12v_imon     = 0x043U;
    r->telemetry.v24_imon0         = 0x044U;
    r->telemetry.v24_imon1         = 0x045U;
    r->telemetry.v24_imon2         = 0x046U;
    r->telemetry.v12_imon0         = 0x047U;
    r->telemetry.v12_imon1         = 0x048U;
    r->telemetry.v12_imon2         = 0x049U;
    r->telemetry.v24_bus_voltage   = 0x5DEU;
    r->telemetry.v12_bus_a_voltage = 0x2F0U;
    r->telemetry.v5                = 0x13CU;
    r->telemetry.v5_curr           = 0x04AU;
    r->telemetry.v3v3              = 0x0CEU;
    r->telemetry.v3v3_curr         = 0x04BU;
    r->telemetry.aux_24v           = 0x5DFU;
    r->telemetry.aux_24v_curr      = 0x04CU;
    r->telemetry.aux_12v           = 0x2F1U;
    r->telemetry.aux_12v_curr      = 0x04DU;
    r->telemetry.estop_rec_a_12v   = 0x2F2U;
    r->telemetry.estop_rec_b_12v   = 0x2F3U;
    r->telemetry.v12_b_imon3       = 0x04EU;
    r->telemetry.v12_b_imon4       = 0x04FU;
    r->telemetry.hv_or             = 0x966U;
    r->telemetry.mb_sp_imon        = 0x021U;
    r->telemetry.temp0             = 0x0FAU;
    r->telemetry.temp1             = 0x0FBU;
    r->telemetry.temp2             = 0x0FCU;
    r->telemetry.temp3             = 0x0FDU;
    r->telemetry.temp4             = 0x0FEU;
    r->telemetry.temp5             = 0x0FFU;
    r->telemetry.ex_temp0          = 0x100U;
    r->telemetry.ex_temp1          = 0x101U;
    r->telemetry.lidar_a_12v       = 0x2F4U;
    r->telemetry.lidar_b_12v       = 0x2F5U;
    r->telemetry.sto_24v           = 0x5E0U;
    r->telemetry.chg_hvil_out      = 0x102U;
    r->telemetry.eth_2v5           = 0x09CU;
    r->telemetry.eth_1v2           = 0x04BU;
    r->telemetry.ecat_3v3          = 0x0CFU;
    r->telemetry.ecat_1v2          = 0x04CU;

    /* Status — some bits set */
    r->status.comp_pdg[0]       = 0xA5U;
    r->status.comp_pdg[1]       = 0x5AU;
    r->status.comp_pdg[2]       = 0x03U;
    r->status.pdg_fault_grp1[0] = 0x1FU;
    r->status.pdg_fault_grp1[1] = 0xFFU;
    r->status.pdg_fault_grp1[2] = 0x02U;
    r->status.pdg_fault_grp2[0] = 0xC3U;
    r->status.pdg_fault_grp2[1] = 0x01U;
    r->status.pdg_fault_grp3[0] = 0xE7U;
    r->status.pdg_fault_grp3[1] = 0x01U;

    /* Power Enable — some enables set with correct parity */
    r->pwr_enable.grp1    = 0x02U; /* ESC_EEPROM_WC1n=1, parity=0 */
    r->pwr_enable.grp2[0] = 0xFEU;
    r->pwr_enable.grp2[1] = 0x7EU;
    r->pwr_enable.grp2[2] = 0x7EU;
    r->pwr_enable.grp3[0] = 0x0EU;
    r->pwr_enable.grp3[1] = 0x0EU;
    r->pwr_enable.grp3[2] = 0x0EU;
    r->pwr_enable.grp3[3] = 0x7EU;
    r->pwr_enable.periph_inputs = 0x00U;
    r->pwr_enable.grp4[0] = 0x06U;
    r->pwr_enable.grp4[1] = 0x1EU;
    r->pwr_enable.grp4[2] = 0x1EU;

    /* Calibration offsets — non-zero signed values */
    r->calib_offset.chg_curr_ofs    = 10;
    r->calib_offset.batt1_curr_ofs  = -5;
    r->calib_offset.batt2_curr_ofs  = 7;
    r->calib_offset.ll_imon_ofs     = 3;
    r->calib_offset.rl_imon_ofs     = -3;
    r->calib_offset.la_imon_ofs     = 4;
    r->calib_offset.hv_ofs          = 100;
    r->calib_offset.temp0_ofs       = -50;
    r->calib_offset.temp1_ofs       = -48;
    r->calib_offset.temp2_ofs       = -46;

    /* Calibration scales — 1:1 for simplicity */
    r->calib_scale.chg_curr_scl     = 4095;
    r->calib_scale.batt1_curr_scl   = 4095;
    r->calib_scale.batt2_curr_scl   = 4095;
    r->calib_scale.hv_scl           = 4095;
    r->calib_scale.temp0_scl        = 4095;
    r->calib_scale.temp1_scl        = 4095;
    r->calib_scale.temp2_scl        = 4095;

    /* Soft limits */
    r->soft_limits.chg_curr_lim     = 5000;
    r->soft_limits.batt1_curr_plim  = 4000;
    r->soft_limits.batt2_curr_plim  = 4000;
    r->soft_limits.hv_lim           = 0x0FFF;
    r->soft_limits.temp0_plim       = 800;
    r->soft_limits.temp0_nlim       = -200;
    r->soft_limits.batt1_curr_nlim  = -100;
    r->soft_limits.batt2_curr_nlim  = -100;

    /* Error code */
    r->error_code = 0x12345678UL;

    /* Fault registers (some latched faults) */
    r->fault_regs.reg1    = 0x06U;
    r->fault_regs.reg2[0] = 0xFEU;
    r->fault_regs.reg2[1] = 0x00U;
    r->fault_regs.reg2[2] = 0x00U;

    /* Limit fault flags */
    r->lim_faults.soft_grp1[0] = 0xA5U;
    r->lim_faults.soft_grp1[1] = 0x5AU;
    r->lim_faults.hard_grp1[0] = 0x0FU;

    /* Build info */
    r->build_info.hw_rev = 0x0002U;
    r->build_info.fw_rev = 0x0003U;

    /* Write protect — all unlocked */
    r->write_protect.b1 = WP_LIM_BIT | WP_OFS_BIT | WP_SCL_BIT | WP_REV_BIT;
    r->write_protect.b2 = 0x01U;
    r->write_protect.b3 = 0x07U;
    r->write_protect.b4 = 0x0FU;
    r->write_protect.b5 = 0x07U;
}


/* ===== EXPECTED VALUE FUNCTIONS ===== */
static void expect_00(uint8_t e[8], PMB_Registers_t *r) {
    p12(&e[0],&e[1],r->telemetry.chg_curr);
    p12(&e[2],&e[3],r->telemetry.batt1_curr);
    p12(&e[4],&e[5],r->telemetry.batt2_curr);
}

static void expect_01(uint8_t e[8], PMB_Registers_t *r) {
    p12(&e[0],&e[1],r->telemetry.ll_imon);
    p12(&e[2],&e[3],r->telemetry.rl_imon);
    p12(&e[4],&e[5],r->telemetry.la_imon);
}

static void expect_02(uint8_t e[8], PMB_Registers_t *r) {
    p12(&e[0],&e[1],r->telemetry.ra_imon);
    p12(&e[2],&e[3],r->telemetry.tr_imon);
    p12(&e[4],&e[5],r->telemetry.chg_in);
}

static void expect_03(uint8_t e[8], PMB_Registers_t *r) {
    p12(&e[0],&e[1],r->telemetry.hv);
    p12(&e[2],&e[3],r->telemetry.batt1_in);
    p12(&e[4],&e[5],r->telemetry.batt2_in);
}

static void expect_04(uint8_t e[8], PMB_Registers_t *r) {
    p12(&e[0],&e[1],r->telemetry.nk_imon);
    p12(&e[2],&e[3],r->telemetry.mot_bus);
    p12(&e[4],&e[5],r->telemetry.chg_hv);
}

static void expect_05(uint8_t e[8], PMB_Registers_t *r) {
    p12(&e[0],&e[1],r->telemetry.user_hv);
    p12(&e[2],&e[3],r->telemetry.user_hv_curr);
}

static void expect_06(uint8_t e[8], PMB_Registers_t *r) {
    p12(&e[0],&e[1],r->telemetry.pc_24v);
    p12(&e[2],&e[3],r->telemetry.pc_24v_imon);
}

static void expect_07(uint8_t e[8], PMB_Registers_t *r) {
    p12(&e[0],&e[1],r->telemetry.user_24v);
    p12(&e[2],&e[3],r->telemetry.user_24v_imon);
}

static void expect_08(uint8_t e[8], PMB_Registers_t *r) {
    p12(&e[0],&e[1],r->telemetry.pc_a_12v);
    p12(&e[2],&e[3],r->telemetry.pc_a_12v_imon);
}

static void expect_09(uint8_t e[8], PMB_Registers_t *r) {
    p12(&e[0],&e[1],r->telemetry.pc_b_12v);
    p12(&e[2],&e[3],r->telemetry.pc_b_12v_imon);
}

static void expect_0A(uint8_t e[8], PMB_Registers_t *r) {
    p12(&e[0],&e[1],r->telemetry.v24_imon0);
    p12(&e[2],&e[3],r->telemetry.v24_imon1);
    p12(&e[4],&e[5],r->telemetry.v24_imon2);
}

static void expect_0B(uint8_t e[8], PMB_Registers_t *r) {
    p12(&e[0],&e[1],r->telemetry.v12_imon0);
    p12(&e[2],&e[3],r->telemetry.v12_imon1);
    p12(&e[4],&e[5],r->telemetry.v12_imon2);
}

static void expect_0C(uint8_t e[8], PMB_Registers_t *r) {
    p12(&e[0],&e[1],r->telemetry.v24_bus_voltage);
    p12(&e[2],&e[3],r->telemetry.v12_bus_a_voltage);
}

static void expect_0D(uint8_t e[8], PMB_Registers_t *r) {
    p12(&e[0],&e[1],r->telemetry.v5);
    p12(&e[2],&e[3],r->telemetry.v5_curr);
}

static void expect_0E(uint8_t e[8], PMB_Registers_t *r) {
    p12(&e[0],&e[1],r->telemetry.v3v3);
    p12(&e[2],&e[3],r->telemetry.v3v3_curr);
}

static void expect_0F(uint8_t e[8], PMB_Registers_t *r) {
    p12(&e[0],&e[1],r->telemetry.aux_24v);
    p12(&e[2],&e[3],r->telemetry.aux_24v_curr);
}

static void expect_10(uint8_t e[8], PMB_Registers_t *r) {
    p12(&e[0],&e[1],r->telemetry.aux_12v);
    p12(&e[2],&e[3],r->telemetry.aux_12v_curr);
}

static void expect_11(uint8_t e[8], PMB_Registers_t *r) {
    p12(&e[0],&e[1],r->telemetry.estop_rec_a_12v);
    p12(&e[2],&e[3],r->telemetry.estop_rec_b_12v);
}

static void expect_12(uint8_t e[8], PMB_Registers_t *r) {
    p12(&e[0],&e[1],r->telemetry.v12_b_imon3);
    p12(&e[2],&e[3],r->telemetry.v12_b_imon4);
}

static void expect_13(uint8_t e[8], PMB_Registers_t *r) {
    p12(&e[0],&e[1],r->telemetry.hv_or);
    p12(&e[2],&e[3],r->telemetry.mb_sp_imon);
}

static void expect_14(uint8_t e[8], PMB_Registers_t *r) {
    p12(&e[0],&e[1],r->telemetry.temp0);
    p12(&e[2],&e[3],r->telemetry.temp1);
    p12(&e[4],&e[5],r->telemetry.temp2);
}

static void expect_15(uint8_t e[8], PMB_Registers_t *r) {
    p12(&e[0],&e[1],r->telemetry.temp3);
    p12(&e[2],&e[3],r->telemetry.temp4);
    p12(&e[4],&e[5],r->telemetry.temp5);
}

static void expect_16(uint8_t e[8], PMB_Registers_t *r) {
    p12(&e[0],&e[1],r->telemetry.ex_temp0);
    p12(&e[2],&e[3],r->telemetry.ex_temp1);
}

static void expect_17(uint8_t e[8], PMB_Registers_t *r) {
    p12(&e[0],&e[1],r->telemetry.lidar_a_12v);
    p12(&e[2],&e[3],r->telemetry.lidar_b_12v);
}

static void expect_18(uint8_t e[8], PMB_Registers_t *r) {
    p12(&e[0],&e[1],r->telemetry.sto_24v);
    p12(&e[2],&e[3],r->telemetry.chg_hvil_out);
}

static void expect_19(uint8_t e[8], PMB_Registers_t *r) {
    p12(&e[0],&e[1],r->telemetry.eth_2v5);
    p12(&e[2],&e[3],r->telemetry.eth_1v2);
}

static void expect_1A(uint8_t e[8], PMB_Registers_t *r) {
    p12(&e[0],&e[1],r->telemetry.ecat_3v3);
    p12(&e[2],&e[3],r->telemetry.ecat_1v2);
}

static void expect_1B(uint8_t e[8], PMB_Registers_t *r) {
    PMB_Status_t *st = &r->status;
    e[0]=st->comp_pdg[0];
    e[1]=st->comp_pdg[1];
    e[2]=st->comp_pdg[2];
}

static void expect_1C(uint8_t e[8], PMB_Registers_t *r) {
    PMB_Status_t *st = &r->status;
    e[0]=st->pdg_fault_grp1[0];
    e[1]=st->pdg_fault_grp1[1];
    e[2]=st->pdg_fault_grp1[2];
}

static void expect_1D(uint8_t e[8], PMB_Registers_t *r) {
    PMB_Status_t *st = &r->status;
    e[0]=st->pdg_fault_grp2[0];
    e[1]=st->pdg_fault_grp2[1];
}

static void expect_1E(uint8_t e[8], PMB_Registers_t *r) {
    PMB_Status_t *st = &r->status;
    e[0]=st->pdg_fault_grp3[0];
    e[1]=st->pdg_fault_grp3[1];
}

static void expect_1F(uint8_t e[8], PMB_Registers_t *r) {
    PMB_PowerEnable_t *pe = &r->pwr_enable;
    e[0]=pe->grp1;
}

static void expect_20(uint8_t e[8], PMB_Registers_t *r) {
    PMB_PowerEnable_t *pe = &r->pwr_enable;
    e[0]=pe->grp2[0];
    e[1]=pe->grp2[1];
    e[2]=pe->grp2[2];
}

static void expect_21(uint8_t e[8], PMB_Registers_t *r) {
    PMB_PowerEnable_t *pe = &r->pwr_enable;
    e[0]=pe->grp3[0];
    e[1]=pe->grp3[1];
    e[2]=pe->grp3[2];
    e[3]=pe->grp3[3];
}

static void expect_22(uint8_t e[8], PMB_Registers_t *r) {
    PMB_PowerEnable_t *pe = &r->pwr_enable;
    e[0]=pe->periph_inputs;
}

static void expect_23(uint8_t e[8], PMB_Registers_t *r) {
    PMB_PowerEnable_t *pe = &r->pwr_enable;
    e[0]=pe->grp4[0];
    e[1]=pe->grp4[1];
    e[2]=pe->grp4[2];
}

static void expect_30(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_offset.chg_curr_ofs);
    p16(&e[2],&e[3],(int16_t)r->calib_offset.batt1_curr_ofs);
    p16(&e[4],&e[5],(int16_t)r->calib_offset.batt2_curr_ofs);
}

static void expect_31(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_offset.ll_imon_ofs);
    p16(&e[2],&e[3],(int16_t)r->calib_offset.rl_imon_ofs);
    p16(&e[4],&e[5],(int16_t)r->calib_offset.la_imon_ofs);
}

static void expect_32(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_offset.ra_imon_ofs);
    p16(&e[2],&e[3],(int16_t)r->calib_offset.tr_imon_ofs);
    p16(&e[4],&e[5],(int16_t)r->calib_offset.chg_in_ofs);
}

static void expect_33(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_offset.hv_ofs);
    p16(&e[2],&e[3],(int16_t)r->calib_offset.batt1_in_ofs);
    p16(&e[4],&e[5],(int16_t)r->calib_offset.batt2_in_ofs);
}

static void expect_34(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_offset.nk_imon_ofs);
    p16(&e[2],&e[3],(int16_t)r->calib_offset.mot_bus_ofs);
    p16(&e[4],&e[5],(int16_t)r->calib_offset.chg_hv_ofs);
}

static void expect_35(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_offset.user_hv_ofs);
    p16(&e[2],&e[3],(int16_t)r->calib_offset.user_hv_curr_ofs);
}

static void expect_36(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_offset.pc_24v_ofs);
    p16(&e[2],&e[3],(int16_t)r->calib_offset.pc_24v_imon_ofs);
}

static void expect_37(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_offset.user_24v_ofs);
    p16(&e[2],&e[3],(int16_t)r->calib_offset.user_24v_imon_ofs);
}

static void expect_38(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_offset.pc_a_12v_ofs);
    p16(&e[2],&e[3],(int16_t)r->calib_offset.pc_a_12v_imon_ofs);
}

static void expect_39(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_offset.pc_b_12v_ofs);
    p16(&e[2],&e[3],(int16_t)r->calib_offset.pc_b_12v_imon_ofs);
}

static void expect_3A(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_offset.v24_imon0_ofs);
    p16(&e[2],&e[3],(int16_t)r->calib_offset.v24_imon1_ofs);
    p16(&e[4],&e[5],(int16_t)r->calib_offset.v24_imon2_ofs);
}

static void expect_3B(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_offset.v12_imon0_ofs);
    p16(&e[2],&e[3],(int16_t)r->calib_offset.v12_imon1_ofs);
    p16(&e[4],&e[5],(int16_t)r->calib_offset.v12_imon2_ofs);
}

static void expect_3C(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_offset.v24_ofs);
    p16(&e[2],&e[3],(int16_t)r->calib_offset.v12_ofs);
}

static void expect_3D(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_offset.v5_ofs);
    p16(&e[2],&e[3],(int16_t)r->calib_offset.v5_curr_ofs);
}

static void expect_3E(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_offset.v3v3_ofs);
    p16(&e[2],&e[3],(int16_t)r->calib_offset.v3v3_curr_ofs);
}

static void expect_3F(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_offset.aux_24v_ofs);
    p16(&e[2],&e[3],(int16_t)r->calib_offset.aux_24v_curr_ofs);
}

static void expect_40(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_offset.aux_12v_ofs);
    p16(&e[2],&e[3],(int16_t)r->calib_offset.aux_12v_curr_ofs);
}

static void expect_41(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_offset.estop_rec_a_12v_ofs);
    p16(&e[2],&e[3],(int16_t)r->calib_offset.estop_rec_b_12v_ofs);
}

static void expect_42(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_offset.v12_b_imon3_ofs);
    p16(&e[2],&e[3],(int16_t)r->calib_offset.v12_b_imon4_ofs);
}

static void expect_43(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_offset.hv_or_ofs);
    p16(&e[2],&e[3],(int16_t)r->calib_offset.mb_sp_imon_ofs);
}

static void expect_44(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_offset.temp0_ofs);
    p16(&e[2],&e[3],(int16_t)r->calib_offset.temp1_ofs);
    p16(&e[4],&e[5],(int16_t)r->calib_offset.temp2_ofs);
}

static void expect_45(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_offset.temp3_ofs);
    p16(&e[2],&e[3],(int16_t)r->calib_offset.temp4_ofs);
    p16(&e[4],&e[5],(int16_t)r->calib_offset.temp5_ofs);
}

static void expect_46(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_offset.ex_temp0_ofs);
    p16(&e[2],&e[3],(int16_t)r->calib_offset.ex_temp1_ofs);
}

static void expect_47(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_offset.lidar_a_12v_ofs);
    p16(&e[2],&e[3],(int16_t)r->calib_offset.lidar_b_12v_ofs);
}

static void expect_48(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_offset.sto_24v_ofs);
    p16(&e[2],&e[3],(int16_t)r->calib_offset.chg_hvil_out_ofs);
}

static void expect_49(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_offset.eth_2v5_ofs);
    p16(&e[2],&e[3],(int16_t)r->calib_offset.eth_1v2_ofs);
}

static void expect_4A(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_offset.ecat_3v3_ofs);
    p16(&e[2],&e[3],(int16_t)r->calib_offset.ecat_1v2_ofs);
}

static void expect_50(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_scale.chg_curr_scl);
    p16(&e[2],&e[3],(int16_t)r->calib_scale.batt1_curr_scl);
    p16(&e[4],&e[5],(int16_t)r->calib_scale.batt2_curr_scl);
}

static void expect_51(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_scale.ll_imon_scl);
    p16(&e[2],&e[3],(int16_t)r->calib_scale.rl_imon_scl);
    p16(&e[4],&e[5],(int16_t)r->calib_scale.la_imon_scl);
}

static void expect_52(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_scale.ra_imon_scl);
    p16(&e[2],&e[3],(int16_t)r->calib_scale.tr_imon_scl);
    p16(&e[4],&e[5],(int16_t)r->calib_scale.chg_in_scl);
}

static void expect_53(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_scale.hv_scl);
    p16(&e[2],&e[3],(int16_t)r->calib_scale.batt1_in_scl);
    p16(&e[4],&e[5],(int16_t)r->calib_scale.batt2_in_scl);
}

static void expect_54(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_scale.nk_imon_scl);
    p16(&e[2],&e[3],(int16_t)r->calib_scale.mot_bus_scl);
    p16(&e[4],&e[5],(int16_t)r->calib_scale.chg_hv_scl);
}

static void expect_55(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_scale.user_hv_scl);
    p16(&e[2],&e[3],(int16_t)r->calib_scale.user_hv_curr_scl);
}

static void expect_56(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_scale.pc_24v_scl);
    p16(&e[2],&e[3],(int16_t)r->calib_scale.pc_24v_imon_scl);
}

static void expect_57(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_scale.user_24v_scl);
    p16(&e[2],&e[3],(int16_t)r->calib_scale.user_24v_imon_scl);
}

static void expect_58(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_scale.pc_a_12v_scl);
    p16(&e[2],&e[3],(int16_t)r->calib_scale.pc_a_12v_imon_scl);
}

static void expect_59(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_scale.pc_b_12v_scl);
    p16(&e[2],&e[3],(int16_t)r->calib_scale.pc_b_12v_imon_scl);
}

static void expect_5A(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_scale.v24_imon0_scl);
    p16(&e[2],&e[3],(int16_t)r->calib_scale.v24_imon1_scl);
    p16(&e[4],&e[5],(int16_t)r->calib_scale.v24_imon2_scl);
}

static void expect_5B(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_scale.v12_imon0_scl);
    p16(&e[2],&e[3],(int16_t)r->calib_scale.v12_imon1_scl);
    p16(&e[4],&e[5],(int16_t)r->calib_scale.v12_imon2_scl);
}

static void expect_5C(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_scale.v24_scl);
    p16(&e[2],&e[3],(int16_t)r->calib_scale.v12_scl);
}

static void expect_5D(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_scale.v5_scl);
    p16(&e[2],&e[3],(int16_t)r->calib_scale.v5_curr_scl);
}

static void expect_5E(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_scale.v3v3_scl);
    p16(&e[2],&e[3],(int16_t)r->calib_scale.v3v3_curr_scl);
}

static void expect_5F(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_scale.aux_24v_scl);
    p16(&e[2],&e[3],(int16_t)r->calib_scale.aux_24v_curr_scl);
}

static void expect_60(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_scale.aux_12v_scl);
    p16(&e[2],&e[3],(int16_t)r->calib_scale.aux_12v_curr_scl);
}

static void expect_61(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_scale.estop_rec_a_12v_scl);
    p16(&e[2],&e[3],(int16_t)r->calib_scale.estop_rec_b_12v_scl);
}

static void expect_62(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_scale.v12_b_imon3_scl);
    p16(&e[2],&e[3],(int16_t)r->calib_scale.v12_b_imon4_scl);
}

static void expect_63(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_scale.hv_or_scl);
    p16(&e[2],&e[3],(int16_t)r->calib_scale.mb_sp_imon_scl);
}

static void expect_64(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_scale.temp0_scl);
    p16(&e[2],&e[3],(int16_t)r->calib_scale.temp1_scl);
    p16(&e[4],&e[5],(int16_t)r->calib_scale.temp2_scl);
}

static void expect_65(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_scale.temp3_scl);
    p16(&e[2],&e[3],(int16_t)r->calib_scale.temp4_scl);
    p16(&e[4],&e[5],(int16_t)r->calib_scale.temp5_scl);
}

static void expect_66(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_scale.ex_temp0_scl);
    p16(&e[2],&e[3],(int16_t)r->calib_scale.ex_temp1_scl);
}

static void expect_67(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_scale.lidar_a_12v_scl);
    p16(&e[2],&e[3],(int16_t)r->calib_scale.lidar_b_12v_scl);
}

static void expect_68(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_scale.sto_24v_scl);
    p16(&e[2],&e[3],(int16_t)r->calib_scale.chg_hvil_out_scl);
}

static void expect_69(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_scale.eth_2v5_scl);
    p16(&e[2],&e[3],(int16_t)r->calib_scale.eth_1v2_scl);
}

static void expect_6A(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->calib_scale.ecat_3v3_scl);
    p16(&e[2],&e[3],(int16_t)r->calib_scale.ecat_1v2_scl);
}

static void expect_70(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->eu.chg_curr_eu);
    p16(&e[2],&e[3],(int16_t)r->eu.batt1_curr_eu);
    p16(&e[4],&e[5],(int16_t)r->eu.batt2_curr_eu);
}

static void expect_71(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->eu.ll_imon_eu);
    p16(&e[2],&e[3],(int16_t)r->eu.rl_imon_eu);
    p16(&e[4],&e[5],(int16_t)r->eu.la_imon_eu);
}

static void expect_72(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->eu.ra_imon_eu);
    p16(&e[2],&e[3],(int16_t)r->eu.tr_imon_eu);
    p16(&e[4],&e[5],(int16_t)r->eu.chg_in_eu);
}

static void expect_73(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->eu.hv_eu);
    p16(&e[2],&e[3],(int16_t)r->eu.batt1_in_eu);
    p16(&e[4],&e[5],(int16_t)r->eu.batt2_in_eu);
}

static void expect_74(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->eu.nk_imon_eu);
    p16(&e[2],&e[3],(int16_t)r->eu.mot_bus_eu);
    p16(&e[4],&e[5],(int16_t)r->eu.chg_hv_eu);
}

static void expect_75(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->eu.user_hv_eu);
    p16(&e[2],&e[3],(int16_t)r->eu.user_hv_curr_eu);
}

static void expect_76(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->eu.pc_24v_eu);
    p16(&e[2],&e[3],(int16_t)r->eu.pc_24v_imon_eu);
}

static void expect_77(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->eu.user_24v_eu);
    p16(&e[2],&e[3],(int16_t)r->eu.user_24v_imon_eu);
}

static void expect_78(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->eu.pc_a_12v_eu);
    p16(&e[2],&e[3],(int16_t)r->eu.pc_a_12v_imon_eu);
}

static void expect_79(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->eu.pc_b_12v_eu);
    p16(&e[2],&e[3],(int16_t)r->eu.pc_b_12v_imon_eu);
}

static void expect_7A(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->eu.v24_imon0_eu);
    p16(&e[2],&e[3],(int16_t)r->eu.v24_imon1_eu);
    p16(&e[4],&e[5],(int16_t)r->eu.v24_imon2_eu);
}

static void expect_7B(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->eu.v12_imon0_eu);
    p16(&e[2],&e[3],(int16_t)r->eu.v12_imon1_eu);
    p16(&e[4],&e[5],(int16_t)r->eu.v12_imon2_eu);
}

static void expect_7C(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->eu.v24_eu);
    p16(&e[2],&e[3],(int16_t)r->eu.v12_eu);
}

static void expect_7D(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->eu.v5_eu);
    p16(&e[2],&e[3],(int16_t)r->eu.v5_curr_eu);
}

static void expect_7E(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->eu.v3v3_eu);
    p16(&e[2],&e[3],(int16_t)r->eu.v3v3_curr_eu);
}

static void expect_7F(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->eu.aux_24v_eu);
    p16(&e[2],&e[3],(int16_t)r->eu.aux_24v_curr_eu);
}

static void expect_80(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->eu.aux_12v_eu);
    p16(&e[2],&e[3],(int16_t)r->eu.aux_12v_curr_eu);
}

static void expect_81(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->eu.estop_rec_a_12v_eu);
    p16(&e[2],&e[3],(int16_t)r->eu.estop_rec_b_12v_eu);
}

static void expect_82(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->eu.v12_b_imon3_eu);
    p16(&e[2],&e[3],(int16_t)r->eu.v12_b_imon4_eu);
}

static void expect_83(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->eu.hv_or_eu);
    p16(&e[2],&e[3],(int16_t)r->eu.mb_sp_imon_eu);
}

static void expect_84(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->eu.temp0_eu);
    p16(&e[2],&e[3],(int16_t)r->eu.temp1_eu);
    p16(&e[4],&e[5],(int16_t)r->eu.temp2_eu);
}

static void expect_85(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->eu.temp3_eu);
    p16(&e[2],&e[3],(int16_t)r->eu.temp4_eu);
    p16(&e[4],&e[5],(int16_t)r->eu.temp5_eu);
}

static void expect_86(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->eu.ex_temp0_eu);
    p16(&e[2],&e[3],(int16_t)r->eu.ex_temp1_eu);
}

static void expect_87(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->eu.lidar_a_12v_eu);
    p16(&e[2],&e[3],(int16_t)r->eu.lidar_b_12v_eu);
}

static void expect_88(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->eu.sto_24v_eu);
    p16(&e[2],&e[3],(int16_t)r->eu.chg_hvil_out_eu);
}

static void expect_89(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->eu.eth_2v5_eu);
    p16(&e[2],&e[3],(int16_t)r->eu.eth_1v2_eu);
}

static void expect_8A(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->eu.ecat_3v3_eu);
    p16(&e[2],&e[3],(int16_t)r->eu.ecat_1v2_eu);
}

static void expect_90(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],r->eu.v12_a_total_curr_eu);
    p16(&e[2],&e[3],r->eu.v12_b_total_curr_eu);
    p16(&e[4],&e[5],r->eu.v24_total_curr_eu);
}

static void expect_91(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],r->eu.batt_total_curr_eu);
}

static void expect_A0(uint8_t e[8], PMB_Registers_t *r) {
    e[0]=(uint8_t)(r->error_code&0xFFU);
    e[1]=(uint8_t)((r->error_code>>8U)&0xFFU);
    e[2]=(uint8_t)((r->error_code>>16U)&0xFFU);
    e[3]=(uint8_t)((r->error_code>>24U)&0xFFU);
}

static void expect_A8(uint8_t e[8], PMB_Registers_t *r) {
    PMB_FaultRegs_t *fr = &r->fault_regs;
    e[0]=fr->reg1;
}

static void expect_A9(uint8_t e[8], PMB_Registers_t *r) {
    PMB_FaultRegs_t *fr = &r->fault_regs;
    e[0]=fr->reg2[0];
    e[1]=fr->reg2[1];
    e[2]=fr->reg2[2];
}

static void expect_AA(uint8_t e[8], PMB_Registers_t *r) {
    PMB_FaultRegs_t *fr = &r->fault_regs;
    e[0]=fr->reg3[0];
    e[1]=fr->reg3[1];
    e[2]=fr->reg3[2];
    e[3]=fr->reg3[3];
}

static void expect_B0(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->soft_limits.chg_curr_lim);
    p16(&e[2],&e[3],(int16_t)r->soft_limits.batt1_curr_plim);
    p16(&e[4],&e[5],(int16_t)r->soft_limits.batt2_curr_plim);
}

static void expect_B1(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->soft_limits.ll_imon_lim);
    p16(&e[2],&e[3],(int16_t)r->soft_limits.rl_imon_lim);
    p16(&e[4],&e[5],(int16_t)r->soft_limits.la_imon_lim);
}

static void expect_B2(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->soft_limits.ra_imon_lim);
    p16(&e[2],&e[3],(int16_t)r->soft_limits.tr_imon_lim);
    p16(&e[4],&e[5],(int16_t)r->soft_limits.chg_in_lim);
}

static void expect_B3(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->soft_limits.hv_lim);
    p16(&e[2],&e[3],(int16_t)r->soft_limits.batt1_in_lim);
    p16(&e[4],&e[5],(int16_t)r->soft_limits.batt2_in_lim);
}

static void expect_B4(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->soft_limits.nk_imon_lim);
    p16(&e[2],&e[3],(int16_t)r->soft_limits.mot_bus_lim);
    p16(&e[4],&e[5],(int16_t)r->soft_limits.chg_hv_lim);
}

static void expect_B5(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->soft_limits.user_hv_lim);
    p16(&e[2],&e[3],(int16_t)r->soft_limits.user_hv_curr_lim);
}

static void expect_B6(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->soft_limits.pc_24v_lim);
    p16(&e[2],&e[3],(int16_t)r->soft_limits.pc_24v_imon_lim);
}

static void expect_B7(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->soft_limits.user_24v_lim);
    p16(&e[2],&e[3],(int16_t)r->soft_limits.user_24v_imon_lim);
}

static void expect_B8(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->soft_limits.pc_a_12v_lim);
    p16(&e[2],&e[3],(int16_t)r->soft_limits.pc_a_12v_imon_lim);
}

static void expect_B9(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->soft_limits.pc_b_12v_lim);
    p16(&e[2],&e[3],(int16_t)r->soft_limits.pc_b_12v_imon_lim);
}

static void expect_BA(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->soft_limits.v24_imon0_lim);
    p16(&e[2],&e[3],(int16_t)r->soft_limits.v24_imon1_lim);
    p16(&e[4],&e[5],(int16_t)r->soft_limits.v24_imon2_lim);
}

static void expect_BB(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->soft_limits.v12_imon0_lim);
    p16(&e[2],&e[3],(int16_t)r->soft_limits.v12_imon1_lim);
    p16(&e[4],&e[5],(int16_t)r->soft_limits.v12_imon2_lim);
}

static void expect_BC(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->soft_limits.v24_lim);
    p16(&e[2],&e[3],(int16_t)r->soft_limits.v12_lim);
}

static void expect_BD(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->soft_limits.v5_lim);
    p16(&e[2],&e[3],(int16_t)r->soft_limits.v5_curr_lim);
}

static void expect_BE(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->soft_limits.v3v3_lim);
    p16(&e[2],&e[3],(int16_t)r->soft_limits.v3v3_curr_lim);
}

static void expect_BF(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->soft_limits.aux_24v_lim);
    p16(&e[2],&e[3],(int16_t)r->soft_limits.aux_24v_curr_lim);
}

static void expect_C0(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->soft_limits.aux_12v_lim);
    p16(&e[2],&e[3],(int16_t)r->soft_limits.aux_12v_curr_lim);
}

static void expect_C1(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->soft_limits.estop_rec_a_12v_lim);
    p16(&e[2],&e[3],(int16_t)r->soft_limits.estop_rec_b_12v_lim);
}

static void expect_C2(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->soft_limits.v12_b_imon3_lim);
    p16(&e[2],&e[3],(int16_t)r->soft_limits.v12_b_imon4_lim);
}

static void expect_C3(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->soft_limits.hv_or_lim);
    p16(&e[2],&e[3],(int16_t)r->soft_limits.mb_sp_imon_lim);
}

static void expect_C4(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->soft_limits.temp0_plim);
    p16(&e[2],&e[3],(int16_t)r->soft_limits.temp1_plim);
    p16(&e[4],&e[5],(int16_t)r->soft_limits.temp2_plim);
}

static void expect_C5(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->soft_limits.temp3_plim);
    p16(&e[2],&e[3],(int16_t)r->soft_limits.temp4_plim);
    p16(&e[4],&e[5],(int16_t)r->soft_limits.temp5_plim);
}

static void expect_C6(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->soft_limits.ex_temp0_plim);
    p16(&e[2],&e[3],(int16_t)r->soft_limits.ex_temp1_plim);
}

static void expect_C7(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->soft_limits.lidar_a_12v_lim);
    p16(&e[2],&e[3],(int16_t)r->soft_limits.lidar_b_12v_lim);
}

static void expect_C8(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->soft_limits.sto_24v_lim);
    p16(&e[2],&e[3],(int16_t)r->soft_limits.chg_hvil_out_lim);
}

static void expect_C9(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->soft_limits.eth_2v5_lim);
    p16(&e[2],&e[3],(int16_t)r->soft_limits.eth_1v2_lim);
}

static void expect_CA(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->soft_limits.ecat_3v3_lim);
    p16(&e[2],&e[3],(int16_t)r->soft_limits.ecat_1v2_lim);
}

static void expect_CB(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->soft_limits.v12_a_total_curr_lim);
    p16(&e[2],&e[3],(int16_t)r->soft_limits.v12_b_total_curr_lim);
    p16(&e[4],&e[5],(int16_t)r->soft_limits.v24_total_curr_lim);
}

static void expect_CC(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[2],&e[3],(int16_t)r->soft_limits.batt1_curr_nlim);
    p16(&e[4],&e[5],(int16_t)r->soft_limits.batt2_curr_nlim);
}

static void expect_CD(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->soft_limits.temp0_nlim);
    p16(&e[2],&e[3],(int16_t)r->soft_limits.temp1_nlim);
    p16(&e[4],&e[5],(int16_t)r->soft_limits.temp2_nlim);
}

static void expect_CE(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->soft_limits.temp3_nlim);
    p16(&e[2],&e[3],(int16_t)r->soft_limits.temp4_nlim);
    p16(&e[4],&e[5],(int16_t)r->soft_limits.temp5_nlim);
}

static void expect_CF(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->soft_limits.ex_temp0_nlim);
    p16(&e[2],&e[3],(int16_t)r->soft_limits.ex_temp1_nlim);
}

static void expect_D0(uint8_t e[8], PMB_Registers_t *r) {
    PMB_LimFaults_t *lf = &r->lim_faults;
    e[0]=lf->soft_grp1[0];
    e[1]=lf->soft_grp1[1];
    e[2]=lf->soft_grp1[2];
    e[3]=lf->soft_grp1[3];
    e[4]=lf->soft_grp1[4];
    e[5]=lf->soft_grp1[5];
}

static void expect_D1(uint8_t e[8], PMB_Registers_t *r) {
    PMB_LimFaults_t *lf = &r->lim_faults;
    e[0]=lf->soft_grp2[0];
    e[1]=lf->soft_grp2[1];
    e[2]=lf->soft_grp2[2];
}

static void expect_D8(uint8_t e[8], PMB_Registers_t *r) {
    PMB_LimFaults_t *lf = &r->lim_faults;
    e[0]=lf->hard_grp1[0];
    e[1]=lf->hard_grp1[1];
    e[2]=lf->hard_grp1[2];
    e[3]=lf->hard_grp1[3];
    e[4]=lf->hard_grp1[4];
    e[5]=lf->hard_grp1[5];
}

static void expect_D9(uint8_t e[8], PMB_Registers_t *r) {
    PMB_LimFaults_t *lf = &r->lim_faults;
    e[0]=lf->hard_grp2[0];
    e[1]=lf->hard_grp2[1];
    e[2]=lf->hard_grp2[2];
}

static void expect_1F8(uint8_t e[8], PMB_Registers_t *r) {
    p16(&e[0],&e[1],(int16_t)r->build_info.hw_rev);
    p16(&e[2],&e[3],(int16_t)r->build_info.fw_rev);
}

static void expect_1FA(uint8_t e[8], PMB_Registers_t *r) {
    e[0]=r->write_protect.b1; e[1]=r->write_protect.b2;
    e[2]=r->write_protect.b3; e[3]=r->write_protect.b4; e[4]=r->write_protect.b5;
}

/* ===== TEST TABLE: 161 cases ===== */
#define PMB_LOOPBACK_TOTAL_TESTS 161U

typedef struct { uint16_t data_id; void (*fn)(uint8_t e[8], PMB_Registers_t *r); } TC_t;
static const TC_t k_tests[] = {
    { 0x00U, expect_00 },  /* Telem 0x00 */
    { 0x01U, expect_01 },  /* Telem 0x01 */
    { 0x02U, expect_02 },  /* Telem 0x02 */
    { 0x03U, expect_03 },  /* Telem 0x03 */
    { 0x04U, expect_04 },  /* Telem 0x04 */
    { 0x05U, expect_05 },  /* Telem 0x05 */
    { 0x06U, expect_06 },  /* Telem 0x06 */
    { 0x07U, expect_07 },  /* Telem 0x07 */
    { 0x08U, expect_08 },  /* Telem 0x08 */
    { 0x09U, expect_09 },  /* Telem 0x09 */
    { 0x0AU, expect_0A },  /* Telem 0x0A */
    { 0x0BU, expect_0B },  /* Telem 0x0B */
    { 0x0CU, expect_0C },  /* Telem 0x0C */
    { 0x0DU, expect_0D },  /* Telem 0x0D */
    { 0x0EU, expect_0E },  /* Telem 0x0E */
    { 0x0FU, expect_0F },  /* Telem 0x0F */
    { 0x10U, expect_10 },  /* Telem 0x10 */
    { 0x11U, expect_11 },  /* Telem 0x11 */
    { 0x12U, expect_12 },  /* Telem 0x12 */
    { 0x13U, expect_13 },  /* Telem 0x13 */
    { 0x14U, expect_14 },  /* Telem 0x14 */
    { 0x15U, expect_15 },  /* Telem 0x15 */
    { 0x16U, expect_16 },  /* Telem 0x16 */
    { 0x17U, expect_17 },  /* Telem 0x17 */
    { 0x18U, expect_18 },  /* Telem 0x18 */
    { 0x19U, expect_19 },  /* Telem 0x19 */
    { 0x1AU, expect_1A },  /* Telem 0x1A */
    { 0x1BU, expect_1B },  /* COMP_PDG */
    { 0x1CU, expect_1C },  /* PDG_FAULT_GRP1 */
    { 0x1DU, expect_1D },  /* PDG_FAULT_GRP2 */
    { 0x1EU, expect_1E },  /* PDG_FAULT_GRP3 */
    { 0x1FU, expect_1F },  /* PWR_EN_GRP1 */
    { 0x20U, expect_20 },  /* PWR_EN_GRP2 */
    { 0x21U, expect_21 },  /* PWR_EN_GRP3 */
    { 0x22U, expect_22 },  /* PERIPH_INPUTS */
    { 0x23U, expect_23 },  /* PWR_EN_GRP4 */
    { 0x30U, expect_30 },  /* OFS 0x30 */
    { 0x31U, expect_31 },  /* OFS 0x31 */
    { 0x32U, expect_32 },  /* OFS 0x32 */
    { 0x33U, expect_33 },  /* OFS 0x33 */
    { 0x34U, expect_34 },  /* OFS 0x34 */
    { 0x35U, expect_35 },  /* OFS 0x35 */
    { 0x36U, expect_36 },  /* OFS 0x36 */
    { 0x37U, expect_37 },  /* OFS 0x37 */
    { 0x38U, expect_38 },  /* OFS 0x38 */
    { 0x39U, expect_39 },  /* OFS 0x39 */
    { 0x3AU, expect_3A },  /* OFS 0x3A */
    { 0x3BU, expect_3B },  /* OFS 0x3B */
    { 0x3CU, expect_3C },  /* OFS 0x3C */
    { 0x3DU, expect_3D },  /* OFS 0x3D */
    { 0x3EU, expect_3E },  /* OFS 0x3E */
    { 0x3FU, expect_3F },  /* OFS 0x3F */
    { 0x40U, expect_40 },  /* OFS 0x40 */
    { 0x41U, expect_41 },  /* OFS 0x41 */
    { 0x42U, expect_42 },  /* OFS 0x42 */
    { 0x43U, expect_43 },  /* OFS 0x43 */
    { 0x44U, expect_44 },  /* OFS 0x44 */
    { 0x45U, expect_45 },  /* OFS 0x45 */
    { 0x46U, expect_46 },  /* OFS 0x46 */
    { 0x47U, expect_47 },  /* OFS 0x47 */
    { 0x48U, expect_48 },  /* OFS 0x48 */
    { 0x49U, expect_49 },  /* OFS 0x49 */
    { 0x4AU, expect_4A },  /* OFS 0x4A */
    { 0x50U, expect_50 },  /* SCL 0x50 */
    { 0x51U, expect_51 },  /* SCL 0x51 */
    { 0x52U, expect_52 },  /* SCL 0x52 */
    { 0x53U, expect_53 },  /* SCL 0x53 */
    { 0x54U, expect_54 },  /* SCL 0x54 */
    { 0x55U, expect_55 },  /* SCL 0x55 */
    { 0x56U, expect_56 },  /* SCL 0x56 */
    { 0x57U, expect_57 },  /* SCL 0x57 */
    { 0x58U, expect_58 },  /* SCL 0x58 */
    { 0x59U, expect_59 },  /* SCL 0x59 */
    { 0x5AU, expect_5A },  /* SCL 0x5A */
    { 0x5BU, expect_5B },  /* SCL 0x5B */
    { 0x5CU, expect_5C },  /* SCL 0x5C */
    { 0x5DU, expect_5D },  /* SCL 0x5D */
    { 0x5EU, expect_5E },  /* SCL 0x5E */
    { 0x5FU, expect_5F },  /* SCL 0x5F */
    { 0x60U, expect_60 },  /* SCL 0x60 */
    { 0x61U, expect_61 },  /* SCL 0x61 */
    { 0x62U, expect_62 },  /* SCL 0x62 */
    { 0x63U, expect_63 },  /* SCL 0x63 */
    { 0x64U, expect_64 },  /* SCL 0x64 */
    { 0x65U, expect_65 },  /* SCL 0x65 */
    { 0x66U, expect_66 },  /* SCL 0x66 */
    { 0x67U, expect_67 },  /* SCL 0x67 */
    { 0x68U, expect_68 },  /* SCL 0x68 */
    { 0x69U, expect_69 },  /* SCL 0x69 */
    { 0x6AU, expect_6A },  /* SCL 0x6A */
    { 0x70U, expect_70 },  /* EU 0x70 */
    { 0x71U, expect_71 },  /* EU 0x71 */
    { 0x72U, expect_72 },  /* EU 0x72 */
    { 0x73U, expect_73 },  /* EU 0x73 */
    { 0x74U, expect_74 },  /* EU 0x74 */
    { 0x75U, expect_75 },  /* EU 0x75 */
    { 0x76U, expect_76 },  /* EU 0x76 */
    { 0x77U, expect_77 },  /* EU 0x77 */
    { 0x78U, expect_78 },  /* EU 0x78 */
    { 0x79U, expect_79 },  /* EU 0x79 */
    { 0x7AU, expect_7A },  /* EU 0x7A */
    { 0x7BU, expect_7B },  /* EU 0x7B */
    { 0x7CU, expect_7C },  /* EU 0x7C */
    { 0x7DU, expect_7D },  /* EU 0x7D */
    { 0x7EU, expect_7E },  /* EU 0x7E */
    { 0x7FU, expect_7F },  /* EU 0x7F */
    { 0x80U, expect_80 },  /* EU 0x80 */
    { 0x81U, expect_81 },  /* EU 0x81 */
    { 0x82U, expect_82 },  /* EU 0x82 */
    { 0x83U, expect_83 },  /* EU 0x83 */
    { 0x84U, expect_84 },  /* EU 0x84 */
    { 0x85U, expect_85 },  /* EU 0x85 */
    { 0x86U, expect_86 },  /* EU 0x86 */
    { 0x87U, expect_87 },  /* EU 0x87 */
    { 0x88U, expect_88 },  /* EU 0x88 */
    { 0x89U, expect_89 },  /* EU 0x89 */
    { 0x8AU, expect_8A },  /* EU 0x8A */
    { 0x90U, expect_90 },  /* Derived EU Group 1 */
    { 0x91U, expect_91 },  /* Derived EU Group 2 */
    { 0xA0U, expect_A0 },  /* Error Register */
    { 0xA8U, expect_A8 },  /* FAULT_REG1 */
    { 0xA9U, expect_A9 },  /* FAULT_REG2 */
    { 0xAAU, expect_AA },  /* FAULT_REG3 */
    { 0xB0U, expect_B0 },  /* LIM 0xB0 */
    { 0xB1U, expect_B1 },  /* LIM 0xB1 */
    { 0xB2U, expect_B2 },  /* LIM 0xB2 */
    { 0xB3U, expect_B3 },  /* LIM 0xB3 */
    { 0xB4U, expect_B4 },  /* LIM 0xB4 */
    { 0xB5U, expect_B5 },  /* LIM 0xB5 */
    { 0xB6U, expect_B6 },  /* LIM 0xB6 */
    { 0xB7U, expect_B7 },  /* LIM 0xB7 */
    { 0xB8U, expect_B8 },  /* LIM 0xB8 */
    { 0xB9U, expect_B9 },  /* LIM 0xB9 */
    { 0xBAU, expect_BA },  /* LIM 0xBA */
    { 0xBBU, expect_BB },  /* LIM 0xBB */
    { 0xBCU, expect_BC },  /* LIM 0xBC */
    { 0xBDU, expect_BD },  /* LIM 0xBD */
    { 0xBEU, expect_BE },  /* LIM 0xBE */
    { 0xBFU, expect_BF },  /* LIM 0xBF */
    { 0xC0U, expect_C0 },  /* LIM 0xC0 */
    { 0xC1U, expect_C1 },  /* LIM 0xC1 */
    { 0xC2U, expect_C2 },  /* LIM 0xC2 */
    { 0xC3U, expect_C3 },  /* LIM 0xC3 */
    { 0xC4U, expect_C4 },  /* LIM 0xC4 */
    { 0xC5U, expect_C5 },  /* LIM 0xC5 */
    { 0xC6U, expect_C6 },  /* LIM 0xC6 */
    { 0xC7U, expect_C7 },  /* LIM 0xC7 */
    { 0xC8U, expect_C8 },  /* LIM 0xC8 */
    { 0xC9U, expect_C9 },  /* LIM 0xC9 */
    { 0xCAU, expect_CA },  /* LIM 0xCA */
    { 0xCBU, expect_CB },  /* LIM 0xCB */
    { 0xCCU, expect_CC },  /* LIM 0xCC */
    { 0xCDU, expect_CD },  /* LIM 0xCD */
    { 0xCEU, expect_CE },  /* LIM 0xCE */
    { 0xCFU, expect_CF },  /* LIM 0xCF */
    { 0xD0U, expect_D0 },  /* LimFlt 0xD0 */
    { 0xD1U, expect_D1 },  /* LimFlt 0xD1 */
    { 0xD8U, expect_D8 },  /* LimFlt 0xD8 */
    { 0xD9U, expect_D9 },  /* LimFlt 0xD9 */
    { 0x1F8U, expect_1F8 },  /* HW/FW Revision */
    { 0x1FAU, expect_1FA },  /* Write Protect */
};

/* =========================================================================
 * PUBLIC API
 * ========================================================================= */

bool PMB_LoopbackTest_Run(PMB_LoopbackResult_t *results, uint16_t *count)
{
    bool all_passed = true;
    uint16_t n = 0U;
    PMB_Registers_t *r = PMB_GetRegs();

    preset_registers();
    PMB_UpdateEU();  /* compute EU from preset calibration */

    for (uint16_t i = 0U; i < PMB_LOOPBACK_TOTAL_TESTS; i++)
    {
        PMB_LoopbackResult_t *res = &results[n];
        memset(res, 0, sizeof(*res));
        res->data_id = k_tests[i].data_id;

        /* Inject read request and let emulator process it */
        inject_read_request(k_tests[i].data_id);
        delay_cycles(2000U);
        PMB_Emulator_Run();
        delay_cycles(500U);

        /* Read response and build expected */
        read_tx_mo(res->received);
        k_tests[i].fn(res->expected, r);

        /* Compare bytes 0-5 (skip bytes 6-7 = packet counter) */
        res->passed = true;
        for (int b = 0; b < 6; b++) {
            if (res->received[b] != res->expected[b]) {
                res->passed = false;
                break;
            }
        }
        if (!res->passed) all_passed = false;
        n++;
    }

    *count = n;
    return all_passed;
}
