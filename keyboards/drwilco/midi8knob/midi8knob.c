// Copyright 2023 DocWilco (@DocWilco)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "quantum.h"

fast_timer_t previous = 0;

extern MidiDevice midi_device;

bool encoder_update_kb(uint8_t index, bool clockwise) {
    fast_timer_t now = timer_read_fast();
    fast_timer_t diff = now - previous;
    previous = now;

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
    dprintf("Encoder %d: %s %lu\n", index, clockwise ? "CW" : "CCW", diff);
    return false;
}
