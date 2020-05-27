/*
  * The USB-Userport *
  Copyright 2020  René Trapp (rene [dot] trapp (-at-) web [dot] de)

  * LUFA Library *
  Copyright 2017  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/


/** @file
 *  @brief Application Configuration Header File
 *
 *  This is a header file which is used to configure some of
 *  the application's compile time options, as an alternative to
 *  specifying the compile time constants supplied through a
 *  makefile or build system.
 *
 *  For information on what each token does, refer to the
 *  @ref Sec_Options section of the application documentation.
 */


#ifndef _APP_CONFIG_H_
#define _APP_CONFIG_H_


   #define RELEASE_MAJOR                  0
   /**< Defines the MAJOR device release level. */

   #define RELEASE_MINOR                  0
   /**< Defines the MINOR device release level. */

   #define RELEASE_REVISION               1
   /**< Defines the device revision. */


   #define GENERIC_REPORT_SIZE            8  // Byte
   /**< Defines the maximum byte size of report payload. ReportIDs
        are excluded. */


   #define MAX_CURRENT_DRAW             100  // Milliampères
   /**< Defines maximum power consumption. This is the topmost
        current draw of the device. The host monitors this limit
        and turns off the USB power supply to the device if exceeded.
        The value shall be given as milliamps in the range from
        50 to 500.
        @note The exact value depends on board usage and external
        circuitry connected. As a guess the µC with all its 3 LEDs
        lit will draw 50 mA. The remainder is for your extra
        circuitry. */


   #define POLLING_INTERVAL               5  // Milliseconds for low-/full-speed devices
   /**< Defines the host polling interval. This is how often the
        host shall check the asynchronous event queue (Interrupt
        In pipe) for updates.
        The value shall be given as milliseconds from 1 to 255 for
        low- or full-speed devices.
        @note Carefully check your OS documentation, e. g.
        https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/usbspec/ns-usbspec-_usb_endpoint_descriptor
        or you might experience some unwelcome suprises by sloppy
        USB-host implementations. */

// ---------------------------------------------------------------

   /** Lists the defined USB-IN- and USB-OUT-report IDs.
       Each ID serves a certain purpose. */
   enum USB_IO_ReportIDs_t
   {
      REPORT_ID_DEVICE_LEDS   = 0x01, /**< Defines the ID of USB-IN and -OUT reports handling on-board LEDs. */
      REPORT_ID_LINES_GPIO1   = 0x02, /**< Defines the ID of USB-IN and -OUT reports handling GPIO1 lines. */
      REPORT_ID_DIRS_GPIO1    = 0x03, /**< Defines the ID of USB-IN and -OUT reports handling GPIO1 directions. */
      REPORT_ID_LINES_GPIO2   = 0x04, /**< Defines the ID of USB-IN and -OUT reports handling GPIO2 lines. */
      REPORT_ID_DIRS_GPIO2    = 0x05, /**< Defines the ID of USB-IN and -OUT reports handling GPIO2 directions. */
//    REPORT_ID_LINES_GPIO3   = 0x06, /**< Defines the ID of USB-IN and -OUT reports handling GPIO3 lines. */
//    REPORT_ID_DIRS_GPIO3    = 0x07, /**< Defines the ID of USB-IN and -OUT reports handling GPIO3 directions. */
//    REPORT_ID_LINES_GPIO4   = 0x08, /**< Defines the ID of USB-IN and -OUT reports handling GPIO4 lines. */
//    REPORT_ID_DIRS_GPIO4    = 0x09, /**< Defines the ID of USB-IN and -OUT reports handling GPIO4 directions. */
      REPORT_ID_IRQEN_GPIO    = 0x0E, /**< Defines the ID of USB-IN and -OUT reports controlling the generation of asynchronous data to the host. */
      REPORT_ID_IRQ_GPIO      = 0x0F, /**< Defines the ID of USB-IN reports handling GPIO asynchronous data to the host (INTERRUPT IN). */
   };

   /** Lists the defined USB-FEATURE-report IDs.
       Each ID serves a certain purpose. */
   enum USB_Feature_ReportIDs_t
   {
      FEATURE_ID_REFLASH      =   42, /**< Defines the ID of the USB-FEATURE report performing a device reset. */
   };

// ---------------------------------------------------------------

   #define REPORT_SIZE_DEVICE_LEDS        1
   /**< Defines the ID of USB-IN and -OUT reports handling on-
     board LEDs. */

   #define RXLED_POS                      0
   /**< Defines the LED bit position in the corresponding report
        data. */

   #define TXLED_POS                      1
   /**< Defines the LED bit position in the corresponding report
        data. */

// ---------------------------------------------------------------

   #define REPORT_SIZE_SET_GPIO           4
   /**< Defines the SIZE of USB-OUT reports handling GPIO lines. */
   
   #define REPORT_SIZE_GET_GPIO           2
   /**< Defines the SIZE of USB-IN reports handling GPIO lines. */

   #define REPORT_SIZE_IRQEN_GPIO         4
   /**< Defines the SIZE of USB-IN and -OUT reports controlling the
        generation of asynchronous data to the host (INTERRUPT IN). */

   #define REPORT_SIZE_IRQ_GPIO           8
   /**< Defines the SIZE of USB-IN reports handling asynchronous
        data to the host (INTERRUPT IN). */

// ---------------------------------------------------------------

   #define FEATURE_SIZE_REFLASH           7
   /**< Defines the SIZE of the USB-FEATURE report performing a
        device reset. */

// ---------------------------------------------------------------

#endif
