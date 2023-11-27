// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        QK_MIDI_NOTE_G_SHARP_0, QK_MIDI_NOTE_A_0, QK_MIDI_NOTE_A_SHARP_0, QK_MIDI_NOTE_B_0,
        QK_MIDI_NOTE_C_1, QK_MIDI_NOTE_C_SHARP_1, QK_MIDI_NOTE_D_1, QK_MIDI_NOTE_D_SHARP_1
    )
};

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  debug_enable=true;
  debug_matrix=true;
  //debug_keyboard=true;
  //debug_mouse=true;
}
