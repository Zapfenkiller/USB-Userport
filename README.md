# USB Userport

DIY electronic projects can interface to the modern OS again using
USB and the generic HID class driver. Now we are getting back to
the old days of virtually unlimited direct HW access to homebrew
electronics. Ahead the additional microcontroller in the interface
can enable things the host is not capable of at all.


## Features

1. Direct control of hardware connections from user programs
   running on the PC.
2. No installation required. Just connect the USB-Userport and
   start your application program.
3. Adressable, supports more than just one USB-Userport attached
   to the computer.
4. Open source design to those who want to look under the hood.
5. 18 GPIO lines in total when using an off-the-shelf Sparkfun
   ProMicro.
6. Exact usage can vary, every HW module of the micro can be used:
   *  PWM controlled output lines
   *  10 bit analog inputs
   *  TWI (I²C) bus
   *  SPI bus
   *  Timers / Capture

There is no UART emulation intended. You will find enough other
sources of USB-to-UART adapters. Please take a look yourself (or
consider using the
[LUFA](http://www.fourwalledcubicle.com/LUFA.php)
VirtualSerial demo).


## Current Status

Revision 1.0.0:
*  Technically the same as 0.0.4, just C++ Host Demo is added.

Revision 0.0.4:
*  18 GPIO lines, individual direction and change report control.
*   4 GPIOs usable as analog inputs alternatively.
*   2 GPIOs alternatively usable to control a servo each.
*     All I/O registers of the µC are accessible by host application.

Revision 0.0.3:
*  18 GPIO lines, individual direction and change report control.
*   4 GPIOs usable as analog inputs alternatively.
*   2 GPIOs alternatively usable to control a servo each.

Revision 0.0.2:
*  18 GPIO lines, individual direction and change report control.
*   4 GPIOs usable as analog inputs alternatively.

Revision 0.0.1:
*  18 GPIO lines, individual direction and change report control.


## Hardware

The hardware is quite affordable using a
[Sparkfun ProMicro](https://www.sparkfun.com/products/12640)
(or clone). Be aware, that there are two kinds around and the
USB-Userport as it is uses the "5 V / 16 MHz" variant. Flashing
the unmodified firmware into any "8 MHz" variant will render your
8-MHz-ProMicro useless. You can unbrick it by a reset and flashing some
application software respecting the 8 MHz clock.

As an alternative you can also use an
[Arduino Leonardo](https://www.arduino.cc/en/Main/Arduino_BoardLeonardo)
the same way the ProMicro works. The Leonardo gives a few more I/Os to
the user.

Using the ProMicro or Leonardo means an ATmega32U4 reigns the USB-Userport
offering most of its I/O-facilities to the user. Other
microcontrollers can be used, the code has to be adapted, of
course. In some cases it is okay to use the USB as a power source.
In other cases the USB-Userport needs to get its own power source.

As it turns out a Teensy2.0++ might be also a rather good choice,
giving more ressources than just the ProMicro. It employs an
AT90USB1286 instead of the ATmega32U4. But this is some future
improvement, not covered here yet. The LUFA instantly compiles for
many 8-bit AVR controllers. You need to adapt some code of the
USB-Userport itself for your board.


## Software

With this project we have two sides to cover. The device side is
what makes the thing attaching to the USB. The host application
brings it to live finally. This is in contrast to what average
Arduino users do. They use the PC to develop their program code,
the USB to flash the code into the board and the board to run the
code on its own.


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

From the code examples one should be able to see the usage of
generic HID API on a windows system. Other OS should be similar.

The code examples for the host are dedicated as macros to be used with
a Microsoft Excel. Maybe (but this is untestet for the time being) some
recent LibreOffice Calc might also work with them.

In addition to the exotic Excel stuff there is also a C++ demo
using
[wxWidgets 3.0.5](https://www.wxwidgets.org/),
[hidapi 0.11.0](https://github.com/libusb/hidapi)
and
[Code::Blocks 20.03](https://www.codeblocks.org/).

Look into the example to get an idea of how you could connect your own
GUI to your electronics.
This is not the most professional programming but it should do as a
starting point.
This demo code is just verified on a Win10 installation; according to
the libraries' documentation it should do also with other operating
systems.


### Device

All the stuff is prepared. Those who trust me (or are not aware of
the threats of a
[USB rubber ducky](https://github.com/hak5darren/USB-Rubber-Ducky/))
can flash the premade .hex into their ProMicro the usual way. All
the others can `make` the .hex themselves from the sources given
here.


## Make the Thing

You need to get a Sparkfun ProMicro (or one of its clones or close
relatives). Check its schematic for differences to the original!
Here the 16 MHz variant (5 V) is used.
If there are differences, you need to check if the code shall be
adapted. In either case, an Arduino-compatible (AVR109) bootloader
needs to be flashed there already.

You need to get a WinAVR-20100110 compiler suite to (re)compile
the .hex and the avrdude for flashing via USB.

The Arduino IDE you do **not** need for this project. When your
ProMicro is to be reused for an Arduino project, you need to give
a hardware reset just before the Arduino IDE reflashes it. You have
roughly 8 s (depending on the bootloader) and I think the blink
example will do.


## Credits, links and further readings

The first credits go to Dean Camera for his really fine
[LUFA](http://www.fourwalledcubicle.com/LUFA.php)
project. See there for lots of examples on LUFA usage and How-Tos.
In fact when I came across this project, I started thinking about
the new possibilites. Immediately I wanted to use it as the
foundation for my USB-Userport project. LUFA is my key to the
device side of the USB-Userport.

Next comes the VBA code from
[Jan Axelson's](http://janaxelson.com/hidpage.htm)
[Usbhidio2](http://janaxelson.com/files/usbhidio2.zip)
application. It gave me a first insight how to use the µ$ HID API
from excel macros. This I found useful for the host side of the
USB-Userport.

µ$ documentation is somewhat useful when writing host applications
based on windows:  
[Introduction to Human Interface Devices (HID)](https://docs.microsoft.com/en-us/windows-hardware/drivers/hid/)
[Human Interface Devices (HID)](https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/_hid/)  
[hidsdi.h header](https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/hidsdi/)  
For Linux you could probably start at  
[HID I/O Transport Drivers](https://www.kernel.org/doc/html/latest/hid/hid-transport.html)

Give a visit to the
[Sparkfun ProMicro page](https://www.sparkfun.com/products/12640).
Check the "Documents" there to find additional information like the
[Sparkfun ProMicro schematics](http://cdn.sparkfun.com/datasheets/Dev/Arduino/Boards/Pro_Micro_v13b.pdf).

And, of course, the datasheet of the
[ATmega32U4](http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7766-8-bit-AVR-ATmega16U4-32U4_Datasheet.pdf)
is essential to understand the properties offered when you want to
enhance the thing to your needs.

For those willing to learn a little bit more on the USB itself,
the most recent specifications are found at the
[USB-IF site](https://www.usb.org).
The information found there also could help with some aspects
of the generic HID device firmware. You might consider the
[Device Class Definition for Human Interface Devices (HID)](https://www.usb.org/sites/default/files/documents/hid1_11.pdf).
There are
[more related documents](https://usb.org/documents?search=HID&items_per_page=50)
around there.
And also the
[USB 2.0 specification](https://www.usb.org/sites/default/files/usb_20_20190524.zip).
