/*
 * main.c — BMS RS485 diagnostic
 *
 * Sends one analog-read request, prints the TX echo to confirm the
 * send path works, then prints whatever the BMS sends back.
 *
 * Expected ECHO output: first 16 of the 20 TX bytes (RXFIFO capacity).
 * If ECHO is empty  → TX path broken (check P2.14 wiring / USIC config).
 * If ECHO ok, BMS none → BMS not responding (check A/B polarity, query format).
 * If both present  → proceed to frame parsing.
 */

#include "DAVE.h"
#include "xmc_uart.h"

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
    XMC_USIC_CH_RXFIFO_Flush(BMS_CH);

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

        /* --- 2. Read TX echo from RXFIFO --- */
        XMC_DEBUG("ECHO:");
        uint32_t echo_n = 0;
        while (!XMC_USIC_CH_RXFIFO_IsEmpty(BMS_CH)) {
            XMC_DEBUG(" %02X", (unsigned)XMC_USIC_CH_RXFIFO_GetData(BMS_CH));
            echo_n++;
        }
        if (echo_n == 0)
            XMC_DEBUG(" (none — TX path problem?)");
        XMC_DEBUG("\r\n");

        /*
         * --- 3. Wait for BMS response and print every byte ---
         *
         * ticks counts down at ~5 cycles/iteration (144 MHz → ~28800/ms).
         * Starts at ~500 ms; resets to ~50 ms on each received byte so
         * we stop ~50 ms after the last byte without cutting a response short.
         */
        XMC_DEBUG("BMS :");
        uint32_t rx_n  = 0;
        uint32_t ticks = 500U * 28800U;
        while (ticks > 0) {
            if (!XMC_USIC_CH_RXFIFO_IsEmpty(BMS_CH)) {
                XMC_DEBUG(" %02X", (unsigned)XMC_USIC_CH_RXFIFO_GetData(BMS_CH));
                rx_n++;
                ticks = 50U * 28800U;
            } else {
                ticks--;
            }
        }
        if (rx_n == 0)
            XMC_DEBUG(" (none — BMS not responding)");
        XMC_DEBUG("\r\n\r\n");

        delay_ms(2000);
    }
}
