/*
 * main.c — BMS RS485 diagnostic
 *
 * Sends one analog-read request every 2 s, collects the BMS response
 * into a buffer without any printf inside the receive loop (avoids
 * RXFIFO overflow), then prints all received bytes via SWO.
 *
 * rs485_send flushes the TX echo only after a 10 ms guard delay so
 * that the THVD1406DR auto-direction turnaround completes before we
 * listen for the BMS reply.
 *
 * If BMS shows "(none)" → check A/B polarity and frame format.
 */

#include "DAVE.h"
#include "xmc_uart.h"
#include "scud_bms.h"

extern void initialise_monitor_handles(void);

#define BMS_CH (UART_0.channel)

/*
 * Busy-wait. At 144 MHz the inner loop body is ~5 cycles,
 * so 28800 iterations ≈ 1 ms.
 */
static void delay_ms(uint32_t ms)
{
    for (volatile uint32_t i = 0; i < ms * 28800U; i++) {}
}

/*
 * Transmit buf[0..len-1] over RS485.
 * Blocks until the USIC shift register has finished the last byte so that
 * all echo bytes have arrived in RXFIFO before the caller reads them.
 */
static void rs485_send(const uint8_t *buf, uint32_t len)
{
    for (uint32_t i = 0; i < len; i++) {
        while (XMC_USIC_CH_TXFIFO_IsFull(BMS_CH)) {}
        XMC_USIC_CH_TXFIFO_PutData(BMS_CH, (uint16_t)buf[i]);
    }
    while (!XMC_USIC_CH_TXFIFO_IsEmpty(BMS_CH)) {}
    while (!(XMC_UART_CH_GetStatusFlag(BMS_CH) & XMC_UART_CH_STATUS_FLAG_TRANSMISSION_IDLE)) {}
}

int main(void)
{
    DAVE_Init();
    initialise_monitor_handles();

    /* Polling mode — disable the DAVE-enabled FIFO interrupts. */
    NVIC_DisableIRQ((IRQn_Type)91); /* USIC1 SR1 — TX FIFO */
    NVIC_DisableIRQ((IRQn_Type)90); /* USIC1 SR0 — RX FIFO */

    XMC_DEBUG("BMS RS485 diagnostic (115200 8N1, USIC1_CH0 P2.14/P2.15)\r\n");

    /*
     * Analog read request frame (hand-verified against scud_bms.c build_frame):
     *   ~ VER  ADR  CID1 CID2 LENGTH INFO CHKSUM \r
     *   ~ 22   00   4A   42   E002   01   FD29   \r
     *
     * CHKSUM = two's-complement of ASCII body bytes = 0xFD29.
     */
    static const uint8_t req[] = {
        '~','2','2','0','0','4','A','4','2',
        'E','0','0','2','0','1','F','D','2','9','\r'
    };

    while (1)
    {
        /* --- 1. Print the frame we are about to send --- */
        XMC_DEBUG("TX  (%u bytes):", (unsigned)sizeof(req));
        for (uint32_t i = 0; i < sizeof(req); i++)
            XMC_DEBUG(" %02X", (unsigned)req[i]);
        XMC_DEBUG("\r\n");

        rs485_send(req, sizeof(req));

        /* --- 2. Collect everything: echo bytes then BMS response --- */
        uint8_t  raw[256];
        uint32_t raw_n = 0;
        uint32_t ticks = 500U * 28800U;   /* initial wait ~500 ms */
        while (ticks > 0 && raw_n < sizeof(raw)) {
            if (!XMC_USIC_CH_RXFIFO_IsEmpty(BMS_CH)) {
                raw[raw_n++] = (uint8_t)XMC_USIC_CH_RXFIFO_GetData(BMS_CH);
                ticks = 50U * 28800U;     /* extend ~50 ms after each byte */
            } else {
                ticks--;
            }
        }

        /* --- 3. Print everything raw --- */
        XMC_DEBUG("RAW  (%u bytes):", (unsigned)raw_n);
        for (uint32_t i = 0; i < raw_n; i++)
            XMC_DEBUG(" %02X", (unsigned)raw[i]);
        if (raw_n == 0)
            XMC_DEBUG(" (none)");
        XMC_DEBUG("\r\n");

        /* --- 4. Decode BMS response (raw[] = echo[0..19] + frame[20..]) --- */
        const uint32_t echo_len = sizeof(req);
        if (raw_n > echo_len)
        {
            ScudAnalog_t  analog;
            ScudStatus_t  st = SCUD_DecodeAnalog(&raw[echo_len], raw_n - echo_len, &analog);
            if (st == SCUD_OK)
                SCUD_PrintAnalog(&analog);
            else
                XMC_DEBUG("Decode: %s\r\n", SCUD_StatusStr(st));
        }
        XMC_DEBUG("\r\n");

        delay_ms(2000);
    }
}