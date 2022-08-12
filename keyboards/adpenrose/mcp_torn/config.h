// Copyright 2022 Arturo Avila (@ADPenrose)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "config_common.h"

#define MATRIX_ROWS 2
#define MATRIX_COLS 4

/* Keyboard Matrix Assignments */
#define MATRIX_ROW_PINS \
    { B3,B6 }
#define MATRIX_COL_PINS \
    { C6, D7 }
#define SECONDARY_ROW_PINS \
    { (1 << 8), (1 << 9) }
#define SECONDARY_COL_PINS \
    { (1 << 0), (1 << 1) }

/* Diode direction */
#define DIODE_DIRECTION COL2ROW

#define DEBOUNCE 5
