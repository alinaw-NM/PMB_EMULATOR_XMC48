/*******************************************************************************
 Copyright (c) 2016, Infineon Technologies AG                                 **
 All rights reserved.                                                         **
	                                                                            **
 Redistribution and use in source and binary forms, with or without           **
 modification, are permitted provided that the following conditions are met:  **
                                                                              **
 *Redistributions of source code must retain the above copyright notice,      **
 this list of conditions and the following disclaimer.                        **
 *Redistributions in binary form must reproduce the above copyright notice,   **
 this list of conditions and the following disclaimer in the documentation    **
 and/or other materials provided with the distribution.                       **
 *Neither the name of the copyright holders nor the names of its contributors **
 may be used to endorse or promote products derived from this software without**
 specific prior written permission.                                           **
                                                                              **
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"  **
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE    **
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE   **
 ARE  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE   **
 LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR         **
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF         **
 SUBSTITUTE GOODS OR  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS    **
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN      **
 CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)       **
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE   **
 POSSIBILITY OF SUCH DAMAGE.                                                  **
                                                                              **
 To improve the quality of the software, users are encouraged to share        **
 modifications, enhancements or bug fixes with Infineon Technologies AG       **
 dave@infineon.com).                                                          **
*******************************************************************************/

#include <DAVE.h>
#include <stddef.h>
#include <string.h>
#include "PMB/pmb_v2_can_emulator.h"

/* MO indices for the fixture side — these are the Node2 objects used only
 * during loopback verification and are not touched by the emulator itself. */
#define FIXTURE_TX_MO  (3U)   /* Node2 TX: injects PMB read requests */
#define FIXTURE_RX_MO  (4U)   /* Node2 RX: receives PMB responses     */

/* Packet counter bytes injected by the fixture; must be echoed back. */
#define TEST_PKT_HI  (0x12U)
#define TEST_PKT_LO  (0x34U)

static volatile uint8_t  s_fixture_rx_ready = 0U;
static          uint8_t  s_fixture_rx_data[8];
static          uint8_t  s_fixture_rx_dlc;
static          uint32_t s_fixture_rx_id;

extern void initialise_monitor_handles(void);

void CAN_IRQHandler(void);

/* Configure MO3/MO4 so they can act as a PMB fixture for loopback testing.
 * MO3 becomes a plain extended TX; MO4 is filtered to accept PMB responses
 * addressed to FIXTURE_ADDR. */
static void fixture_init(void)
{
    XMC_CAN_MO_t *tx_mo = MULTICAN_CONFIG_0.lmobj_ptr[FIXTURE_TX_MO]->mo_ptr;
    XMC_CAN_MO_t *rx_mo = MULTICAN_CONFIG_0.lmobj_ptr[FIXTURE_RX_MO]->mo_ptr;

    tx_mo->can_id_mode     = XMC_CAN_FRAME_TYPE_EXTENDED_29BITS;
    tx_mo->can_mo_type     = XMC_CAN_MO_TYPE_TRANSMSGOBJ;
    tx_mo->can_data_length = 8U;
    XMC_CAN_MO_Config(tx_mo);

    /* Accept any response where bits[15:8] (dst) == FIXTURE_ADDR */
    rx_mo->can_identifier  = PMB_CAN_BUILD_ID(0U, 0U, FIXTURE_ADDR, 0U);
    rx_mo->can_id_mask     = 0x0000FF00UL;
    rx_mo->can_id_mode     = XMC_CAN_FRAME_TYPE_EXTENDED_29BITS;
    rx_mo->can_mo_type     = XMC_CAN_MO_TYPE_RECMSGOBJ;
    rx_mo->can_data_length = 8U;
    XMC_CAN_MO_Config(rx_mo);
}

