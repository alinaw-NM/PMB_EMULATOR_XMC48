/**
 * @file pmb_can_driver.h
 * @brief PMB CAN Protocol Driver for XMC4800
 *
 * Implements master (broadcast) and slave (receive) CAN communication
 * for the PMB CAN Protocol v1.
 *
 * Protocol details:
 *   - 8-byte data frames (Bytes 8..1, big-endian in spec)
 *   - Bytes 8-7 are packet counter (reserved / header)
 *   - Data ID encoded in CAN arbitration ID
 *   - Even parity at the byte level for R/W registers (Mod 2 = 0)
 *   - Calibrated Value = (Raw - Offset) * Scale / 4095
 */

#ifndef PMB_CAN_DRIVER_H
#define PMB_CAN_DRIVER_H

#include <xmc_can.h>
#include <xmc_gpio.h>
#include <stdint.h>
#include <stdbool.h>

/* -----------------------------------------------------------------------
 * Configuration – adjust to your hardware
 * --------------------------------------------------------------------- */

/** CAN module to use (CAN0 on XMC4800) */
#define PMB_CAN_MODULE          CAN

/** CAN node number (0, 1, or 2 on XMC4800) */
#define PMB_CAN_NODE_NUM        0
#define PMB_CAN_NODE            CAN_NODE0

/** CAN bus bit-rate in bits/second */
#define PMB_CAN_BAUD_RATE       500000U

/** CAN peripheral clock (fCAN) – typically 144 MHz on XMC4800 */
#define PMB_CAN_FREQ_HZ         144000000U

/** TX Message Object index (0-63 available on XMC4800) */
#define PMB_CAN_TX_MO_IDX       0

/** RX Message Object index */
#define PMB_CAN_RX_MO_IDX       1

/** CAN TX pin  – P1.12 (alt function 2 on XMC4800, node 0) */
#define PMB_CAN_TX_PORT         XMC_GPIO_PORT1
#define PMB_CAN_TX_PIN          12U
#define PMB_CAN_TX_ALT          XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT2

/** CAN RX pin  – P1.13 (node 0 input B) */
#define PMB_CAN_RX_PORT         XMC_GPIO_PORT1
#define PMB_CAN_RX_PIN          13U
#define PMB_CAN_RX_INPUT        XMC_CAN_NODE_RECEIVE_INPUT_RXDB

/* -----------------------------------------------------------------------
 * PMB protocol constants
 * --------------------------------------------------------------------- */

/** Payload size in bytes (spec: Data Length = 8) */
#define PMB_FRAME_LEN           8U

/** Packet-counter bytes (bytes 8-7 in the spec = data[0..1] in CAN frame) */
#define PMB_PKT_CTR_HI_IDX      0U
#define PMB_PKT_CTR_LO_IDX      1U

/* -----------------------------------------------------------------------
 * PMB Data IDs (from spec sheet)
 * Only a representative set is listed – extend as needed.
 * --------------------------------------------------------------------- */
typedef enum {
    /* Telemetry – Read Only */
    PMB_ID_TELEM_GRP1           = 0x00,
    PMB_ID_TELEM_GRP2           = 0x01,
    PMB_ID_TELEM_GRP3           = 0x02,
    PMB_ID_TELEM_GRP4           = 0x03,
    PMB_ID_TELEM_GRP5           = 0x04,
    PMB_ID_TELEM_GRP6           = 0x05,
    PMB_ID_TELEM_GRP7           = 0x06,
    PMB_ID_TELEM_GRP8           = 0x07,
    PMB_ID_TELEM_GRP9           = 0x08,

    /* Commands / Status – Read Only */
    PMB_ID_COMP_PDG             = 0x1B,
    PMB_ID_PG_FAULT_GRP1        = 0x1C,
    PMB_ID_PG_FAULT_GRP2        = 0x1D,
    PMB_ID_PG_FAULT_GRP3        = 0x1E,

    /* Power Enable – R/W */
    PMB_ID_PWR_EN_GRP1          = 0x1F,
    PMB_ID_PWR_EN_GRP2          = 0x20,
    PMB_ID_PWR_EN_GRP3          = 0x21,
    PMB_ID_PERIPH_INPUTS        = 0x22,
    PMB_ID_PWR_EN_GRP4          = 0x23,

    /* Calibration Offsets – R/W */
    PMB_ID_ADC_OFS_GRP1         = 0x30,

    /* Calibration Scale – R/W */
    PMB_ID_ADC_SCL_GRP1         = 0x50,

    /* Calibrated EU Values – Read Only */
    PMB_ID_EU_GRP1              = 0x70,

    /* Fault Registers – R/W */
    PMB_ID_FAULT_REG1           = 0xA8,
    PMB_ID_FAULT_REG2           = 0xA9,
    PMB_ID_FAULT_REG3           = 0xAA,

    /* Soft Limit Faults – R/W */
    PMB_ID_SOFT_LIM_FAULT_GRP1  = 0xD0,
    PMB_ID_SOFT_LIM_FAULT_GRP2  = 0xD1,

    /* Hard Limit Faults – R/W */
    PMB_ID_HARD_LIM_FAULT_GRP1  = 0xD8,
    PMB_ID_HARD_LIM_FAULT_GRP2  = 0xD9,

    /* Build Info – R/W */
    PMB_ID_HW_FW_REV            = 0x1F8,

    /* Write Protect */
    PMB_ID_WRITE_PROTECT        = 0x1FA,
} PMB_DataID_t;

