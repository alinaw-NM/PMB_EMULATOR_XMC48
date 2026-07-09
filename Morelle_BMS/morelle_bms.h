/*
 * morelle_bms.h - CAN driver for Morelle Falcon 15 EB battery BMS
 *
 * Protocol: custom extended 29-bit CAN, 250 kbps
 * Frame ID format:
 *   Request  (host -> battery): 0x18 <DataID> <BattAddr> <HostAddr>
 *   Response (battery -> host): 0x18 <DataID> <HostAddr> <BattAddr>
 */

#ifndef MORELLE_BMS_H
#define MORELLE_BMS_H

#include <stdint.h>

/* Set to 1 to enable internal CAN loopback (bypasses the transceiver/bus).
 * Use to verify the CAN peripheral is working when TX sees Bit0 errors. */
#ifndef MORELLE_CAN_LOOPBACK
#define MORELLE_CAN_LOOPBACK  0
#endif

/* Default addresses from PDF spec */
#define MORELLE_HOST_ADDR   0x40U
#define MORELLE_BATT_ADDR   0x02U   /* Battery unit 2, matches DAVE RX filter 0x18904002 */

/* CAN Data IDs */
#define MORELLE_DID_PACK_TOTALS    0x90U
#define MORELLE_DID_CELL_EXTREMES  0x91U
#define MORELLE_DID_TEMP_EXTREMES  0x92U
#define MORELLE_DID_MOS_STATE      0x93U
#define MORELLE_DID_STATUS_INFO    0x94U
#define MORELLE_DID_CELL_VOLTAGES  0x95U
#define MORELLE_DID_TEMPERATURES   0x96U
#define MORELLE_DID_CELL_BALANCE   0x97U
#define MORELLE_DID_FAULT_STATUS   0x98U

/* Build request CAN ID for a given DataID */
#define MORELLE_REQ_ID(did) \
    (0x18000000UL | ((uint32_t)(did) << 16) | ((uint32_t)MORELLE_BATT_ADDR << 8) | (uint32_t)MORELLE_HOST_ADDR)

/* Extract DataID from a received 29-bit extended CAN ID */
#define MORELLE_DID_FROM_ID(id)   (((id) >> 16) & 0xFFU)

/* 0x90 - Pack Totals (voltage, current, SoC) */
typedef struct {
    float    pack_v;       /* Pack voltage [V], resolution 0.1V */
    float    current_a;    /* Current [A], +charging -discharging, resolution 0.1A */
    float    soc_pct;      /* State of charge [%], resolution 0.1% */
} Morelle_PackTotals_t;

/* 0x91 - Cell Voltage Extremes */
typedef struct {
    uint16_t max_mv;
    uint8_t  max_cell;     /* 1-based cell index */
    uint16_t min_mv;
    uint8_t  min_cell;
} Morelle_CellExtremes_t;

/* 0x92 - Temperature Extremes */
typedef struct {
    int8_t   max_c;
    uint8_t  max_sensor;   /* 1-based sensor index */
    int8_t   min_c;
    uint8_t  min_sensor;
} Morelle_TempExtremes_t;

/* 0x93 - MOS State and Capacity */
typedef struct {
    uint8_t  state;         /* 0=idle 1=charging 2=discharging */
    uint8_t  chg_mos;       /* 0=off 1=on */
    uint8_t  dsg_mos;       /* 0=off 1=on */
    uint8_t  life_ctr;      /* 0-255 wrapping counter */
    uint32_t remaining_mah;
} Morelle_MosState_t;

/* 0x94 - Status Information (cell count, sensor count) */
typedef struct {
    uint8_t num_cells;
    uint8_t num_temps;
} Morelle_StatusInfo_t;

/* 0x95 - Individual Cell Voltages (multi-frame: 3 cells per response frame) */
#define MORELLE_MAX_CELLS  16U
typedef struct {
    uint16_t mv[MORELLE_MAX_CELLS];
    uint8_t  count;
} Morelle_CellVoltages_t;

/* 0x96 - Individual Temperatures (up to 7 temps per frame) */
#define MORELLE_MAX_TEMPS  8U
typedef struct {
    int8_t   c[MORELLE_MAX_TEMPS];
    uint8_t  count;
} Morelle_Temperatures_t;

/* 0x97 - Cell Balance State (bitmask per 8 cells) */
typedef struct {
    uint8_t mask[3];   /* mask[0] bit0=cell1, mask[1] bit0=cell9, mask[2] bit0=cell17 */
} Morelle_CellBalance_t;

/* 0x98 - Fault Status (8 bytes of fault flags) */
typedef struct {
    uint8_t b[8];
} Morelle_FaultStatus_t;

/* Return codes */
typedef enum {
    MORELLE_OK = 0,
    MORELLE_ERR_TIMEOUT,
    MORELLE_ERR_TX_FAIL,
} Morelle_Status_t;

/* Last received 8 raw payload bytes — valid immediately after any Morelle_Get*()
 * returns MORELLE_OK. For multi-frame responses (0x95, 0x96) reflects the last frame. */
extern uint8_t morelle_last_rx_raw[8];

/* Configure CAN Node 0 for 250 kbps external bus and assign message objects */
void             Morelle_NodeInit(void);

/* High-level getters: send request, wait for response, decode */
Morelle_Status_t Morelle_GetPackTotals(Morelle_PackTotals_t *out);
Morelle_Status_t Morelle_GetCellExtremes(Morelle_CellExtremes_t *out);
Morelle_Status_t Morelle_GetTempExtremes(Morelle_TempExtremes_t *out);
Morelle_Status_t Morelle_GetMosState(Morelle_MosState_t *out);
Morelle_Status_t Morelle_GetStatusInfo(Morelle_StatusInfo_t *out);
Morelle_Status_t Morelle_GetCellVoltages(Morelle_CellVoltages_t *out, uint8_t num_cells);
Morelle_Status_t Morelle_GetTemperatures(Morelle_Temperatures_t *out, uint8_t num_sensors);
Morelle_Status_t Morelle_GetCellBalance(Morelle_CellBalance_t *out);
Morelle_Status_t Morelle_GetFaultStatus(Morelle_FaultStatus_t *out);

#endif /* MORELLE_BMS_H */
