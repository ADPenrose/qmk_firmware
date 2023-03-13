/* Copyright 2020 GhostSeven <work@ghost7.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

#define KC_COPY LCTL(KC_C)
#define KC_CUT LCTL(KC_X)
#define KC_PASTE LCTL(KC_V)

bool is_alt_tab_active = false; // ADD this near the beginning of keymap.c
uint16_t alt_tab_timer = 0;     // we will be using them soon.

 enum custom_keycodes {          // Make sure have the awesome keycode ready
  ALT_TAB = SAFE_RANGE,
  ALT_TAB_R,
  ALT_TAB_L,
  KC_SS,
  KC_LOVE,
};

// Light LEDs 1 to 16 green when caps lock is active. Hard to ignore!
const rgblight_segment_t PROGMEM my_capslock_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {1, 11, HSV_GREEN}       // Light all LEDs, starting with LED 1
);
// Light LEDs 1 to 16 in cyan when keyboard layer 1 is active
const rgblight_segment_t PROGMEM my_layer1_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {1, 11, HSV_CYAN}
);
// Light LEDs 1 to 16 in purple when keyboard layer 2 is active
const rgblight_segment_t PROGMEM my_layer2_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {1, 11, HSV_PURPLE}
);
// Light LEDs 1 to 16 in green when keyboard layer 3 is active
const rgblight_segment_t PROGMEM my_layer3_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {1, 11, HSV_GOLD}
);

// Now define the array of layers. Later layers take precedence
const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(
    my_capslock_layer,
    my_layer1_layer,    // Overrides caps lock layer
    my_layer2_layer,    // Overrides other layers
    my_layer3_layer     // Overrides other layers  
);

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
        |                  |                     | Knob : Page Up/Dn    |
        |        Copy      |         Cut         |    Press: Mute    |
        |        Paste     |  Locking Caps Lock  |   Toggle layer    |
     */
    [0] = LAYOUT(
        KC_COPY, KC_CUT, KC_MUTE,
        KC_PASTE, KC_LCAP, TO(1)
    ),
    /*
        |               |                        | Knob : Vol Up/Dn|
        | Previous Track   |   Next Track      |  Toggle RGB    |
        | Play/Pause Track |Launch Media Player|  Toggle layer  |
     */
    [1] = LAYOUT(
        KC_MPRV, KC_MNXT, RGB_TOG,
        KC_MPLY, KC_MSEL, TO(2)
    ),
    /*
        |               |                 |               |
        |   Print Screen  |        @      |               |
        |Launch Calculator|  I love you   | Toggle layer  |
     */
   [2] = LAYOUT(
        KC_SS, KC_AT, ALT_TAB,
        KC_CALC, KC_LOVE, TO(3)
    ),
    /* 
        |                   |                              |               |
        |Cycle through modes|Cycle through modes in reverse| Toggle RGB    |
        |     Increase hue  |     Increase saturation      | Toggle layer  |
     */
    [3] = LAYOUT(
        RGB_MODE_FORWARD, RGB_MODE_REVERSE, RGB_TOG,
        RGB_HUI, RGB_SAI,  TO(0)
    ),

};

/* Encoder */
#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [0] =   { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [1] =  { ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [2] =  { ENCODER_CCW_CW(ALT_TAB_L, ALT_TAB_R) },
    [3] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) }
};
#endif

// Animate tap
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    /* Super Alt+Tab */
    switch (keycode) { // This will do most of the grunt work with the keycodes.
    case ALT_TAB:
      if (record->event.pressed) {
        if (!is_alt_tab_active) {
          is_alt_tab_active = true;
          register_code(KC_LALT);
        }
        alt_tab_timer = timer_read();
        register_code(KC_TAB);
      } else {
        unregister_code(KC_TAB);
      }
        break;
    case ALT_TAB_R:
        if (record->event.pressed && is_alt_tab_active == true) {
        tap_code(KC_RIGHT);
        alt_tab_timer = timer_read();
        }
        break;
    case ALT_TAB_L:
        if (record->event.pressed && is_alt_tab_active == true) {
        tap_code(KC_LEFT);
        alt_tab_timer = timer_read();
        }
        break;
    case KC_SS:
        if (record->event.pressed) {
          SEND_STRING(SS_LGUI(SS_LSFT("s")));
        } else {
            // when keycode is released
        }
        break;
    case KC_LOVE:
        if (record->event.pressed) {
          SEND_STRING("Te amo Fernandita");
        } else {
            // when keycode is released
        }
        break;
  }
    return true;

}

void keyboard_post_init_user(void) {
    // Enable the LED layers
    rgblight_layers = my_rgb_layers;
}

bool led_update_user(led_t led_state) {
    rgblight_set_layer_state(0, led_state.caps_lock);
    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    rgblight_set_layer_state(1, layer_state_cmp(state, 1)); 
    rgblight_set_layer_state(2, layer_state_cmp(state, 2));  
    rgblight_set_layer_state(3, layer_state_cmp(state, 3));
    return state;
}

void matrix_scan_user(void) { // The very important timer.
  if (is_alt_tab_active) {
    if (timer_elapsed(alt_tab_timer) > 1000) {
      unregister_code(KC_LALT);
      is_alt_tab_active = false;
    }
  }
}
