/*
 * SCUD BMS RS485 driver for XMC4800.
 *
 * Implements the YDT 1363.3-style ASCII-hex frame protocol used by SCUD BMS
 * modules (ref: "SCUD BMS485 协议 A2 修订版 20251015.pdf").
 *
 * Wire format (all chars between SOI and EOI are uppercase ASCII hex):
 *   ~ VER(2) ADR(2) CID1(2) CID2(2) LENGTH(4) INFO(LENID) CHKSUM(4) \r
 *
 * Requires two DAVE APPs in the project:
 *   UART_0           — 115200 8N1, TX=P2.14, RX=P2.15 (USIC1_CH0)
 *   RS485_DE         — output, P5.8, initial LOW (RX mode)
 */

#include "scud_bms.h"
#include "DAVE.h"

/* Access the underlying USIC channel from the DAVE UART APP handle. */
#define BMS_CH  (UART_0.channel)

/* -------------------------------------------------------------------------
 * Protocol constants
 * ---------------------------------------------------------------------- */
#define SCUD_VER          0x22U   /* protocol version 2.2 */
#define SCUD_ADR          0x00U   /* pack address */
#define SCUD_CID1         0x4AU   /* BMS device type */
#define SCUD_CID2_ANALOG  0x42U
#define SCUD_CID2_ALARM   0x44U
#define SCUD_CMD_GROUP    0x01U   /* pack 1 */

/*
 * Timeout expressed as a tight-loop iteration count.
 * At 144 MHz with ~4 cycles/iteration this is roughly 1 second,
 * conservative enough to cover the BMS response time.
 */
#define SCUD_TIMEOUT_TICKS   36000000U


/* -------------------------------------------------------------------------
 * ASCII hex helpers
 * ---------------------------------------------------------------------- */

static uint8_t nibble_to_hex(uint8_t n)
{
    return (n < 10U) ? ((uint8_t)'0' + n) : ((uint8_t)'A' + (n - 10U));
}

static uint8_t hex_to_nibble(uint8_t c)
{
    if (c >= (uint8_t)'0' && c <= (uint8_t)'9') return (uint8_t)(c - '0');
    if (c >= (uint8_t)'A' && c <= (uint8_t)'F') return (uint8_t)(c - 'A' + 10U);
    if (c >= (uint8_t)'a' && c <= (uint8_t)'f') return (uint8_t)(c - 'a' + 10U);
    return 0U;
}

static uint8_t hex2(const uint8_t *s)
{
    return (uint8_t)((hex_to_nibble(s[0]) << 4) | hex_to_nibble(s[1]));
}

static uint16_t hex4(const uint8_t *s)
{
    return (uint16_t)(((uint16_t)hex_to_nibble(s[0]) << 12) |
                      ((uint16_t)hex_to_nibble(s[1]) <<  8) |
                      ((uint16_t)hex_to_nibble(s[2]) <<  4) |
                       (uint16_t)hex_to_nibble(s[3]));
}

/* -------------------------------------------------------------------------
 * Frame encoding
 * ---------------------------------------------------------------------- */

/* 4-bit LCHKSUM over a 12-bit LENID: sum the three nibbles, negate, mask. */
static uint8_t lchksum(uint16_t lenid)
{
    uint8_t s = (uint8_t)(((lenid >> 8) & 0xFU) + ((lenid >> 4) & 0xFU) + (lenid & 0xFU));
    return (uint8_t)((-s) & 0xFU);
}

/*
 * Build a complete request frame into out_buf.
 * The INFO for a BMS read request is always one byte (cmd_group).
 * Returns frame length on success, 0 if out_buf is too small.
 */
