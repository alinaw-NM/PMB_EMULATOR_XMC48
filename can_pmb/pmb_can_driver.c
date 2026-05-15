/**
 * @file pmb_can_driver.c
 * @brief PMB CAN Protocol Driver – XMC4800 implementation
 *
 * Uses Infineon's XMC Peripheral Library (XMClib) CAN driver,
 * following the same DAVE-based project structure as the EtherCAT
 * reference project (ETHCAT_SSC_XMC48).
 *
 * Build environment: DAVE 4.x / GCC ARM
 * Include paths required:
 *   Libraries/XMCLib/inc
 *   Libraries/CMSIS/Include
 *   Libraries/CMSIS/Infineon/XMC4800_series/Include
 */

#include "pmb_can_driver.h"
#include <string.h>   /* memset, memcpy */

/* -----------------------------------------------------------------------
 * Private state
 * --------------------------------------------------------------------- */

static PMB_RxCallback_t s_rx_cb = NULL;
static uint16_t         s_pkt_ctr = 0U;   /* rolling packet counter */
static bool             s_initialized = false;

/* -----------------------------------------------------------------------
 * Forward declarations
 * --------------------------------------------------------------------- */
static void     can_gpio_init(void);
static bool     can_node_init(void);
static bool     can_mo_tx_init(void);
static bool     can_mo_rx_init(void);
static uint8_t  compute_parity(uint8_t byte);

/* =======================================================================
 * Public API
 * ===================================================================== */

bool PMB_CAN_Init(void)
{
    if (s_initialized) {
        return true;
    }

    /* 1. Enable CAN module clock */
    XMC_CAN_Init(PMB_CAN_MODULE, XMC_CAN_CANCLKSRC_FPERI, PMB_CAN_FREQ_HZ);

    /* 2. Configure GPIO pins */
    can_gpio_init();

    /* 3. Configure the CAN node (bit timing) */
    if (!can_node_init()) {
        return false;
    }

    /* 4. Configure TX message object */
    if (!can_mo_tx_init()) {
        return false;
    }

    /* 5. Configure RX message object (accept all IDs by default) */
    if (!can_mo_rx_init()) {
        return false;
    }

    /* 6. Enable the node */
    XMC_CAN_NODE_EnableConfiguration(PMB_CAN_NODE);
    XMC_CAN_NODE_ReSetInitBit(PMB_CAN_NODE);   /* go bus-on */

    s_initialized = true;
    return true;
}

void PMB_CAN_RegisterRxCallback(PMB_RxCallback_t cb)
{
    s_rx_cb = cb;
}

bool PMB_CAN_Transmit(const PMB_Frame_t *frame)
{
    if (!s_initialized || frame == NULL) {
        return false;
    }

    XMC_CAN_MO_t tx_mo;
    memset(&tx_mo, 0, sizeof(tx_mo));

    /* Set arbitration ID (11-bit standard frame) */
    tx_mo.can_id_mode    = XMC_CAN_FRAME_TYPE_STANDARD_11BITS;
    tx_mo.can_identifier = (uint32_t)frame->data_id & 0x7FFU;
    tx_mo.can_id_mask    = 0x7FFU;
    tx_mo.can_mo_type    = XMC_CAN_MO_TYPE_TRANSMSGOBJ;
    tx_mo.can_data_length = PMB_FRAME_LEN;

    /* Copy payload – XMClib stores data[0] in the lower word (Byte0=LSByte) */
    memcpy(tx_mo.can_data_byte, frame->data, PMB_FRAME_LEN);

    /* Load the message object */
    XMC_CAN_MO_Config(&tx_mo);
    XMC_CAN_AllocateMOtoNodeList(PMB_CAN_MODULE, PMB_CAN_NODE_NUM,
                                 PMB_CAN_TX_MO_IDX);

    /* Request transmission */
    XMC_CAN_MO_SetStatus(&tx_mo, XMC_CAN_MO_RESET_STATUS_TX_PENDING);
    XMC_CAN_MO_Transmit(&tx_mo);

    return true;
}

