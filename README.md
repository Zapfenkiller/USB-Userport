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
4. 18 GPIO lines in total (for a Sparkfun ProMicro)
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
[Sparkfun ProMicro](https://www.sparkfun.com/products/12640)
(or clone). Be aware, that there are two kinds around and the
USB-Userport as it is uses the "5 V / 16 MHz" variant. Flashing
the unmodified firmware into any "3.3 V / 8 MHz" variant will
render your ProMicro useless. You can unbrick it by a reset and
flashing some application software respecting the 8 MHz clock.

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

### Host
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

### Device
All the stuff is prepared. Those who trust me (or are not aware of
the threats of a
[USB rubber ducky](https://github.com/hak5darren/USB-Rubber-Ducky/))
can flash it to their ProMicro the usual way. All the others can
`make` the .hex themselves from the sources given here.


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
application. I reworked this into an Excel macro for a first way to
connect to the USB-Userport device. Its macro code should serve as
some template for your own hacking in whatever programming
language you prefer. Dealing with the stuff I also had to read
through the µ$ documentation of the API. Since you might use
another OS you better take a journey with your preferred search
engine.

For those willing to learn a little bit more on the USB itself,
the most recent specifications are found at the [USB.org](usb.org)
site. The information found there also could help with some aspects
of the generic HID device firmware. You might consider the
[Device Class Definition for Human Interface Devices (HID)](https://www.usb.org/sites/default/files/documents/hid1_11.pdf).
There are
[more related documents](https://usb.org/documents?search=HID&items_per_page=50)
around there.
And also the
[USB 2.0 specification](https://www.usb.org/sites/default/files/usb_20_20190524.zip).

Give a visit to the
[Sparkfun ProMicro page](https://www.sparkfun.com/products/12640).
Check the "Documents" there to find additional information like the
[Sparkfun ProMicro schematics](http://cdn.sparkfun.com/datasheets/Dev/Arduino/Boards/Pro_Micro_v13b.pdf).

And, of course, the datasheet of the
[ATmega32U4](http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7766-8-bit-AVR-ATmega16U4-32U4_Datasheet.pdf)
is essential to understand the properties offered when you want to
enhance the thing to your needs.
