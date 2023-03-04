/* Copyright 2022 adpenrose
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
#include "animation_frames.h"

bool is_alt_tab_active = false; // ADD this near the beginning of keymap.c
uint16_t alt_tab_timer = 0;     // we will be using them soon.


/* Base layout:
 * ,---------------------------------------------------------------------|
 * |`  |1  |2  |3  |4  |5  |6  |7  |8  |9  |0   |-   |=  |Backspace| OLED|
 * |---------------------------------------------------------------      |
 * |Tab  |Q  |W  |E  |R  |T  |Y  |U  |I  |O  |P  |[  | ]  |   \      OLED|
 * |---------------------------------------------------------------------|
 * |Caps  |A  |S  |D  |F  |G  |H  |J  |K  |L  |;  |'  |  Enter     | ENC |
 * |---------------------------------------------------------------------|
 * |Shft    |Z  |X  |C  |V  |B  |N  |M  |,  |.  |/  |Shift    |Up| M0(3) |
 * |---------------------------------------------------------------------|
 * |Ctrl|GUI |Alt |     Space               |MO(1) |MO(2)|   |Lt |Dn |Rt |
 * `---------------------------------------------------------------------|'
 */

 enum custom_keycodes {          // Make sure have the awesome keycode ready
  ALT_TAB = SAFE_RANGE,
  ALT_TAB_R,
  ALT_TAB_L
};

// Light LEDs 1 to 16 green when caps lock is active. Hard to ignore!
const rgblight_segment_t PROGMEM my_capslock_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {1, 16, HSV_GREEN}       // Light all LEDs, starting with LED 1
);
// Light LEDs 1 to 16 in cyan when keyboard layer 1 is active
const rgblight_segment_t PROGMEM my_layer1_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {1, 16, HSV_CYAN}
);
// Light LEDs 1 to 16 in purple when keyboard layer 2 is active
const rgblight_segment_t PROGMEM my_layer2_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {1, 16, HSV_PURPLE}
);
// Light LEDs 1 to 16 in green when keyboard layer 3 is active
const rgblight_segment_t PROGMEM my_layer3_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {1, 16, HSV_GOLD}
);


// Now define the array of layers. Later layers take precedence
const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(
    my_capslock_layer,
    my_layer1_layer,    // Overrides caps lock layer
    my_layer2_layer,    // Overrides other layers
    my_layer3_layer     // Overrides other layers  
);

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [0] = LAYOUT(
          KC_ESC, KC_1,    KC_2,    KC_3,    KC_4,   KC_5,   KC_6,   KC_7,   KC_8,   KC_9,    KC_0,    KC_MINS,  KC_EQL,  KC_BSPC,
          KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,   KC_T,   KC_Y,   KC_U,   KC_I,   KC_O,    KC_P,    KC_LBRC,  KC_RBRC, KC_BSLS,
          KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,   KC_G,   KC_H,   KC_J,   KC_K,   KC_L,    KC_SCLN, KC_QUOT,   KC_ENT,           KC_MUTE,
          KC_LSFT,          KC_Z,    KC_X,    KC_C,   KC_V,   KC_B,   KC_N,   KC_M,   KC_COMM, KC_DOT,  KC_SLSH,  KC_RSFT,   KC_UP,    MO(3),
          KC_LCTL, KC_LGUI, KC_LALT,                        KC_SPC,                   MO(1), MO(2),           KC_LEFT, KC_DOWN,  KC_RIGHT
    ),
    [1] = LAYOUT(
      _______,  KC_F1,    KC_F2,    KC_F3,    KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,    KC_F10,    KC_F11,  KC_F12, KC_DEL,
      _______,  _______,    _______,    _______,    _______,   _______,   _______,   _______,   _______,   _______,    _______,    _______,  _______, _______,
      QK_BOOT,  _______,    _______,    _______,    _______,   _______,   _______,   _______,   _______,   _______,    _______,    _______,  _______,            KC_MPLY,
      _______,              _______,    _______,    _______,   _______,   _______,   _______,   _______,   _______,    _______,    _______,  _______, _______,   _______,
      _______,  _______,    _______,                           _______,                                    _______,    _______,              _______,  _______,  _______
    ),
    [2] = LAYOUT(
      _______,  _______,    _______,    _______,    _______,   _______,   _______,   _______,   _______,   _______,    _______,    _______,  _______, _______,
      _______,  _______,    _______,    _______,    _______,   _______,   _______,   _______,   _______,   _______,    _______,    _______,  _______, _______,
      _______,  _______,    _______,    _______,    _______,   _______,   _______,   _______,   _______,   _______,    _______,    _______,  _______,            ALT_TAB,
      _______,              _______,    _______,    _______,   _______,   _______,   _______,   _______,   _______,    _______,    _______,  _______, _______,   _______,
      _______,  _______,    _______,                           _______,                                    _______,    _______,              _______,  _______,  _______
    ),
    [3] = LAYOUT(
      _______,    _______,      _______,      _______,      _______,     _______,     _______,     _______,     _______,     _______,     _______,     _______,   _______,  _______,
      _______,  _______,    _______,    _______,    _______,   _______,   _______,   _______,   _______,   _______,   _______,    _______,    _______,  _______,   
      _______,    _______,    _______,    _______,    _______,   _______,   _______,   _______,   _______,   _______,    _______,    _______,  _______,            _______,
      _______,              _______,    _______,    _______,   _______,   _______,   _______,   _______,   _______,    _______,    _______,  _______, _______,   _______,
      _______,  _______,    _______,                           _______,                                    _______,    _______,              _______,  _______,  _______
    ),
};

