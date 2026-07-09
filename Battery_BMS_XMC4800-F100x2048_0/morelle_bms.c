/*
 * morelle_bms.c - CAN driver for Morelle Falcon 15 EB BMS.
 *
 * Node 0 @ 250 kbps, extended 29-bit IDs. On-board IFX1051LE transceiver
 * (D-Sub DE-9) is wired to P2.0 (N0_TXD, ALT1) and P14.3 (N0_RXDB).
 * TX = CAN_MO0, RX = CAN_MO1 (mask 0x1F00FF00 = any DID replying to host 0x40).
 */

#include "morelle_bms.h"
#include <DAVE.h>
#include <string.h>

/* ~1 s at 144 MHz */
#define CAN_TIMEOUT_TICKS  36000000UL

#define TX_MO  (MULTICAN_CONFIG_0.lmobj_ptr[0]->mo_ptr)
#define RX_MO  (MULTICAN_CONFIG_0.lmobj_ptr[1]->mo_ptr)

void Morelle_NodeInit(void)
{
    const XMC_CAN_NODE_NOMINAL_BIT_TIME_CONFIG_t baud_cfg = {
        .can_frequency = 144000000UL,
        .baudrate      = 250000UL,
        .sample_point  = (uint16_t)(80U * 100U),
        .sjw           = 1U
    };

    XMC_CAN_NODE_NominalBitTimeConfigure(CAN_NODE0, &baud_cfg);

    XMC_CAN_NODE_EnableConfigurationChange(CAN_NODE0);
    XMC_CAN_NODE_SetInitBit(CAN_NODE0);
    XMC_CAN_NODE_SetReceiveInput(CAN_NODE0, XMC_CAN_NODE_RECEIVE_INPUT_RXDCB);
    XMC_CAN_NODE_DisableConfigurationChange(CAN_NODE0);
    XMC_CAN_NODE_ResetInitBit(CAN_NODE0);

    XMC_CAN_AllocateMOtoNodeList(CAN, 0U, 0U);
    XMC_CAN_AllocateMOtoNodeList(CAN, 0U, 1U);

    const XMC_GPIO_CONFIG_t tx_pin_cfg = {
        .mode         = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT1,
        .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH
    };
    XMC_GPIO_Init(XMC_GPIO_PORT2, 0U, &tx_pin_cfg);

    /* Pull-up so pin sits recessive if the transceiver is absent; otherwise
     * the CAN engine waits forever for 11 recessive bits and never TXes. */
    const XMC_GPIO_CONFIG_t rx_pin_cfg = { .mode = XMC_GPIO_MODE_INPUT_PULL_UP };
    XMC_GPIO_Init(XMC_GPIO_PORT14, 3U, &rx_pin_cfg);

#if MORELLE_CAN_LOOPBACK
    /* Internal loopback: TX routes to RX without the transceiver.
     * If this works but external doesn't, blame wiring/transceiver. */
    XMC_CAN_NODE_EnableConfigurationChange(CAN_NODE0);
    XMC_CAN_NODE_SetInitBit(CAN_NODE0);
    CAN_NODE0->NPCR |= CAN_NODE_NPCR_LBM_Msk;
    XMC_CAN_NODE_DisableConfigurationChange(CAN_NODE0);
    XMC_CAN_NODE_ResetInitBit(CAN_NODE0);
#endif
}

/* Last received 8 raw payload bytes; updated on every recv_frame_raw(). */
uint8_t morelle_last_rx_raw[8];

static uint32_t wait_tx_result(void)
{
    uint32_t nsr;
    uint32_t timeout = 30000000UL;
    while (timeout--) {
        nsr = CAN_NODE0->NSR;
        if (nsr & (CAN_NODE_NSR_TXOK_Msk | CAN_NODE_NSR_LEC_Msk)) return nsr;
    }
    return CAN_NODE0->NSR;
}

static Morelle_Status_t send_request(uint8_t data_id)
{
    uint32_t timeout;
    uint8_t i;

    timeout = CAN_TIMEOUT_TICKS;
    while ((XMC_CAN_MO_GetStatus(TX_MO) & (uint32_t)XMC_CAN_MO_STATUS_TX_REQUEST) && --timeout) {}
    if (timeout == 0U) return MORELLE_ERR_TIMEOUT;

    TX_MO->can_identifier = MORELLE_REQ_ID(data_id);
    for (i = 0U; i < 8U; i++) TX_MO->can_data_byte[i] = 0U;

    XMC_CAN_MO_Config(TX_MO);

    /* Clear sticky TXOK/LEC so wait_tx_result() sees only this frame. */
    XMC_CAN_NODE_ClearStatus(CAN_NODE0, XMC_CAN_NODE_STATUS_TX_OK);
    CAN_NODE0->NSR &= ~CAN_NODE_NSR_LEC_Msk;

    /* XMC_DEBUG("Requesting data frame 0x%02X\n", (unsigned)data_id); */

    if (XMC_CAN_MO_Transmit(TX_MO) != XMC_CAN_STATUS_SUCCESS) {
        return MORELLE_ERR_TX_FAIL;
    }
    (void)wait_tx_result();
    return MORELLE_OK;
}

