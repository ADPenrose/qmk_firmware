// Copyright 2022 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include "gpio.h"
#include "matrix.h"
#include "mcp23018.h"
#include "util.h"
#include "wait.h"
#include "debug.h"

#define I2C_ADDR 0x20

static uint8_t mcp23018_errors = 0;

static void expander_init(void) {
    mcp23018_init(I2C_ADDR);
}

static void expander_init_cols(void) {
    mcp23018_errors += !mcp23018_set_config(I2C_ADDR, mcp23018_PORTA, ALL_INPUT);
    mcp23018_errors += !mcp23018_set_config(I2C_ADDR, mcp23018_PORTB, ALL_INPUT);
}

static void expander_select_row(uint8_t row) {
    if (mcp23018_errors) {
        // wait to mimic i2c interactions
        wait_us(100);
        return;
    }

    mcp23018_errors += !mcp23018_set_config(I2C_ADDR, mcp23018_PORTB, ~(1 << (row + 1)));
}

static void expander_unselect_row(uint8_t row) {
    // No need to unselect row as the next `select_row` will blank everything anyway
}

static void expander_unselect_rows(void) {
    if (mcp23018_errors) {
        return;
    }

    mcp23018_errors += !mcp23018_set_config(I2C_ADDR, mcp23018_PORTB, ALL_INPUT);
}

static matrix_row_t expander_read_row(void) {
    if (mcp23018_errors) {
        return 0;
    }

    uint8_t ret = 0xFF;
    mcp23018_errors += !mcp23018_readPins(I2C_ADDR, mcp23018_PORTA, &ret);

    ret = bitrev(~ret);
    ret = ((ret & 0b11111000) >> 1) | (ret & 0b00000011);

    return ((uint16_t)ret) << 7;
}

static void expander_scan(void) {
    if (!mcp23018_errors) {
        return;
    }

    static uint16_t mcp23018_reset_loop = 0;
    if (++mcp23018_reset_loop > 0x1FFF) {
        // tuned to about 5s given the current scan rate
        dprintf("trying to reset mcp23018\n");
        mcp23018_reset_loop = 0;
        mcp23018_errors     = 0;
        expander_unselect_rows();
        expander_init_cols();
    }
}

/* Column pin configuration
 *
 * Pro Micro: 6    5    4    3    2    1    0
 *            PD3  PD2  PD4  PC6  PD7  PE6  PB4
 *
 * Expander:  13   12   11   10   9    8    7
 */
static void init_cols(void) {
    // Pro Micro
    setPinInputHigh(B3);
    setPinInputHigh(B2);
    setPinInputHigh(B6);

    // Expander
    expander_init_cols();
}

static matrix_row_t read_cols(void) {
    // clang-format off
    return expander_read_row() |
        (readPin(B6) ? 0 : (1<<2)) |
        (readPin(B2) ? 0 : (1<<1)) |
        (readPin(B3) ? 0 : (1<<0)) ;
    // clang-format on
}

/* Row pin configuration
 *
 * Pro Micro: 0   1   2   3   4   5
 *            F4  F5  F6  F7  B1  B2
 *
 * Expander:  0   1   2   3   4   5
 */
static void unselect_rows(void) {
    // Pro Micro
    setPinInput(D4);
    setPinInput(C6);
    writePinLow(D4);
    writePinLow(C6);

    // Expander
    expander_unselect_rows();
}

static void unselect_row(uint8_t row) {
    // Pro Micro
    switch (row) {
        case 0:
            setPinInput(C6);
            writePinLow(C6);
            break;
        case 1:
            setPinInput(D4);
            writePinLow(D4);
            break;
    }

    // Expander
    expander_unselect_row(row);
}

static void select_row(uint8_t row) {
    // Pro Micro
    switch (row) {
        case 0:
            setPinOutput(C6);
            writePinLow(C6);
            break;
        case 1:
            setPinOutput(D4);
            writePinLow(D4);
            break;
    }

    // Expander
    expander_select_row(row);
}

static bool read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row) {
    // Store last value of row prior to reading
    matrix_row_t last_row_value = current_matrix[current_row];

    // Clear data in matrix row
    current_matrix[current_row] = 0;

    // Select row and wait for row selection to stabilize
    select_row(current_row);
    // Skip the wait_us(30); as i2c is slow enough to debounce the io changes

    current_matrix[current_row] = read_cols();

    unselect_row(current_row);

    return (last_row_value != current_matrix[current_row]);
}

void matrix_init_custom(void) {
    expander_init();

    unselect_rows();
    init_cols();
}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    expander_scan();

    bool changed = false;
    for (uint8_t current_row = 0; current_row < MATRIX_ROWS; current_row++) {
        changed |= read_cols_on_row(current_matrix, current_row);
    }
    return changed;
}
