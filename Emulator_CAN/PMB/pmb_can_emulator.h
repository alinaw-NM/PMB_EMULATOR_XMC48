/**
 * @file pmb_can_emulator.h
 * @brief PMB CAN Protocol Emulator - Header for XMC4800
 *
 * CAN ID format (29-bit Extended):
 *   Bits [28:26] = Priority (3-bit, 0=highest)
 *   Bits [25:16] = Data ID / Register Address (10-bit)
 *   Bits [15:8]  = Destination Address (8-bit)
 *   Bits [7:0]   = Source Address (8-bit)
 *
 * PMB address:            0xC0
 * Testing Fixture address: 0xA0
 *
 * Frame layout (8 bytes, DLC=8):
 *   data[0] = Byte 1 (LSB per spec), data[7] = Byte 8 (MSB per spec)
 *   data[7]:data[6] = Packet counter (reserved, Bytes 8-7 per spec)
 *   data[0..5]      = payload (Bytes 1-6)
 *
 * Data widths:
 *   12-bit unsigned: Byte N = FIELD[7:0], Byte N+1 = {4b0000, FIELD[11:8]}
 *   16-bit signed:   Byte N = FIELD[7:0], Byte N+1 = FIELD[15:8]   (little-endian)
 *   Bit-mapped:      full byte, parity bit at bit0 for R/W registers
 *
 * Parity rule (R/W parity-protected bytes): even parity, Mod 2 = 0.
 *   Bit 0 is the parity bit; bits [7:1] are the data bits.
 *   Set parity bit so that popcount(byte) is even.
 */

#ifndef PMB_CAN_EMULATOR_H
#define PMB_CAN_EMULATOR_H

#include "DAVE.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* =========================================================================
 * ADDRESSES
 * ========================================================================= */
#define PMB_ADDR                0xC0U
#define FIXTURE_ADDR            0xA0U

/* =========================================================================
 * CAN ID CONSTRUCTION/EXTRACTION  (29-bit Extended)
 * ========================================================================= */
#define PMB_CAN_BUILD_ID(prio, did, dst, src) \
    ( (((uint32_t)(prio) & 0x7UL)   << 26U) | \
      (((uint32_t)(did)  & 0x3FFUL) << 16U) | \
      (((uint32_t)(dst)  & 0xFFUL)  <<  8U) | \
      (((uint32_t)(src)  & 0xFFUL)       ) )

#define PMB_GET_PRIORITY(id)  (((id) >> 26U) & 0x7UL)
#define PMB_GET_DATA_ID(id)   (((id) >> 16U) & 0x3FFUL)
#define PMB_GET_DST(id)       (((id) >>  8U) & 0xFFUL)
#define PMB_GET_SRC(id)       (((id)       ) & 0xFFUL)

/* =========================================================================
 * DATA ID DEFINITIONS  (register addresses)
 * ========================================================================= */

/* --- Telemetry Raw ADC (Read Only) --- */
#define REG_TELEM_GRP1          0x00U  /* CHG_CURR, BATT1_CURR, BATT2_CURR (12-bit) */
#define REG_TELEM_GRP2          0x01U  /* LL_IMON, RL_IMON, LA_IMON */
#define REG_TELEM_GRP3          0x02U  /* RA_IMON, TR_IMON, CHG_IN */
#define REG_TELEM_GRP4          0x03U  /* HV, BATT1_IN, BATT2_IN */
#define REG_TELEM_GRP5          0x04U  /* NK_IMON, MOT_BUS, CHG_HV */
#define REG_TELEM_GRP6          0x05U  /* USER_HV, USER_HV_CURR */
#define REG_TELEM_GRP7          0x06U  /* PC_24V, PC_24V_IMON */
#define REG_TELEM_GRP8          0x07U  /* USER_24V, USER_24V_IMON */
#define REG_TELEM_GRP9          0x08U  /* PC_A_12V, PC_A_12V_IMON */
#define REG_TELEM_GRP10         0x09U  /* PC_B_12V, PC_B_12V_IMON */
#define REG_TELEM_GRP11         0x0AU  /* 24V_IMON0, 24V_IMON1, 24V_IMON2 */
#define REG_TELEM_GRP12         0x0BU  /* 12V_IMON0, 12V_IMON1, 12V_IMON2 */
#define REG_TELEM_GRP13         0x0CU  /* 24V_BUS_VOLTAGE, 12V_BUS_A_VOLTAGE */
#define REG_TELEM_GRP14         0x0DU  /* 5V, 5V_CURR */
#define REG_TELEM_GRP15         0x0EU  /* 3.3V, 3.3V_CURR */
#define REG_TELEM_GRP16         0x0FU  /* AUX_24V, AUX_24V_CURR */
#define REG_TELEM_GRP17         0x10U  /* AUX_12V, AUX_12V_CURR */
#define REG_TELEM_GRP18         0x11U  /* ESTOP_REC_A_12V, ESTOP_REC_B_12V */
#define REG_TELEM_GRP19         0x12U  /* 12V_B_IMON3, 12V_B_IMON4 */
#define REG_TELEM_GRP20         0x13U  /* HV_OR, MB_SP_IMON */
#define REG_TELEM_GRP21         0x14U  /* TEMP0, TEMP1, TEMP2 */
#define REG_TELEM_GRP22         0x15U  /* TEMP3, TEMP4, TEMP5 */
#define REG_TELEM_GRP23         0x16U  /* EX_TEMP0, EX_TEMP1 */
#define REG_TELEM_GRP24         0x17U  /* LIDAR_A_12V, LIDAR_B_12V */
#define REG_TELEM_GRP25         0x18U  /* STO_24V, CHG_HVIL_OUT */
#define REG_TELEM_GRP26         0x19U  /* ETH_2.5V, ETH_1.2V */
#define REG_TELEM_GRP27         0x1AU  /* ECAT_3.3V, ECAT_1.2V */

