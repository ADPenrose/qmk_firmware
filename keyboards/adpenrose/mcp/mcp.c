// Copyright 2022 Arturo Avila (@ADPenrose)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "mcp.h"

#ifdef OLED_ENABLE
oled_rotation_t oled_init_kb(oled_rotation_t rotation) {
    return OLED_ROTATION_270;
}

static void render_logo(void) {
/* Kintsugi logo render: */
    static const char PROGMEM logo_1[] = {
        0x83, 0x84, 0x85, 0x86, 0x87, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0x00
    };
    static const char PROGMEM logo_2[] = {
        0x88, 0x89, 0x8A, 0x8B, 0x8C, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0x00
    };
    static const char PROGMEM logo_3[] = {
        0x8D, 0x8E, 0x8F, 0x90, 0x91, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0x00
    };
    oled_set_cursor(0,3);
    oled_write_P(logo_1, false);
    oled_set_cursor(0,7);
    oled_write_P(logo_2, false);
    oled_set_cursor(0,11);
    oled_write_P(logo_3, false);
}

bool oled_task_kb(void) {
    if (!oled_task_user()) { 
        return false; 
    }
    else {
        render_logo();
    }
    
    return true;
}
#endif
