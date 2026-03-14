// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

extern debug_config_t debug_config;

enum my_keycodes {
    KC_ENC1A = QK_USER,
    KC_ENC1B,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_ortho_1x3(
        KC_ESC, KC_ENC1A, KC_ENC1B
    )
};

static bool enc1a_pressed = false;
static bool enc1b_pressed = false;
static bool enc1a_known = false;
static bool enc1b_known = false;

// Change this is if you have regular encoders as well, should be higher than
// your highest encoder index
const uint8_t enc1_index = 0;

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        if (clockwise) {
            dprintf("Encoder 1 clockwise\n");
            tap_code(KC_PGDN);
        } else {
            dprintf("Encoder 1 counter-clockwise\n");
            tap_code(KC_PGUP);
        }
        return false;
    }
    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode >= KC_ENC1A && keycode <= KC_ENC1B) {
        switch (keycode) {
            case KC_ENC1A:
                enc1a_pressed = record->event.pressed;
                enc1a_known = true;
                if (enc1a_known && enc1b_known) {
                    // Clockwise, when A changes, B is different
                    encoder_update_user(enc1_index, enc1a_pressed != enc1b_pressed);
                }
                break;
            case KC_ENC1B:
                enc1b_pressed = record->event.pressed;
                enc1b_known = true;
                if (enc1a_known && enc1b_known) {
                    // Clockwise, when B changes, A is the same
                    encoder_update_user(enc1_index, enc1a_pressed == enc1b_pressed);
                }
                break;
        }
        // No further handling of these codes
        return false;
    }
    // Process all other keycodes normally
    return true;
}

void keyboard_post_init_user(void) {
    debug_config.enable = true;
    debug_config.matrix = true;
    debug_config.keyboard = false;
    debug_config.mouse = false;
}