/* Transmit a PMB read request (payload bytes 0-5 all zero) via MO3. */
static void fixture_send_read(uint16_t data_id)
{
    XMC_CAN_MO_t *mo = MULTICAN_CONFIG_0.lmobj_ptr[FIXTURE_TX_MO]->mo_ptr;
    uint8_t i;

    while (XMC_CAN_MO_GetStatus(mo) & (uint32_t)XMC_CAN_MO_STATUS_TX_REQUEST) {}

    mo->can_identifier  = PMB_CAN_BUILD_ID(4U, data_id, PMB_ADDR, FIXTURE_ADDR);
    mo->can_data_length = 8U;
    for (i = 0U; i < 6U; i++) mo->can_data_byte[i] = 0U; /* read request */
    mo->can_data_byte[6] = TEST_PKT_LO;
    mo->can_data_byte[7] = TEST_PKT_HI;

    if (XMC_CAN_MO_UpdateData(mo) != XMC_CAN_STATUS_SUCCESS)
    {
        XMC_DEBUG("fixture: UpdateData failed\n");
        while (1U) {}
    }
    if (XMC_CAN_MO_Transmit(mo) != XMC_CAN_STATUS_SUCCESS)
    {
        XMC_DEBUG("fixture: Transmit failed\n");
        while (1U) {}
    }
}

/* chk_lo..chk_hi (exclusive) is the slice of data[0..5] the emulator actively
 * packs for this data_id.  Bytes outside that range are not checked because
 * XMC_CAN_MO_Receive does not overwrite MO data bytes that the incoming frame
 * leaves unchanged, so they may contain stale data from the previous frame. */
static void loopback_test(uint16_t data_id, const uint8_t expected[6],
                          uint8_t chk_lo, uint8_t chk_hi)
{
    s_fixture_rx_ready = 0U;
    fixture_send_read(data_id);
    while (!s_fixture_rx_ready)
    {
        PMB_Emulator_Run();
    }

    /* Verify response fields */
    if (s_fixture_rx_dlc != 8U)
    {
        XMC_DEBUG("loopback: DLC mismatch\n");
        while (1U) {}
    }
    if (s_fixture_rx_data[6] != TEST_PKT_LO || s_fixture_rx_data[7] != TEST_PKT_HI)
    {
        XMC_DEBUG("loopback: packet counter not echoed\n");
        while (1U) {}
    }
    if (PMB_GET_DST(s_fixture_rx_id) != FIXTURE_ADDR)
    {
        XMC_DEBUG("loopback: response dst wrong\n");
        while (1U) {}
    }
    if (PMB_GET_SRC(s_fixture_rx_id) != PMB_ADDR)
    {
        XMC_DEBUG("loopback: response src wrong\n");
        while (1U) {}
    }
    uint8_t i;
    for (i = chk_lo; i < chk_hi; i++)
    {
        if (s_fixture_rx_data[i] != expected[i])
        {
            XMC_DEBUG("loopback: data mismatch\n");
            while (1U) {}
        }
    }

}

/* ============================================================
 * SOS LED blink (Morse: ... --- ...)
 * Timing unit ≈ 50 ms at 120 MHz.
 * ============================================================ */
#define SOS_DOT    6000000U   /* 1 unit  on  */
#define SOS_DASH  18000000U   /* 3 units on  */
#define SOS_EGAP   6000000U   /* 1 unit  off — between elements */
#define SOS_LGAP  18000000U   /* 3 units off — between letters  */
#define SOS_WGAP  42000000U   /* 7 units off — after full SOS   */

static void led_on(uint32_t cycles)
{
    XMC_GPIO_SetOutputHigh(DIGITAL_IO_0.gpio_port, DIGITAL_IO_0.gpio_pin);
    for (volatile uint32_t d = 0U; d < cycles; d++) {}
}

static void led_off(uint32_t cycles)
{
    XMC_GPIO_SetOutputLow(DIGITAL_IO_0.gpio_port, DIGITAL_IO_0.gpio_pin);
    for (volatile uint32_t d = 0U; d < cycles; d++) {}
}

