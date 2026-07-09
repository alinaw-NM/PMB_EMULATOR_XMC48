/*******************************************************************************
 * pmb_v2_can_emulator.h
 * PMB CAN Protocol v2 emulator — header
 *
 * CAN ID (29-bit extended):  [28:26]=priority  [25:16]=data_id(10b)
 *                             [15:8]=dst        [7:0]=src
 * Frame: DLC=8.  Byte8-7 (data[7:6]) = packet counter (echoed in response).
 *                Byte6-1 (data[5:0]) = payload.
 * Read request:  payload all-zero.
 * Write request: non-zero payload.
 * Parity (bit-mapped R/W registers): even parity over bits[7:1], stored in bit0.
 ******************************************************************************/
#ifndef PMB_V2_CAN_EMULATOR_H
#define PMB_V2_CAN_EMULATOR_H

#include <stdint.h>
#include <DAVE.h>

/* ---- Protocol addresses ---- */
#define PMB_ADDR      (0xC0U)
#define FIXTURE_ADDR  (0xA0U)

/* ---- CAN ID helpers ---- */
#define PMB_CAN_BUILD_ID(pri, did, dst, src) \
    (((uint32_t)(pri)  << 26U) | \
     ((uint32_t)(did)  << 16U) | \
     ((uint32_t)(dst)  <<  8U) | \
      (uint32_t)(src))
#define PMB_GET_DATA_ID(id)  (((id) >> 16U) & 0x3FFU)
#define PMB_GET_DST(id)      (((id) >>  8U) & 0xFFU)
#define PMB_GET_SRC(id)      ( (id)          & 0xFFU)
#define PMB_GET_PRI(id)      (((id) >> 26U) & 0x07U)

