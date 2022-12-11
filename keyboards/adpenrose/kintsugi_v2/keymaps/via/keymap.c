#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_all(
                 KC_ESC, KC_1,    KC_2,    KC_3,    KC_4,   KC_5,   KC_6,   KC_7,   KC_8,   KC_9,    KC_0,    KC_MINS,  KC_EQL,  KC_BSPC, KC_BSPC, KC_1,
                 KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,   KC_T,   KC_Y,   KC_U,   KC_I,   KC_O,    KC_P,    KC_LBRC,  KC_RBRC, KC_BSLS, KC_2,
                 KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,   KC_G,   KC_H,   KC_J,   KC_K,   KC_L,    KC_SCLN, KC_QUOT, KC_NUHS,  KC_ENT, KC_3,
                 KC_LSFT, KC_BSLS, KC_Z,    KC_X,    KC_C,   KC_V,   KC_B,   KC_N,   KC_M,   KC_COMM, KC_DOT,  KC_SLSH,  KC_RSFT,   KC_UP,   KC_4,
        KC_MUTE, KC_LCTL, KC_LGUI, KC_LALT, KC_SPC,                   KC_SPC,                KC_RALT, KC_RGUI, KC_RCTL,   KC_LEFT, KC_DOWN,  KC_RIGHT  
  ),

  [1] = LAYOUT_all(
                 KC_ESC, KC_1,    KC_2,    KC_3,    KC_4,   KC_5,   KC_6,   KC_7,   KC_8,   KC_9,    KC_0,    KC_MINS,  KC_EQL,  KC_BSPC, KC_BSPC, KC_1,
                 KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,   KC_T,   KC_Y,   KC_U,   KC_I,   KC_O,    KC_P,    KC_LBRC,  KC_RBRC, KC_BSLS, KC_2,
                 KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,   KC_G,   KC_H,   KC_J,   KC_K,   KC_L,    KC_SCLN, KC_QUOT, KC_NUHS,  KC_ENT, KC_3,
                 KC_LSFT, KC_BSLS, KC_Z,    KC_X,    KC_C,   KC_V,   KC_B,   KC_N,   KC_M,   KC_COMM, KC_DOT,  KC_SLSH,  KC_RSFT,   KC_UP,   KC_4,
        KC_MUTE, KC_LCTL, KC_LGUI, KC_LALT, KC_SPC,                   KC_SPC,                KC_RALT, KC_RGUI, KC_RCTL,   KC_LEFT, KC_DOWN,  KC_RIGHT  
  )  
};