static void blink_sos(void)
{
    /* S: dot dot dot */
    led_on(SOS_DOT);  led_off(SOS_EGAP);
    led_on(SOS_DOT);  led_off(SOS_EGAP);
    led_on(SOS_DOT);  led_off(SOS_LGAP);
    /* O: dash dash dash */
    led_on(SOS_DASH); led_off(SOS_EGAP);
    led_on(SOS_DASH); led_off(SOS_EGAP);
    led_on(SOS_DASH); led_off(SOS_LGAP);
    /* S: dot dot dot */
    led_on(SOS_DOT);  led_off(SOS_EGAP);
    led_on(SOS_DOT);  led_off(SOS_EGAP);
    led_on(SOS_DOT);  led_off(SOS_WGAP);
}

/* ============================================================
 * Table-driven full loopback test suite
 * ============================================================
 *
 * Each entry maps a data_id to the byte range in PMB_Registers_t that
 * the emulator reads when building the response.  The runner writes a fixed
 * seed pattern to those raw struct bytes, then issues a CAN read request and
 * verifies that exactly those bytes come back in the response payload.
 *
 * Seed odd bytes (hi of each uint16_t pair) are 0x01/0x04/0x07 — all ≤ 0x0F
 * — so pack12's upper-nibble mask leaves them unchanged and expected == seed
 * for every register type without any special casing.
 *
 * Special case 0xCC (REG_NEG_LIM_GRP2): the emulator packs its two fields at
 * payload bytes [2..5] instead of [0..3].  payload_off captures this offset.
 * ============================================================ */

typedef struct {
    uint16_t data_id;
    uint16_t reg_offset;   /* offsetof(PMB_Registers_t, field) */
    uint8_t  n_active;     /* bytes written to the struct / verified in response */
    uint8_t  payload_off;  /* byte index in data[0..5] where the fields begin    */
} PMBTestVec_t;

static const uint8_t s_seed6[6] = {0x23U, 0x01U, 0x56U, 0x04U, 0x89U, 0x07U};

#define OFS(f) ((uint16_t)offsetof(PMB_Registers_t, f))

