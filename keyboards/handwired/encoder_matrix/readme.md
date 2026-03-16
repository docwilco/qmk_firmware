# encoder_matrix

While technically not a keyboard, this demonstrates three rotary encoders wired
into the keyboard matrix, while still using QMK's encoder subsystem for proper
encoder event handling.

Encoder 1 is wired to `[0, 1]` and `[0, 2]`, encoder 2 to `[0, 4]` and `[0, 5]`,
and encoder 3 to `[1, 4]` and `[1, 5]` (sharing A/B columns with encoder 2 on a
different row). Each encoder also has a switch: encoder 1 at `[0, 0]`, encoder 2
at `[0, 3]`, and encoder 3 at `[1, 3]`.

The custom encoder driver in `encoder_matrix.c` reads the quadrature signals
directly from the debounced matrix via `peek_matrix()`, bypassing
`MATRIX_MASKED` so the encoder positions don't generate key events. It feeds
rotation events into QMK's encoder queue via `encoder_queue_event()`. The
`encoder_update_kb()` function then sends prints debug messages to the console.

The encoder switch at `[0, 0]` doubles as the bootmagic key. An encoder without
detents might be "pressing down" a key because only 1 out of 4 states is _not_
connecting A or B to C. And 2 out of 4 will connect a specific terminal, so if A
or B are wired to `[0, 0]` there's a 50% chance of triggering bootmagic when
plugging in.

## Wiring diagram
![Alt text](https://i.imgur.com/ErTWy7w.png)

If you're using the `keyboard.json` as is, wire the above diagram to your
STM32G431 like this:

* `COL0` -> `B10` (encoder 1 switch)
* `COL1` -> `A15` (encoder 1 A)
* `COL2` -> `B4` (encoder 1 B)
* `COL3` -> `A5` (encoder 2 switch)
* `COL4` -> `A9` (encoder 2+3 A)
* `COL5` -> `C11` (encoder 2+3 B)
* `ROW0` -> `B12` (encoder 1+2 C / common)
* `ROW1` -> `B14` (encoder 3 C / common)

As mentioned above, there's a 50% chance for each encoder terminal to act like a
closed switch, so we need NKRO enabled, and diodes on one side of every switch
and all encoder A/B terminals to prevent ghosting. The matrix is `COL2ROW`, so
there are diodes between the column wire and the switch/encoder terminals. With
the cathode (band) toward the terminal, and the anode toward the column wire.
The C terminal on the encoders and the other side of the switch are wired
directly to the row wire.

Of course, you can wire it up any way you want if you change keyboard.json. You
can even put in a different microcontroller. Just adjust `keyboard.json`
accordingly.

## General Info

* Keyboard Maintainer: [DocWilco](https://github.com/DocWilco)
* Hardware Supported: Handwired STM32G431 & any rotary encoder. Built in
  switches are optional, but don't put an encoder's A or B on `[0, 0]`, see
  above.

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

* **Bootmagic reset**: Hold down encoder 1's switch and plug in the keyboard
* **Physical BOOT0 button**: Hold the BOOT0 button on the STM32G431 board and
  plug in (or press reset while holding BOOT0).
* **Keycode in layout**: Map a switch to `QK_BOOT` in `keymap.json`