/* ---- Data ID #defines ---- */
#define REG_TELEMETRY_GROUP_1                              (0x00U)
#define REG_TELEMETRY_GROUP_2                              (0x01U)
#define REG_TELEMETRY_GROUP_3                              (0x02U)
#define REG_TELEMETRY_GROUP_4                              (0x03U)
#define REG_TELEMETRY_GROUP_5                              (0x04U)
#define REG_TELEMETRY_GROUP_6                              (0x05U)
#define REG_TELEMETRY_GROUP_7                              (0x06U)
#define REG_TELEMETRY_GROUP_8                              (0x07U)
#define REG_TELEMETRY_GROUP_9                              (0x08U)
#define REG_TELEMETRY_GROUP_10                             (0x09U)
#define REG_TELEMETRY_GROUP_11                             (0x0AU)
#define REG_TELEMETRY_GROUP_12                             (0x0BU)
#define REG_TELEMETRY_GROUP_13                             (0x0CU)
#define REG_TELEMETRY_GROUP_14                             (0x0DU)
#define REG_TELEMETRY_GROUP_15                             (0x0EU)
#define REG_TELEMETRY_GROUP_16                             (0x0FU)
#define REG_TELEMETRY_GROUP_17                             (0x10U)
#define REG_TELEMETRY_GROUP_18                             (0x11U)
#define REG_TELEMETRY_GROUP_19                             (0x12U)
#define REG_TELEMETRY_GROUP_20                             (0x13U)
#define REG_TELEMETRY_GROUP_21                             (0x14U)
#define REG_TELEMETRY_GROUP_22                             (0x15U)
#define REG_TELEMETRY_GROUP_23                             (0x16U)
#define REG_TELEMETRY_GROUP_24                             (0x17U)
#define REG_TELEMETRY_GROUP_25                             (0x18U)
#define REG_TELEMETRY_GROUP_26                             (0x19U)
#define REG_TELEMETRY_GROUP_27                             (0x1AU)
#define REG_COMP_PDG                                       (0x1BU)
#define REG_PDG_FAULT_GRP1                                 (0x1CU)
#define REG_PDG_FAULT_GRP2                                 (0x1DU)
#define REG_PDG_FAULT_GRP3                                 (0x1EU)
#define REG_POWER_ENABLE_GRP1                              (0x1FU)
#define REG_POWER_ENABLE_GRP2                              (0x20U)
#define REG_POWER_ENABLE_GRP3                              (0x21U)
#define REG_PERIPHERAL_INPUTS                              (0x22U)
#define REG_POWER_ENABLE_GRP4                              (0x23U)
#define REG_ADC_OFFSET_GRP1                                (0x30U)
#define REG_ADC_OFFSET_GRP2                                (0x31U)
#define REG_ADC_OFFSET_GRP3                                (0x32U)
#define REG_ADC_OFFSET_GRP4                                (0x33U)
#define REG_ADC_OFFSET_GRP5                                (0x34U)
#define REG_ADC_OFFSET_GRP6                                (0x35U)
#define REG_ADC_OFFSET_GRP7                                (0x36U)
#define REG_ADC_OFFSET_GRP8                                (0x37U)
#define REG_ADC_OFFSET_GRP9                                (0x38U)
#define REG_ADC_OFFSET_GRP10                               (0x39U)
#define REG_ADC_OFFSET_GRP11                               (0x3AU)
#define REG_ADC_OFFSET_GRP12                               (0x3BU)
#define REG_ADC_OFFSET_GRP13                               (0x3CU)
#define REG_ADC_OFFSET_GRP14                               (0x3DU)
#define REG_ADC_OFFSET_GRP15                               (0x3EU)
#define REG_ADC_OFFSET_GRP16                               (0x3FU)
#define REG_ADC_OFFSET_GRP17                               (0x40U)
#define REG_ADC_OFFSET_GRP18                               (0x41U)
#define REG_ADC_OFFSET_GRP19                               (0x42U)
#define REG_ADC_OFFSET_GRP20                               (0x43U)
#define REG_ADC_OFFSET_GRP21                               (0x44U)
#define REG_ADC_OFFSET_GRP22                               (0x45U)
#define REG_ADC_OFFSET_GRP23                               (0x46U)
#define REG_ADC_OFFSET_GRP24                               (0x47U)
#define REG_ADC_OFFSET_GRP25                               (0x48U)
#define REG_ADC_OFFSET_GRP26                               (0x49U)
#define REG_ADC_OFFSET_GRP27                               (0x4AU)
#define REG_ADC_SCALE_GRP1                                 (0x50U)
#define REG_ADC_SCALE_GRP2                                 (0x51U)
#define REG_ADC_SCALE_GRP3                                 (0x52U)
#define REG_ADC_SCALE_GRP4                                 (0x53U)
#define REG_ADC_SCALE_GRP5                                 (0x54U)
#define REG_ADC_SCALE_GRP6                                 (0x55U)
#define REG_ADC_SCALE_GRP7                                 (0x56U)
#define REG_ADC_SCALE_GRP8                                 (0x57U)
#define REG_ADC_SCALE_GRP9                                 (0x58U)
#define REG_ADC_SCALE_GRP10                                (0x59U)
#define REG_ADC_SCALE_GRP11                                (0x5AU)
#define REG_ADC_SCALE_GRP12                                (0x5BU)
#define REG_ADC_SCALE_GRP13                                (0x5CU)
#define REG_ADC_SCALE_GRP14                                (0x5DU)
#define REG_ADC_SCALE_GRP15                                (0x5EU)
#define REG_ADC_SCALE_GRP16                                (0x5FU)
#define REG_ADC_SCALE_GRP17                                (0x60U)
#define REG_ADC_SCALE_GRP18                                (0x61U)
#define REG_ADC_SCALE_GRP19                                (0x62U)
#define REG_ADC_SCALE_GRP20                                (0x63U)
#define REG_ADC_SCALE_GRP21                                (0x64U)
#define REG_ADC_SCALE_GRP22                                (0x65U)
#define REG_ADC_SCALE_GRP23                                (0x66U)
#define REG_ADC_SCALE_GRP24                                (0x67U)
#define REG_ADC_SCALE_GRP25                                (0x68U)
#define REG_ADC_SCALE_GRP26                                (0x69U)
#define REG_ADC_SCALE_GRP27                                (0x6AU)
#define REG_EU_GRP1                                        (0x70U)
#define REG_EU_GRP2                                        (0x71U)
#define REG_EU_GRP3                                        (0x72U)
#define REG_EU_GRP4                                        (0x73U)
#define REG_EU_GRP5                                        (0x74U)
#define REG_EU_GRP6                                        (0x75U)
#define REG_EU_GRP7                                        (0x76U)
#define REG_EU_GRP8                                        (0x77U)
#define REG_EU_GRP9                                        (0x78U)
#define REG_EU_GRP10                                       (0x79U)
#define REG_EU_GRP11                                       (0x7AU)
#define REG_EU_GRP12                                       (0x7BU)
#define REG_EU_GRP13                                       (0x7CU)
#define REG_EU_GRP14                                       (0x7DU)
#define REG_EU_GRP15                                       (0x7EU)
#define REG_EU_GRP16                                       (0x7FU)
#define REG_EU_GRP17                                       (0x80U)
#define REG_EU_GRP18                                       (0x81U)
#define REG_EU_GRP19                                       (0x82U)
#define REG_EU_GRP20                                       (0x83U)
#define REG_EU_GRP21                                       (0x84U)
#define REG_EU_GRP22                                       (0x85U)
#define REG_EU_GRP23                                       (0x86U)
#define REG_EU_GRP24                                       (0x87U)
#define REG_EU_GRP25                                       (0x88U)
#define REG_EU_GRP26                                       (0x89U)
#define REG_EU_GRP27                                       (0x8AU)
#define REG_DERIVED_EU_GRP1                                (0x90U)
#define REG_DERIVED_EU_GRP2                                (0x91U)
#define REG_ERROR_CODE                                     (0xA0U)
#define REG_FAULT_REG1                                     (0xA8U)
#define REG_FAULT_REG2                                     (0xA9U)
#define REG_FAULT_REG3                                     (0xAAU)
#define REG_SOFT_LIM_GRP1                                  (0xB0U)
#define REG_SOFT_LIM_GRP2                                  (0xB1U)
#define REG_SOFT_LIM_GRP3                                  (0xB2U)
#define REG_SOFT_LIM_GRP4                                  (0xB3U)
#define REG_SOFT_LIM_GRP5                                  (0xB4U)
#define REG_SOFT_LIM_GRP6                                  (0xB5U)
#define REG_SOFT_LIM_GRP7                                  (0xB6U)
#define REG_SOFT_LIM_GRP8                                  (0xB7U)
#define REG_SOFT_LIM_GRP9                                  (0xB8U)
#define REG_SOFT_LIM_GRP10                                 (0xB9U)
#define REG_SOFT_LIM_GRP11                                 (0xBAU)
#define REG_SOFT_LIM_GRP12                                 (0xBBU)
#define REG_SOFT_LIM_GRP13                                 (0xBCU)
#define REG_SOFT_LIM_GRP14                                 (0xBDU)
#define REG_SOFT_LIM_GRP15                                 (0xBEU)
#define REG_SOFT_LIM_GRP16                                 (0xBFU)
#define REG_SOFT_LIM_GRP17                                 (0xC0U)
#define REG_SOFT_LIM_GRP18                                 (0xC1U)
#define REG_SOFT_LIM_GRP19                                 (0xC2U)
#define REG_SOFT_LIM_GRP20                                 (0xC3U)
#define REG_SOFT_LIM_GRP21                                 (0xC4U)
#define REG_SOFT_LIM_GRP22                                 (0xC5U)
#define REG_SOFT_LIM_GRP23                                 (0xC6U)
#define REG_SOFT_LIM_GRP24                                 (0xC7U)
#define REG_SOFT_LIM_GRP25                                 (0xC8U)
#define REG_SOFT_LIM_GRP26                                 (0xC9U)
#define REG_SOFT_LIM_GRP27                                 (0xCAU)
#define REG_NEG_LIM_GRP1                                   (0xCBU)
#define REG_NEG_LIM_GRP2                                   (0xCCU)
#define REG_NEG_LIM_GRP3                                   (0xCDU)
#define REG_NEG_LIM_GRP4                                   (0xCEU)
#define REG_NEG_LIM_GRP5                                   (0xCFU)
#define REG_SOFT_LIM_FAULT_GRP1                            (0xD0U)
#define REG_SOFT_LIM_FAULT_GRP2                            (0xD1U)
#define REG_HARD_LIM_FAULT_GRP1                            (0xD8U)
#define REG_HARD_LIM_FAULT_GRP2                            (0xD9U)
#define REG_HW_FW_REVISION                                 (0x1F8U)
#define REG_WRITE_PROTECT                                  (0x1FAU)

