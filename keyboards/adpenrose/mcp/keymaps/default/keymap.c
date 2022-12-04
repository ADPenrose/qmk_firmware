#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ┌───┬───┬───┬───┐
     * │ 7 │ 8 │ 9 │ / │
     * ├───┼───┼───┼───┤
     * │ 4 │ 5 │ 6 │ * │
     * ├───┼───┼───┼───┤
     * │ 1 │ 2 │ 3 │ - │
     * ├───┼───┼───┼───┤
     * │ 0 │ . │Ent│ + │
     * └───┴───┴───┴───┘
     */
    [0] = LAYOUT_all(
        KC_1,   KC_2,   KC_3,   KC_4,   
	KC_LSFT,   KC_6,   KC_7,   MO(1)    
    ),
    [1] = LAYOUT_all(
        QK_BOOT,   KC_8,   KC_9,   KC_0,   
	KC_Q,   KC_W,   KC_R,   KC_TRNS    
    )

};
