/*
 * main.c — SCUD BMS RS485 reader on XMC4800
 *
 * Polls analog telemetry and alarms once per second and prints via ITM/SWO.
 * ScudAnalog_t is kept as a module-level variable so future CAN logic can
 * read it without having to re-poll the BMS.
 */

#include "DAVE.h"
#include "scud_bms.h"

extern void initialise_monitor_handles(void);

/* ~1 second busy-wait at 144 MHz (volatile prevents optimisation out). */
static void delay_1s(void)
{
    for (volatile uint32_t i = 0U; i < 36000000U; i++) {}
}

/* Most-recent BMS data — accessible to future CAN / application code. */
static ScudAnalog_t g_bms_analog;
static ScudAlarms_t g_bms_alarms;

int main(void)
{
    DAVE_STATUS_t status = DAVE_Init();
    initialise_monitor_handles();

    if (status != DAVE_STATUS_SUCCESS)
    {
        XMC_DEBUG("DAVE init failed\n");
        while (1U) {}
    }

    /* We use polling, not interrupt-driven TX/RX. Disable the UART FIFO
     * interrupts that DAVE enabled — without handlers they hit Default_Handler. */
    NVIC_DisableIRQ((IRQn_Type)91); /* USIC1 SR1 — TX FIFO */
    NVIC_DisableIRQ((IRQn_Type)90); /* USIC1 SR0 — RX FIFO */

    XMC_DEBUG("SCUD BMS RS485 reader starting  (115200 8N1, addr=0x00)\n");


    while (1U)
    {
        ScudStatus_t st = SCUD_ReadAnalog(&g_bms_analog);
        if (st == SCUD_OK)
        {
            SCUD_PrintAnalog(&g_bms_analog);
        }
        else
        {
            XMC_DEBUG("analog read: %s\n", SCUD_StatusStr(st));
        }

        st = SCUD_ReadAlarms(&g_bms_alarms);
        if (st == SCUD_OK)
        {
            uint8_t any = g_bms_alarms.voltage  | g_bms_alarms.current  |
                          g_bms_alarms.cell_temp | g_bms_alarms.other_temp |
                          g_bms_alarms.capacity  | g_bms_alarms.other1   |
                          g_bms_alarms.other2;
            if (any)
            {
                XMC_DEBUG("ALARM: V=%02X I=%02X CT=%02X OT=%02X CAP=%02X O1=%02X O2=%02X\n",
                          (unsigned int)g_bms_alarms.voltage,
                          (unsigned int)g_bms_alarms.current,
                          (unsigned int)g_bms_alarms.cell_temp,
                          (unsigned int)g_bms_alarms.other_temp,
                          (unsigned int)g_bms_alarms.capacity,
                          (unsigned int)g_bms_alarms.other1,
                          (unsigned int)g_bms_alarms.other2);
            }
        }
        else
        {
            XMC_DEBUG("alarm read: %s\n", SCUD_StatusStr(st));
        }

        delay_1s();
    }
}
