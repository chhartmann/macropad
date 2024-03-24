# Firmware for 3 button keyboard
The keyboard was bought on Aliexpress https://de.aliexpress.com/item/1005006498826681.html?spm=a2g0o.productlist.main.33.657f6f41KaYzvy&algo_pvid=3afdf448-664d-4bd5-8ced-1f356025f6e6&algo_exp_id=3afdf448-664d-4bd5-8ced-1f356025f6e6-16&pdp_npi=4%40dis%21EUR%2112.15%210.46%21%21%2112.88%210.49%21%402103835c17112633249598035e0698%2112000037424029047%21sea%21DE%210%21AB&curPageLogUid=kA4DLUVZwk32&utparam-url=scene%3Asearch%7Cquery_from%3A

There seem to be different type of macro keyboards containing an CH552 controller available on Aliexpress. As it contains an CH552, this is pretty convenient. One can have a programmable macro keyboard without building their own.

The project has been inspired by https://hackaday.io/project/189914-rgb-macropad-custom-firmware/log/216585-programming-with-ch55xduino-mapping-the-3-keys

It is based on the Arduino board package: https://github.com/DeqingSun/ch55xduino

# Features
This firmware only sets up a serial connection to receive the buttons pressed and to control the LEDs. It does not provide a USB HID device (which is also easily possible with the Arduino board package).

# How To
To put the controller in boot mode, there are two solder eyes on the board, which can be connected via a 10K resistor. The resistor does not to be soldered on the board. It can just be hold there.
The resistor must connect the two contacts before the board is plugged in. The resistor must stay connected until the firmware is flashed the first time.
After that, the resistor can be removed and the board can automatically be flashed again and again with the Arduino environment, as this firmware only uses serial protocol and does not interfere with the arduino download mechanism.