static const PMBTestVec_t s_test_table[] = {

    /* === TELEMETRY (0x00-0x1A, pack12, uint16_t fields) === */
    { REG_TELEMETRY_GROUP_1,  OFS(telem.chg_curr),          6U, 0U },
    { REG_TELEMETRY_GROUP_2,  OFS(telem.ll_imon),           6U, 0U },
    { REG_TELEMETRY_GROUP_3,  OFS(telem.ra_imon),           6U, 0U },
    { REG_TELEMETRY_GROUP_4,  OFS(telem.hv),                6U, 0U },
    { REG_TELEMETRY_GROUP_5,  OFS(telem.nk_imon),           6U, 0U },
    { REG_TELEMETRY_GROUP_6,  OFS(telem.user_hv),           4U, 0U },
    { REG_TELEMETRY_GROUP_7,  OFS(telem.pc_24v),            4U, 0U },
    { REG_TELEMETRY_GROUP_8,  OFS(telem.user_24v),          4U, 0U },
    { REG_TELEMETRY_GROUP_9,  OFS(telem.pc_a_12v),          4U, 0U },
    { REG_TELEMETRY_GROUP_10, OFS(telem.pc_b_12v),          4U, 0U },
    { REG_TELEMETRY_GROUP_11, OFS(telem.v24_imon0),         6U, 0U },
    { REG_TELEMETRY_GROUP_12, OFS(telem.v12_imon0),         6U, 0U },
    { REG_TELEMETRY_GROUP_13, OFS(telem.v24_bus_voltage),   4U, 0U },
    { REG_TELEMETRY_GROUP_14, OFS(telem.v5),                4U, 0U },
    { REG_TELEMETRY_GROUP_15, OFS(telem.v3v3),              4U, 0U },
    { REG_TELEMETRY_GROUP_16, OFS(telem.aux_24v),           4U, 0U },
    { REG_TELEMETRY_GROUP_17, OFS(telem.aux_12v),           4U, 0U },
    { REG_TELEMETRY_GROUP_18, OFS(telem.estop_rec_a_12v),   4U, 0U },
    { REG_TELEMETRY_GROUP_19, OFS(telem.v12_b_imon3),       4U, 0U },
    { REG_TELEMETRY_GROUP_20, OFS(telem.hv_or),             4U, 0U },
    { REG_TELEMETRY_GROUP_21, OFS(telem.temp0),             6U, 0U },
    { REG_TELEMETRY_GROUP_22, OFS(telem.temp3),             6U, 0U },
    { REG_TELEMETRY_GROUP_23, OFS(telem.ex_temp0),          4U, 0U },
    { REG_TELEMETRY_GROUP_24, OFS(telem.lidar_a_12v),       4U, 0U },
    { REG_TELEMETRY_GROUP_25, OFS(telem.sto_24v),           4U, 0U },
    { REG_TELEMETRY_GROUP_26, OFS(telem.eth_2v5),           4U, 0U },
    { REG_TELEMETRY_GROUP_27, OFS(telem.ecat_3v3),          4U, 0U },

    /* === STATUS BIT-MAPPED (0x1B-0x1E, byte fields) === */
    { REG_COMP_PDG,       OFS(status.comp_pdg),       3U, 0U },
    { REG_PDG_FAULT_GRP1, OFS(status.pdg_fault_grp1), 3U, 0U },
    { REG_PDG_FAULT_GRP2, OFS(status.pdg_fault_grp2), 2U, 0U },
    { REG_PDG_FAULT_GRP3, OFS(status.pdg_fault_grp3), 2U, 0U },

    /* === POWER ENABLE (0x1F-0x23, byte fields) === */
    { REG_POWER_ENABLE_GRP1, OFS(pwr_enable.grp1),          1U, 0U },
    { REG_POWER_ENABLE_GRP2, OFS(pwr_enable.grp2),          3U, 0U },
    { REG_POWER_ENABLE_GRP3, OFS(pwr_enable.grp3),          4U, 0U },
    { REG_PERIPHERAL_INPUTS, OFS(pwr_enable.periph_inputs),  1U, 0U },
    { REG_POWER_ENABLE_GRP4, OFS(pwr_enable.grp4),          3U, 0U },

    /* === ADC OFFSETS (0x30-0x4A, pack16, int16_t fields) === */
    { REG_ADC_OFFSET_GRP1,  OFS(calib_offset.chg_curr_ofs),          6U, 0U },
    { REG_ADC_OFFSET_GRP2,  OFS(calib_offset.ll_imon_ofs),           6U, 0U },
    { REG_ADC_OFFSET_GRP3,  OFS(calib_offset.ra_imon_ofs),           6U, 0U },
    { REG_ADC_OFFSET_GRP4,  OFS(calib_offset.hv_ofs),                6U, 0U },
    { REG_ADC_OFFSET_GRP5,  OFS(calib_offset.nk_imon_ofs),           6U, 0U },
    { REG_ADC_OFFSET_GRP6,  OFS(calib_offset.user_hv_ofs),           4U, 0U },
    { REG_ADC_OFFSET_GRP7,  OFS(calib_offset.pc_24v_ofs),            4U, 0U },
    { REG_ADC_OFFSET_GRP8,  OFS(calib_offset.user_24v_ofs),          4U, 0U },
    { REG_ADC_OFFSET_GRP9,  OFS(calib_offset.pc_a_12v_ofs),          4U, 0U },
    { REG_ADC_OFFSET_GRP10, OFS(calib_offset.pc_b_12v_ofs),          4U, 0U },
    { REG_ADC_OFFSET_GRP11, OFS(calib_offset.v24_imon0_ofs),         6U, 0U },
    { REG_ADC_OFFSET_GRP12, OFS(calib_offset.v12_imon0_ofs),         6U, 0U },
    { REG_ADC_OFFSET_GRP13, OFS(calib_offset.v24_ofs),               4U, 0U },
    { REG_ADC_OFFSET_GRP14, OFS(calib_offset.v5_ofs),                4U, 0U },
    { REG_ADC_OFFSET_GRP15, OFS(calib_offset.v3v3_ofs),              4U, 0U },
    { REG_ADC_OFFSET_GRP16, OFS(calib_offset.aux_24v_ofs),           4U, 0U },
    { REG_ADC_OFFSET_GRP17, OFS(calib_offset.aux_12v_ofs),           4U, 0U },
    { REG_ADC_OFFSET_GRP18, OFS(calib_offset.estop_rec_a_12v_ofs),   4U, 0U },
    { REG_ADC_OFFSET_GRP19, OFS(calib_offset.v12_b_imon3_ofs),       4U, 0U },
    { REG_ADC_OFFSET_GRP20, OFS(calib_offset.hv_or_ofs),             4U, 0U },
    { REG_ADC_OFFSET_GRP21, OFS(calib_offset.temp0_ofs),             6U, 0U },
    { REG_ADC_OFFSET_GRP22, OFS(calib_offset.temp3_ofs),             6U, 0U },
    { REG_ADC_OFFSET_GRP23, OFS(calib_offset.ex_temp0_ofs),          4U, 0U },
    { REG_ADC_OFFSET_GRP24, OFS(calib_offset.lidar_a_12v_ofs),       4U, 0U },
    { REG_ADC_OFFSET_GRP25, OFS(calib_offset.sto_24v_ofs),           4U, 0U },
    { REG_ADC_OFFSET_GRP26, OFS(calib_offset.eth_2v5_ofs),           4U, 0U },
    { REG_ADC_OFFSET_GRP27, OFS(calib_offset.ecat_3v3_ofs),          4U, 0U },

    /* === ADC SCALES (0x50-0x6A, pack16, int16_t fields) === */
    { REG_ADC_SCALE_GRP1,  OFS(calib_scale.chg_curr_scl),           6U, 0U },
    { REG_ADC_SCALE_GRP2,  OFS(calib_scale.ll_imon_scl),            6U, 0U },
    { REG_ADC_SCALE_GRP3,  OFS(calib_scale.ra_imon_scl),            6U, 0U },
    { REG_ADC_SCALE_GRP4,  OFS(calib_scale.hv_scl),                 6U, 0U },
    { REG_ADC_SCALE_GRP5,  OFS(calib_scale.nk_imon_scl),            6U, 0U },
    { REG_ADC_SCALE_GRP6,  OFS(calib_scale.user_hv_scl),            4U, 0U },
    { REG_ADC_SCALE_GRP7,  OFS(calib_scale.pc_24v_scl),             4U, 0U },
    { REG_ADC_SCALE_GRP8,  OFS(calib_scale.user_24v_scl),           4U, 0U },
    { REG_ADC_SCALE_GRP9,  OFS(calib_scale.pc_a_12v_scl),           4U, 0U },
    { REG_ADC_SCALE_GRP10, OFS(calib_scale.pc_b_12v_scl),           4U, 0U },
    { REG_ADC_SCALE_GRP11, OFS(calib_scale.v24_imon0_scl),          6U, 0U },
    { REG_ADC_SCALE_GRP12, OFS(calib_scale.v12_imon0_scl),          6U, 0U },
    { REG_ADC_SCALE_GRP13, OFS(calib_scale.v24_scl),                4U, 0U },
    { REG_ADC_SCALE_GRP14, OFS(calib_scale.v5_scl),                 4U, 0U },
    { REG_ADC_SCALE_GRP15, OFS(calib_scale.v3v3_scl),               4U, 0U },
    { REG_ADC_SCALE_GRP16, OFS(calib_scale.aux_24v_scl),            4U, 0U },
    { REG_ADC_SCALE_GRP17, OFS(calib_scale.aux_12v_scl),            4U, 0U },
    { REG_ADC_SCALE_GRP18, OFS(calib_scale.estop_rec_a_12v_scl),    4U, 0U },
    { REG_ADC_SCALE_GRP19, OFS(calib_scale.v12_b_imon3_scl),        4U, 0U },
    { REG_ADC_SCALE_GRP20, OFS(calib_scale.hv_or_scl),              4U, 0U },
    { REG_ADC_SCALE_GRP21, OFS(calib_scale.temp0_scl),              6U, 0U },
    { REG_ADC_SCALE_GRP22, OFS(calib_scale.temp3_scl),              6U, 0U },
    { REG_ADC_SCALE_GRP23, OFS(calib_scale.ex_temp0_scl),           4U, 0U },
    { REG_ADC_SCALE_GRP24, OFS(calib_scale.lidar_a_12v_scl),        4U, 0U },
    { REG_ADC_SCALE_GRP25, OFS(calib_scale.sto_24v_scl),            4U, 0U },
    { REG_ADC_SCALE_GRP26, OFS(calib_scale.eth_2v5_scl),            4U, 0U },
    { REG_ADC_SCALE_GRP27, OFS(calib_scale.ecat_3v3_scl),           4U, 0U },

    /* === EU (0x70-0x8A, pack16, int16_t fields) === */
    { REG_EU_GRP1,  OFS(eu.chg_curr_eu),           6U, 0U },
    { REG_EU_GRP2,  OFS(eu.ll_imon_eu),            6U, 0U },
    { REG_EU_GRP3,  OFS(eu.ra_imon_eu),            6U, 0U },
    { REG_EU_GRP4,  OFS(eu.hv_eu),                 6U, 0U },
    { REG_EU_GRP5,  OFS(eu.nk_imon_eu),            6U, 0U },
    { REG_EU_GRP6,  OFS(eu.user_hv_eu),            4U, 0U },
    { REG_EU_GRP7,  OFS(eu.pc_24v_eu),             4U, 0U },
    { REG_EU_GRP8,  OFS(eu.user_24v_eu),           4U, 0U },
    { REG_EU_GRP9,  OFS(eu.pc_a_12v_eu),           4U, 0U },
    { REG_EU_GRP10, OFS(eu.pc_b_12v_eu),           4U, 0U },
    { REG_EU_GRP11, OFS(eu.v24_imon0_eu),          6U, 0U },
    { REG_EU_GRP12, OFS(eu.v12_imon0_eu),          6U, 0U },
    { REG_EU_GRP13, OFS(eu.v24_eu),                4U, 0U },
    { REG_EU_GRP14, OFS(eu.v5_eu),                 4U, 0U },
    { REG_EU_GRP15, OFS(eu.v3v3_eu),               4U, 0U },
    { REG_EU_GRP16, OFS(eu.aux_24v_eu),            4U, 0U },
    { REG_EU_GRP17, OFS(eu.aux_12v_eu),            4U, 0U },
    { REG_EU_GRP18, OFS(eu.estop_rec_a_12v_eu),    4U, 0U },
    { REG_EU_GRP19, OFS(eu.v12_b_imon3_eu),        4U, 0U },
    { REG_EU_GRP20, OFS(eu.hv_or_eu),              4U, 0U },
    { REG_EU_GRP21, OFS(eu.temp0_eu),              6U, 0U },
    { REG_EU_GRP22, OFS(eu.temp3_eu),              6U, 0U },
    { REG_EU_GRP23, OFS(eu.ex_temp0_eu),           4U, 0U },
    { REG_EU_GRP24, OFS(eu.lidar_a_12v_eu),        4U, 0U },
    { REG_EU_GRP25, OFS(eu.sto_24v_eu),            4U, 0U },
    { REG_EU_GRP26, OFS(eu.eth_2v5_eu),            4U, 0U },
    { REG_EU_GRP27, OFS(eu.ecat_3v3_eu),           4U, 0U },
    /* Derived EU (0x90-0x91) */
    { REG_DERIVED_EU_GRP1, OFS(eu.v12_a_total_curr_eu), 6U, 0U },
    { REG_DERIVED_EU_GRP2, OFS(eu.batt_total_curr_eu),  2U, 0U },

    /* === ERROR CODE (0xA0, uint32_t little-endian) === */
    { REG_ERROR_CODE, OFS(error_code), 4U, 0U },

    /* === FAULT REGISTERS (0xA8-0xAA, byte fields) === */
    { REG_FAULT_REG1, OFS(fault_regs.reg1), 1U, 0U },
    { REG_FAULT_REG2, OFS(fault_regs.reg2), 3U, 0U },
    { REG_FAULT_REG3, OFS(fault_regs.reg3), 4U, 0U },

    /* === SOFT LIMITS (0xB0-0xCA, pack16, int16_t fields) === */
    { REG_SOFT_LIM_GRP1,  OFS(soft_limits.chg_curr_lim),           6U, 0U },
    { REG_SOFT_LIM_GRP2,  OFS(soft_limits.ll_imon_lim),            6U, 0U },
    { REG_SOFT_LIM_GRP3,  OFS(soft_limits.ra_imon_lim),            6U, 0U },
    { REG_SOFT_LIM_GRP4,  OFS(soft_limits.hv_lim),                 6U, 0U },
    { REG_SOFT_LIM_GRP5,  OFS(soft_limits.nk_imon_lim),            6U, 0U },
    { REG_SOFT_LIM_GRP6,  OFS(soft_limits.user_hv_lim),            4U, 0U },
    { REG_SOFT_LIM_GRP7,  OFS(soft_limits.pc_24v_lim),             4U, 0U },
    { REG_SOFT_LIM_GRP8,  OFS(soft_limits.user_24v_lim),           4U, 0U },
    { REG_SOFT_LIM_GRP9,  OFS(soft_limits.pc_a_12v_lim),           4U, 0U },
    { REG_SOFT_LIM_GRP10, OFS(soft_limits.pc_b_12v_lim),           4U, 0U },
    { REG_SOFT_LIM_GRP11, OFS(soft_limits.v24_imon0_lim),          6U, 0U },
    { REG_SOFT_LIM_GRP12, OFS(soft_limits.v12_imon0_lim),          6U, 0U },
    { REG_SOFT_LIM_GRP13, OFS(soft_limits.v24_lim),                4U, 0U },
    { REG_SOFT_LIM_GRP14, OFS(soft_limits.v5_lim),                 4U, 0U },
    { REG_SOFT_LIM_GRP15, OFS(soft_limits.v3v3_lim),               4U, 0U },
    { REG_SOFT_LIM_GRP16, OFS(soft_limits.aux_24v_lim),            4U, 0U },
    { REG_SOFT_LIM_GRP17, OFS(soft_limits.aux_12v_lim),            4U, 0U },
    { REG_SOFT_LIM_GRP18, OFS(soft_limits.estop_rec_a_12v_lim),    4U, 0U },
    { REG_SOFT_LIM_GRP19, OFS(soft_limits.v12_b_imon3_lim),        4U, 0U },
    { REG_SOFT_LIM_GRP20, OFS(soft_limits.hv_or_lim),              4U, 0U },
    { REG_SOFT_LIM_GRP21, OFS(soft_limits.temp0_plim),             6U, 0U },
    { REG_SOFT_LIM_GRP22, OFS(soft_limits.temp3_plim),             6U, 0U },
    { REG_SOFT_LIM_GRP23, OFS(soft_limits.ex_temp0_plim),          4U, 0U },
    { REG_SOFT_LIM_GRP24, OFS(soft_limits.lidar_a_12v_lim),        4U, 0U },
    { REG_SOFT_LIM_GRP25, OFS(soft_limits.sto_24v_lim),            4U, 0U },
    { REG_SOFT_LIM_GRP26, OFS(soft_limits.eth_2v5_lim),            4U, 0U },
    { REG_SOFT_LIM_GRP27, OFS(soft_limits.ecat_3v3_lim),           4U, 0U },
    /* Neg / derived limits (0xCB-0xCF) */
    { REG_NEG_LIM_GRP1, OFS(soft_limits.v12_a_total_curr_lim), 6U, 0U },
    /* 0xCC packs fields at data[2..5], not data[0..3] — payload_off=2 */
    { REG_NEG_LIM_GRP2, OFS(soft_limits.batt1_curr_nlim),      4U, 2U },
    { REG_NEG_LIM_GRP3, OFS(soft_limits.temp0_nlim),           6U, 0U },
    { REG_NEG_LIM_GRP4, OFS(soft_limits.temp3_nlim),           6U, 0U },
    { REG_NEG_LIM_GRP5, OFS(soft_limits.ex_temp0_nlim),        4U, 0U },

    /* === LIMIT FAULT FLAGS (0xD0-0xD9, byte arrays) === */
    { REG_SOFT_LIM_FAULT_GRP1, OFS(lim_faults.soft_grp1), 6U, 0U },
    { REG_SOFT_LIM_FAULT_GRP2, OFS(lim_faults.soft_grp2), 3U, 0U },
    { REG_HARD_LIM_FAULT_GRP1, OFS(lim_faults.hard_grp1), 6U, 0U },
    { REG_HARD_LIM_FAULT_GRP2, OFS(lim_faults.hard_grp2), 3U, 0U },

    /* === BUILD INFO + WRITE PROTECT === */
    { REG_HW_FW_REVISION, OFS(build_info.hw_rev),  4U, 0U },
    { REG_WRITE_PROTECT,  OFS(write_protect.b1),   5U, 0U },
};

