/*
  * The USB-Userport *
  Copyright 2020  René Trapp (rene [dot] trapp (-at-) web [dot] de)

  * LUFA Library *
  Copyright 2017  Dean Camera (dean [at] fourwalledcubicle [dot] com)
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
 *  in Makefile-Optionen versteckt.
 *
 *  Im Kapitel \ref Sec_Options der USB-Userport Dokumentation
 *  finden sich Informationen welche Token angepasst werden können.
 */


#ifndef _APP_CONFIG_H_
#define _APP_CONFIG_H_


   #define DEVICE_SERIAL_NUMBER           L"01"
   /**<
    * \~English
    *  With the USB-Userport project the serial number is (ab)used
    *  to address a certain device in case there are more than one
    *  attached to the computer. If you have such usage scenarios
    *  consider to compile the whole project as often as needed and
    *  readjust this string for each compile run.
    *
    *  The USB-Userport just uses two digits, '0' to '9' each.
    *  "00" is an excluded combination. The default value is "01".
    *  It is possible to use 99 different "addresses".
    *
    *  Since the serial number literally is a string you are not
    *  restricted to just digits '0' to '9'. Everything is possible,
    *  even chinese runes have been seen out there in the wild. And
    *  as the thing drops to the host just as data <i>bytes</i> you
    *  are free to do what you see fit. But be aware, there are claims
    *  about windows to not enumerate the device if digits are others
    *  than '0'..'9' or 'A' to 'F'.
    *
    * \~German
    *  Beim USB-Userport wird die Seriennummer zweckentfremdet um im
    *  Bedarfsfall mehrere gleichzeitig angeschlossene USB-Userports
    *  gezielt ansprechen zu können. Für einen solchen Anwendungsfall
    *  wird das Projekt mehrfach kompiliert, jeweils mit individuell
    *  eingestellter Seriennummer.
    *
    *  Der USB-Userport verwendet zwei Zeichen, jeweils von '0' bis
    *  '9'. Die Kombination "00" ist nicht erlaubt. Der Startwert
    *  ist "01" und es ergeben sich insgesamt 99 verschiedene
    *  "Adressen".
    *
    *  Weil die Seriennummer buchstäblich ein String ist, der aus
    *  <i>Bytes</i> besteht, ist die Verwendung nicht auf die Ziffern
    *  '0' bis '9' beschränkt. Alles ist möglich, sogar chinesische
    *  Schriftzeichen wurden schon gesichtet. Aber aufgepasst: Es
    *  gibt Beanstandungen, dass unter Windows nur die Zeichen '0'
    *  bis '9' und 'A' bis 'F' akzeptiert würden und bei Verstößen
    *  das Gerät nicht enumeriert würde.
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
    *  stellt die maximal auftretende Stromaufnahme des Gerätes ein.
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

   #define RELEASE_REVISION               3
   /**<
    * \~English defines the device revision.
    * \~German  gibt den Überarbeitungsstand des Produktes an.
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
      REPORT_ID_SERVO_PWM     = 0x20, /**< \~English Servo control \~German Servoansteuerung */
      REPORT_ID_MEM_ADR       = 0xF8, /**< \~English Set address for direct memory access
                                           \~German  Adresse für direkten Speicherzugriff setzen */
      REPORT_ID_MEM_ACCESS    = 0xF9, /**< \~English Memory access \~German Speicherzugriff */
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

   #define GENERIC_REPORT_SIZE            8  // Byte
   /**<
    * \~English
    *  defines the maximum byte size of report payload.
    *  @note ReportIDs are excluded.
    * \~German
    *  definiert die Byteanzahl der Nutzdaten im längsten Report.
    *  @note ReportIDs werden nicht mitgezählt.
    */

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

// ---------------------------------------------------------------

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

   #define REPORT_SIZE_MEM_ADR            2
   /**<
    * \~English
    *  defines the size of USB-OUT reports setting the memory
    *  address.
    * \~German
    *  gibt die Länge des USB-OUT-Reports an, mit dem die
    *  Speicheradresse vorgegeben wird.
    */

   #define REPORT_SIZE_MEM_ACCESS         1
   /**<
    * \~English
    *  defines the size of USB-OUT reports controlling the direct
    *  memory access.
    * \~German
    *  gibt die Länge des USB-OUT-Reports an, mit dem das Datenbyte
    *  vom / zum Speicher übertragen wird.
    */

// ---------------------------------------------------------------

   #define REPORT_SIZE_SERVO              2
   /**<
    * \~English
    *  defines the size of USB-IN and -OUT reports controlling the
    *  servo PWM generator. Given as count of bytes.
    * \~German
    *  gibt die Länge der USB-Reports für die Servo-Kontrolle in
    *  Bytes an.
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
