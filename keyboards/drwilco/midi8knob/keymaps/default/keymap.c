// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

extern MidiDevice midi_device;
extern midi_config_t midi_config;
extern debug_config_t debug_config;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {[0] = LAYOUT(
    MI_Gs, MI_A, MI_As, MI_B, MI_C1, MI_Cs1, MI_D1, MI_Ds1
)};

void keyboard_post_init_user(void) {
    /* turn off octave shifting so the above notes are not translated */
    midi_config.octave = QK_MIDI_OCTAVE_0 - MIDI_OCTAVE_MIN;

    rgb_matrix_enable_noeeprom();
    rgb_matrix_mode_noeeprom(RGB_MATRIX_BREATHING);
    //debug_config.enable = true;
    //debug_config.matrix = true;
    //debug_config.keyboard = true;
    //debug_config.mouse = true;

}
