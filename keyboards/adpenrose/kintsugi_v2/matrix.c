// Copyright 2013 Oleg Kostuyk, 2020 Pierre Chevalier, 2022 Arturo Avila (@ADPenrose)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "i2c_master.h"

extern i2c_status_t mcp23017_status;
#define I2C_TIMEOUT 1000

// I2C address:
// All address pins of the mcp23017 are connected to the ground on the Kintsugi_V2
// | 0  | 1  | 0  | 0  | A2 | A1 | A0 |
// | 0  | 1  | 0  | 0  | 0  | 0  | 0  |
#define I2C_ADDR 0b0100000
// Left shift the address bytes and addition of the R/W bit.
#define I2C_ADDR_WRITE ((I2C_ADDR << 1) | I2C_WRITE)
#define I2C_ADDR_READ ((I2C_ADDR << 1) | I2C_READ)

// Register addresses:
#define IODIRA 0x00  // i/o direction register
#define IODIRB 0x01
#define GPPUA 0x0C  // GPIO pull-up resistor register
#define GPPUB 0x0D
#define GPIOA 0x12  // general purpose i/o port register (write modifies OLAT)
#define GPIOB 0x13

bool i2c_initialized = 0;
i2c_status_t mcp23017_status = I2C_ADDR;

// Init expander:
uint8_t init_mcp23017(void) {
    print("starting init");
    mcp23017_status = I2C_ADDR;

    // I2C subsystem:
    if (i2c_initialized == 0) {
        i2c_init();
        i2c_initialized = true;
        wait_ms(I2C_TIMEOUT);
    }

    // Set pin direction (how are we planning to use each pin on GPIOA and GPIOB):
    // - Unused: 1
    // - Reading (for COLS): 1
    // - Writing (for ROWS): 0
    // This means: We will read all the bits on GPIOA (COLS).
    // This means: We will write to the pins 0-5 (from right to left) on GPIOB (ROWS).
    uint8_t buf[]   = {IODIRA, 0b11111111, 0b11000000};
    mcp23017_status = i2c_transmit(I2C_ADDR_WRITE, buf, sizeof(buf), I2C_TIMEOUT);
    if (!mcp23017_status) {
        // Set pull-up (which pins should have pull-up enabled, and which should not):
        // - Unused: 1
        // - Reading (for COLS): 1
        // - Writing (for ROWS): 0
        // This means: We will read all the bits on GPIOA (COLS).
        // This means: We will write to the pins 0-5 (from right to left) on GPIOB (ROWS).
        uint8_t pullup_buf[] = {GPPUA, 0b11111111, 0b11000000};
        mcp23017_status      = i2c_transmit(I2C_ADDR_WRITE, pullup_buf, sizeof(pullup_buf), I2C_TIMEOUT);
    }
    return mcp23017_status;
}

/* Matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];  // Debounced values.
static uint8_t mcp23017_reset_loop;

static void init_cols(void) {
    // Init on MCU:
    pin_t matrix_col_pins_mcu[MATRIX_COLS_PER_SIDE] = MATRIX_COL_PINS_MCU;
    for (int pin_index = 0; pin_index < MATRIX_COLS_PER_SIDE; pin_index++) {
        pin_t pin = matrix_col_pins_mcu[pin_index];
        setPinInput(pin);
        writePinHigh(pin);
    }
    // Init on MCP23017 not needed. Already done as part of init_mcp23017().
}

static void select_row(uint8_t row) {
    if (row < MATRIX_ROWS_PER_SIDE) {
        // Select on MCU:
        pin_t matrix_row_pins_mcu[MATRIX_ROWS_PER_SIDE] = MATRIX_ROW_PINS_MCU;
        pin_t pin                                       = matrix_row_pins_mcu[row];
        setPinOutput(pin);
        writePinLow(pin);
    } else {
        // Select on MCP23017:
        if (mcp23017_status) {  // If there was an error...
                                // Do nothing.
        } else {
            // Select the desired row by writing a byte for the entire GPIOB bus where only the bit representing the row we want to select is a zero (write instruction) and every other bit is a one.
            // Note that the row - MATRIX_ROWS_PER_SIDE reflects the fact that being on the right hand, the columns are numbered from MATRIX_ROWS_PER_SIDE to MATRIX_ROWS, but the pins we want to write to are indexed from zero up on the GPIOB bus.
            uint8_t buf[]   = {GPIOB, 0xFF & ~(1 << (row - MATRIX_ROWS_PER_SIDE))};
            mcp23017_status = i2c_transmit(I2C_ADDR_WRITE, buf, sizeof(buf), I2C_TIMEOUT);
        }
    }
}

static matrix_row_t read_cols(uint8_t row) {
    if (row < MATRIX_ROWS_PER_SIDE) {
        pin_t        matrix_col_pins_mcu[MATRIX_COLS_PER_SIDE] = MATRIX_COL_PINS_MCU;
        matrix_row_t current_row_value                         = 0;
        // For each COL...
        for (uint8_t col_index = 0; col_index < MATRIX_COLS_PER_SIDE; col_index++) {
            // Select the COL pin to read (active low)
            uint8_t pin_state = readPin(matrix_col_pins_mcu[col_index]);

            // Populate the matrix ROW with the state of the COL pin
            current_row_value |= pin_state ? 0 : (MATRIX_ROW_SHIFTER << col_index);
        }
        return current_row_value;
    } else {
        if (mcp23017_status) {  // If there was an error...
            return 0;
        } else {
            uint8_t buf[]   = {GPIOA};
            mcp23017_status = i2c_transmit(I2C_ADDR_WRITE, buf, sizeof(buf), I2C_TIMEOUT);
            // We read all the pins on GPIOA.
            // The initial state was all ones and any depressed key at a given column for the currently selected row will have its bit flipped to zero.
            // The return value is a row as represented in the generic matrix code were the rightmost bits represent the lower columns and zeroes represent non-depressed keys while ones represent depressed keys.
            // Since the pins connected to eact columns are sequential, and counting from zero up (col 5 -> GPIOA0, col 6 -> GPIOA1 and so on), the only transformation needed is a bitwise not to swap all zeroes and ones.
            uint8_t data[] = {0};
            if (!mcp23017_status) {
                mcp23017_status = i2c_receive(I2C_ADDR_READ, data, sizeof(data), I2C_TIMEOUT);
                data[0]         = ~(data[0]);
            }
            return data[0];
        }
    }
}

// Reads and stores a row, returning whether a change occurred.
static inline bool store_matrix_row(matrix_row_t current_matrix[], uint8_t index) {
    matrix_row_t temp = read_cols(index);
    if (current_matrix[index] != temp) {
        current_matrix[index] = temp;
        return true;
    }
    return false;
}

static void unselect_rows(void) {
    // Unselect rows on MCU:
    pin_t matrix_row_pins_mcu[MATRIX_ROWS_PER_SIDE] = MATRIX_ROW_PINS_MCU;
    for (int pin_index = 0; pin_index < MATRIX_ROWS_PER_SIDE; pin_index++) {
        pin_t pin = matrix_row_pins_mcu[pin_index];
        setPinInput(pin);
        writePinLow(pin);
    }
    // No need to unselect on MCP23017, because the select step sets all
    // the other row bits high, and it's not changing to a different
    // direction.
}

void matrix_init_custom(void) {
    // Initialize Row and Col:
    mcp23017_status = init_mcp23017();

    unselect_rows();
    init_cols();

    // Initialize matrix state - All keys off:
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
    }
}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    if (mcp23017_status) {  // If there was an error...
        if (++mcp23017_reset_loop == 0) {
            // Since MCP23017_reset_loop is 8 bit - we'll try to reset once in 255 matrix scans.
            // This will be approx. a bit more frequent than once per second.
            dprint("Trying to reset MCP23017...\n");
            mcp23017_status = init_mcp23017();
            if (mcp23017_status) {
                dprint("Right side not responding.\n");
            } else {
                dprint("Right side attached.\n");
            }
        }
    }

    bool matrix_has_changed = false;
    for (uint8_t i = 0; i < MATRIX_ROWS_PER_SIDE; i++) {
        // Select rows from left and right hands:
        uint8_t left_index  = i;
        uint8_t right_index = i + MATRIX_ROWS_PER_SIDE;
        select_row(left_index);
        select_row(right_index);

        // We don't need a 30us delay anymore, because selecting a left-hand row requires more than 30us for i2c.
        matrix_has_changed |= store_matrix_row(current_matrix, left_index);
        matrix_has_changed |= store_matrix_row(current_matrix, right_index);

        unselect_rows();
    }

    return matrix_has_changed;
}