/* -----------------------------------------------------------------------
 * Public types
 * --------------------------------------------------------------------- */

/** Raw 8-byte PMB frame */
typedef struct {
    uint8_t data[PMB_FRAME_LEN];   /**< data[0]=Byte8 (MSB), data[7]=Byte1 (LSB) */
    uint16_t data_id;              /**< PMB Data ID (used as CAN arbitration ID) */
} PMB_Frame_t;

/** Callback type invoked when a frame is received (slave mode) */
typedef void (*PMB_RxCallback_t)(const PMB_Frame_t *frame);

/* -----------------------------------------------------------------------
 * Public API
 * --------------------------------------------------------------------- */

/**
 * @brief  Initialise the CAN peripheral and configure TX/RX message objects.
 * @return true on success, false on error.
 */
bool PMB_CAN_Init(void);

/**
 * @brief  Register a callback for received frames (slave role).
 *         Set to NULL to disable.
 */
void PMB_CAN_RegisterRxCallback(PMB_RxCallback_t cb);

/**
 * @brief  Transmit a PMB frame as master.
 * @param  frame  Pointer to frame to send (data_id used as arbitration ID).
 * @return true if frame was placed in TX FIFO, false on error.
 */
bool PMB_CAN_Transmit(const PMB_Frame_t *frame);

/**
 * @brief  Poll for received frames (call from main loop or task).
 *         Invokes the registered RxCallback if a new frame arrived.
 */
void PMB_CAN_Poll(void);

/**
 * @brief  Build a PMB frame with the packet counter pre-filled and
 *         even parity applied to the payload bytes (bytes 3-2 downto 1
 *         for R/W registers).
 *
 * @param  frame    Output frame.
 * @param  data_id  PMB Data ID.
 * @param  payload  6 payload bytes (payload[0] = Byte6, payload[5] = Byte1).
 * @param  apply_parity  Set true for R/W registers that require parity.
 */
void PMB_CAN_BuildFrame(PMB_Frame_t *frame, uint16_t data_id,
                        const uint8_t payload[6], bool apply_parity);

/**
 * @brief  Decode a 12-bit raw ADC value from two consecutive frame bytes.
 * @param  hi_byte  Upper byte (e.g. data[2] for Byte6).
 * @param  lo_byte  Lower byte (e.g. data[3] for Byte5).
 * @return 12-bit unsigned ADC count.
 */
uint16_t PMB_CAN_Decode12Bit(uint8_t hi_byte, uint8_t lo_byte);

/**
 * @brief  Decode a 16-bit signed engineering-unit value from two frame bytes.
 */
int16_t PMB_CAN_Decode16Bit(uint8_t hi_byte, uint8_t lo_byte);

/**
 * @brief  Apply calibration: Calibrated = (raw - offset) * scale / 4095
 */
int32_t PMB_CAN_Calibrate(uint16_t raw, int16_t offset, int16_t scale);

/**
 * @brief  Verify even parity on a byte (returns true if parity is correct).
 *         The spec defines parity bit as the LSB of each payload byte for
 *         R/W registers; Mod 2 sum of all bits = 0.
 */
bool PMB_CAN_CheckParity(uint8_t byte);

#endif /* PMB_CAN_DRIVER_H */