#define TEST_TABLE_LEN ((uint8_t)(sizeof(s_test_table) / sizeof(s_test_table[0])))

static void run_all_loopback_tests(void)
{
    uint8_t expected[6];
    uint8_t i, j;

    for (i = 0U; i < TEST_TABLE_LEN; i++)
    {
        const PMBTestVec_t *tv = &s_test_table[i];
        PMB_Registers_t *regs  = PMB_GetRegsMutable();

        /* Stamp seed bytes directly into the struct at the field's raw address.
         * For payload_off > 0 (only 0xCC today) the seed slice that maps to
         * the active payload positions is used so expected == seed. */
        memcpy((uint8_t *)regs + tv->reg_offset,
               s_seed6 + tv->payload_off,
               tv->n_active);

        /* Build expected: seed at [payload_off .. payload_off+n_active), zeros elsewhere */
        memset(expected, 0U, 6U);
        for (j = 0U; j < tv->n_active; j++) {
            expected[tv->payload_off + j] = s_seed6[tv->payload_off + j];
        }

        loopback_test(tv->data_id, expected,
                      tv->payload_off, (uint8_t)(tv->payload_off + tv->n_active));
    }
}

int main(void)
{
    DAVE_STATUS_t app_status;

    app_status = DAVE_Init();
    initialise_monitor_handles();

    if (app_status == DAVE_STATUS_FAILURE)
    {
        XMC_DEBUG("DAVE APPs initialization failed\n");
        while (1U) {}
    }
    XMC_DEBUG("hello world\n");

    PMB_Emulator_Init();
    fixture_init();

    run_all_loopback_tests();

    /* All tests passed — blink SOS continuously */
    while (1U)
    {
        blink_sos();
    }
}

/* ISR for MO4 (FIXTURE_RX_MO) service request — fires when a PMB response
 * arrives on the fixture RX during loopback verification. */
void CAN_IRQHandler(void)
{
    uint8_t i;
    XMC_CAN_MO_t *mo = MULTICAN_CONFIG_0.lmobj_ptr[FIXTURE_RX_MO]->mo_ptr;

    XMC_CAN_MO_Receive(mo);
    s_fixture_rx_id  = mo->can_identifier;
    s_fixture_rx_dlc = mo->can_data_length;
    for (i = 0U; i < 8U; i++) s_fixture_rx_data[i] = mo->can_data_byte[i];
    s_fixture_rx_ready = 1U;
}
