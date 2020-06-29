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
 *  \~English
 *   @brief Application Configuration Header File
 *
 *  This is a header file which is used to configure some of
 *  the application's compile time options, as an alternative to
 *  specifying the compile time constants supplied through a
 *  makefile or build system.
 *
 *  For information on which tokens can be adapted, refer to the
 *  \ref Sec_Options section of the USB-Userport documentation.
 *
 *  \~German
 *   @brief Applikationseinstellungen (Header File)
 *
 *  In dieser Datei sind zahlreiche Konfigurationseinstellungen
 *  zentral zusammengefasst. Dadurch sind diese Konstanten nicht
 *  in Compiler-Optionen versteckt.
 *
 *  Im Kapitel \ref Sec_Options der USB-Userport Dokumentation
 *  finden sich Informationen welche Token angepasst werden können.
 */


#ifndef _APP_CONFIG_H_
#define _APP_CONFIG_H_


   #define RELEASE_MAJOR                  0
   /**<
    * \~English defines the MAJOR device release level.
    * \~German  gibt die MAJOR Freigabestufe des Produktes an.
    */

   #define RELEASE_MINOR                  0
   /**<
    * \~English defines the MINOR device release level.
    * \~German  gibt die MINOR Freigabestufe des Produktes an.
    */

   #define RELEASE_REVISION               2
   /**<
    * \~English defines the device revision.
    * \~German  gibt den Überarbeitungsstand des Produktes an.
    */


   #define GENERIC_REPORT_SIZE            8  // Byte
   /**<
    * \~English
    *  defines the maximum byte size of report payload.
    *  @note ReportIDs are excluded.
    * \~German
    *  definiert die Byteanzahl der Nutzdaten im längsten Report.
    *  @note ReportIDs werden nicht mitgezählt.
    */


   #define MAX_CURRENT_DRAW             100  // Milliampères
   /**<
    * \~English
    *  defines the topmost current draw of the device. The host
    *  monitors this limit and turns off the USB power supply to
    *  the device if exceeded. The value shall be given as
    *  milliamps in the range from 50 to 500. The default ist 100.
    *  @note The exact value depends on board usage and external
    *  circuitry connected. As a guess the ProMicro with all its
    *  3 LEDs lit will draw 50 mA. The remainder is for your extra
    *  circuitry.
    * \~German
    *  stellt die maximal gewünschte Stromaufnahme des Gerätes ein.
    *  Der Host überwacht diesen Grenzwert und schaltet bei dessen
    *  Überschreitung den USB-Anschluss aus. Der Wert ist im
    *  Bereich 50 bis 500 anzugeben. Die Einheit ist Milliampère.
    *  Der Vorgabewert ist 100.
    *  @note Der genaue Wert hängt vom Anwendungsfall und der
    *  angeschlossenen Schaltung ab. Als Richtwert sind für den
    *  ProMicro 50 mA abgeschätzt, wenn alle seine 3 LEDs leuchten.
    *  Der Rest steht für die angeschlossene Elektronik zur
    *  Verfügung.
    */


   #define POLLING_INTERVAL               5  // Milliseconds for low-/full-speed devices
   /**<
    * \~English
    *  defines the host polling interval. This is how often the
    *  host shall check the asynchronous event queue (Interrupt
    *  In pipe) for updates.
    *  The value shall be given as milliseconds from 1 to 255 for
    *  low- or full-speed devices. The default is 5.
    *  @note Carefully check your OS documentation, e. g.
    *  https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/usbspec/ns-usbspec-_usb_endpoint_descriptor
    *  or you might experience some unwelcome suprises by sloppy
    *  USB-host implementations.
    * \~German
    *  gibt dem Host das Abfrageintervall für die Interrupt In Pipe
    *  vor. Der Wert ist in Millisekunden anzugeben, der erlaubte
    *  Bereich ist 1 bis 255. Der Vorgabewert ist 5.
    *  @note Die Dokumentation des jeweiligen Betriebssystems ist
    *  sorgfältig zu prüfen, sonst könnte es sehr unliebsame
    *  Überraschungen mit 'eigenwilligen' USB-Hosts geben.
    *  Zum Beispiel:
    *  https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/usbspec/ns-usbspec-_usb_endpoint_descriptor
    */