/* ---- Typed register structs ---- */

typedef struct {
    /* 0x00 */ uint16_t chg_curr;       uint16_t batt1_curr;     uint16_t batt2_curr;
    /* 0x01 */ uint16_t ll_imon;        uint16_t rl_imon;        uint16_t la_imon;
    /* 0x02 */ uint16_t ra_imon;        uint16_t tr_imon;        uint16_t chg_in;
    /* 0x03 */ uint16_t hv;             uint16_t batt1_in;       uint16_t batt2_in;
    /* 0x04 */ uint16_t nk_imon;        uint16_t mot_bus;        uint16_t chg_hv;
    /* 0x05 */ uint16_t user_hv;        uint16_t user_hv_curr;
    /* 0x06 */ uint16_t pc_24v;         uint16_t pc_24v_imon;
    /* 0x07 */ uint16_t user_24v;       uint16_t user_24v_imon;
    /* 0x08 */ uint16_t pc_a_12v;       uint16_t pc_a_12v_imon;
    /* 0x09 */ uint16_t pc_b_12v;       uint16_t pc_b_12v_imon;
    /* 0x0A */ uint16_t v24_imon0;      uint16_t v24_imon1;      uint16_t v24_imon2;
    /* 0x0B */ uint16_t v12_imon0;      uint16_t v12_imon1;      uint16_t v12_imon2;
    /* 0x0C */ uint16_t v24_bus_voltage; uint16_t v12_bus_a_voltage;
    /* 0x0D */ uint16_t v5;             uint16_t v5_curr;
    /* 0x0E */ uint16_t v3v3;           uint16_t v3v3_curr;
    /* 0x0F */ uint16_t aux_24v;        uint16_t aux_24v_curr;
    /* 0x10 */ uint16_t aux_12v;        uint16_t aux_12v_curr;
    /* 0x11 */ uint16_t estop_rec_a_12v; uint16_t estop_rec_b_12v;
    /* 0x12 */ uint16_t v12_b_imon3;    uint16_t v12_b_imon4;
    /* 0x13 */ uint16_t hv_or;          uint16_t mb_sp_imon;
    /* 0x14 */ uint16_t temp0;          uint16_t temp1;          uint16_t temp2;
    /* 0x15 */ uint16_t temp3;          uint16_t temp4;          uint16_t temp5;
    /* 0x16 */ uint16_t ex_temp0;       uint16_t ex_temp1;
    /* 0x17 */ uint16_t lidar_a_12v;    uint16_t lidar_b_12v;
    /* 0x18 */ uint16_t sto_24v;        uint16_t chg_hvil_out;
    /* 0x19 */ uint16_t eth_2v5;        uint16_t eth_1v2;
    /* 0x1A */ uint16_t ecat_3v3;       uint16_t ecat_1v2;
} PMB_Telemetry_t;

