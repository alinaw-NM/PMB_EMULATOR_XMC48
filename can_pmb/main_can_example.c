/**
 * @file main.c
 * @brief PMB CAN Driver usage example for XMC4800 IoT Amazon FreeRTOS Kit
 *
 * Demonstrates:
 *  1. CAN initialisation
 *  2. Master role – periodic broadcast of a Power Enable command (0x20)
 *  3. Slave role  – receiving any PMB frame and decoding Telemetry Group 1 (0x00)
 *
 * Build on top of a DAVE 4 project (same structure as ETHCAT_SSC_XMC48).
 * Replace the EtherCAT APP includes with the CAN driver files.
 *
 * Connections (XMC4800 Relax / IoT Kit):
 *   P1.12  →  CAN TX  (connect to CAN transceiver TXD)
 *   P1.13  ←  CAN RX  (connect to CAN transceiver RXD)
 *   Transceiver CANH/CANL → bus (120 Ω termination at each end)
 */

#include <DAVE.h>               /* DAVE generated init */
#include "pmb_can_driver.h"
#include <stdio.h>

/* -----------------------------------------------------------------------
 * Helper: pretty-print a received frame over UART/SWO
 * --------------------------------------------------------------------- */
static void print_frame(const PMB_Frame_t *f)
{
    printf("RX ID=0x%03X  [%02X %02X %02X %02X %02X %02X %02X %02X]\r\n",
           f->data_id,
           f->data[0], f->data[1], f->data[2], f->data[3],
           f->data[4], f->data[5], f->data[6], f->data[7]);
}

/* -----------------------------------------------------------------------
 * Slave RX callback – decode and act on received frames
 * --------------------------------------------------------------------- */
static void on_can_rx(const PMB_Frame_t *frame)
{
    switch (frame->data_id) {

    case PMB_ID_TELEM_GRP1: {
        /*
         * Telemetry Group 1 (0x00) layout (spec, read-only):
         *   data[0..1] = packet counter (RSVD in spec = bytes 8-7)
         *   data[2..3] = RSVD
         *   data[4..5] = BATT2_CURR [11:8 | 7:0]  (12-bit)
         *   data[4]    = {4b0000, BATT2_CURR[11:8]}
         *   data[5]    = BATT2_CURR[7:0]
         *   data[6..7] → same pattern for BATT1_CURR and CHG_CURR
         *
         * Note: spec uses Byte8..Byte1 (MSB first in the frame).
         * data[0] = Byte8 (MSB), data[7] = Byte1 (LSB).
         */
        uint16_t batt2_curr = PMB_CAN_Decode12Bit(frame->data[4], frame->data[5]);
        uint16_t batt1_curr = PMB_CAN_Decode12Bit(frame->data[6], frame->data[7]);
        /* CHG_CURR would be in data[2..3] if present */

        printf("TELEM GRP1: BATT2_CURR=%u raw, BATT1_CURR=%u raw\r\n",
               batt2_curr, batt1_curr);
        break;
    }

    case PMB_ID_PG_FAULT_GRP1: {
        /* Power Good & Fault Group 1 (0x1C) – bit-mapped */
        uint8_t byte2 = frame->data[5]; /* Byte2 in spec */
        uint8_t byte1 = frame->data[6]; /* Byte1 in spec */
        printf("PG_FAULT GRP1: Byte2=0x%02X Byte1=0x%02X\r\n", byte2, byte1);

        if (byte1 & 0x01U) {
            printf("  >> USER_HV_FLTn asserted!\r\n");
        }
        if (byte1 & 0x02U) {
            printf("  >> MOT_BUS_FLTn asserted!\r\n");
        }
        break;
    }

    case PMB_ID_PWR_EN_GRP2: {
        /* Power Enable Group 2 (0x20) – R/W, check parity */
        bool parity_ok = true;
        for (uint8_t i = 2U; i < PMB_FRAME_LEN; i++) {
            if (!PMB_CAN_CheckParity(frame->data[i])) {
                parity_ok = false;
                break;
            }
        }
        printf("PWR_EN GRP2 received. Parity: %s\r\n",
               parity_ok ? "OK" : "FAIL");
        break;
    }

    default:
        /* Log unknown frames */
        print_frame(frame);
        break;
    }
}