void PMB_CAN_Poll(void)
{
    if (!s_initialized) {
        return;
    }

    XMC_CAN_MO_t rx_mo;
    memset(&rx_mo, 0, sizeof(rx_mo));
    rx_mo.can_mo_ptr = (CAN_MO_TypeDef *)((uint32_t)CAN_MO +
                        (0x20U * PMB_CAN_RX_MO_IDX));

    /* Check if a new message was received */
    uint32_t status = XMC_CAN_MO_GetStatus(&rx_mo);
    if (!(status & XMC_CAN_MO_STATUS_RX_PENDING)) {
        return;   /* nothing yet */
    }

    /* Read the message */
    XMC_CAN_MO_Receive(&rx_mo);

    /* Clear pending flag */
    XMC_CAN_MO_ResetStatus(&rx_mo, XMC_CAN_MO_RESET_STATUS_RX_PENDING);

    if (s_rx_cb != NULL) {
        PMB_Frame_t frame;
        frame.data_id = (uint16_t)(rx_mo.can_identifier & 0x7FFU);
        memcpy(frame.data, rx_mo.can_data_byte, PMB_FRAME_LEN);
        s_rx_cb(&frame);
    }
}

void PMB_CAN_BuildFrame(PMB_Frame_t *frame, uint16_t data_id,
                        const uint8_t payload[6], bool apply_parity)
{
    if (frame == NULL || payload == NULL) {
        return;
    }

    frame->data_id = data_id;

    /* Bytes 8-7 (data[0..1]): packet counter (big-endian) */
    frame->data[PMB_PKT_CTR_HI_IDX] = (uint8_t)(s_pkt_ctr >> 8U);
    frame->data[PMB_PKT_CTR_LO_IDX] = (uint8_t)(s_pkt_ctr & 0xFFU);
    s_pkt_ctr++;

    /* Bytes 6-1 (data[2..7]): application payload */
    for (uint8_t i = 0; i < 6U; i++) {
        frame->data[2U + i] = payload[i];
    }

    /*
     * Even parity per the spec:
     * "Even Parity for R/W registers at the byte level, Mod 2 = 0"
     * The LSB of each payload byte IS the parity bit.
     * We recompute it so the total popcount of each byte is even.
     */
    if (apply_parity) {
        for (uint8_t i = 2U; i < PMB_FRAME_LEN; i++) {
            /* Clear LSB, then set it to make popcount even */
            frame->data[i] = (frame->data[i] & 0xFEU) |
                              compute_parity(frame->data[i] & 0xFEU);
        }
    }
}

uint16_t PMB_CAN_Decode12Bit(uint8_t hi_byte, uint8_t lo_byte)
{
    /*
     * Spec: e.g. {4b0000, BATT2_CURR[11:8]}, BATT2_CURR[7:0]
     * Upper nibble of hi_byte is padding zeros.
     */
    return (uint16_t)(((uint16_t)(hi_byte & 0x0FU) << 8U) | lo_byte);
}

int16_t PMB_CAN_Decode16Bit(uint8_t hi_byte, uint8_t lo_byte)
{
    return (int16_t)(((uint16_t)hi_byte << 8U) | lo_byte);
}

int32_t PMB_CAN_Calibrate(uint16_t raw, int16_t offset, int16_t scale)
{
    /* Calibrated = (raw - offset) * scale / 4095 */
    int32_t tmp = (int32_t)raw - (int32_t)offset;
    return (tmp * (int32_t)scale) / 4095;
}

bool PMB_CAN_CheckParity(uint8_t byte)
{
    /* Even parity: number of 1-bits must be even (Mod 2 = 0) */
    uint8_t count = 0U;
    uint8_t b = byte;
    while (b) {
        count += (b & 1U);
        b >>= 1U;
    }
    return (count % 2U) == 0U;
}

/* =======================================================================
 * Private helpers
 * ===================================================================== */

static uint8_t compute_parity(uint8_t byte)
{
    /* Returns 1 if an odd number of bits are set (so adding makes it even) */
    uint8_t p = byte;
    p ^= (p >> 4U);
    p ^= (p >> 2U);
    p ^= (p >> 1U);
    return p & 1U;
}