typedef struct {
    uint8_t comp_pdg[3];       /* 0x1B: bytes data[2],data[1],data[0] */
    uint8_t pdg_fault_grp1[3]; /* 0x1C */
    uint8_t pdg_fault_grp2[2]; /* 0x1D */
    uint8_t pdg_fault_grp3[2]; /* 0x1E */
} PMB_Status_t;

typedef struct {
    uint8_t grp1;              /* 0x1F: Byte1 only */
    uint8_t grp2[3];           /* 0x20: Bytes 1,2,3 */
    uint8_t grp3[4];           /* 0x21: Bytes 1,2,3,4 */
    uint8_t periph_inputs;     /* 0x22: Read Only */
    uint8_t grp4[3];           /* 0x23: Bytes 1,2,3 */
} PMB_PowerEnable_t;

typedef struct {
    /* 0x30 */ int16_t chg_curr_ofs;        int16_t batt1_curr_ofs;     int16_t batt2_curr_ofs;
    /* 0x31 */ int16_t ll_imon_ofs;         int16_t rl_imon_ofs;        int16_t la_imon_ofs;
    /* 0x32 */ int16_t ra_imon_ofs;         int16_t tr_imon_ofs;        int16_t chg_in_ofs;
    /* 0x33 */ int16_t hv_ofs;              int16_t batt1_in_ofs;       int16_t batt2_in_ofs;
    /* 0x34 */ int16_t nk_imon_ofs;         int16_t mot_bus_ofs;        int16_t chg_hv_ofs;
    /* 0x35 */ int16_t user_hv_ofs;         int16_t user_hv_curr_ofs;
    /* 0x36 */ int16_t pc_24v_ofs;          int16_t pc_24v_imon_ofs;
    /* 0x37 */ int16_t user_24v_ofs;        int16_t user_24v_imon_ofs;
    /* 0x38 */ int16_t pc_a_12v_ofs;        int16_t pc_a_12v_imon_ofs;
    /* 0x39 */ int16_t pc_b_12v_ofs;        int16_t pc_b_12v_imon_ofs;
    /* 0x3A */ int16_t v24_imon0_ofs;       int16_t v24_imon1_ofs;      int16_t v24_imon2_ofs;
    /* 0x3B */ int16_t v12_imon0_ofs;       int16_t v12_imon1_ofs;      int16_t v12_imon2_ofs;
    /* 0x3C */ int16_t v24_ofs;             int16_t v12_ofs;
    /* 0x3D */ int16_t v5_ofs;              int16_t v5_curr_ofs;
    /* 0x3E */ int16_t v3v3_ofs;            int16_t v3v3_curr_ofs;
    /* 0x3F */ int16_t aux_24v_ofs;         int16_t aux_24v_curr_ofs;
    /* 0x40 */ int16_t aux_12v_ofs;         int16_t aux_12v_curr_ofs;
    /* 0x41 */ int16_t estop_rec_a_12v_ofs; int16_t estop_rec_b_12v_ofs;
    /* 0x42 */ int16_t v12_b_imon3_ofs;     int16_t v12_b_imon4_ofs;
    /* 0x43 */ int16_t hv_or_ofs;           int16_t mb_sp_imon_ofs;
    /* 0x44 */ int16_t temp0_ofs;           int16_t temp1_ofs;          int16_t temp2_ofs;
    /* 0x45 */ int16_t temp3_ofs;           int16_t temp4_ofs;          int16_t temp5_ofs;
    /* 0x46 */ int16_t ex_temp0_ofs;        int16_t ex_temp1_ofs;
    /* 0x47 */ int16_t lidar_a_12v_ofs;     int16_t lidar_b_12v_ofs;
    /* 0x48 */ int16_t sto_24v_ofs;         int16_t chg_hvil_out_ofs;
    /* 0x49 */ int16_t eth_2v5_ofs;         int16_t eth_1v2_ofs;
    /* 0x4A */ int16_t ecat_3v3_ofs;        int16_t ecat_1v2_ofs;
} PMB_CalibOffset_t;

