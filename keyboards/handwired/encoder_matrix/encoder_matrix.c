// Copyright 2023 DocWilco (@DocWilco)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "quantum.h"

// Keyboard-level custom keycodes for matrix-wired encoder pins
enum encoder_matrix_keycodes {
    ENC1_A = QK_KB_0,
    ENC1_B,
    ENC2_A,
    ENC2_B,
    ENC3_A,
    ENC3_B,
};

#define NUM_MATRIX_ENCODERS 3

// Custom encoder driver stubs — encoder events are queued from
// process_record_kb when the matrix detects encoder pin changes.
void encoder_driver_init(void) {}
void encoder_driver_task(void) {}

void keyboard_post_init_kb(void) {
    debug_enable = true;
    keyboard_post_init_user();
}

// Quadrature state tracking per encoder
static bool enc_a_pressed[NUM_MATRIX_ENCODERS] = {false};
static bool enc_b_pressed[NUM_MATRIX_ENCODERS] = {false};
static bool enc_state_initialized = false;

// Set the initialized flag after the first matrix scan completes,
// so we don't interpret the initial state as rotation.
void housekeeping_task_kb(void) {
    if (!enc_state_initialized) {
        enc_state_initialized = true;
    }
}

static void handle_encoder_a(uint8_t index, bool pressed) {
    enc_a_pressed[index] = pressed;
    if (enc_state_initialized) {
        // When A changes, clockwise if B is different
        encoder_queue_event(index, enc_a_pressed[index] != enc_b_pressed[index]);
    }
}

static void handle_encoder_b(uint8_t index, bool pressed) {
    enc_b_pressed[index] = pressed;
    if (enc_state_initialized) {
        // When B changes, clockwise if A is the same
        encoder_queue_event(index, enc_a_pressed[index] == enc_b_pressed[index]);
    }
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case ENC1_A: handle_encoder_a(0, record->event.pressed); return false;
        case ENC1_B: handle_encoder_b(0, record->event.pressed); return false;
        case ENC2_A: handle_encoder_a(1, record->event.pressed); return false;
        case ENC2_B: handle_encoder_b(1, record->event.pressed); return false;
        case ENC3_A: handle_encoder_a(2, record->event.pressed); return false;
        case ENC3_B: handle_encoder_b(2, record->event.pressed); return false;
    }
    return process_record_user(keycode, record);
}

// MIDI encoder behavior (velocity-sensitive, from midi8knob)
static fast_timer_t previous[NUM_MATRIX_ENCODERS] = {0};

extern MidiDevice midi_device;

bool encoder_update_kb(uint8_t index, bool clockwise) {
    if (!encoder_update_user(index, clockwise)) {
        return false;
    }

    fast_timer_t now = timer_read_fast();
    fast_timer_t diff = now - previous[index];
    previous[index] = now;

    // 24 PPR encoders can be quite jittery, this seems like a good filter
    if (diff <= 2) {
        return false;
    }

    uint8_t amount = 1;
    if (diff < 50 && diff > 10) {
        amount = 2;
    } else if (diff <= 10 && diff > 5) {
        amount = 4;
    } else if (diff <= 5) {
        amount = 8;
    }
    midi_send_cc(&midi_device, 0, 0x10 + index, (clockwise ? 0 : 64) + amount);
    dprintf("Encoder %d: %s %lu %d\n", index, clockwise ? "CW" : "CCW", diff, amount);
    return false;
}
