// Copyright 2023 DocWilco (@DocWilco)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#define MIDI_ADVANCED

// PWM WS2812 configuration
// PA7 = TIM3_CH2 (AF2)
#define WS2812_PWM_DRIVER PWMD3
#define WS2812_PWM_CHANNEL 2
#define WS2812_PWM_PAL_MODE 2
#define WS2812_PWM_DMAMUX_ID STM32_DMAMUX1_TIM3_UP