/* --- Command & Status (Read Only, bit-mapped) --- */
#define REG_COMP_PDG            0x1BU  /* Power Good from Comparator */
#define REG_PDG_FAULT_GRP1      0x1CU  /* Power Good & Fault Group 1 */
#define REG_PDG_FAULT_GRP2      0x1DU  /* Power Good & Fault Group 2 */
#define REG_PDG_FAULT_GRP3      0x1EU  /* Power Good & Fault Group 3 */

/* --- Power Enable (Read/Write, parity-protected) --- */
#define REG_PWR_EN_GRP1         0x1FU  /* ESC_EEPROM_WC, PC ignite + parity */
#define REG_PWR_EN_GRP2         0x20U  /* Main power enables + parity */
#define REG_PWR_EN_GRP3         0x21U  /* Battery/motor enables + parity */
#define REG_PERIPH_INPUTS       0x22U  /* Peripheral inputs (Read Only) */
#define REG_PWR_EN_GRP4         0x23U  /* Panel LEDs, ISO ctrl, PC inputs + parity */

/* --- Calibration Offsets 0x30-0x4A (Read/Write, 16-bit signed LE) --- */
#define REG_ADC_OFS_GRP1        0x30U  /* CHG_CURR_OFS, BATT1_CURR_OFS, BATT2_CURR_OFS */
#define REG_ADC_OFS_GRP2        0x31U  /* LL_IMON_OFS, RL_IMON_OFS, LA_IMON_OFS */
#define REG_ADC_OFS_GRP3        0x32U  /* RA_IMON_OFS, TR_IMON_OFS, CHG_IN_OFS */
#define REG_ADC_OFS_GRP4        0x33U  /* HV_OFS, BATT1_IN_OFS, BATT2_IN_OFS */
#define REG_ADC_OFS_GRP5        0x34U  /* NK_IMON_OFS, MOT_BUS_OFS, CHG_HV_OFS */
#define REG_ADC_OFS_GRP6        0x35U  /* USER_HV_OFS, USER_HV_CURR_OFS */
#define REG_ADC_OFS_GRP7        0x36U  /* PC_24V_OFS, PC_24V_IMON_OFS */
#define REG_ADC_OFS_GRP8        0x37U  /* USER_24V_OFS, USER_24V_IMON_OFS */
#define REG_ADC_OFS_GRP9        0x38U  /* PC_A_12V_OFS, PC_A_12V_IMON_OFS */
#define REG_ADC_OFS_GRP10       0x39U  /* PC_B_12V_OFS, PC_B_12V_IMON_OFS */
#define REG_ADC_OFS_GRP11       0x3AU  /* 24V_IMON0_OFS, 24V_IMON1_OFS, 24V_IMON2_OFS */
#define REG_ADC_OFS_GRP12       0x3BU  /* 12V_IMON0_OFS, 12V_IMON1_OFS, 12V_IMON2_OFS */
#define REG_ADC_OFS_GRP13       0x3CU  /* 24V_OFS, 12V_OFS */
#define REG_ADC_OFS_GRP14       0x3DU  /* 5V_OFS, 5V_CURR_OFS */
#define REG_ADC_OFS_GRP15       0x3EU  /* 3.3V_OFS, 3.3V_CURR_OFS */
#define REG_ADC_OFS_GRP16       0x3FU  /* AUX_24V_OFS, AUX_24V_CURR_OFS */
#define REG_ADC_OFS_GRP17       0x40U  /* AUX_12V_OFS, AUX_12V_CURR_OFS */
#define REG_ADC_OFS_GRP18       0x41U  /* ESTOP_REC_A_12V_OFS, ESTOP_REC_B_12V_OFS */
#define REG_ADC_OFS_GRP19       0x42U  /* 12V_B_IMON3_OFS, 12V_B_IMON4_OFS */
#define REG_ADC_OFS_GRP20       0x43U  /* HV_OR_OFS, MB_SP_IMON_OFS */
#define REG_ADC_OFS_GRP21       0x44U  /* TEMP0_OFS, TEMP1_OFS, TEMP2_OFS */
#define REG_ADC_OFS_GRP22       0x45U  /* TEMP3_OFS, TEMP4_OFS, TEMP5_OFS */
#define REG_ADC_OFS_GRP23       0x46U  /* EX_TEMP0_OFS, EX_TEMP1_OFS */
#define REG_ADC_OFS_GRP24       0x47U  /* LIDAR_A_12V_OFS, LIDAR_B_12V_OFS */
#define REG_ADC_OFS_GRP25       0x48U  /* STO_24V_OFS, CHG_HVIL_OUT_OFS */
#define REG_ADC_OFS_GRP26       0x49U  /* ETH_2.5V_OFS, ETH_1.2V_OFS */
#define REG_ADC_OFS_GRP27       0x4AU  /* ECAT_3.3V_OFS, ECAT_1.2V_OFS */

