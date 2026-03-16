# encoder_onekey

While technically not a keyboard, this demonstrates two rotary encoders wired
into the keyboard matrix, while still using QMK's encoder subsystem for proper
encoder event handling.

Encoder 1 is wired to `[0, 1]` and `[0, 2]`, encoder 2 to `[0, 4]` and `[0, 5]`,
and encoder 3 to `[1, 4]` and `[1, 5]` (sharing A/B columns with encoder 2 on a
different row). The
keyboard-level code in `encoder_matrix.c` decodes the quadrature signals from
matrix key events and feeds them into QMK's encoder queue via
`encoder_queue_event()`. The `encoder_update_kb()` function then sends
velocity-sensitive MIDI CC messages (relative encoding, CC 0x10+).

The switch part is wired to `[0, 0]` for bootmagic purposes. An encoder without
detents might be "pressing down" a key because only 1 out of 4 states is _not_
connecting A or B to C. And 2 out of 4 will connect a specific terminal, so if A
or B are wired to `[0, 0]` there's a 50% chance of triggering bootmagic when
plugging in.

## Wiring diagram
![Alt text](wiring_diagram.png)

If you're using the `keyboard.json` as is, wire the above diagram to your STM32G431
like this:

* `COL0` -> `B10` (encoder 1 switch)
* `COL1` -> `A15` (encoder 1 A)
* `COL2` -> `B4` (encoder 1 B)
* `COL3` -> `A5` (encoder 2 switch)
* `COL4` -> `A9` (encoder 2+3 A)
* `COL5` -> `C11` (encoder 2+3 B)
* `ROW0` -> `B12` (encoder 1+2 C / common)
* `ROW1` -> `B14` (encoder 3 C / common)

Of course, you can wire it up any way you want if you change keyboard.json. You can
even put in a different microcontroller. Just adjust `keyboard.json` accordingly.

## General Info

* Keyboard Maintainer: [DocWilco](https://github.com/DocWilco)
* Hardware Supported: Handwired STM32G431 & any rotary encoder. Built in
  switch is optional, but don't put it on `[0, 0]`, see above.

## Building and flashing
Make example for this keyboard (after setting up your build environment):

    qmk compile -kb handwired/encoder_matrix

Flashing example for this keyboard:

    qmk flash -kb handwired/encoder_matrix

See the [build environment
setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make
instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more
information. Brand new to QMK? Start with our [Complete Newbs
Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 3 ways:

* **Bootmagic reset**: Hold down the rotary encoder and plug in the keyboard
* **Physical bootsel button**: Hold the button on the Raspberry Pi Pico (near
  the USB port) and plug in the keyboard.
* **Keycode in layout**: Map the switch to `QK_BOOT` instead of `QK_ESC` in `keymap.c`