typedef struct {
    /* 0x50 */ int16_t chg_curr_scl;        int16_t batt1_curr_scl;     int16_t batt2_curr_scl;
    /* 0x51 */ int16_t ll_imon_scl;         int16_t rl_imon_scl;        int16_t la_imon_scl;
    /* 0x52 */ int16_t ra_imon_scl;         int16_t tr_imon_scl;        int16_t chg_in_scl;
    /* 0x53 */ int16_t hv_scl;              int16_t batt1_in_scl;       int16_t batt2_in_scl;
    /* 0x54 */ int16_t nk_imon_scl;         int16_t mot_bus_scl;        int16_t chg_hv_scl;
    /* 0x55 */ int16_t user_hv_scl;         int16_t user_hv_curr_scl;
    /* 0x56 */ int16_t pc_24v_scl;          int16_t pc_24v_imon_scl;
    /* 0x57 */ int16_t user_24v_scl;        int16_t user_24v_imon_scl;
    /* 0x58 */ int16_t pc_a_12v_scl;        int16_t pc_a_12v_imon_scl;
    /* 0x59 */ int16_t pc_b_12v_scl;        int16_t pc_b_12v_imon_scl;
    /* 0x5A */ int16_t v24_imon0_scl;       int16_t v24_imon1_scl;      int16_t v24_imon2_scl;
    /* 0x5B */ int16_t v12_imon0_scl;       int16_t v12_imon1_scl;      int16_t v12_imon2_scl;
    /* 0x5C */ int16_t v24_scl;             int16_t v12_scl;
    /* 0x5D */ int16_t v5_scl;              int16_t v5_curr_scl;
    /* 0x5E */ int16_t v3v3_scl;            int16_t v3v3_curr_scl;
    /* 0x5F */ int16_t aux_24v_scl;         int16_t aux_24v_curr_scl;
    /* 0x60 */ int16_t aux_12v_scl;         int16_t aux_12v_curr_scl;
    /* 0x61 */ int16_t estop_rec_a_12v_scl; int16_t estop_rec_b_12v_scl;
    /* 0x62 */ int16_t v12_b_imon3_scl;     int16_t v12_b_imon4_scl;
    /* 0x63 */ int16_t hv_or_scl;           int16_t mb_sp_imon_scl;
    /* 0x64 */ int16_t temp0_scl;           int16_t temp1_scl;          int16_t temp2_scl;
    /* 0x65 */ int16_t temp3_scl;           int16_t temp4_scl;          int16_t temp5_scl;
    /* 0x66 */ int16_t ex_temp0_scl;        int16_t ex_temp1_scl;
    /* 0x67 */ int16_t lidar_a_12v_scl;     int16_t lidar_b_12v_scl;
    /* 0x68 */ int16_t sto_24v_scl;         int16_t chg_hvil_out_scl;
    /* 0x69 */ int16_t eth_2v5_scl;         int16_t eth_1v2_scl;
    /* 0x6A */ int16_t ecat_3v3_scl;        int16_t ecat_1v2_scl;
} PMB_CalibScale_t;