/* --- Calibration Scales 0x50-0x6A (same layout as offsets, _SCL suffix) --- */
#define REG_ADC_SCL_GRP1        0x50U
#define REG_ADC_SCL_GRP2        0x51U
#define REG_ADC_SCL_GRP3        0x52U
#define REG_ADC_SCL_GRP4        0x53U
#define REG_ADC_SCL_GRP5        0x54U
#define REG_ADC_SCL_GRP6        0x55U
#define REG_ADC_SCL_GRP7        0x56U
#define REG_ADC_SCL_GRP8        0x57U
#define REG_ADC_SCL_GRP9        0x58U
#define REG_ADC_SCL_GRP10       0x59U
#define REG_ADC_SCL_GRP11       0x5AU
#define REG_ADC_SCL_GRP12       0x5BU
#define REG_ADC_SCL_GRP13       0x5CU
#define REG_ADC_SCL_GRP14       0x5DU
#define REG_ADC_SCL_GRP15       0x5EU
#define REG_ADC_SCL_GRP16       0x5FU
#define REG_ADC_SCL_GRP17       0x60U
#define REG_ADC_SCL_GRP18       0x61U
#define REG_ADC_SCL_GRP19       0x62U
#define REG_ADC_SCL_GRP20       0x63U
#define REG_ADC_SCL_GRP21       0x64U
#define REG_ADC_SCL_GRP22       0x65U
#define REG_ADC_SCL_GRP23       0x66U
#define REG_ADC_SCL_GRP24       0x67U
#define REG_ADC_SCL_GRP25       0x68U
#define REG_ADC_SCL_GRP26       0x69U
#define REG_ADC_SCL_GRP27       0x6AU

/* --- Calibrated Engineering Unit Values 0x70-0x8A (Read Only, 16-bit signed LE) ---
 * EU formula: (raw - offset) * scale / 4095
 * Current: 1 count = 10mA, Voltage: 1 count = 10mV, Temp: 1 count = 100mC */
#define REG_EU_GRP1             0x70U  /* CHG_CURR_EU, BATT1_CURR_EU, BATT2_CURR_EU */
#define REG_EU_GRP2             0x71U  /* LL_IMON_EU, RL_IMON_EU, LA_IMON_EU */
#define REG_EU_GRP3             0x72U  /* RA_IMON_EU, TR_IMON_EU, CHG_IN_EU */
#define REG_EU_GRP4             0x73U  /* HV_EU, BATT1_IN_EU, BATT2_IN_EU */
#define REG_EU_GRP5             0x74U  /* NK_IMON_EU, MOT_BUS_EU, CHG_HV_EU */
#define REG_EU_GRP6             0x75U  /* USER_HV_EU [B2-1], USER_HV_CURR_EU [B4-3] */
#define REG_EU_GRP7             0x76U  /* PC_24V_EU [B2-1], PC_24V_IMON_EU [B4-3] */
#define REG_EU_GRP8             0x77U  /* USER_24V_EU, USER_24V_IMON_EU */
#define REG_EU_GRP9             0x78U  /* PC_A_12V_EU, PC_A_12V_IMON_EU */
#define REG_EU_GRP10            0x79U  /* PC_B_12V_EU, PC_B_12V_IMON_EU */
#define REG_EU_GRP11            0x7AU  /* 24V_IMON0_EU, 24V_IMON1_EU, 24V_IMON2_EU */
#define REG_EU_GRP12            0x7BU  /* 12V_IMON0_EU, 12V_IMON1_EU, 12V_IMON2_EU */
#define REG_EU_GRP13            0x7CU  /* 24V_EU, 12V_EU */
#define REG_EU_GRP14            0x7DU  /* 5V_EU, 5V_CURR_EU */
#define REG_EU_GRP15            0x7EU  /* 3.3V_EU, 3.3V_CURR_EU */
#define REG_EU_GRP16            0x7FU  /* AUX_24V_EU, AUX_24V_CURR_EU */
#define REG_EU_GRP17            0x80U  /* AUX_12V_EU, AUX_12V_CURR_EU */
#define REG_EU_GRP18            0x81U  /* ESTOP_REC_A_12V_EU, ESTOP_REC_B_12V_EU */
#define REG_EU_GRP19            0x82U  /* 12V_B_IMON3_EU, 12V_B_IMON4_EU */
#define REG_EU_GRP20            0x83U  /* HV_OR_EU, MB_SP_IMON_EU */
#define REG_EU_GRP21            0x84U  /* TEMP0_EU, TEMP1_EU, TEMP2_EU */
#define REG_EU_GRP22            0x85U  /* TEMP3_EU, TEMP4_EU, TEMP5_EU */
#define REG_EU_GRP23            0x86U  /* EX_TEMP0_EU, EX_TEMP1_EU */
#define REG_EU_GRP24            0x87U  /* LIDAR_A_12V_EU, LIDAR_B_12V_EU */
#define REG_EU_GRP25            0x88U  /* STO_24V_EU, CHG_HVIL_OUT_EU */
#define REG_EU_GRP26            0x89U  /* ETH_2.5V_EU, ETH_1.2V_EU */
#define REG_EU_GRP27            0x8AU  /* ECAT_3.3V_EU, ECAT_1.2V_EU */

