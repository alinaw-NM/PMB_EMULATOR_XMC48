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

static void loopback_test(uint16_t data_id, const uint8_t expected[6]){
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
    for (i = 0U; i < 6U; i++)
    {
        if (s_fixture_rx_data[i] != expected[i])
        {
            XMC_DEBUG("loopback: data mismatch\n");
            while (1U) {}
        }
    }

    /* Blink LED once to signal loopback pass */
    XMC_GPIO_SetOutputHigh(DIGITAL_IO_0.gpio_port, DIGITAL_IO_0.gpio_pin);
    for (volatile uint32_t d = 0U; d < 14400000U; d++) {}
    XMC_GPIO_SetOutputLow(DIGITAL_IO_0.gpio_port, DIGITAL_IO_0.gpio_pin);
    for (volatile uint32_t d = 0U; d < 14400000U; d++) {}
}

int main(void)
{
    DAVE_STATUS_t app_status;

    app_status = DAVE_Init();
    if (app_status == DAVE_STATUS_FAILURE)
    {
        XMC_DEBUG("DAVE APPs initialization failed\n");
        while (1U) {}
    }

    PMB_Emulator_Init();
    fixture_init();

    /* Pre-load known telemetry values so bytes 0-5 of the response are
     * non-zero and predictable for data integrity verification. */
    {
        PMB_Registers_t *regs = PMB_GetRegsMutable();
        regs->telem.chg_curr   = 0x123U;
        regs->telem.batt1_curr = 0x456U;
        regs->telem.batt2_curr = 0x789U;
    }
     {
        PMB_Registers_t *regs = PMB_GetRegsMutable();
        regs->telem.ll_imon   = 0x123U;
        regs->telem.rl_imon = 0x456U;
        regs->telem.la_imon = 0x789U;
    }

    static const uint8_t grp1_expected[6] = {0x23U, 0x01U, 0x56U, 0x04U, 0x89U, 0x07U};
    static const uint8_t grp2_expected[6] = {0x23U, 0x01U, 0x56U, 0x04U, 0x89U, 0x07U};

    loopback_test(REG_TELEMETRY_GROUP_1, grp1_expected);
    loopback_test(REG_TELEMETRY_GROUP_2, grp2_expected);

    /* Normal emulator loop */
    while (1U)
    {
        PMB_Emulator_Run();
        PMB_UpdateEU();
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
