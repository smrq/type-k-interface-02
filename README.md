# TYPE-K Interface 02

120% keyboard design

## PCB

The design for the PCB was done in KiCad. The PCB is designed according to the specifications of manufacturing with [JLCPCB](https://jlcpcb.com/).

### BOM

* **D1-128** [MiniMELF Diode (LL4148 L1G)](https://www.mouser.com/ProductDetail/Taiwan-Semiconductor/LL4148-L1G?qs=Mv7BduZupUgb22r9g5AxCQ%3D%3D)
* **D129-256** [WS2812B-Mini RGB LED](https://lcsc.com/product-detail/Light-Emitting-Diodes-LED_Worldsemi-WS2812B-Mini_C527089.html)
* **J1** 2x3-pin 2.54mm header (optional)
* **J2** 0.91" OLED Display (e.g. [Amazon link](https://www.amazon.com/MakerFocus-Display-SSD1306-3-3V-5V-Arduino/dp/B079BN2J8V))
* **R1-3** 4K7Ω 0805 resistor
* **SW1-128** Cherry MX RGB switch (either plate or PCB mount)
* **SW16** Alps EC11E rotary encoder
* **SW129** [MJTP1117 equivalent reset switch](https://keeb.io/collections/diy-parts/products/reset-pushbutton-switch)
* **U1** [Elite-C v4](https://keeb.io/products/elite-c-low-profile-version-usb-c-pro-micro-replacement-atmega32u4)
* **U2** [74HC154 4-to-16 demultiplexer](https://lcsc.com/product-detail/Signal-Switches-Encoders-Decoders-Multiplexers_Texas-Instruments-CD74HC154M96_C2832236.html)


## Case

Prerequisites:

* pyenv ([*nix](https://github.com/pyenv/pyenv) or [Windows](https://github.com/pyenv-win/pyenv-win))
* [Poetry](https://python-poetry.org/) package manager

Building:

* `poetry install`
* `poetry run python keyboard_layout.py`


## Firmware

Prerequisites:

* Docker
* dfu-programmer

Building:

* `make`

Flashing:

* `make flash`
