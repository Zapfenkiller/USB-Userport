# USB Userport

DIY electronic projects can interface to the modern OS again using
USB and the generic HID class driver. Now we are getting back to
the old days of virtually unlimited direct HW access to homebrew
electronics. Ahead the additional microcontroller in the interface
can enable things the host is not capable of at all.


## Features

1. Open hardware, open software design
2. Direct control of hardware connections from user programs
3. Exact usage can vary, every HW module of the micro can be used
4. Up to 18 GPIO lines in total
5. I²C bus with full multi-master-slave operation and INT support
6. SPI bus master (and some day the slave mode also?)
7. PWM controlled output lines
8. 10 bit analog inputs
9. 1-wire bus master
10. Neopixel control
11. Servo drive
12. ft-Computing Interface connections

There is no UART emulation intended. Meanwhile there are enough
other sources of USB-to-UART adapters around. Please take a look
yourself (or consider using the
[LUFA](http://www.fourwalledcubicle.com/LUFA.php)
VirtualSerial demo).


## Hardware

The hardware is quite affordable using a
[Sparkfun MicroPro](https://www.sparkfun.com/products/12640)
(clone). Be aware, that there are two kinds around and the
USB-Userport as it is uses the "5 V / 16 MHz" variant. Flashing
the unmodified firmware into any "3.3 V / 8 MHz" variant will
render your ProMicro useless. You can unbrick it by a reset and
flashing some appllication software respecting the 8 MHz clock.

Using the ProMicro means an ATmega32U4 reigns the USB-Userport
offering most of its I/O-facilities to the user. Other
microcontrollers can be used, the code has to be adapted, of
course. In some cases it is okay to use the USB as a power source.
In other cases the USB-Userport needs to get its own power source.
As it turns out a Teensy2.0++ might be also a rather good choice,
giving more ressources than just the ProMicro. It employs an
AT90USB1286 instead of the ATmega32U4. But this is some future
improvement, not covered here yet.


## Software

No admin privileges are necessary. Just any user account should
work. The generic HID class is supported by recent operating
systems (Linux, Windows, ...).

As a drawback you need to learn how to handle the generic HID
driver API of your OS. Even if this might be daunting it gives far
less hazzle than using some rather special direct IO drivers. Those
still are found on the web but need admin privileges to install
and access legacy interfaces not available on recent off-the-shelf
hardware. USB seems to be the better approach nowadays.

With the knowledge how to use the generic HID driver the userport
can be controlled by virtually any program you write. It is your
duty to bring the thing to live. With this we come close to those
old days where you just issue some IN or OUT instructions to
toggle some port lines for whatever you see fit. Now you need to
have the generic HID driver to do the communication instead.


## Credits, links and further readings

The first credits go to Dean Camera for his really fine
[LUFA](http://www.fourwalledcubicle.com/LUFA.php)
project. See there for lots of examples on LUFA usage and How-Tos.
In fact when I came across this project, I started thinking about
the new possibilites. Immediately I wanted to use it as the
foundation for my USB-Userport project.

Next comes the VBA code from
[Jan Axelson's](http://janaxelson.com/hidpage.htm)
[Usbhidio2](http://janaxelson.com/files/usbhidio2.zip)
application. I reworked this into an Excel macro for a first
way to connect to the USB-Userport device. Its macro code should
serve as some template for your own hacking in whatever
programming language you prefer.

For those willing to learn a little bit more on the USB itself,
the most recent specifications are found at the [USB.org](usb.org)
site. The information found there also could help with some aspects
of the generic HID device firmware.

[Sparkfun MicroPro page](https://www.sparkfun.com/products/12640)
Check the "Documents" there to find additional information like the
[Sparkfun ProMicro schematics](http://cdn.sparkfun.com/datasheets/Dev/Arduino/Boards/Pro_Micro_v13b.pdf).

And, of course, the datasheet of the
[ATmega32U4](http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7766-8-bit-AVR-ATmega16U4-32U4_Datasheet.pdf)
is essential to understand the properties offered.
