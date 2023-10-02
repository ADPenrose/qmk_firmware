// Copyright 2022 Arturo Avila (@Arturo Avila)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/* Key Matrix size */
#define MATRIX_ROWS 12
#define MATRIX_COLS 8

#define MATRIX_ROWS_PER_SIDE (MATRIX_ROWS / 2)
#define MATRIX_COLS_PER_SIDE MATRIX_COLS

#define UNUSED_MCU
#define UNUSED_MCP 2

// wiring
#define MATRIX_ROW_PINS_MCU \
    { 0, 1, 29, 28, 26, 27 }
    // { D3, D2, F4, F5, F7, F6 }
#define MATRIX_COL_PINS_MCU \
    { 4, 23, 21, 5, 6, 7, 8, 9 }
    // { D4, B2, B6, C6, D7, E6, B4, B5 }
#define UNUSED_PINS_MCU
#define MATRIX_ROW_PINS_MCP \
    { B0, B1, B2, B3, B4, B5 }
#define MATRIX_COL_PINS_MCP \
    { A0, A1, A2, A3, A4, A5, A6, A7 }
#define UNUSED_PINS_MCP

/* COL2ROW, ROW2COL*/
#define DIODE_DIRECTION COL2ROW

/* Bootmagic Lite key configuration */
#define BOOTMAGIC_LITE_ROW 0
#define BOOTMAGIC_LITE_COLUMN 1

#define DYNAMIC_KEYMAP_LAYER_COUNT 2

// Reset boot-loader behaviour
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET // Activates the double-tap behavior
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 200U // Timeout window in ms in which the double tap can occur.
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_LED GP17 // Specify a optional status led by GPIO number which blinks when entering the bootloader