static void print_rx(uint32_t id, const uint8_t *d)
{
    /* XMC_DEBUG("[RX] ID=0x%08lX DATA: %02X %02X %02X %02X %02X %02X %02X %02X\n",
              (unsigned long)id,
              (unsigned)d[0], (unsigned)d[1], (unsigned)d[2], (unsigned)d[3],
              (unsigned)d[4], (unsigned)d[5], (unsigned)d[6], (unsigned)d[7]); */
    (void)id;
    (void)d;
}

/* Capture-only: no printf, safe inside multi-frame bursts. */
static Morelle_Status_t recv_frame_raw(uint8_t *payload_out, uint32_t *id_out)
{
    uint32_t timeout = CAN_TIMEOUT_TICKS;

    while (!(XMC_CAN_MO_GetStatus(RX_MO) & (uint32_t)XMC_CAN_MO_STATUS_NEW_DATA) && --timeout) {}
    if (timeout == 0U) return MORELLE_ERR_TIMEOUT;

    XMC_CAN_MO_Receive(RX_MO);

    memcpy(morelle_last_rx_raw, RX_MO->can_data_byte, 8U);
    if (id_out)      *id_out      = RX_MO->can_identifier;
    if (payload_out) memcpy(payload_out, RX_MO->can_data_byte, 8U);
    return MORELLE_OK;
}

static Morelle_Status_t recv_frame(uint8_t *payload_out, uint8_t *did_out)
{
    uint8_t pl[8];
    uint32_t rx_id;
    Morelle_Status_t st = recv_frame_raw(pl, &rx_id);
    if (st != MORELLE_OK) {
        XMC_DEBUG("[RX] TIMEOUT (no response frame)\n");
        return st;
    }
    print_rx(rx_id, pl);
    if (did_out)     *did_out     = (uint8_t)MORELLE_DID_FROM_ID(rx_id);
    if (payload_out) memcpy(payload_out, pl, 8U);
    return MORELLE_OK;
}

static Morelle_Status_t request_response(uint8_t data_id, uint8_t *payload_out)
{
    Morelle_Status_t st = send_request(data_id);
    if (st != MORELLE_OK) return st;
    return recv_frame(payload_out, NULL);
}

static uint16_t be16(const uint8_t *p)
{
    return (uint16_t)(((uint16_t)p[0] << 8) | (uint16_t)p[1]);
}

static uint32_t be32(const uint8_t *p)
{
    return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) |
           ((uint32_t)p[2] <<  8) |  (uint32_t)p[3];
}

Morelle_Status_t Morelle_GetPackTotals(Morelle_PackTotals_t *out)
{
    uint8_t pl[8];
    Morelle_Status_t st = request_response(MORELLE_DID_PACK_TOTALS, pl);
    if (st != MORELLE_OK) return st;

    out->pack_v    = (float)be16(&pl[0]) * 0.1f;
    /* raw 30000 = 0 A; > = charging, < = discharging */
    out->current_a = (float)((int32_t)be16(&pl[4]) - 30000) * 0.1f;
    out->soc_pct   = (float)be16(&pl[6]) * 0.1f;
    return MORELLE_OK;
}

Morelle_Status_t Morelle_GetCellExtremes(Morelle_CellExtremes_t *out)
{
    uint8_t pl[8];
    Morelle_Status_t st = request_response(MORELLE_DID_CELL_EXTREMES, pl);
    if (st != MORELLE_OK) return st;

    out->max_mv   = be16(&pl[0]);
    out->max_cell = pl[2];
    out->min_mv   = be16(&pl[3]);
    out->min_cell = pl[5];
    return MORELLE_OK;
}

Morelle_Status_t Morelle_GetTempExtremes(Morelle_TempExtremes_t *out)
{
    uint8_t pl[8];
    Morelle_Status_t st = request_response(MORELLE_DID_TEMP_EXTREMES, pl);
    if (st != MORELLE_OK) return st;

    out->max_c      = (int8_t)((int16_t)pl[0] - 40);
    out->max_sensor = pl[1];
    out->min_c      = (int8_t)((int16_t)pl[2] - 40);
    out->min_sensor = pl[3];
    return MORELLE_OK;
}

Morelle_Status_t Morelle_GetMosState(Morelle_MosState_t *out)
{
    uint8_t pl[8];
    Morelle_Status_t st = request_response(MORELLE_DID_MOS_STATE, pl);
    if (st != MORELLE_OK) return st;

    out->state         = pl[0];
    out->chg_mos       = pl[1];
    out->dsg_mos       = pl[2];
    out->life_ctr      = pl[3];
    out->remaining_mah = be32(&pl[4]);
    return MORELLE_OK;
}