static uint32_t build_frame(uint8_t cid2, uint8_t cmd_group,
                             uint8_t *out_buf, uint32_t out_max)
{
    /* INFO = cmd_group (1 byte) → 2 ASCII hex chars → LENID = 2 */
    const uint16_t lenid  = 2U;
    const uint16_t length = (uint16_t)((lchksum(lenid) << 12U) | lenid);

    /*
     * Body (everything between ~ and CHKSUM):
     * VER(2) ADR(2) CID1(2) CID2(2) LENGTH(4) INFO(2) = 14 ASCII chars
     */
    uint8_t body[14];
    body[ 0] = nibble_to_hex(SCUD_VER >> 4);       body[ 1] = nibble_to_hex(SCUD_VER & 0xFU);
    body[ 2] = nibble_to_hex(SCUD_ADR >> 4);       body[ 3] = nibble_to_hex(SCUD_ADR & 0xFU);
    body[ 4] = nibble_to_hex(SCUD_CID1 >> 4);      body[ 5] = nibble_to_hex(SCUD_CID1 & 0xFU);
    body[ 6] = nibble_to_hex(cid2 >> 4);           body[ 7] = nibble_to_hex(cid2 & 0xFU);
    body[ 8] = nibble_to_hex((uint8_t)((length >> 12) & 0xFU));
    body[ 9] = nibble_to_hex((uint8_t)((length >>  8) & 0xFU));
    body[10] = nibble_to_hex((uint8_t)((length >>  4) & 0xFU));
    body[11] = nibble_to_hex((uint8_t)( length        & 0xFU));
    body[12] = nibble_to_hex(cmd_group >> 4);      body[13] = nibble_to_hex(cmd_group & 0xFU);

    /* 16-bit two's-complement checksum of all body bytes */
    uint32_t sum = 0U;
    for (uint32_t i = 0U; i < 14U; i++) sum += body[i];
    const uint16_t chksum = (uint16_t)((-sum) & 0xFFFFU);

    /* Total: ~ (1) + body (14) + chksum (4) + \r (1) = 20 bytes */
    if (out_max < 20U) return 0U;

    uint32_t fi = 0U;
    out_buf[fi++] = (uint8_t)'~';
    for (uint32_t i = 0U; i < 14U; i++) out_buf[fi++] = body[i];
    out_buf[fi++] = nibble_to_hex((uint8_t)((chksum >> 12) & 0xFU));
    out_buf[fi++] = nibble_to_hex((uint8_t)((chksum >>  8) & 0xFU));
    out_buf[fi++] = nibble_to_hex((uint8_t)((chksum >>  4) & 0xFU));
    out_buf[fi++] = nibble_to_hex((uint8_t)( chksum        & 0xFU));
    out_buf[fi++] = (uint8_t)'\r';
    return fi; /* 20 */
}

/* -------------------------------------------------------------------------
 * RS485 low-level I/O
 * ---------------------------------------------------------------------- */

static void rs485_transmit(const uint8_t *buf, uint32_t len)
{
    XMC_USIC_CH_RXFIFO_Flush(BMS_CH); /* discard stale RX bytes before TX */

    for (uint32_t i = 0U; i < len; i++)
    {
        while (XMC_USIC_CH_TXFIFO_IsFull(BMS_CH)) {}
        XMC_USIC_CH_TXFIFO_PutData(BMS_CH, (uint16_t)buf[i]);
    }

    while (!XMC_USIC_CH_TXFIFO_IsEmpty(BMS_CH)) {}
    /* Wait for shift register to finish the last byte so all echo bytes
     * have arrived in RXFIFO before we flush them. */
    while (!(XMC_UART_CH_GetStatusFlag(BMS_CH) & XMC_UART_CH_STATUS_FLAG_TRANSMISSION_IDLE)) {}

    /* THVD1406 receiver is always on; flush the TX echo. */
    XMC_USIC_CH_RXFIFO_Flush(BMS_CH);
}

/*
 * Read one complete frame (SOI=~ ... EOI=\r) into buf.
 * Resets the per-byte timeout each time a byte arrives so the overall
 * deadline applies to inter-byte gaps, not the full frame.
 * Returns byte count (≥ 2) on success, 0 on timeout.
 */
