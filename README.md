# USB Userport

DIY electronic projects can interface to the modern OS again using
the generic HID class driver. Now we are getting back to the old
days of virtually unlimited direct HW access to homebrew
electronics. Ahead the additional microcontroller in the interface
can enable things the host is not capable at all.


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

There is no UART emulation intended, there are enough other
sources of USB-to-UART adapters around.


## Hardware

The hardware is quite affordable using a Sparkfun MicroPro (clone).
This means an ATmega32U4 reigns the USB-Userport offering its
I/O-facilities to the user. Other microcontrollers can be used,
the code has to be adapted, of course.


## Software

No admin privileges are necessary. Just any user account should
work. The generic HID class is supported by recent OS (Linux,
Windows, ...). But you need to learn how to handle the generic HID
driver API.

With the knowledge how to use the generic HID driver the userport
can be controlled by virtually any program the user writes.


## Credits, links and further readings

The first credits go to Dean Camera for his really fine
[LUFA](http://www.fourwalledcubicle.com/LUFA.php)
project. See there for lots of examples on LUFA usage and How-Tos.
In fact when I came across this project, I started thinking about
the new possibilites. Immediately I wanted to use it as the
foundation for this particular project.

Next comes the VBA code from
[Jan Axelson's](http://janaxelson.com/hidpage.htm)
[Usbhidio2](http://janaxelson.com/files/usbhidio2.zip)
application. I reworked this into an Excel macro for a first
way to connect to the USB-Userport.

For those willing to learn a little bit more on the USB itself,
the ultimate specifications are found at the [USB.org](usb.org)
site.

And, of course, the datasheet of the
[ATmega32U4](http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7766-8-bit-AVR-ATmega16U4-32U4_Datasheet.pdf)
is essential to understand the properties offered.