/* Encoder */
#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [0] =   { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [1] =  { ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [2] =  { ENCODER_CCW_CW(ALT_TAB_L, ALT_TAB_R) },
    [3] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS) }
};
#endif

#ifdef OLED_ENABLE
#define IDLE_FRAME_DURATION 200 // Idle animation iteration rate in ms

oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_270; }

uint32_t anim_timer         = 0;
uint32_t anim_sleep         = 0;
uint8_t  current_idle_frame = 0;

bool tap_anim        = false;
bool tap_anim_toggle = false;


// Decompress and write a precompressed bitmap frame to the OLED.
// Documentation and python compression script available at:
// https://github.com/nullbitsco/squeez-o
#ifdef USE_OLED_BITMAP_COMPRESSION
static void oled_write_compressed_P(const char* input_block_map, const char* input_block_list) {
    uint16_t block_index = 0;
    for (uint16_t i=0; i<NUM_OLED_BYTES; i++) {
        uint8_t bit = i%8;
        uint8_t map_index = i/8;
        uint8_t _block_map = (uint8_t)pgm_read_byte_near(input_block_map + map_index);
        uint8_t nonzero_byte = (_block_map & (1 << bit));
        if (nonzero_byte) {
            const char data = (const char)pgm_read_byte_near(input_block_list + block_index++);
            oled_write_raw_byte(data, i);
        } else {
            const char data = (const char)0x00;
            oled_write_raw_byte(data, i);
        }
    }
}
#endif

static void render_anim(void) {
    // Idle animation
    void animation_phase(void) {
        if (!tap_anim) {
            current_idle_frame = (current_idle_frame + 1) % NUM_IDLE_FRAMES;
            uint8_t idx = abs((NUM_IDLE_FRAMES - 1) - current_idle_frame);
            #ifdef USE_OLED_BITMAP_COMPRESSION
            oled_write_compressed_P(idle_block_map[idx], idle_frames[idx]);
            #else
            oled_write_raw_P(idle_frames[idx], NUM_OLED_BYTES);
            #endif
        }
    }

    // Idle behaviour
    if (get_current_wpm() != 000) {  // prevent sleep
        oled_on();
        if (timer_elapsed32(anim_timer) > IDLE_FRAME_DURATION) {
            anim_timer = timer_read32();
            animation_phase();
        }
        anim_sleep = timer_read32();
    } else {  // Turn off screen when timer threshold elapsed or reset time since last input
        if (timer_elapsed32(anim_sleep) > OLED_TIMEOUT) {
            oled_off();
        } else {
            if (timer_elapsed32(anim_timer) > IDLE_FRAME_DURATION) {
                anim_timer = timer_read32();
                animation_phase();
            }
        }
    }
}

bool oled_task_user(void) {
    render_anim();
    oled_set_cursor(0, 0);

    uint8_t n = get_current_wpm();
    char wpm_counter[6];
    wpm_counter[5] = '\0';
    wpm_counter[4] = '0' + n % 10;
    wpm_counter[3] = '0' + (n /= 10) % 10;
    wpm_counter[2] = '0' + n / 10 ;
    wpm_counter[1] = '0';
    wpm_counter[0] = '>';
    oled_write_ln(wpm_counter, false);

    return false;
}
#endif

// Animate tap
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    #ifdef OLED_ENABLE
    // Check if non-mod
    if ((keycode >= KC_A && keycode <= KC_0) || (keycode >= KC_TAB && keycode <= KC_SLASH)) {
        if (record->event.pressed) {
            // Display tap frames
            tap_anim_toggle = !tap_anim_toggle;
            #ifdef USE_OLED_BITMAP_COMPRESSION
            oled_write_compressed_P(tap_block_map[tap_anim_toggle], tap_frames[tap_anim_toggle]);
            #else
            oled_write_raw_P(tap_frames[tap_anim_toggle], NUM_OLED_BYTES);
            #endif
        }
    }
    #endif
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