static uint32_t rs485_read_frame(uint8_t *buf, uint32_t max_len)
{
    uint32_t n     = 0U;
    uint32_t ticks = 0U;

    /* Hunt for SOI */
    while (ticks < SCUD_TIMEOUT_TICKS)
    {
        if (!XMC_USIC_CH_RXFIFO_IsEmpty(BMS_CH))
        {
            uint8_t b = (uint8_t)XMC_USIC_CH_RXFIFO_GetData(BMS_CH);
            if (b == (uint8_t)'~') { buf[n++] = b; break; }
        }
        ticks++;
    }
    if (n == 0U) return 0U;

    /* Accumulate until EOI (\r) */
    ticks = 0U;
    while (ticks < SCUD_TIMEOUT_TICKS && n < max_len)
    {
        if (!XMC_USIC_CH_RXFIFO_IsEmpty(BMS_CH))
        {
            uint8_t b = (uint8_t)XMC_USIC_CH_RXFIFO_GetData(BMS_CH);
            buf[n++] = b;
            if (b == (uint8_t)'\r') return n;
            ticks = 0U;
        }
        else
        {
            ticks++;
        }
    }
    /* Dump whatever arrived so we can see the raw frame */
    XMC_DEBUG("rx_frame timeout: n=%lu bytes:", (unsigned long)n);
    for (uint32_t i = 0U; i < n && i < 32U; i++)
        XMC_DEBUG(" %02X", (unsigned int)buf[i]);
    XMC_DEBUG("\n");
    return 0U;
}

/* -------------------------------------------------------------------------
 * Frame parsing
 * ---------------------------------------------------------------------- */

/*
 * Parse a raw on-wire frame into its binary INFO payload.
 * Verifies both the 16-bit CHKSUM and the 4-bit LCHKSUM embedded in LENGTH.
 * Writes the RTN/CID2 byte and decoded info bytes on SCUD_OK.
 *
 * Frame layout:
 *   raw[0]              = '~'
 *   raw[1 .. L-6]       = body: VER ADR CID1 RTN LENGTH INFO  (L-6 bytes)
 *   raw[L-5 .. L-2]     = CHKSUM (4 ASCII hex chars)
 *   raw[L-1]            = '\r'
 */
static ScudStatus_t parse_frame(const uint8_t *raw, uint32_t raw_len,
                                uint8_t *rtn_out,
                                uint8_t *info_out, uint32_t *info_len_out)
{
    /* Minimum: ~ + 12-char header + 4-char chksum + \r = 18 bytes */
    if (raw_len < 18U || raw[0] != (uint8_t)'~' || raw[raw_len - 1U] != (uint8_t)'\r')
        return SCUD_ERR_FRAMING;

    const uint8_t *body        = &raw[1];
    uint32_t       body_no_ck  = raw_len - 6U; /* body bytes before the 4-char chksum */

    /* 16-bit two's-complement checksum */
    uint32_t sum = 0U;
    for (uint32_t i = 0U; i < body_no_ck; i++) sum += body[i];
    uint16_t expected = (uint16_t)((-sum) & 0xFFFFU);
    uint16_t got      = hex4(&raw[raw_len - 5U]); /* CCCC before \r */
    if (expected != got) return SCUD_ERR_CHECKSUM;

    *rtn_out = hex2(&body[6]); /* CID2 in a response is the RTN code */

    /* LENGTH field: bits[15:12] = LCHKSUM, bits[11:0] = LENID */
    uint16_t length = hex4(&body[8]);
    uint16_t lenid  = length & 0x0FFFU;
    if (lchksum(lenid) != (uint8_t)(length >> 12)) return SCUD_ERR_CHECKSUM;
    if (lenid % 2U != 0U) return SCUD_ERR_FRAMING; /* must be even (pairs of hex chars) */

    uint32_t info_bytes = lenid / 2U;
    *info_len_out = info_bytes;
    for (uint32_t i = 0U; i < info_bytes; i++)
        info_out[i] = hex2(&body[12U + 2U * i]);

    return SCUD_OK;
}