Morelle_Status_t Morelle_GetStatusInfo(Morelle_StatusInfo_t *out)
{
    uint8_t pl[8];
    Morelle_Status_t st = request_response(MORELLE_DID_STATUS_INFO, pl);
    if (st != MORELLE_OK) return st;

    out->num_cells = pl[0];
    out->num_temps = pl[1];
    return MORELLE_OK;
}

#define MORELLE_MAX_VOLT_FRAMES  (((MORELLE_MAX_CELLS + 2U) / 3U) + 2U)

/* 0x95: multi-frame. Frame layout [0]=frame_num (1-based), [1..6]=3 x uint16 BE mV.
 * Route by pl[0] and skip 0/0xFF sentinels — the BMS emits them alongside
 * data frames. Capture raw into a local buffer and print AFTER: printf via
 * semihosting is slow enough to let subsequent burst frames overwrite the
 * single RX MO buffer before we get back to poll it. */
Morelle_Status_t Morelle_GetCellVoltages(Morelle_CellVoltages_t *out, uint8_t num_cells)
{
    uint8_t  raw[MORELLE_MAX_VOLT_FRAMES][8];
    uint32_t ids[MORELLE_MAX_VOLT_FRAMES];
    uint8_t  seen[MORELLE_MAX_CELLS];
    uint8_t  max_frames;
    uint8_t  captured;
    uint8_t  collected;
    uint8_t  attempts;
    uint8_t  frame_no;
    uint8_t  base;
    uint8_t  k;
    uint8_t  idx;
    Morelle_Status_t st;

    if (num_cells > MORELLE_MAX_CELLS) num_cells = (uint8_t)MORELLE_MAX_CELLS;
    for (k = 0U; k < num_cells; k++) seen[k] = 0U;
    collected = 0U;
    captured  = 0U;
    out->count = 0U;

    max_frames = (uint8_t)(((num_cells + 2U) / 3U) + 2U);
    if (max_frames > MORELLE_MAX_VOLT_FRAMES) max_frames = MORELLE_MAX_VOLT_FRAMES;

    st = send_request(MORELLE_DID_CELL_VOLTAGES);
    if (st != MORELLE_OK) return st;

    for (attempts = 0U; attempts < max_frames && collected < num_cells; attempts++) {
        st = recv_frame_raw(raw[captured], &ids[captured]);
        if (st != MORELLE_OK) break;
        captured++;

        frame_no = raw[captured - 1U][0];
        if (frame_no == 0U || frame_no == 0xFFU) continue;

        base = (uint8_t)((frame_no - 1U) * 3U);
        for (k = 0U; k < 3U; k++) {
            idx = (uint8_t)(base + k);
            if (idx >= num_cells) break;
            if (!seen[idx]) {
                out->mv[idx] = be16(&raw[captured - 1U][1U + k * 2U]);
                seen[idx] = 1U;
                collected++;
            }
        }
    }

    for (k = 0U; k < captured; k++) {
        print_rx(ids[k], raw[k]);
    }

    out->count = collected;
    return (collected == num_cells) ? MORELLE_OK : MORELLE_ERR_TIMEOUT;
}

/* 0x96: single frame; [0]=frame_num, [1..7]=uint8 temps (raw - 40 = °C). */
Morelle_Status_t Morelle_GetTemperatures(Morelle_Temperatures_t *out, uint8_t num_sensors)
{
    uint8_t pl[8];
    uint8_t i;
    Morelle_Status_t st;

    if (num_sensors > MORELLE_MAX_TEMPS) num_sensors = (uint8_t)MORELLE_MAX_TEMPS;

    st = send_request(MORELLE_DID_TEMPERATURES);
    if (st != MORELLE_OK) return st;
    st = recv_frame(pl, NULL);
    if (st != MORELLE_OK) return st;

    out->count = 0U;
    for (i = 0U; i < num_sensors && i < 7U; i++) {
        out->c[i] = (int8_t)((int16_t)pl[1U + i] - 40);
        out->count++;
    }
    return MORELLE_OK;
}

Morelle_Status_t Morelle_GetCellBalance(Morelle_CellBalance_t *out)
{
    uint8_t pl[8];
    Morelle_Status_t st = request_response(MORELLE_DID_CELL_BALANCE, pl);
    if (st != MORELLE_OK) return st;

    out->mask[0] = pl[0];
    out->mask[1] = pl[1];
    out->mask[2] = pl[2];
    return MORELLE_OK;
}

Morelle_Status_t Morelle_GetFaultStatus(Morelle_FaultStatus_t *out)
{
    uint8_t pl[8];
    Morelle_Status_t st = request_response(MORELLE_DID_FAULT_STATUS, pl);
    if (st != MORELLE_OK) return st;

    memcpy(out->b, pl, 8U);
    return MORELLE_OK;
}