/* --- Derived Values (Read Only, 16-bit signed LE) --- */
#define REG_DERIVED_GRP1        0x90U  /* 12V_A_TOTAL_CURR_EU [B2-1], 12V_B_TOTAL_CURR_EU [B4-3], 24V_TOTAL [B6-5] */
#define REG_DERIVED_GRP2        0x91U  /* BATT_TOTAL_CURR_EU [B2-1] */

/* --- Error Code (Read Only) --- */
#define REG_ERROR_REG           0xA0U  /* ERROR_CODE[31:0] in bytes 4-1 (32-bit unsigned) */

/* --- Fault Registers (Read/Write, parity at bit0, clear-on-write-1) --- */
#define REG_FAULT_REG1          0xA8U  /* Byte 1: {4b0000, PC_24V_IGNITE, PC_A_12V_IGNITE, PC_B_12V_IGNITE, Parity} */
#define REG_FAULT_REG2          0xA9U  /* Bytes 3-1: same bit layout as PWR_EN_GRP2 */
#define REG_FAULT_REG3          0xAAU  /* Bytes 4-1: same bit layout as PWR_EN_GRP3 */

/* --- Positive (Upper) Soft Limits 0xB0-0xCA (Read/Write, 16-bit signed LE) --- */
#define REG_PLIM_GRP1           0xB0U  /* CHG_CURR_LIM, BATT1_CURR_PLIM, BATT2_CURR_PLIM */
#define REG_PLIM_GRP2           0xB1U  /* LL_IMON_LIM, RL_IMON_LIM, LA_IMON_LIM */
#define REG_PLIM_GRP3           0xB2U  /* RA_IMON_LIM, TR_IMON_LIM, CHG_IN_LIM */
#define REG_PLIM_GRP4           0xB3U  /* HV_LIM, BATT1_IN_LIM, BATT2_IN_LIM */
#define REG_PLIM_GRP5           0xB4U  /* NK_IMON_LIM, MOT_BUS_LIM, CHG_HV_LIM */
#define REG_PLIM_GRP6           0xB5U  /* USER_HV_LIM, USER_HV_CURR_LIM */
#define REG_PLIM_GRP7           0xB6U  /* PC_24V_LIM, PC_24V_IMON_LIM */
#define REG_PLIM_GRP8           0xB7U  /* USER_24V_LIM, USER_24V_IMON_LIM */
#define REG_PLIM_GRP9           0xB8U  /* PC_A_12V_LIM, PC_A_12V_IMON_LIM */
#define REG_PLIM_GRP10          0xB9U  /* PC_B_12V_LIM, PC_B_12V_IMON_LIM */
#define REG_PLIM_GRP11          0xBAU  /* 24V_IMON0_LIM, 24V_IMON1_LIM, 24V_IMON2_LIM */
#define REG_PLIM_GRP12          0xBBU  /* 12V_IMON0_LIM, 12V_IMON1_LIM, 12V_IMON2_LIM */
#define REG_PLIM_GRP13          0xBCU  /* 24V_LIM, 12V_LIM */
#define REG_PLIM_GRP14          0xBDU  /* 5V_LIM, 5V_CURR_LIM */
#define REG_PLIM_GRP15          0xBEU  /* 3.3V_LIM, 3.3V_CURR_LIM */
#define REG_PLIM_GRP16          0xBFU  /* AUX_24V_LIM, AUX_24V_CURR_LIM */
#define REG_PLIM_GRP17          0xC0U  /* AUX_12V_LIM, AUX_12V_CURR_LIM */
#define REG_PLIM_GRP18          0xC1U  /* ESTOP_REC_A_12V_LIM, ESTOP_REC_B_12V_LIM */
#define REG_PLIM_GRP19          0xC2U  /* 12V_B_IMON3_LIM, 12V_B_IMON4_LIM */
#define REG_PLIM_GRP20          0xC3U  /* HV_OR_LIM, MB_SP_IMON_LIM */
#define REG_PLIM_GRP21          0xC4U  /* TEMP0_PLIM, TEMP1_PLIM, TEMP2_PLIM */
#define REG_PLIM_GRP22          0xC5U  /* TEMP3_PLIM, TEMP4_PLIM, TEMP5_PLIM */
#define REG_PLIM_GRP23          0xC6U  /* EX_TEMP0_PLIM, EX_TEMP1_PLIM */
#define REG_PLIM_GRP24          0xC7U  /* LIDAR_A_12V_LIM, LIDAR_B_12V_LIM */
#define REG_PLIM_GRP25          0xC8U  /* STO_24V_LIM, CHG_HVIL_OUT_LIM */
#define REG_PLIM_GRP26          0xC9U  /* ETH_2.5V_LIM, ETH_1.2V_LIM */
#define REG_PLIM_GRP27          0xCAU  /* ECAT_3.3V_LIM, ECAT_1.2V_LIM */

/* --- Negative (Lower) Soft Limits 0xCB-0xCF --- */
#define REG_NLIM_GRP1           0xCBU  /* 12V_A_TOTAL_CURR_LIM, 12V_B_TOTAL_CURR_LIM, 24V_TOTAL_CURR_LIM */
#define REG_NLIM_GRP2           0xCCU  /* BATT1_CURR_NLIM [B4-3], BATT2_CURR_NLIM [B6-5] */
#define REG_NLIM_GRP3           0xCDU  /* TEMP0_NLIM, TEMP1_NLIM, TEMP2_NLIM */
#define REG_NLIM_GRP4           0xCEU  /* TEMP3_NLIM, TEMP4_NLIM, TEMP5_NLIM */
#define REG_NLIM_GRP5           0xCFU  /* EX_TEMP0_NLIM, EX_TEMP1_NLIM */