typedef struct {
    /* 0x70 */ int16_t chg_curr_eu;         int16_t batt1_curr_eu;      int16_t batt2_curr_eu;
    /* 0x71 */ int16_t ll_imon_eu;          int16_t rl_imon_eu;         int16_t la_imon_eu;
    /* 0x72 */ int16_t ra_imon_eu;          int16_t tr_imon_eu;         int16_t chg_in_eu;
    /* 0x73 */ int16_t hv_eu;               int16_t batt1_in_eu;        int16_t batt2_in_eu;
    /* 0x74 */ int16_t nk_imon_eu;          int16_t mot_bus_eu;         int16_t chg_hv_eu;
    /* 0x75 */ int16_t user_hv_eu;          int16_t user_hv_curr_eu;
    /* 0x76 */ int16_t pc_24v_eu;           int16_t pc_24v_imon_eu;
    /* 0x77 */ int16_t user_24v_eu;         int16_t user_24v_imon_eu;
    /* 0x78 */ int16_t pc_a_12v_eu;         int16_t pc_a_12v_imon_eu;
    /* 0x79 */ int16_t pc_b_12v_eu;         int16_t pc_b_12v_imon_eu;
    /* 0x7A */ int16_t v24_imon0_eu;        int16_t v24_imon1_eu;       int16_t v24_imon2_eu;
    /* 0x7B */ int16_t v12_imon0_eu;        int16_t v12_imon1_eu;       int16_t v12_imon2_eu;
    /* 0x7C */ int16_t v24_eu;              int16_t v12_eu;
    /* 0x7D */ int16_t v5_eu;               int16_t v5_curr_eu;
    /* 0x7E */ int16_t v3v3_eu;             int16_t v3v3_curr_eu;
    /* 0x7F */ int16_t aux_24v_eu;          int16_t aux_24v_curr_eu;
    /* 0x80 */ int16_t aux_12v_eu;          int16_t aux_12v_curr_eu;
    /* 0x81 */ int16_t estop_rec_a_12v_eu;  int16_t estop_rec_b_12v_eu;
    /* 0x82 */ int16_t v12_b_imon3_eu;      int16_t v12_b_imon4_eu;
    /* 0x83 */ int16_t hv_or_eu;            int16_t mb_sp_imon_eu;
    /* 0x84 */ int16_t temp0_eu;            int16_t temp1_eu;           int16_t temp2_eu;
    /* 0x85 */ int16_t temp3_eu;            int16_t temp4_eu;           int16_t temp5_eu;
    /* 0x86 */ int16_t ex_temp0_eu;         int16_t ex_temp1_eu;
    /* 0x87 */ int16_t lidar_a_12v_eu;      int16_t lidar_b_12v_eu;
    /* 0x88 */ int16_t sto_24v_eu;          int16_t chg_hvil_out_eu;
    /* 0x89 */ int16_t eth_2v5_eu;          int16_t eth_1v2_eu;
    /* 0x8A */ int16_t ecat_3v3_eu;         int16_t ecat_1v2_eu;
    /* 0x90 */ int16_t v12_a_total_curr_eu; int16_t v12_b_total_curr_eu; int16_t v24_total_curr_eu;
    /* 0x91 */ int16_t batt_total_curr_eu;
} PMB_EU_t;