/* -------------------------------------------------------------------------
 * Transaction helper
 * ---------------------------------------------------------------------- */

static ScudStatus_t do_txn(uint8_t cid2, uint8_t *info_out, uint32_t *info_len_out)
{
    uint8_t  tx_buf[24];
    uint8_t  rx_buf[256];
    uint8_t  rtn;

    uint32_t frame_len = build_frame(cid2, SCUD_CMD_GROUP, tx_buf, sizeof(tx_buf));
    if (frame_len == 0U) return SCUD_ERR_FRAMING;

    rs485_transmit(tx_buf, frame_len);

    uint32_t rx_len = rs485_read_frame(rx_buf, sizeof(rx_buf));
    if (rx_len == 0U) return SCUD_ERR_TIMEOUT;

    ScudStatus_t st = parse_frame(rx_buf, rx_len, &rtn, info_out, info_len_out);
    if (st != SCUD_OK) return st;
    if (rtn != 0x00U)  return SCUD_ERR_RTN;
    return SCUD_OK;
}

/* -------------------------------------------------------------------------
 * Public API
 * ---------------------------------------------------------------------- */

ScudStatus_t SCUD_ReadAnalog(ScudAnalog_t *out)
{
    uint8_t  info[128];
    uint32_t info_len;

    ScudStatus_t st = do_txn(SCUD_CID2_ANALOG, info, &info_len);
    if (st != SCUD_OK) return st;
    if (info_len < 27U) return SCUD_ERR_TOO_SHORT;

    out->data_flag   = info[0];
    out->pack_num    = info[1];
    out->total_mv    = (uint32_t)info[2]        | ((uint32_t)info[3]  << 8)
                     | ((uint32_t)info[4] << 16) | ((uint32_t)info[5] << 24);
    out->current_ma  = (int32_t)((uint32_t)info[6]        | ((uint32_t)info[7]  << 8)
                               | ((uint32_t)info[8] << 16) | ((uint32_t)info[9] << 24));
    out->soc         = info[10];
    out->soh         = info[11];
    out->cycle_count = (uint16_t)((uint16_t)info[12] | ((uint16_t)info[13] << 8));
    out->rmc_mah     = (uint32_t)info[14]        | ((uint32_t)info[15] << 8)
                     | ((uint32_t)info[16] << 16) | ((uint32_t)info[17] << 24);
    out->fcc_mah     = (uint32_t)info[18]        | ((uint32_t)info[19] << 8)
                     | ((uint32_t)info[20] << 16) | ((uint32_t)info[21] << 24);
    out->design_mah  = (uint32_t)info[22]        | ((uint32_t)info[23] << 8)
                     | ((uint32_t)info[24] << 16) | ((uint32_t)info[25] << 24);

    uint8_t  m   = info[26];
    uint32_t off = 27U;
    if (m > SCUD_MAX_CELLS) m = SCUD_MAX_CELLS;
    out->cell_count = m;

    if (info_len < off + (uint32_t)m * 2U + 4U) return SCUD_ERR_TOO_SHORT;
    for (uint8_t i = 0U; i < m; i++)
    {
        out->cell_mv[i] = (uint16_t)((uint16_t)info[off] | ((uint16_t)info[off + 1U] << 8));
        off += 2U;
    }
    out->cell_high_idx = info[off++];
    out->cell_low_idx  = info[off++];
    out->cell_vdiff_mv = (uint16_t)((uint16_t)info[off] | ((uint16_t)info[off + 1U] << 8));
    off += 2U;

    uint8_t n = info[off++];
    if (n > SCUD_MAX_TEMPS) n = SCUD_MAX_TEMPS;
    out->cell_temp_count = n;
    if (info_len < off + (uint32_t)n * 2U) return SCUD_ERR_TOO_SHORT;
    for (uint8_t i = 0U; i < n; i++)
    {
        out->cell_temps_c[i] = (int16_t)((uint16_t)info[off] | ((uint16_t)info[off + 1U] << 8));
        off += 2U;
    }

    out->other_temp_count = 0U;
    if (off < info_len)
    {
        uint8_t k = info[off++];
        if (k > SCUD_MAX_TEMPS) k = SCUD_MAX_TEMPS;
        out->other_temp_count = k;
        if (info_len >= off + (uint32_t)k * 2U)
        {
            for (uint8_t i = 0U; i < k; i++)
            {
                out->other_temps_c[i] =
                    (int16_t)((uint16_t)info[off] | ((uint16_t)info[off + 1U] << 8));
                off += 2U;
            }
        }
    }
    return SCUD_OK;
}