/* --- Soft Limit Fault Flags 0xD0-0xD1 (Read/Write, clear-on-write-1) --- */
#define REG_SOFT_LIM_FLT_GRP1   0xD0U  /* 6 bytes of fault flags */
#define REG_SOFT_LIM_FLT_GRP2   0xD1U  /* 3 bytes of fault flags */

/* --- Hard Limit Fault Flags 0xD8-0xD9 (same layout as soft) --- */
#define REG_HARD_LIM_FLT_GRP1   0xD8U
#define REG_HARD_LIM_FLT_GRP2   0xD9U

/* --- Build Info --- */
#define REG_HW_FW_REVISION       0x1F8U /* HARDWARE_REVISION [B2-1], FIRMWARE_REVISION [B4-3] */

/* --- Write Protect --- */
#define REG_WRITE_PROTECT        0x1FAU

/* Write Protect byte 1 bits */
#define WP_LIM_BIT   (1U << 0)   /* Enables writes to limit registers */
#define WP_OFS_BIT   (1U << 1)   /* Enables writes to offset registers */
#define WP_SCL_BIT   (1U << 2)   /* Enables writes to scale registers */
#define WP_REV_BIT   (1U << 3)   /* Enables writes to revision register */

/* =========================================================================
 * EMULATED REGISTER STATE STRUCTS
 * ========================================================================= */

/** Raw 12-bit ADC telemetry values (27 groups, up to 3 channels each) */
typedef struct {
    uint16_t chg_curr;           /* 0x00 B2-1 */
    uint16_t batt1_curr;         /* 0x00 B4-3 */
    uint16_t batt2_curr;         /* 0x00 B6-5 */
    uint16_t ll_imon;            /* 0x01 B2-1 */
    uint16_t rl_imon;            /* 0x01 B4-3 */
    uint16_t la_imon;            /* 0x01 B6-5 */
    uint16_t ra_imon;            /* 0x02 B2-1 */
    uint16_t tr_imon;            /* 0x02 B4-3 */
    uint16_t chg_in;             /* 0x02 B6-5 */
    uint16_t hv;                 /* 0x03 B2-1 */
    uint16_t batt1_in;           /* 0x03 B4-3 */
    uint16_t batt2_in;           /* 0x03 B6-5 */
    uint16_t nk_imon;            /* 0x04 B2-1 */
    uint16_t mot_bus;            /* 0x04 B4-3 */
    uint16_t chg_hv;             /* 0x04 B6-5 */
    uint16_t user_hv;            /* 0x05 B2-1 */
    uint16_t user_hv_curr;       /* 0x05 B4-3 */
    uint16_t pc_24v;             /* 0x06 B2-1 */
    uint16_t pc_24v_imon;        /* 0x06 B4-3 */
    uint16_t user_24v;           /* 0x07 B2-1 */
    uint16_t user_24v_imon;      /* 0x07 B4-3 */
    uint16_t pc_a_12v;           /* 0x08 B2-1 */
    uint16_t pc_a_12v_imon;      /* 0x08 B4-3 */
    uint16_t pc_b_12v;           /* 0x09 B2-1 */
    uint16_t pc_b_12v_imon;      /* 0x09 B4-3 */
    uint16_t v24_imon0;          /* 0x0A B2-1 */
    uint16_t v24_imon1;          /* 0x0A B4-3 */
    uint16_t v24_imon2;          /* 0x0A B6-5 */
    uint16_t v12_imon0;          /* 0x0B B2-1 */
    uint16_t v12_imon1;          /* 0x0B B4-3 */
    uint16_t v12_imon2;          /* 0x0B B6-5 */
    uint16_t v24_bus_voltage;    /* 0x0C B2-1 */
    uint16_t v12_bus_a_voltage;  /* 0x0C B4-3 */
    uint16_t v5;                 /* 0x0D B2-1 */
    uint16_t v5_curr;            /* 0x0D B4-3 */
    uint16_t v3v3;               /* 0x0E B2-1 */
    uint16_t v3v3_curr;          /* 0x0E B4-3 */
    uint16_t aux_24v;            /* 0x0F B2-1 */
    uint16_t aux_24v_curr;       /* 0x0F B4-3 */
    uint16_t aux_12v;            /* 0x10 B2-1 */
    uint16_t aux_12v_curr;       /* 0x10 B4-3 */
    uint16_t estop_rec_a_12v;    /* 0x11 B2-1 */
    uint16_t estop_rec_b_12v;    /* 0x11 B4-3 */
    uint16_t v12_b_imon3;        /* 0x12 B2-1 */
    uint16_t v12_b_imon4;        /* 0x12 B4-3 */
    uint16_t hv_or;              /* 0x13 B2-1 */
    uint16_t mb_sp_imon;         /* 0x13 B4-3 */
    uint16_t temp0;              /* 0x14 B2-1 */
    uint16_t temp1;              /* 0x14 B4-3 */
    uint16_t temp2;              /* 0x14 B6-5 */
    uint16_t temp3;              /* 0x15 B2-1 */
    uint16_t temp4;              /* 0x15 B4-3 */
    uint16_t temp5;              /* 0x15 B6-5 */
    uint16_t ex_temp0;           /* 0x16 B2-1 */
    uint16_t ex_temp1;           /* 0x16 B4-3 */
    uint16_t lidar_a_12v;        /* 0x17 B2-1 */
    uint16_t lidar_b_12v;        /* 0x17 B4-3 */
    uint16_t sto_24v;            /* 0x18 B2-1 */
    uint16_t chg_hvil_out;       /* 0x18 B4-3 */
    uint16_t eth_2v5;            /* 0x19 B2-1 */
    uint16_t eth_1v2;            /* 0x19 B4-3 */
    uint16_t ecat_3v3;           /* 0x1A B2-1 */
    uint16_t ecat_1v2;           /* 0x1A B4-3 */
} PMB_Telemetry_t;