/* -----------------------------------------------------------------------
 * Master: build and broadcast a Power Enable Group 2 command (0x20)
 *
 * Example: enable 24V rail (bit6 of Byte1 = 1) with parity.
 * Byte1 layout: {24V_EN, 12V_EN, 12V_B_EN, 5V_EN, PC_24V_ON,
 *                AUX_24V_ON, USER_24V_ON, Parity Bit}
 * --------------------------------------------------------------------- */
static void master_send_power_enable(void)
{
    /*
     * payload[0..5] maps to frame data[2..7]  (Bytes 6..1 in spec, MSB first)
     *
     * For PMB_ID_PWR_EN_GRP2 (0x20):
     *   Byte3 (payload[1]) = {AUX_12V_EN, MCU_ALARM_EN, ECAT_ENn,
     *                          ETH_ENn, ESC_RST, ESTOP_REC_A_12V_EN,
     *                          ESTOP_REC_B_12V_EN, Parity}
     *   Byte2 (payload[2]) = {STO_24V_EN, PC_A_12V_EN, PC_B_12V_EN,
     *                          LIDAR_A_12V_EN, LIDAR_B_12V_EN,
     *                          MCU_STO_EN, USER_HV_ON, Parity}
     *   Byte1 (payload[3]) = {24V_EN, 12V_EN, 12V_B_EN, 5V_EN,
     *                          PC_24V_ON, AUX_24V_ON, USER_24V_ON, Parity}
     * Parity is auto-computed by PMB_CAN_BuildFrame when apply_parity=true.
     */
    uint8_t payload[6] = {
        0x00U,  /* Byte6 – RSVD */
        0x00U,  /* Byte3 – all enables off (ECAT_ENn / ETH_ENn are active-low) */
        0x00U,  /* Byte2 – all enables off */
        0x40U,  /* Byte1 – set bit6 = 24V_EN (parity TBD by BuildFrame) */
        0x00U,  /* padding */
        0x00U,  /* padding */
    };

    PMB_Frame_t frame;
    PMB_CAN_BuildFrame(&frame, PMB_ID_PWR_EN_GRP2, payload, /*apply_parity=*/true);
    PMB_CAN_Transmit(&frame);
    printf("TX: Power Enable GRP2 (0x20) sent\r\n");
}

/* -----------------------------------------------------------------------
 * Master: request telemetry by sending a zero-length "read request"
 * (protocol-level: send a frame with the desired Data ID; slave responds).
 * If your PMB target auto-broadcasts, this may be unnecessary.
 * --------------------------------------------------------------------- */
static void master_request_telemetry(uint16_t data_id)
{
    uint8_t payload[6] = {0};
    PMB_Frame_t frame;
    PMB_CAN_BuildFrame(&frame, data_id, payload, /*apply_parity=*/false);
    PMB_CAN_Transmit(&frame);
}

/* -----------------------------------------------------------------------
 * Main
 * --------------------------------------------------------------------- */
int main(void)
{
    /* DAVE generated initialisation (clocks, pins configured in DAVE GUI) */
    DAVE_STATUS_t dave_status = DAVE_Init();
    if (dave_status != DAVE_STATUS_SUCCESS) {
        /* DAVE init failed – halt */
        while (1) { }
    }

    /* Initialise PMB CAN driver */
    if (!PMB_CAN_Init()) {
        printf("ERROR: PMB_CAN_Init failed\r\n");
        while (1) { }
    }

    printf("PMB CAN driver ready. Baud=%u bps\r\n", PMB_CAN_BAUD_RATE);

    /* Register slave receive callback */
    PMB_CAN_RegisterRxCallback(on_can_rx);

    /* Simple tick counter (replace with FreeRTOS task / SysTick as needed) */
    uint32_t tick = 0U;

    while (1) {
        tick++;

        /* --- Slave role: poll for incoming frames --- */
        PMB_CAN_Poll();

        /* --- Master role: broadcast every ~1000 iterations --- */
        if ((tick % 1000U) == 0U) {
            /* Broadcast a power-enable command */
            master_send_power_enable();

            /* Request Telemetry Group 1 */
            master_request_telemetry(PMB_ID_TELEM_GRP1);
        }

        /* Small software delay (replace with FreeRTOS vTaskDelay) */
        for (volatile uint32_t d = 0U; d < 1000U; d++) { }
    }

    return 0;
}
