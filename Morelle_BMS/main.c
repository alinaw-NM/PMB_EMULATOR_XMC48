/*
 * main.c - Morelle Falcon 15 EB BMS diagnostic over CAN
 *
 * Polls all BMS data IDs every 2 seconds and prints raw CAN bytes plus
 * decoded values via XMC_DEBUG (SWO / J-Link trace, view in DAVE Debug console).
 *
 * CAN: 250 kbps, extended 29-bit IDs, Node 0, P2.0 TX / P14.3 RX (RXDCB)
 */

#include "DAVE.h"
#include "morelle_bms.h"
#include <stdint.h>

extern void initialise_monitor_handles(void);

/* ~1 ms delay at 144 MHz: 28800 iterations ≈ 1 ms (calibrated same as RS485 project) */
static void delay_ms(uint32_t ms)
{
    volatile uint32_t i;
    for (i = 0U; i < ms * 28800U; i++) {}
}


int main(void)
{
    DAVE_STATUS_t status;

    status = DAVE_Init();
    initialise_monitor_handles();

    if (status != DAVE_STATUS_SUCCESS)
    {
        XMC_DEBUG("DAVE APPs initialization failed\n");
        while (1U) {}
    }

    /* Configure CAN Node 0 for 250 kbps on-board transceiver (P2.0/P14.3) */
    Morelle_NodeInit();

    /* Verify Node 0 is running + MOs are in list 1 (= CAN_NODE0). LIST is
     * MOSTAT[3:0]; a value of 1 means the MO is allocated to Node 0. */
    XMC_DEBUG("Node0 NCR=0x%08lX  NBTR=0x%08lX  NSR=0x%08lX\n",
              (unsigned long)CAN_NODE0->NCR,
              (unsigned long)CAN_NODE0->NBTR,
              (unsigned long)CAN_NODE0->NSR);
    XMC_DEBUG("MO0(TX) MOSTAT=0x%08lX  LIST=%lu\n",
              (unsigned long)CAN_MO0->MOSTAT,
              (unsigned long)((CAN_MO0->MOSTAT & CAN_MO_MOSTAT_LIST_Msk) >> CAN_MO_MOSTAT_LIST_Pos));
    XMC_DEBUG("MO1(RX) MOSTAT=0x%08lX  LIST=%lu\n",
              (unsigned long)CAN_MO1->MOSTAT,
              (unsigned long)((CAN_MO1->MOSTAT & CAN_MO_MOSTAT_LIST_Msk) >> CAN_MO_MOSTAT_LIST_Pos));

    XMC_DEBUG("Morelle BMS monitor started (250 kbps CAN, battery addr 0x%02X)\n",
              (unsigned)MORELLE_BATT_ADDR);

    for (;;)
    {
        /* All locals at top of block — required for GCC 4.9 / arm-none-eabi-gcc -std=gnu99 */
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

        XMC_DEBUG("\n========== BMS Poll ==========\n");

        /* 0x90 Pack Totals — scale floats to tenths; -specs=nano.specs strips %f. */
        st = Morelle_GetPackTotals(&pack);
        if (st == MORELLE_OK) {
            uint32_t pv_dv = (uint32_t)(pack.pack_v * 10.0f + 0.5f);
            int32_t  cu_da = (int32_t)(pack.current_a * 10.0f);
            uint32_t so_dp = (uint32_t)(pack.soc_pct * 10.0f + 0.5f);
            uint32_t cu_abs = (uint32_t)((cu_da < 0) ? -cu_da : cu_da);
            XMC_DEBUG("[0x90] Pack Totals: Pack=%lu.%luV  I=%s%lu.%luA  SoC=%lu.%lu%%\n",
                      (unsigned long)(pv_dv / 10U), (unsigned long)(pv_dv % 10U),
                      (cu_da < 0) ? "-" : "",
                      (unsigned long)(cu_abs / 10U), (unsigned long)(cu_abs % 10U),
                      (unsigned long)(so_dp / 10U), (unsigned long)(so_dp % 10U));
        }

        /* 0x91 Cell Voltage Extremes */
        st = Morelle_GetCellExtremes(&cext);
        if (st == MORELLE_OK) {
            XMC_DEBUG("[0x91] Max: %u mV (cell %u)  Min: %u mV (cell %u)\n",
                      (unsigned)cext.max_mv, (unsigned)cext.max_cell,
                      (unsigned)cext.min_mv, (unsigned)cext.min_cell);
        }

        /* 0x92 Temperature Extremes */
        st = Morelle_GetTempExtremes(&text);
        if (st == MORELLE_OK) {
            XMC_DEBUG("[0x92] Max: %d C (sensor %u)  Min: %d C (sensor %u)\n",
                      (int)text.max_c, (unsigned)text.max_sensor,
                      (int)text.min_c, (unsigned)text.min_sensor);
        }

        /* 0x93 MOS State */
        st = Morelle_GetMosState(&mos);
        if (st == MORELLE_OK) {
            mos_str = (mos.state == 1U) ? "CHG" :
                      (mos.state == 2U) ? "DSG" : "IDLE";
            XMC_DEBUG("[0x93] State: %s  ChgMOS: %u  DsgMOS: %u  Rem: %lu mAh\n",
                      mos_str, (unsigned)mos.chg_mos, (unsigned)mos.dsg_mos,
                      (unsigned long)mos.remaining_mah);
        }

        /* 0x94 Status Info — must precede 0x95/0x96 (feeds their count). */
        st = Morelle_GetStatusInfo(&info);
        if (st == MORELLE_OK) {
            XMC_DEBUG("[0x94] Status Info: Cells=%u  Temp sensors=%u\n",
                      (unsigned)info.num_cells, (unsigned)info.num_temps);
        }

        /* 0x95 Cell Voltages (multi-frame) */
        st = Morelle_GetCellVoltages(&vcells, info.num_cells);
        if (st == MORELLE_OK) {
            XMC_DEBUG("[0x95] Cell mV:");
            for (i = 0U; i < vcells.count; i++) {
                XMC_DEBUG(" %u", (unsigned)vcells.mv[i]);
            }
            XMC_DEBUG("\n");
        }

        /* 0x96 Temperatures */
        st = Morelle_GetTemperatures(&vtemps, info.num_temps);
        if (st == MORELLE_OK) {
            XMC_DEBUG("[0x96] Temps C:");
            for (i = 0U; i < vtemps.count; i++) {
                XMC_DEBUG(" %d", (int)vtemps.c[i]);
            }
            XMC_DEBUG("\n");
        }

        /* 0x97 Cell Balance */
        st = Morelle_GetCellBalance(&bal);
        if (st == MORELLE_OK) {
            XMC_DEBUG("[0x97] Balance mask: %02X %02X %02X\n",
                      (unsigned)bal.mask[0], (unsigned)bal.mask[1], (unsigned)bal.mask[2]);
        }

        /* 0x98 Fault Status */
        st = Morelle_GetFaultStatus(&fault);
        if (st == MORELLE_OK) {
            any = 0U;
            for (i = 0U; i < 8U; i++) { any |= fault.b[i]; }
            XMC_DEBUG("[0x98] %02X %02X %02X %02X %02X %02X %02X %02X  %s\n",
                      (unsigned)fault.b[0], (unsigned)fault.b[1],
                      (unsigned)fault.b[2], (unsigned)fault.b[3],
                      (unsigned)fault.b[4], (unsigned)fault.b[5],
                      (unsigned)fault.b[6], (unsigned)fault.b[7],
                      any ? "<FAULT>" : "OK");
        }

        delay_ms(2000U);
    }
}