/** Bit-mapped status registers (Read Only) */
typedef struct {
    uint8_t comp_pdg[3];        /* 0x1B B1-B3 */
    uint8_t pdg_fault_grp1[3]; /* 0x1C B1-B3 */
    uint8_t pdg_fault_grp2[2]; /* 0x1D B1-B2 */
    uint8_t pdg_fault_grp3[2]; /* 0x1E B1-B2 */
} PMB_Status_t;

/** Power Enable registers (Read/Write, each byte has parity at bit 0) */
typedef struct {
    uint8_t grp1;               /* 0x1F B1 */
    uint8_t grp2[3];            /* 0x20 B1-B3 */
    uint8_t grp3[4];            /* 0x21 B1-B4 */
    uint8_t periph_inputs;      /* 0x22 B1 (read only) */
    uint8_t grp4[3];            /* 0x23 B1-B3 */
} PMB_PowerEnable_t;

/** Calibration offsets — 16-bit signed, little-endian in CAN frame */
typedef struct {
    int16_t chg_curr_ofs;
    int16_t batt1_curr_ofs;
    int16_t batt2_curr_ofs;
    int16_t ll_imon_ofs;
    int16_t rl_imon_ofs;
    int16_t la_imon_ofs;
    int16_t ra_imon_ofs;
    int16_t tr_imon_ofs;
    int16_t chg_in_ofs;
    int16_t hv_ofs;
    int16_t batt1_in_ofs;
    int16_t batt2_in_ofs;
    int16_t nk_imon_ofs;
    int16_t mot_bus_ofs;
    int16_t chg_hv_ofs;
    int16_t user_hv_ofs;
    int16_t user_hv_curr_ofs;
    int16_t pc_24v_ofs;
    int16_t pc_24v_imon_ofs;
    int16_t user_24v_ofs;
    int16_t user_24v_imon_ofs;
    int16_t pc_a_12v_ofs;
    int16_t pc_a_12v_imon_ofs;
    int16_t pc_b_12v_ofs;
    int16_t pc_b_12v_imon_ofs;
    int16_t v24_imon0_ofs;
    int16_t v24_imon1_ofs;
    int16_t v24_imon2_ofs;
    int16_t v12_imon0_ofs;
    int16_t v12_imon1_ofs;
    int16_t v12_imon2_ofs;
    int16_t v24_ofs;
    int16_t v12_ofs;
    int16_t v5_ofs;
    int16_t v5_curr_ofs;
    int16_t v3v3_ofs;
    int16_t v3v3_curr_ofs;
    int16_t aux_24v_ofs;
    int16_t aux_24v_curr_ofs;
    int16_t aux_12v_ofs;
    int16_t aux_12v_curr_ofs;
    int16_t estop_rec_a_12v_ofs;
    int16_t estop_rec_b_12v_ofs;
    int16_t v12_b_imon3_ofs;
    int16_t v12_b_imon4_ofs;
    int16_t hv_or_ofs;
    int16_t mb_sp_imon_ofs;
    int16_t temp0_ofs;
    int16_t temp1_ofs;
    int16_t temp2_ofs;
    int16_t temp3_ofs;
    int16_t temp4_ofs;
    int16_t temp5_ofs;
    int16_t ex_temp0_ofs;
    int16_t ex_temp1_ofs;
    int16_t lidar_a_12v_ofs;
    int16_t lidar_b_12v_ofs;
    int16_t sto_24v_ofs;
    int16_t chg_hvil_out_ofs;
    int16_t eth_2v5_ofs;
    int16_t eth_1v2_ofs;
    int16_t ecat_3v3_ofs;
    int16_t ecat_1v2_ofs;
} PMB_CalibOffset_t;