// ---------------------------------------------------------------

   /**
    * \~English
    *  lists the defined USB-IN- and USB-OUT-report IDs.
    *  Each ID serves a certain purpose.
    * \~German
    *  zählt die verwendeten ReportIDs der USB-IN- und USB-OUT-
    *  Reports auf. Jeder ID ist eine bestimmte Verwendung
    *  zugeordnet.
    */
   enum USB_IO_ReportIDs_t
   {
      REPORT_ID_DEVICE_LEDS   = 0x01, /**< \~English On-board LEDs \~German LEDs auf der Platine */
      REPORT_ID_LINES_GPIO1   = 0x02, /**< GPIO1 \~English lines \~German Leitungen */
      REPORT_ID_DIRS_GPIO1    = 0x03, /**< GPIO1 \~English directions \~German Richtungen */
      REPORT_ID_LINES_GPIO2   = 0x04, /**< GPIO2 \~English lines \~German Leitungen */
      REPORT_ID_DIRS_GPIO2    = 0x05, /**< GPIO2 \~English directions \~German Richtungen */
//    REPORT_ID_LINES_GPIO3   = 0x06, /**< GPIO3 \~English lines \~German Leitungen */
//    REPORT_ID_DIRS_GPIO3    = 0x07, /**< GPIO3 \~English directions \~German Richtungen */
//    REPORT_ID_LINES_GPIO4   = 0x08, /**< GPIO4 \~English lines \~German Leitungen */
//    REPORT_ID_DIRS_GPIO4    = 0x09, /**< GPIO4 \~English directions \~German Richtungen */
      REPORT_ID_IRQEN_GPIO    = 0x0E, /**< \~English Control of asynchronous data to the host
                                           \~German  Steuerung der asynchronen Daten zum Host */
      REPORT_ID_IRQ_GPIO      = 0x0F, /**< \~English GPIO asynchronous data to the host (INTERRUPT IN)
                                           \~German  Asynchrone Daten vom GPIO zum Host */
      REPORT_ID_SET_ADC       = 0x10, /**< \~English Control of ADC \~German Steuerung des ADC */
//    REPORT_ID_GET_ADC0      = 0x10, /**< \~English ADC channel 0 \~German ADC-Kanal 0 */
//    REPORT_ID_GET_ADC1      = 0x11, /**< \~English ADC channel 1 \~German ADC-Kanal 1 */
//    REPORT_ID_GET_ADC2      = 0x12, /**< \~English ADC channel 2 \~German ADC-Kanal 2 */
//    REPORT_ID_GET_ADC3      = 0x13, /**< \~English ADC channel 3 \~German ADC-Kanal 3 */
      REPORT_ID_GET_ADC4      = 0x14, /**< \~English ADC channel 4 \~German ADC-Kanal 4 */
      REPORT_ID_GET_ADC5      = 0x15, /**< \~English ADC channel 5 \~German ADC-Kanal 5 */
      REPORT_ID_GET_ADC6      = 0x16, /**< \~English ADC channel 6 \~German ADC-Kanal 6 */
      REPORT_ID_GET_ADC7      = 0x17, /**< \~English ADC channel 7 \~German ADC-Kanal 7 */
   };

   /**
    * \~English
    *  lists the defined USB-FEATURE report IDs.
    *  Each ID serves a certain purpose.
    * \~German
    *  zählt die verwendeten ReportIDs der USB-FEATURE Reports auf.
    *  Jeder ID ist eine bestimmte Verwendung zugeordnet.
    */
   enum USB_Feature_ReportIDs_t
   {
      FEATURE_ID_REFLASH      =   42, /**< \~English Go to bootloader \~German Starte den Bootlader */
   };

// ---------------------------------------------------------------

   #define REPORT_SIZE_DEVICE_LEDS        1
   /**<
    * \~English defines the size of USB-IN and -OUT reports handling on-board LEDs. Given as count of bytes.
    * \~German  gibt die Länge beider LED-Reports in Bytes an.
    */

   #define RXLED_POS                      0
   /**<
    * \~English defines the LED bit position in the corresponding report data.
    * \~German  definiert die Bitposition der LED im zugehörigen Report.
    */

   #define TXLED_POS                      1
   /**<
    * \~English defines the LED bit position in the corresponding report data.
    * \~German  definiert die Bitposition der LED im zugehörigen Report.
    */

// ---------------------------------------------------------------

   #define REPORT_SIZE_SET_GPIO           4
   /**<
    * \~English
    *  defines the size of USB-OUT reports handling GPIO lines.
    *  Given as count of bytes.
    * \~German
    *  gibt die Länge des USB-OUT-Reports zum Einstellen der
    *  GPIO-Leitungen in Bytes an.
    */
   
   #define REPORT_SIZE_GET_GPIO           2
   /**<
    * \~English
    *  defines the size of USB-IN reports handling GPIO lines.
    *  Given as count of bytes.
    * \~German
    *  gibt die Länge des USB-IN-Reports zum Lesen der
    *  GPIO-Leitungen in Bytes an.
    */

   #define REPORT_SIZE_IRQEN_GPIO         4
   /**<
    * \~English
    *  defines the size of USB-IN and -OUT reports controlling the
    *  generation of asynchronous data to the host (INTERRUPT IN).
    *  Given as count of bytes.
    * \~German
    *  gibt die Länge der USB-IN und USB-OUT Reports zum
    *  Konfigurieren der USB-INTERRUPT-IN Reports für GPIO-Leitungen
    *  in Bytes an.
    */

   #define REPORT_SIZE_IRQ_GPIO           8
   /**<
    * \~English
    *  defines the size of USB-IN reports handling asynchronous
    *  data to the host (INTERRUPT IN). Given as count of bytes.
    * \~German
    *  gibt die Länge der USB-INTERRUPT-IN Reports für
    *  GPIO-Leitungen in Bytes an.
    */


   #define REPORT_SIZE_SET_ADC            2
   /**<
    * \~English
    *  defines the size of USB-OUT reports controlling the ADC
    *  operation.
    * \~German
    *  gibt die Länge des USB-OUT-Reports an, mit dem der ADC
    *  gesteuert wird.
    */

   #define REPORT_SIZE_GET_ADC            2
   /**<
    * \~English
    *  defines the size of USB-IN reports returning an analog
    *  value. Given as count of bytes.
    * \~German
    *  gibt die Länge des USB-IN-Reports zum Lesen eines analogen
    *  Wertes in Bytes an.
    */

// ---------------------------------------------------------------

   #define FEATURE_SIZE_REFLASH           7
   /**<
    * \~English
    *  defines the size of the USB-FEATURE report performing a
    *  device reset. Given as count of bytes.
    * \~German
    *  gibt die Länge des USB-FEATURE Reports für den Reset in
    *  Bytes an.
    */

// ---------------------------------------------------------------

#endif