typedef struct {
    /* 0xB0 */ int16_t chg_curr_lim;        int16_t batt1_curr_plim;     int16_t batt2_curr_plim;
    /* 0xB1 */ int16_t ll_imon_lim;         int16_t rl_imon_lim;         int16_t la_imon_lim;
    /* 0xB2 */ int16_t ra_imon_lim;         int16_t tr_imon_lim;         int16_t chg_in_lim;
    /* 0xB3 */ int16_t hv_lim;              int16_t batt1_in_lim;        int16_t batt2_in_lim;
    /* 0xB4 */ int16_t nk_imon_lim;         int16_t mot_bus_lim;         int16_t chg_hv_lim;
    /* 0xB5 */ int16_t user_hv_lim;         int16_t user_hv_curr_lim;
    /* 0xB6 */ int16_t pc_24v_lim;          int16_t pc_24v_imon_lim;
    /* 0xB7 */ int16_t user_24v_lim;        int16_t user_24v_imon_lim;
    /* 0xB8 */ int16_t pc_a_12v_lim;        int16_t pc_a_12v_imon_lim;
    /* 0xB9 */ int16_t pc_b_12v_lim;        int16_t pc_b_12v_imon_lim;
    /* 0xBA */ int16_t v24_imon0_lim;       int16_t v24_imon1_lim;       int16_t v24_imon2_lim;
    /* 0xBB */ int16_t v12_imon0_lim;       int16_t v12_imon1_lim;       int16_t v12_imon2_lim;
    /* 0xBC */ int16_t v24_lim;             int16_t v12_lim;
    /* 0xBD */ int16_t v5_lim;              int16_t v5_curr_lim;
    /* 0xBE */ int16_t v3v3_lim;            int16_t v3v3_curr_lim;
    /* 0xBF */ int16_t aux_24v_lim;         int16_t aux_24v_curr_lim;
    /* 0xC0 */ int16_t aux_12v_lim;         int16_t aux_12v_curr_lim;
    /* 0xC1 */ int16_t estop_rec_a_12v_lim; int16_t estop_rec_b_12v_lim;
    /* 0xC2 */ int16_t v12_b_imon3_lim;     int16_t v12_b_imon4_lim;
    /* 0xC3 */ int16_t hv_or_lim;           int16_t mb_sp_imon_lim;
    /* 0xC4 */ int16_t temp0_plim;          int16_t temp1_plim;          int16_t temp2_plim;
    /* 0xC5 */ int16_t temp3_plim;          int16_t temp4_plim;          int16_t temp5_plim;
    /* 0xC6 */ int16_t ex_temp0_plim;       int16_t ex_temp1_plim;
    /* 0xC7 */ int16_t lidar_a_12v_lim;     int16_t lidar_b_12v_lim;
    /* 0xC8 */ int16_t sto_24v_lim;         int16_t chg_hvil_out_lim;
    /* 0xC9 */ int16_t eth_2v5_lim;         int16_t eth_1v2_lim;
    /* 0xCA */ int16_t ecat_3v3_lim;        int16_t ecat_1v2_lim;
    /* 0xCB */ int16_t v12_a_total_curr_lim; int16_t v12_b_total_curr_lim; int16_t v24_total_curr_lim;
    /* 0xCC */ int16_t batt1_curr_nlim;     int16_t batt2_curr_nlim;
    /* 0xCD */ int16_t temp0_nlim;          int16_t temp1_nlim;          int16_t temp2_nlim;
    /* 0xCE */ int16_t temp3_nlim;          int16_t temp4_nlim;          int16_t temp5_nlim;
    /* 0xCF */ int16_t ex_temp0_nlim;       int16_t ex_temp1_nlim;
} PMB_SoftLimits_t;