/** Calibration scales — same field layout as offsets, _scl suffix */
typedef struct {
    int16_t chg_curr_scl;
    int16_t batt1_curr_scl;
    int16_t batt2_curr_scl;
    int16_t ll_imon_scl;
    int16_t rl_imon_scl;
    int16_t la_imon_scl;
    int16_t ra_imon_scl;
    int16_t tr_imon_scl;
    int16_t chg_in_scl;
    int16_t hv_scl;
    int16_t batt1_in_scl;
    int16_t batt2_in_scl;
    int16_t nk_imon_scl;
    int16_t mot_bus_scl;
    int16_t chg_hv_scl;
    int16_t user_hv_scl;
    int16_t user_hv_curr_scl;
    int16_t pc_24v_scl;
    int16_t pc_24v_imon_scl;
    int16_t user_24v_scl;
    int16_t user_24v_imon_scl;
    int16_t pc_a_12v_scl;
    int16_t pc_a_12v_imon_scl;
    int16_t pc_b_12v_scl;
    int16_t pc_b_12v_imon_scl;
    int16_t v24_imon0_scl;
    int16_t v24_imon1_scl;
    int16_t v24_imon2_scl;
    int16_t v12_imon0_scl;
    int16_t v12_imon1_scl;
    int16_t v12_imon2_scl;
    int16_t v24_scl;
    int16_t v12_scl;
    int16_t v5_scl;
    int16_t v5_curr_scl;
    int16_t v3v3_scl;
    int16_t v3v3_curr_scl;
    int16_t aux_24v_scl;
    int16_t aux_24v_curr_scl;
    int16_t aux_12v_scl;
    int16_t aux_12v_curr_scl;
    int16_t estop_rec_a_12v_scl;
    int16_t estop_rec_b_12v_scl;
    int16_t v12_b_imon3_scl;
    int16_t v12_b_imon4_scl;
    int16_t hv_or_scl;
    int16_t mb_sp_imon_scl;
    int16_t temp0_scl;
    int16_t temp1_scl;
    int16_t temp2_scl;
    int16_t temp3_scl;
    int16_t temp4_scl;
    int16_t temp5_scl;
    int16_t ex_temp0_scl;
    int16_t ex_temp1_scl;
    int16_t lidar_a_12v_scl;
    int16_t lidar_b_12v_scl;
    int16_t sto_24v_scl;
    int16_t chg_hvil_out_scl;
    int16_t eth_2v5_scl;
    int16_t eth_1v2_scl;
    int16_t ecat_3v3_scl;
    int16_t ecat_1v2_scl;
} PMB_CalibScale_t;

/** Engineering Unit (calibrated) values — 16-bit signed, same channel layout */
typedef struct {
    int16_t chg_curr_eu;
    int16_t batt1_curr_eu;
    int16_t batt2_curr_eu;
    int16_t ll_imon_eu;
    int16_t rl_imon_eu;
    int16_t la_imon_eu;
    int16_t ra_imon_eu;
    int16_t tr_imon_eu;
    int16_t chg_in_eu;
    int16_t hv_eu;
    int16_t batt1_in_eu;
    int16_t batt2_in_eu;
    int16_t nk_imon_eu;
    int16_t mot_bus_eu;
    int16_t chg_hv_eu;
    int16_t user_hv_eu;
    int16_t user_hv_curr_eu;
    int16_t pc_24v_eu;
    int16_t pc_24v_imon_eu;
    int16_t user_24v_eu;
    int16_t user_24v_imon_eu;
    int16_t pc_a_12v_eu;
    int16_t pc_a_12v_imon_eu;
    int16_t pc_b_12v_eu;
    int16_t pc_b_12v_imon_eu;
    int16_t v24_imon0_eu;
    int16_t v24_imon1_eu;
    int16_t v24_imon2_eu;
    int16_t v12_imon0_eu;
    int16_t v12_imon1_eu;
    int16_t v12_imon2_eu;
    int16_t v24_eu;
    int16_t v12_eu;
    int16_t v5_eu;
    int16_t v5_curr_eu;
    int16_t v3v3_eu;
    int16_t v3v3_curr_eu;
    int16_t aux_24v_eu;
    int16_t aux_24v_curr_eu;
    int16_t aux_12v_eu;
    int16_t aux_12v_curr_eu;
    int16_t estop_rec_a_12v_eu;
    int16_t estop_rec_b_12v_eu;
    int16_t v12_b_imon3_eu;
    int16_t v12_b_imon4_eu;
    int16_t hv_or_eu;
    int16_t mb_sp_imon_eu;
    int16_t temp0_eu;
    int16_t temp1_eu;
    int16_t temp2_eu;
    int16_t temp3_eu;
    int16_t temp4_eu;
    int16_t temp5_eu;
    int16_t ex_temp0_eu;
    int16_t ex_temp1_eu;
    int16_t lidar_a_12v_eu;
    int16_t lidar_b_12v_eu;
    int16_t sto_24v_eu;
    int16_t chg_hvil_out_eu;
    int16_t eth_2v5_eu;
    int16_t eth_1v2_eu;
    int16_t ecat_3v3_eu;
    int16_t ecat_1v2_eu;
    /* Derived */
    int16_t v12_a_total_curr_eu; /* 0x90 B2-1 */
    int16_t v12_b_total_curr_eu; /* 0x90 B4-3 */
    int16_t v24_total_curr_eu;   /* 0x90 B6-5 */
    int16_t batt_total_curr_eu;  /* 0x91 B2-1 */
} PMB_EU_t;

