# DocWilco MIDI 8 Knob

![drwilco/midi8knob](imgur.com image replace me!)

This is the firmware for my MIDI 8 Knob project. It has 8 rotary encoders with built in switches (Bourns PEC11R-xxxxx-Sxxxx, my favorite being the PEC11R-4215F-S0024) and 8 LEDs (WS2812B). It is meant to be used as a MIDI controller for things like MIDI Mixer. 

* Keyboard Maintainer: [DocWilco](https://github.com/DocWilco)
* Hardware Supported: DocWilco MIDI 8 Knob PCB
* Hardware Availability: TBD

Make example for this keyboard (after setting up your build environment):

    make drwilco/midi8knob:default

Flashing example for this keyboard:

    make drwilco/midi8knob:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 3 ways:

* **Bootmagic reset**: Hold down the top left encoder and plug in the keyboard
* **Physical reset button**: Briefly press the RESET button on the PCB while holding BOOTSEL
* **Keycode in layout**: Press the encoder mapped to `QK_BOOT` if you have done that in the keymap
