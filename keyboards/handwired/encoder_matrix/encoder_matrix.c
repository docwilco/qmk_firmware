// Copyright 2023 DocWilco (@DocWilco)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "quantum.h"

// Prototype quadrature_matrix encoder driver:
// Reads encoder A/B pin states directly from the debounced key matrix
// via peek_matrix(), bypassing MATRIX_MASKED so the encoder positions
// don't generate key events.

extern bool peek_matrix(uint8_t row_index, uint8_t col_index, bool raw);

#define NUM_MATRIX_ENCODERS 3

typedef struct {
    uint8_t row_a;
    uint8_t col_a;
    uint8_t row_b;
    uint8_t col_b;
} encoder_matrix_pin_t;

// Matrix positions for each encoder's quadrature pins
static const encoder_matrix_pin_t encoder_matrix_pins[NUM_MATRIX_ENCODERS] = {
    {0, 1, 0, 2}, // Encoder 0: A=[0,1], B=[0,2]
    {0, 4, 0, 5}, // Encoder 1: A=[0,4], B=[0,5]
    {1, 4, 1, 5}, // Encoder 2: A=[1,4], B=[1,5]
};

// Previous pin states for quadrature decoding
static bool prev_a[NUM_MATRIX_ENCODERS];
static bool prev_b[NUM_MATRIX_ENCODERS];
static bool state_initialized = false;

void encoder_driver_init(void) {}

void encoder_driver_task(void) {
    for (uint8_t i = 0; i < NUM_MATRIX_ENCODERS; i++) {
        const encoder_matrix_pin_t *pins = &encoder_matrix_pins[i];

        bool a = peek_matrix(pins->row_a, pins->col_a, false);
        bool b = peek_matrix(pins->row_b, pins->col_b, false);

        if (state_initialized) {
            if (a != prev_a[i]) {
                // A changed: clockwise when A differs from B
                encoder_queue_event(i, a != b);
            }
            if (b != prev_b[i]) {
                // B changed: clockwise when A matches B
                encoder_queue_event(i, a == b);
            }
        }

        prev_a[i] = a;
        prev_b[i] = b;
    }

    state_initialized = true;
}

void keyboard_post_init_kb(void) {
    debug_enable = true;
    debug_matrix = true;
    keyboard_post_init_user();
}

// MIDI encoder behavior (velocity-sensitive)
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