typedef struct {
    uint8_t reg1;     /* 0xA8 */
    uint8_t reg2[3];  /* 0xA9 */
    uint8_t reg3[4];  /* 0xAA */
} PMB_FaultRegs_t;

typedef struct {
    uint8_t soft_grp1[6]; /* 0xD0 */
    uint8_t soft_grp2[3]; /* 0xD1 */
    uint8_t hard_grp1[6]; /* 0xD8 */
    uint8_t hard_grp2[3]; /* 0xD9 */
} PMB_LimFaults_t;

typedef struct {
    uint16_t hw_rev;  /* 0x1F8 Byte2-1 */
    uint16_t fw_rev;  /* 0x1F8 Byte4-3 */
} PMB_BuildInfo_t;

typedef struct {
    uint8_t b1; /* {4b0000, REV, SCL, OFS, LIM} */
    uint8_t b2; /* {7b0000000, 0x1F-Byte1} */
    uint8_t b3; /* {5b00000, 0x20-Byte3, 0x20-Byte2, 0x20-Byte1} */
    uint8_t b4; /* {4b0000, 0x21-Byte4..Byte1} */
    uint8_t b5; /* {5b00000, 0x23-Byte3..Byte1} */
} PMB_WriteProtect_t;

typedef struct {
    PMB_Telemetry_t   telem;
    PMB_Status_t      status;
    PMB_PowerEnable_t pwr_enable;
    PMB_CalibOffset_t calib_offset;
    PMB_CalibScale_t  calib_scale;
    PMB_EU_t          eu;
    uint32_t          error_code;
    PMB_FaultRegs_t   fault_regs;
    PMB_SoftLimits_t  soft_limits;
    PMB_LimFaults_t   lim_faults;
    PMB_BuildInfo_t   build_info;
    PMB_WriteProtect_t write_protect;
} PMB_Registers_t;

/* ---- DAVE MO indices (update after DAVE reconfiguration) ----
 * PMB_RX_MO: lmobj_ptr index whose MO receives incoming requests (dst=PMB_ADDR).
 * PMB_TX_MO: lmobj_ptr index used to transmit responses.
 * Current placeholders map to Node0/MO0 (RX) and Node1/MO1 (TX).
 * For loopback verification: MO3 (Node2) sends test requests; MO4 (Node2) receives responses. */
#define PMB_RX_MO  (0U)
#define PMB_TX_MO  (1U)
extern MULTICAN_CONFIG_t MULTICAN_CONFIG_0;

/* ---- Public API ---- */
void                   PMB_Emulator_Init(void);
void                   PMB_Emulator_Run(void);
void                   PMB_UpdateEU(void);
const PMB_Registers_t *PMB_GetRegs(void);
PMB_Registers_t       *PMB_GetRegsMutable(void);

#endif /* PMB_V2_CAN_EMULATOR_H */
