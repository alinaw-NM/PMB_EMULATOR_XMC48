/*
 * main.c — merged SCUD (RS485) + Morelle (CAN) BMS monitor
 *
 * Every 2 seconds:
 *   1. Poll SCUD BMS over RS485 (USIC1_CH0, P2.14/P2.15 @ 115200 8N1)
 *      via auto-direction transceiver (THVD1406DR).
 *   2. Poll Morelle BMS over CAN Node 0 (P2.0 TX / P14.3 RX @ 250 kbps,
 *      extended 29-bit IDs) via on-board IFX1051LE transceiver.
 *
 * Either BMS may be absent — poll failures are printed but do not stop the loop.
 */

#include "DAVE.h"
#include "xmc_uart.h"
#include "scud_bms.h"
#include "morelle_bms.h"
#include <stdint.h>

extern void initialise_monitor_handles(void);

#define BMS_CH (UART_0.channel)

/* ~1 ms delay at 144 MHz: 28800 iterations ≈ 1 ms. */
static void delay_ms(uint32_t ms)
{
    volatile uint32_t i;
    for (i = 0U; i < ms * 28800U; i++) {}
}

/*
 * Transmit buf[0..len-1] over RS485.
 * Blocks until the USIC shift register has finished the last byte so all
 * echo bytes have arrived in RXFIFO before the caller reads them.
 */
static void rs485_send(const uint8_t *buf, uint32_t len)
{
    for (uint32_t i = 0U; i < len; i++) {
        while (XMC_USIC_CH_TXFIFO_IsFull(BMS_CH)) {}
        XMC_USIC_CH_TXFIFO_PutData(BMS_CH, (uint16_t)buf[i]);
    }
    while (!XMC_USIC_CH_TXFIFO_IsEmpty(BMS_CH)) {}
    while (!(XMC_UART_CH_GetStatusFlag(BMS_CH) & XMC_UART_CH_STATUS_FLAG_TRANSMISSION_IDLE)) {}
}

/* ----- SCUD poll (one analog-read request per 2 s cycle) --------------- */

/*
 * Analog read request frame (hand-verified against scud_bms.c build_frame):
 *   ~ VER  ADR  CID1 CID2 LENGTH INFO CHKSUM \r
 *   ~ 22   00   4A   42   E002   01   FD29   \r
 */
static const uint8_t scud_req[] = {
    '~','2','2','0','0','4','A','4','2',
    'E','0','0','2','0','1','F','D','2','9','\r'
};

static void poll_scud(void)
{
    uint8_t  raw[256];
    uint32_t raw_n = 0U;
    uint32_t ticks;

    XMC_DEBUG("---------- SCUD (RS485) ----------\r\n");

    /* Uncomment to log the outgoing request bytes:
    XMC_DEBUG("TX  (%u bytes):", (unsigned)sizeof(scud_req));
    for (uint32_t i = 0U; i < sizeof(scud_req); i++)
        XMC_DEBUG(" %02X", (unsigned)scud_req[i]);
    XMC_DEBUG("\r\n");
    */

    rs485_send(scud_req, sizeof(scud_req));

    /* Collect echo + BMS response. ~500 ms initial wait, extend 50 ms per byte. */
    ticks = 500U * 28800U;
    while (ticks > 0U && raw_n < sizeof(raw)) {
        if (!XMC_USIC_CH_RXFIFO_IsEmpty(BMS_CH)) {
            raw[raw_n++] = (uint8_t)XMC_USIC_CH_RXFIFO_GetData(BMS_CH);
            ticks = 50U * 28800U;
        } else {
            ticks--;
        }
    }

    /* Uncomment to log the raw received bytes (echo + response):
    XMC_DEBUG("RAW (%u bytes):", (unsigned)raw_n);
    for (uint32_t i = 0U; i < raw_n; i++)
        XMC_DEBUG(" %02X", (unsigned)raw[i]);
    if (raw_n == 0U) XMC_DEBUG(" (none)");
    XMC_DEBUG("\r\n");
    */

    /* Response frame follows the echo (raw[0..echo_len-1]). */
    const uint32_t echo_len = sizeof(scud_req);
    if (raw_n > echo_len) {
        ScudAnalog_t analog;
        ScudStatus_t st = SCUD_DecodeAnalog(&raw[echo_len], raw_n - echo_len, &analog);
        if (st == SCUD_OK) SCUD_PrintAnalog(&analog);
        else               XMC_DEBUG("SCUD decode: %s\r\n", SCUD_StatusStr(st));
    } else {
        XMC_DEBUG("SCUD: no response (battery absent or bus quiet)\r\n");
    }
}

/* ----- Morelle poll (all data IDs each cycle) -------------------------- */

