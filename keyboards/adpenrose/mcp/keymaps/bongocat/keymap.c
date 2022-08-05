#include QMK_KEYBOARD_H
#include "animation_frames.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_all(
        KC_1,   KC_2,   KC_3,   KC_4,   
	KC_5,   KC_6,   KC_7,   MO(1)    
    ),
    [1] = LAYOUT_all(
        QK_BOOT,   KC_8,   KC_9,   KC_0,   
	KC_Q,   KC_W,   KC_R,   KC_TRNS    
    )

};

/* Encoder */
#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
    /* Used to change the layer using the encoder. */
    static int8_t selected_layer = 0; 

    if (clockwise){
        /* Check if left shift is pressed: */
        if (selected_layer < 4 && get_mods() & MOD_BIT(KC_LSFT)){
            selected_layer ++;
            /* If already on the last layer, jumps back to the first layer: */
            if (selected_layer == 4) {
                selected_layer = 0;
            }
            /* Move to the selected layer. */
            layer_move(selected_layer); 
        } else if (get_mods() & MOD_BIT(KC_RSFT)){ /* Check if right shift is pressed: */
            switch (get_highest_layer(layer_state)){
                default:
                    /* Go to the next track. */
                    tap_code(KC_MNXT);
                    break;
            }
        } else {
            /* If shift isn't pressed, encoder will do this stuff: */
            switch (get_highest_layer(layer_state)){
                default:
                    /* Turn up the volume of the system. */
                    tap_code(KC_VOLU);
                    break;
            }
        }
    } else {
        /* Check if left shift is pressed: */
        if (selected_layer > -1 && get_mods() & MOD_BIT(KC_LSFT)){
            selected_layer --;
            /* If already on the first layer, jumps up to the last layer: */
            if (selected_layer == -1) {
                selected_layer = 3;
            }
        /* Move to the selected layer. */
            layer_move(selected_layer);
        } else if (get_mods() & MOD_BIT(KC_RSFT)){ /* Check if right shift is pressed: */
            switch (get_highest_layer(layer_state)){
                default:
                    /* Go to the previous track. */
                    tap_code(KC_MPRV);
                    break;
            }
        } else {
            /* If shift isn't pressed, encoder will do this stuff: */
            switch (get_highest_layer(layer_state)){
                default:
                    /* Turn down the volume of the system. */
                    tap_code(KC_VOLD);
                    break;
            }
        }
    }
    return false;
}
#endif

#ifdef OLED_ENABLE
#define IDLE_FRAME_DURATION 200 // Idle animation iteration rate in ms

oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_90; }

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

    return true;

}
