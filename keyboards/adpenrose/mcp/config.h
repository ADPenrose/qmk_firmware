// Copyright 2022 Arturo Avila (@ADPenrose)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "config_common.h"

/* key matrix size */
#define MATRIX_ROWS 4
#define MATRIX_COLS 4

#define MATRIX_ROWS_PER_SIDE (MATRIX_ROWS / 2)
#define MATRIX_COLS_PER_SIDE (MATRIX_COLS / 2)

#define UNUSED_MCU 12
#define UNUSED_MCP 12

// wiring
#define MATRIX_ROW_PINS_MCU \
    { B3, B2 }
#define MATRIX_COL_PINS_MCU \
    { C6, D7 }
#define UNUSED_PINS_MCU \
    { D2, D3, D4, E6, B4, B5, B6, B1, F7, F6, F5, F4 }
#define MATRIX_ROW_PINS_MCP \
    { B0, B1 }
#define MATRIX_COL_PINS_MCP \
    { A0, A1}
#define UNUSED_PINS_MCP \
    { B2, B3, B4, B5, B6, B7, A2, A3, A4, A5, A6, A7 }

/* COL2ROW, ROW2COL*/
#define DIODE_DIRECTION COL2ROW

/* define if matrix has ghost (lacks anti-ghosting diodes) */
//#define MATRIX_HAS_GHOST

/* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed */
#define DEBOUNCE 5