static void can_gpio_init(void)
{
    /* TX pin: alternate function output */
    XMC_GPIO_CONFIG_t tx_cfg = {
        .mode         = PMB_CAN_TX_ALT,
        .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH
    };
    XMC_GPIO_Init(PMB_CAN_TX_PORT, PMB_CAN_TX_PIN, &tx_cfg);

    /* RX pin: input, no pull (bus termination provides bias) */
    XMC_GPIO_CONFIG_t rx_cfg = {
        .mode = XMC_GPIO_MODE_INPUT_TRISTATE
    };
    XMC_GPIO_Init(PMB_CAN_RX_PORT, PMB_CAN_RX_PIN, &rx_cfg);

    /* Connect RX pin to CAN node input */
    XMC_CAN_NODE_EnableConfigurationChange(PMB_CAN_NODE);
    XMC_CAN_NODE_SetReceiveInput(PMB_CAN_NODE, PMB_CAN_RX_INPUT);
}

static bool can_node_init(void)
{
    XMC_CAN_NODE_NOMINAL_BIT_TIME_CONFIG_t bit_time = {0};

    /* XMClib will calculate BRP/SJW/TSEG1/TSEG2 from freq + baud-rate */
    bit_time.can_frequency = PMB_CAN_FREQ_HZ;
    bit_time.baudrate      = PMB_CAN_BAUD_RATE;
    bit_time.sample_point  = 8000U;   /* 80.00 % sample point (x100) */
    bit_time.sjw           = 1U;

    XMC_CAN_NODE_EnableConfigurationChange(PMB_CAN_NODE);

    XMC_CAN_STATUS_t status =
        XMC_CAN_NODE_NominalBitTimeConfigure(PMB_CAN_NODE, &bit_time);

    return (status == XMC_CAN_STATUS_SUCCESS);
}

static bool can_mo_tx_init(void)
{
    XMC_CAN_MO_t tx_mo;
    memset(&tx_mo, 0, sizeof(tx_mo));

    tx_mo.can_mo_ptr      = (CAN_MO_TypeDef *)((uint32_t)CAN_MO +
                             (0x20U * PMB_CAN_TX_MO_IDX));
    tx_mo.can_id_mode     = XMC_CAN_FRAME_TYPE_STANDARD_11BITS;
    tx_mo.can_identifier  = 0x000U;  /* overwritten on each Transmit() call */
    tx_mo.can_id_mask     = 0x7FFU;
    tx_mo.can_mo_type     = XMC_CAN_MO_TYPE_TRANSMSGOBJ;
    tx_mo.can_data_length = PMB_FRAME_LEN;

    XMC_CAN_MO_Config(&tx_mo);
    XMC_CAN_AllocateMOtoNodeList(PMB_CAN_MODULE, PMB_CAN_NODE_NUM,
                                 PMB_CAN_TX_MO_IDX);
    return true;
}

static bool can_mo_rx_init(void)
{
    XMC_CAN_MO_t rx_mo;
    memset(&rx_mo, 0, sizeof(rx_mo));

    rx_mo.can_mo_ptr      = (CAN_MO_TypeDef *)((uint32_t)CAN_MO +
                             (0x20U * PMB_CAN_RX_MO_IDX));
    rx_mo.can_id_mode     = XMC_CAN_FRAME_TYPE_STANDARD_11BITS;
    rx_mo.can_identifier  = 0x000U;
    rx_mo.can_id_mask     = 0x000U;  /* accept ALL IDs */
    rx_mo.can_mo_type     = XMC_CAN_MO_TYPE_RECVMSGOBJ;
    rx_mo.can_data_length = PMB_FRAME_LEN;

    XMC_CAN_MO_Config(&rx_mo);
    XMC_CAN_AllocateMOtoNodeList(PMB_CAN_MODULE, PMB_CAN_NODE_NUM,
                                 PMB_CAN_RX_MO_IDX);
    return true;
}