ScudStatus_t SCUD_ReadAlarms(ScudAlarms_t *out)
{
    uint8_t  info[32];
    uint32_t info_len;

    ScudStatus_t st = do_txn(SCUD_CID2_ALARM, info, &info_len);
    if (st != SCUD_OK) return st;
    if (info_len < 9U) return SCUD_ERR_TOO_SHORT;

    /* info[0]=DATA_FLAG, info[1]=PACK_NUM, info[2..8] = seven event bytes */
    out->voltage    = info[2];
    out->current    = info[3];
    out->cell_temp  = info[4];
    out->other_temp = info[5];
    out->capacity   = info[6];
    out->other1     = info[7];
    out->other2     = info[8];
    return SCUD_OK;
}

void SCUD_PrintAnalog(const ScudAnalog_t *t)
{
    uint8_t i;

    XMC_DEBUG("V=%lu.%03lu V  I=%ld mA  SOC=%u%%  Vdiff=%u mV  "
              "RMC=%lu.%03lu Ah  FCC=%lu.%03lu Ah  Cyc=%u\n",
              (unsigned long)(t->total_mv / 1000U),
              (unsigned long)(t->total_mv % 1000U),
              (long)t->current_ma,
              (unsigned int)t->soc,
              (unsigned int)t->cell_vdiff_mv,
              (unsigned long)(t->rmc_mah / 1000U),
              (unsigned long)(t->rmc_mah % 1000U),
              (unsigned long)(t->fcc_mah / 1000U),
              (unsigned long)(t->fcc_mah % 1000U),
              (unsigned int)t->cycle_count);

    XMC_DEBUG("Cells [%u]:", (unsigned int)t->cell_count);
    for (i = 0U; i < t->cell_count; i++)
        XMC_DEBUG(" %u", (unsigned int)t->cell_mv[i]);
    XMC_DEBUG("\n");

    XMC_DEBUG("Temps [%u+%u]:",
              (unsigned int)t->cell_temp_count,
              (unsigned int)t->other_temp_count);
    for (i = 0U; i < t->cell_temp_count; i++)
        XMC_DEBUG(" %d", (int)t->cell_temps_c[i]);
    for (i = 0U; i < t->other_temp_count; i++)
        XMC_DEBUG(" %d", (int)t->other_temps_c[i]);
    XMC_DEBUG("\n");
}

const char *SCUD_StatusStr(ScudStatus_t st)
{
    switch (st)
    {
        case SCUD_OK:            return "OK";
        case SCUD_ERR_TIMEOUT:   return "timeout";
        case SCUD_ERR_FRAMING:   return "framing error";
        case SCUD_ERR_CHECKSUM:  return "checksum error";
        case SCUD_ERR_RTN:       return "BMS returned error RTN";
        case SCUD_ERR_TOO_SHORT: return "response too short";
        default:                 return "unknown error";
    }
}