/** Soft Limits — 16-bit signed, same channel layout as offsets/scales */
typedef struct {
    /* Positive limits */
    int16_t chg_curr_lim;
    int16_t batt1_curr_plim;
    int16_t batt2_curr_plim;
    int16_t ll_imon_lim;
    int16_t rl_imon_lim;
    int16_t la_imon_lim;
    int16_t ra_imon_lim;
    int16_t tr_imon_lim;
    int16_t chg_in_lim;
    int16_t hv_lim;
    int16_t batt1_in_lim;
    int16_t batt2_in_lim;
    int16_t nk_imon_lim;
    int16_t mot_bus_lim;
    int16_t chg_hv_lim;
    int16_t user_hv_lim;
    int16_t user_hv_curr_lim;
    int16_t pc_24v_lim;
    int16_t pc_24v_imon_lim;
    int16_t user_24v_lim;
    int16_t user_24v_imon_lim;
    int16_t pc_a_12v_lim;
    int16_t pc_a_12v_imon_lim;
    int16_t pc_b_12v_lim;
    int16_t pc_b_12v_imon_lim;
    int16_t v24_imon0_lim;
    int16_t v24_imon1_lim;
    int16_t v24_imon2_lim;
    int16_t v12_imon0_lim;
    int16_t v12_imon1_lim;
    int16_t v12_imon2_lim;
    int16_t v24_lim;
    int16_t v12_lim;
    int16_t v5_lim;
    int16_t v5_curr_lim;
    int16_t v3v3_lim;
    int16_t v3v3_curr_lim;
    int16_t aux_24v_lim;
    int16_t aux_24v_curr_lim;
    int16_t aux_12v_lim;
    int16_t aux_12v_curr_lim;
    int16_t estop_rec_a_12v_lim;
    int16_t estop_rec_b_12v_lim;
    int16_t v12_b_imon3_lim;
    int16_t v12_b_imon4_lim;
    int16_t hv_or_lim;
    int16_t mb_sp_imon_lim;
    int16_t temp0_plim;
    int16_t temp1_plim;
    int16_t temp2_plim;
    int16_t temp3_plim;
    int16_t temp4_plim;
    int16_t temp5_plim;
    int16_t ex_temp0_plim;
    int16_t ex_temp1_plim;
    int16_t lidar_a_12v_lim;
    int16_t lidar_b_12v_lim;
    int16_t sto_24v_lim;
    int16_t chg_hvil_out_lim;
    int16_t eth_2v5_lim;
    int16_t eth_1v2_lim;
    int16_t ecat_3v3_lim;
    int16_t ecat_1v2_lim;
    /* Total current limits (0xCB) */
    int16_t v12_a_total_curr_lim;
    int16_t v12_b_total_curr_lim;
    int16_t v24_total_curr_lim;
    /* Negative limits (0xCC-0xCF) */
    int16_t batt1_curr_nlim;
    int16_t batt2_curr_nlim;
    int16_t temp0_nlim;
    int16_t temp1_nlim;
    int16_t temp2_nlim;
    int16_t temp3_nlim;
    int16_t temp4_nlim;
    int16_t temp5_nlim;
    int16_t ex_temp0_nlim;
    int16_t ex_temp1_nlim;
} PMB_SoftLimits_t;

/** Fault registers (parity at bit 0, clear-on-write-1) */
typedef struct {
    uint8_t reg1;               /* 0xA8 B1 */
    uint8_t reg2[3];            /* 0xA9 B1-B3 */
    uint8_t reg3[4];            /* 0xAA B1-B4 */
} PMB_FaultRegs_t;

/** Limit fault flags (bit-mapped, clear-on-write-1) */
typedef struct {
    uint8_t soft_grp1[6];       /* 0xD0 B1-B6 */
    uint8_t soft_grp2[3];       /* 0xD1 B1-B3 */
    uint8_t hard_grp1[6];       /* 0xD8 B1-B6 */
    uint8_t hard_grp2[3];       /* 0xD9 B1-B3 */
} PMB_LimFaults_t;

/** Build info */
typedef struct {
    uint16_t hw_rev;            /* 0x1F8 B2-1 */
    uint16_t fw_rev;            /* 0x1F8 B4-3 */
} PMB_BuildInfo_t;

/** Write protect register */
typedef struct {
    uint8_t b1;  /* {4b0000, REV, SCL, OFS, LIM} */
    uint8_t b2;  /* {7b0000000, 0x1F-Byte1} */
    uint8_t b3;  /* {5b00000, 0x20-Byte3, 0x20-Byte2, 0x20-Byte1} */
    uint8_t b4;  /* {4b0000, 0x21-Byte4..Byte1} */
    uint8_t b5;  /* {5b00000, 0x23-Byte3..Byte1} */
} PMB_WriteProtect_t;

/** Complete emulated PMB register state */
typedef struct {
    PMB_Telemetry_t    telemetry;
    PMB_Status_t       status;
    PMB_PowerEnable_t  pwr_enable;
    PMB_CalibOffset_t  calib_offset;
    PMB_CalibScale_t   calib_scale;
    PMB_EU_t           eu;
    uint32_t           error_code;   /* 0xA0 */
    PMB_FaultRegs_t    fault_regs;
    PMB_SoftLimits_t   soft_limits;
    PMB_LimFaults_t    lim_faults;
    PMB_BuildInfo_t    build_info;
    PMB_WriteProtect_t write_protect;
    uint16_t           packet_counter;
} PMB_Registers_t;

/* =========================================================================
 * DAVE CAN MESSAGE OBJECT INDICES
 * MO0 = Receive from fixture, MO1 = Transmit to fixture
 * ========================================================================= */
#define PMB_RX_MO   0U
#define PMB_TX_MO   1U

extern MULTICAN_CONFIG_t MULTICAN_CONFIG_0;

/* =========================================================================
 * PUBLIC API
 * ========================================================================= */
void             PMB_Emulator_Init(void);
void             PMB_Emulator_Run(void);
void             PMB_UpdateEU(void);
PMB_Registers_t *PMB_GetRegs(void);

#endif /* PMB_CAN_EMULATOR_H */
