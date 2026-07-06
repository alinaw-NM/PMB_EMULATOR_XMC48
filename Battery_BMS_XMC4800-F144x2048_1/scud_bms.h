#ifndef SCUD_BMS_H
#define SCUD_BMS_H

#include <stdint.h>

/* Maximum cell and temperature counts this driver will store. */
#define SCUD_MAX_CELLS  32U
#define SCUD_MAX_TEMPS   8U

/* Decoded CID2=0x42 analog telemetry response. */
typedef struct {
    uint8_t  data_flag;
    uint8_t  pack_num;
    uint32_t total_mv;                      /* pack voltage, mV */
    int32_t  current_ma;                    /* mA, positive = charging */
    uint8_t  soc;                           /* % */
    uint8_t  soh;                           /* % */
    uint16_t cycle_count;
    uint32_t rmc_mah;                       /* remaining capacity, mAh */
    uint32_t fcc_mah;                       /* full charge capacity, mAh */
    uint32_t design_mah;
    uint8_t  cell_count;
    uint16_t cell_mv[SCUD_MAX_CELLS];
    uint8_t  cell_high_idx;
    uint8_t  cell_low_idx;
    uint16_t cell_vdiff_mv;
    uint8_t  cell_temp_count;
    int16_t  cell_temps_c[SCUD_MAX_TEMPS];
    uint8_t  other_temp_count;
    int16_t  other_temps_c[SCUD_MAX_TEMPS]; /* extra cell-sensor group ("k") */
} ScudAnalog_t;

/* Decoded CID2=0x44 alarm/event bitfields (one byte per category). */
typedef struct {
    uint8_t voltage;
    uint8_t current;
    uint8_t cell_temp;
    uint8_t other_temp;
    uint8_t capacity;
    uint8_t other1;
    uint8_t other2;
} ScudAlarms_t;

typedef enum {
    SCUD_OK            = 0,
    SCUD_ERR_TIMEOUT,
    SCUD_ERR_FRAMING,
    SCUD_ERR_CHECKSUM,
    SCUD_ERR_RTN,       /* BMS returned a non-zero RTN code */
    SCUD_ERR_TOO_SHORT,
} ScudStatus_t;

ScudStatus_t SCUD_ReadAnalog(ScudAnalog_t *out);
ScudStatus_t SCUD_DecodeAnalog(const uint8_t *raw_frame, uint32_t raw_len, ScudAnalog_t *out);
ScudStatus_t SCUD_ReadAlarms(ScudAlarms_t *out);
void         SCUD_PrintAnalog(const ScudAnalog_t *t);
const char  *SCUD_StatusStr(ScudStatus_t st);

#endif /* SCUD_BMS_H */
