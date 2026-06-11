/**
 * @file pmb_loopback_test.h
 * @brief PMB CAN emulator loopback self-test — covers all 161 Data IDs
 *
 * Tests every register defined in PMB_CAN_Protocol_v1.xlsx by injecting
 * synthetic read requests into MO0 and verifying the emulator's response
 * in MO1 against expected values computed from the same register state.
 */

#ifndef PMB_LOOPBACK_TEST_H
#define PMB_LOOPBACK_TEST_H

#include <stdint.h>
#include <stdbool.h>

/** Per-test result record */
typedef struct {
    uint16_t data_id;      /**< Register address tested */
    bool     passed;       /**< true = bytes 0-5 matched */
    uint8_t  received[8];  /**< Bytes read from TX MO after emulator ran */
    uint8_t  expected[8];  /**< Bytes computed from register state */
} PMB_LoopbackResult_t;

/** Maximum number of results the caller must allocate (>= 161) */
#define PMB_LOOPBACK_MAX_RESULTS  161U

/**
 * @brief Run all 161 register loopback tests.
 *
 * Presets non-zero values into all emulated registers, then for each
 * Data ID: injects a read request, calls PMB_Emulator_Run(), reads the
 * response from MO1, and compares bytes 0-5 against expected values.
 * Bytes 6-7 (packet counter) are skipped.
 *
 * @param results  Caller-allocated array of >= PMB_LOOPBACK_MAX_RESULTS entries.
 * @param count    Set to the number of tests actually run on return.
 * @return true if all tests passed, false if any failed.
 */
bool PMB_LoopbackTest_Run(PMB_LoopbackResult_t *results, uint16_t *count);

#endif /* PMB_LOOPBACK_TEST_H */