static void poll_morelle(void)
{
    Morelle_StatusInfo_t   info   = {13U, 4U};  /* 13-cell default; overridden by 0x94 */
    Morelle_PackTotals_t   pack;
    Morelle_CellExtremes_t cext;
    Morelle_TempExtremes_t text;
    Morelle_MosState_t     mos;
    Morelle_CellVoltages_t vcells;
    Morelle_Temperatures_t vtemps;
    Morelle_CellBalance_t  bal;
    Morelle_FaultStatus_t  fault;
    Morelle_Status_t       st;
    const char            *mos_str;
    uint8_t                any;
    uint8_t                i;

    XMC_DEBUG("---------- Morelle (CAN) ----------\r\n");

    /* 0x90 Pack Totals — scale floats to tenths (no %f under nano.specs). */
    st = Morelle_GetPackTotals(&pack);
    if (st == MORELLE_OK) {
        uint32_t pv_dv  = (uint32_t)(pack.pack_v * 10.0f + 0.5f);
        int32_t  cu_da  = (int32_t)(pack.current_a * 10.0f);
        uint32_t so_dp  = (uint32_t)(pack.soc_pct * 10.0f + 0.5f);
        uint32_t cu_abs = (uint32_t)((cu_da < 0) ? -cu_da : cu_da);
        XMC_DEBUG("[0x90] Pack: %lu.%luV  I=%s%lu.%luA  SoC=%lu.%lu%%\r\n",
                  (unsigned long)(pv_dv / 10U), (unsigned long)(pv_dv % 10U),
                  (cu_da < 0) ? "-" : "",
                  (unsigned long)(cu_abs / 10U), (unsigned long)(cu_abs % 10U),
                  (unsigned long)(so_dp / 10U), (unsigned long)(so_dp % 10U));
    }

    st = Morelle_GetCellExtremes(&cext);
    if (st == MORELLE_OK) {
        XMC_DEBUG("[0x91] Max: %u mV (cell %u)  Min: %u mV (cell %u)\r\n",
                  (unsigned)cext.max_mv, (unsigned)cext.max_cell,
                  (unsigned)cext.min_mv, (unsigned)cext.min_cell);
    }

    st = Morelle_GetTempExtremes(&text);
    if (st == MORELLE_OK) {
        XMC_DEBUG("[0x92] Max: %d C (sensor %u)  Min: %d C (sensor %u)\r\n",
                  (int)text.max_c, (unsigned)text.max_sensor,
                  (int)text.min_c, (unsigned)text.min_sensor);
    }

    st = Morelle_GetMosState(&mos);
    if (st == MORELLE_OK) {
        mos_str = (mos.state == 1U) ? "CHG" :
                  (mos.state == 2U) ? "DSG" : "IDLE";
        XMC_DEBUG("[0x93] State: %s  ChgMOS: %u  DsgMOS: %u  Rem: %lu mAh\r\n",
                  mos_str, (unsigned)mos.chg_mos, (unsigned)mos.dsg_mos,
                  (unsigned long)mos.remaining_mah);
    }

    /* 0x94 must precede 0x95/0x96 (feeds their cell/temp counts). */
    st = Morelle_GetStatusInfo(&info);
    if (st == MORELLE_OK) {
        XMC_DEBUG("[0x94] Status Info: Cells=%u  Temp sensors=%u\r\n",
                  (unsigned)info.num_cells, (unsigned)info.num_temps);
    }

    st = Morelle_GetCellVoltages(&vcells, info.num_cells);
    if (st == MORELLE_OK) {
        XMC_DEBUG("[0x95] Cell mV:");
        for (i = 0U; i < vcells.count; i++) XMC_DEBUG(" %u", (unsigned)vcells.mv[i]);
        XMC_DEBUG("\r\n");
    }

    st = Morelle_GetTemperatures(&vtemps, info.num_temps);
    if (st == MORELLE_OK) {
        XMC_DEBUG("[0x96] Temps C:");
        for (i = 0U; i < vtemps.count; i++) XMC_DEBUG(" %d", (int)vtemps.c[i]);
        XMC_DEBUG("\r\n");
    }

    st = Morelle_GetCellBalance(&bal);
    if (st == MORELLE_OK) {
        XMC_DEBUG("[0x97] Balance mask: %02X %02X %02X\r\n",
                  (unsigned)bal.mask[0], (unsigned)bal.mask[1], (unsigned)bal.mask[2]);
    }

    st = Morelle_GetFaultStatus(&fault);
    if (st == MORELLE_OK) {
        any = 0U;
        for (i = 0U; i < 8U; i++) any |= fault.b[i];
        XMC_DEBUG("[0x98] %02X %02X %02X %02X %02X %02X %02X %02X  %s\r\n",
                  (unsigned)fault.b[0], (unsigned)fault.b[1],
                  (unsigned)fault.b[2], (unsigned)fault.b[3],
                  (unsigned)fault.b[4], (unsigned)fault.b[5],
                  (unsigned)fault.b[6], (unsigned)fault.b[7],
                  any ? "<FAULT>" : "OK");
    }
}

int main(void)
{
    DAVE_STATUS_t status;

    status = DAVE_Init();
    initialise_monitor_handles();

    if (status != DAVE_STATUS_SUCCESS) {
        XMC_DEBUG("DAVE APPs initialization failed\r\n");
        while (1U) {}
    }

    /*
     * SCUD driver polls RX FIFO directly — disable the DAVE-enabled USIC1
     * FIFO ISRs so RX bytes stay in the FIFO for us to read.
     */
    NVIC_DisableIRQ((IRQn_Type)91); /* USIC1 SR1 — TX FIFO */
    NVIC_DisableIRQ((IRQn_Type)90); /* USIC1 SR0 — RX FIFO */

    /* Configure CAN Node 0 (250 kbps, MO0=TX, MO1=RX, RX pin P14.3/RXDCB). */
    Morelle_NodeInit();

    XMC_DEBUG("\r\n=== Battery_BMS started ===\r\n");
    XMC_DEBUG("SCUD:    RS485 115200 8N1, USIC1_CH0 P2.14/P2.15 (auto-dir)\r\n");
    XMC_DEBUG("Morelle: CAN Node 0 250 kbps, P2.0/P14.3, battery addr 0x%02X\r\n\r\n",
              (unsigned)MORELLE_BATT_ADDR);

    for (;;) {
        XMC_DEBUG("\r\n========== BMS Poll ==========\r\n");
        poll_scud();
        poll_morelle();
        delay_ms(2000U);
    }
}
