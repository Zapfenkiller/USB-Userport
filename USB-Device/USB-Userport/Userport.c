/*
  * The USB-Userport *
  Copyright 2020  René Trapp (rene [dot] trapp (-at-) web [dot] de)
  License: "The MIT license" - see license.txt

  * LUFA Library *
  Copyright 2017  Dean Camera (dean [at] fourwalledcubicle [dot] com)
  License: "The MIT license" - see license.txt
*/


/** @file
 *  \~English
 *   @brief USB-Userport main file.
 *
 *  Main source file for the USB-Userport. This file contains the
 *  main tasks and the control logic.
 *
 *  \~German
 *   @brief Zentrale des USB-Userport.
 *
 *  Der Quellcode für den Kern des USB-Userport. Die Datei enthält
 *  die grundsätzlichen Funktionen und die Steuerlogik.
 */


#include "Userport.h"
// #include <avr/io.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>
// #include <string.h>

#include "Descriptors.h"
#include "Board/ADC.h"
#include "Board/GPIOs.h"
#include "Board/LEDs.h"
#include "Board/IOREGs.h"
#include "Board/ft.h"
#include "Config/AppConfig.h"


USB_ClassInfo_HID_Device_t Generic_HID_Interface =
   {
      .Config =
         {
            .InterfaceNumber              = INTERFACE_ID_GenericHID,
            .ReportINEndpoint             =
               {
                  .Address                = GENERIC_IN_EPADDR,
                  .Size                   = GENERIC_EPSIZE,
                  .Banks                  = 1,
               },
            .PrevReportINBuffer           = NULL,
            .PrevReportINBufferSize       = GENERIC_REPORT_SIZE,
         }
   };
/**<
 * \~English
 *  LUFA HID Class driver interface configuration and state
 *  information. This structure is passed to all HID Class driver
 *  functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 *
 * \~German
 *  LUFA HID Class Treiber Schnittstellenkonfiguration und Status
 *  information. Diese Datenstruktur geht an alle HID Class
 *  Treiberfunktionen um mehrfache Instanzen der gleichen Klasse
 *  in einem Gerät zu unterscheiden.
 */


static uint16_t IRQmaskGPIO1;    /**< \~English contains the change monitor control bitmask for GPIO1. \~German enthält die Maske für den Pin-Change-IRQ von GPIO1. */
static uint16_t IRQmaskGPIO2;    /**< \~English contains the change monitor control bitmask for GPIO2. \~German enthält die Maske für den Pin-Change-IRQ von GPIO2. */
static uint16_t previousGPIO1;   /**< \~English tracks the previous line state for change detection on GPIO1. \~German speichert das vorherige Bitmuster von GPIO1. */
static uint16_t previousGPIO2;   /**< \~English tracks the previous line state for change detection on GPIO2. \~German speichert das vorherige Bitmuster von GPIO2. */
uint8_t ftGuiConWD;


void SetupHardware(void)
{
#if (ARCH == ARCH_AVR8)
   /* Disable watchdog if enabled by bootloader/fuses */
   MCUSR &= ~(1 << WDRF);
   wdt_disable();

   /* Disable clock division */
   clock_prescale_set(clock_div_1);
#elif (ARCH == ARCH_XMEGA)
   /* Start the PLL to multiply the 2MHz RC oscillator to 32MHz and switch the CPU core to run from it */
   XMEGACLK_StartPLL(CLOCK_SRC_INT_RC2MHZ, 2000000, F_CPU);
   XMEGACLK_SetCPUClockSource(CLOCK_SRC_PLL);

   /* Start the 32MHz internal RC oscillator and start the DFLL to increase it to 48MHz using the USB SOF as a reference */
   XMEGACLK_StartInternalOscillator(CLOCK_SRC_INT_RC32MHZ);
   XMEGACLK_StartDFLL(CLOCK_SRC_INT_RC32MHZ, DFLL_REF_INT_USBSOF, F_USB);

   PMIC.CTRL = PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm;
#endif

   GPIO_Init();
   previousGPIO1 = GPIO1_GetLines();
   previousGPIO2 = GPIO2_GetLines();
   IRQmaskGPIO1 = 0;
   IRQmaskGPIO2 = 0;

   OCR1A = 375;   // Default servo pulse length =  1500 us
   OCR1B = 375;   // Default servo pulse length =  1500 us

   LEDs_Init();

   // Prepare Timer3 for 5 ms periods, :8, no outputs just OVF-IRQ
   OCR3A = F_CPU / 8 * ft66843_REPETITION_s - 1;
   TCCR3A = 0;
   TCCR3B = (0b01 << WGM32) | (0b010 << CS30);
   TIMSK3 = (1 << OCIE3A);

   USB_Init();
}
/**<
 * \~English Initializes the USB-Userport.
 * \~German  Initialisiert den USB-Userport.
 */


void EVENT_USB_Device_Connect(void)
{
}
/**<
 * \~English Event handler for the library USB Connection event.
 *  Currently this is empty.
 *
 * \~German  Ereignisverarbeitung für "mit USB verbinden".
 *  Gegenwärtig passiert hier nichts.
 */


void EVENT_USB_Device_Disconnect(void)
{
   LEDs_TurnOff(RXLED | TXLED);
   ft66843_Leave();
}
/**<
 * \~English Event handler for the library USB Disconnection event.
 *  Turn off indicator LEDs.
 *  Turn off fischertechnik interface mode, just to be sure.
 *  All actors should stop now.
 *
 * \~German  Ereignisverarbeitung für "vom USB trennen".
 *  Status LEDs ausschalten.
 *  Um sicher zu gehen wird der fischertechnik Interface Modus ausgeschaltet.
 *  Alle Aktoren sollten dadurch stromlos werden.
 */


void EVENT_USB_Device_ConfigurationChanged(void)
{
   bool ConfigSuccess = true;

   ConfigSuccess &= HID_Device_ConfigureEndpoints(&Generic_HID_Interface);

   USB_Device_EnableSOFEvents();
}
/**<
 * \~English Event handler for the library USB Configuration Changed
 *  event.
 *  Unchanged with respect to the LUFA generic HID demo.
 *
 * \~German  Ereignisverarbeitung für "Gerätekonfiguration ändern".
 *  Unverändert vom LUFA generic HID Demo übernommen.
 */


void EVENT_USB_Device_ControlRequest(void)
{
   HID_Device_ProcessControlRequest(&Generic_HID_Interface);
}
/**<
 * \~English Event handler for the library USB Control Request
 *  reception event.
 *  Unchanged with respect to the LUFA generic HID demo.
 *
 * \~German  Ereignisverarbeitung für "USB Control Request" in der
 *  LUFA-Bibliothek.
 *  Unverändert vom LUFA generic HID Demo übernommen.
 */


void EVENT_USB_Device_StartOfFrame(void)
{
   HID_Device_MillisecondElapsed(&Generic_HID_Interface);
}
/**<
 * \~English Event handler for the USB device Start Of Frame event.
 *  Unchanged with respect to the LUFA generic HID demo.
 *
 * \~German  Ereignisverarbeitung für "USB Start Of Frame".
 *  Unverändert vom LUFA generic HID Demo übernommen.
 */


bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                         uint8_t* const ReportID,
                                         const uint8_t ReportType,
                                         void* ReportData,
                                         uint16_t* const ReportSize)
{

   uint8_t *Data = (uint8_t*)ReportData;
   uint16_t temp;

   if (ReportType == HID_REPORT_ITEM_In)
   {
      switch (*ReportID)
      {
         case 0x00: ; // Cyclic host check if there is something new.
                      // This particular data is using Interrupt In pipe,
                      // All other reports answer through the control EP.
            uint16_t readback1 = GPIO1_GetLines();
            uint16_t changed1 = (readback1 ^ previousGPIO1) & IRQmaskGPIO1;
            previousGPIO1 = readback1;
            uint16_t readback2 = GPIO2_GetLines();
            uint16_t changed2 = (readback2 ^ previousGPIO2) & IRQmaskGPIO2;
            previousGPIO2 = readback2;
            if (changed1 || changed2)
            {
               Data[0] = readback1;
               Data[1] = readback1 >> 8;
               Data[2] = changed1;
               Data[3] = changed1 >> 8;
               Data[4] = readback2;
               Data[5] = readback2 >> 8;
               Data[6] = changed2;
               Data[7] = changed2 >> 8;
               *ReportID = REPORT_ID_IRQ_GPIO;
               *ReportSize = REPORT_SIZE_IRQ_GPIO;
               return true;
            }
            break;
         case REPORT_ID_DEVICE_LEDS: ;
            uint8_t  CurrLEDMask = LEDs_GetAll();
            Data[0] = ((CurrLEDMask & RXLED) ? (1 << RXLED_POS) : 0) \
                    | ((CurrLEDMask & TXLED) ? (1 << TXLED_POS) : 0);
            *ReportSize = REPORT_SIZE_DEVICE_LEDS;
            return true;
            break;
         case REPORT_ID_LINES_GPIO1: ;
            temp = GPIO1_GetLines();
            Data[0] = temp;
            Data[1] = temp >> 8;
            *ReportSize = REPORT_SIZE_GET_GPIO;
            return true;
            break;
         case REPORT_ID_DIRS_GPIO1: ;
            temp = GPIO1_GetDirections();
            Data[0] = temp;
            Data[1] = temp >> 8;
            *ReportSize = REPORT_SIZE_GET_GPIO;
            return true;
            break;
         case REPORT_ID_LINES_GPIO2: ;
            temp = GPIO2_GetLines();
            Data[0] = temp;
            Data[1] = temp >> 8;
            *ReportSize = REPORT_SIZE_GET_GPIO;
            return true;
            break;
         case REPORT_ID_DIRS_GPIO2: ;
            temp = GPIO2_GetDirections();
            Data[0] = temp;
            Data[1] = temp >> 8;
            *ReportSize = REPORT_SIZE_GET_GPIO;
            return true;
            break;
         case REPORT_ID_IRQEN_GPIO: ;
            Data[0] = IRQmaskGPIO1;
            Data[1] = IRQmaskGPIO1 >> 8;
            Data[2] = IRQmaskGPIO2;
            Data[3] = IRQmaskGPIO2 >> 8;
            *ReportSize = REPORT_SIZE_IRQEN_GPIO;
            return true;
            break;
         case REPORT_ID_GET_ADC4: ;
            ADC_Get_Sample(ADC_Chan_4, Data);
            *ReportSize = REPORT_SIZE_GET_ADC;
            return true;
            break;
         case REPORT_ID_GET_ADC5: ;
            ADC_Get_Sample(ADC_Chan_5, Data);
            *ReportSize = REPORT_SIZE_GET_ADC;
            return true;
            break;
         case REPORT_ID_GET_ADC6: ;
            ADC_Get_Sample(ADC_Chan_6, Data);
            *ReportSize = REPORT_SIZE_GET_ADC;
            return true;
            break;
         case REPORT_ID_GET_ADC7: ;
            ADC_Get_Sample(ADC_Chan_7, Data);
            *ReportSize = REPORT_SIZE_GET_ADC;
            return true;
            break;
         case REPORT_ID_SERVO_PWM: ;
            Data[0] = (uint8_t)(OCR1A - 250);
            Data[1] = (uint8_t)(OCR1B - 250);
            *ReportSize = REPORT_SIZE_SERVO;
            return true;
            break;
         case REPORT_ID_MEM_ACCESS: ;
            Data[0] = io_read((uint16_t *)memAddress);
            *ReportSize = REPORT_SIZE_MEM_ACCESS;
            return true;
            break;
         case REPORT_ID_ftINP: ;
            if (ft66843_IsEnabled())
            {
               Data[6] = ft66843_PotYGet();
               Data[5] = ft66843_PotXGet();
               Data[4] = ft66843_InputsGetSlave();
               Data[3] = ft66843_InputsGetMaster();
               Data[2] = ft66843_OutputsGetSlave();
               Data[1] = ft66843_OutputsGetMaster();
               Data[0] = ft66843_HasChanges();
               Data[0] |= 0x80;
               ftGuiConWD = ft66843_CON_TIMEOUT_s / ft66843_REPETITION_s;
            }
            else
               Data[0] = 0;
            *ReportSize = REPORT_SIZE_ftINP;
            return true;
            break;
      }
   }
   else // According to the docu it's just the HID_REPORT_ITEM_Feature left
   {
      switch(*ReportID)
      {
//       case FEATURE_ID_ft66843: ;
//          Data[0] = ft66843_IsEnabled() ? 0x01 : 0x00;
//          *ReportSize = FEATURE_SIZE_ft66843;
//          return true;
//          break;
      }
   }
   return false;
}
/**<
 * \~English
 *  HID class driver callback function for the creation of HID
 *  reports to the host. This is where the USB-IN-reports (device
 *  to host) are created. With LUFA's HID class implementation
 *  this is also where Feature-IN-reports (device to host) get
 *  created. Depending on @c ReportType and @c ReportID the
 *  respective answer is prepared. A special consideration need
 *  the INTERRUPT-IN reports, sending the asynchronous data to the
 *  host and getting build here in addition. In this case the
 *  requested Report-ID is '0'. Well, this is special and needs
 *  more thoughts. Since the USB-host actively samples each device
 *  for such asynchronous data it is not really asynchronous to
 *  the bus. And since any INTERRUPT-IN data must wait until the
 *  host asks for it, it is also not really interrupting.
 *  The best way to think of it is a kind of universal request for
 *  unspecific data the device decides to send (or not). The host
 *  receives this and forwards it to the application. From the
 *  application view this report is not requested and interrupts
 *  the application processing.
 *
 *  @note  There is some timeout limitation and any reaction shall
 *  get returned within this limit. Otherwise the host will detect
 *  a device issue.
 *
 *  @param[in]     HIDInterfaceInfo  Pointer to the HID class interface configuration structure being referenced.
 *  @param[in,out] ReportID    Report ID requested by the host if non-zero, otherwise its the INTERRUPT IN poll.
 *  @param[in]     ReportType  Type of the report to create, either @c HID_REPORT_ITEM_In or @c ID_REPORT_ITEM_Feature.
 *  @param[out]    ReportData  Pointer to a buffer where the created report should be stored.
 *  @param[out]    ReportSize  Number of bytes written in the report (or zero if no report is to be sent).
 *
 *  @return Boolean @c true to force the sending of the report,
 *  @c false to not send it. Library automatics are overriden due
 *  to ReportID usage.
 *
 * \~German
 *  Funktion die vom LUFA HID-Klassentreiber aufgerufen wird, wenn
 *  der Host einen USB-IN-Report anfordert. Der übermittelte Report
 *  wird, abhängig von @c ReportType und @c ReportID,
 *  zusammengestellt.
 *  Die Funktion wird auch aufgerufen wenn der Host einen USB-
 *  Feature-Report anfordert.
 *  Eine spezielle Überlegung gilt den INTERRUPT-IN Reports. Die
 *  werden hier ebenfalls zusammengebaut und enthalten asynchrone
 *  Daten. In dem Fall ist die angeforderte Report-ID '0'. Nun, das
 *  ist so etwas seltsam weil der Host ganz gezielt nach dem Report
 *  fragt. Für den Bus ist es also nicht asynchron. Und weil der
 *  Versand des Reports warten muss bis der Host danach fragt, ist
 *  es auch kein Interrupt im eigentlichen Sinn. Am besten stellt
 *  man sich das als universelle Anfrage nach unspezifischen Daten
 *  vor, die das Gerät jetzt übermitteln darf - aber nicht muss.
 *  Der Host empfängt den Report und leitet die Daten an ein
 *  Programm weiter. Aus Sicht dieses Programms kommen die Daten
 *  asynchron herein und unterbrechen den normalen Programmablauf.
 *
 *  @note  Die Reaktion muss innerhalb einer bestimmten Zeit
 *  erfolgen. Anderenfalls nimmt der Host an, dass das Gerät
 *  fehlerhaft arbeitet.
 *
 *  @param[in] HIDInterfaceInfo  zeigt auf die angewählte @c HID @c Class @c Treiber @c Schnittstellenkonfiguration @c und @c Status Struktur.
 *  @param[in] ReportID    übergibt die vom Host angeforderte Report ID. '0' wenn es sich um die INTERRUPT-IN Anforderung handelt.
 *  @param[in] ReportType  ist der Typ des Reports, entweder @c HID_REPORT_ITEM_In oder @c HID_REPORT_ITEM_Feature.
 *  @param[in] ReportData  zeigt auf den Puffer in dem die Datenbytes abzulegen sind.
 *  @param[in] ReportSize  definiert die Reportlänge in Bytes (bzw. '0' wenn kein Report gesendet werden soll.
 *
 *  @return Boolean @c true wenn ein Report gesendet werden soll,
 *  @c false wenn kein Report zurück geht. Die LUFA-Automatik ist
 *  wegen der ReportIDs nicht nutzbar.
 */


void CALLBACK_HID_Device_ProcessHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                          const uint8_t ReportID,
                                          const uint8_t ReportType,
                                          const void* ReportData,
                                          const uint16_t ReportSize)
{
   uint8_t* Data = (uint8_t*)ReportData;

   if (ReportType == HID_REPORT_ITEM_Out)
   {
      switch(ReportID)
      {
         case REPORT_ID_DEVICE_LEDS: ;
            uint8_t  NewLEDMask = LEDS_NO_LEDS;
            NewLEDMask |= ((Data[0] & (1 << RXLED_POS)) ? RXLED : 0); // Map LED state to report bit
            NewLEDMask |= ((Data[0] & (1 << TXLED_POS)) ? TXLED : 0); // Map LED state to report bit
            LEDs_SetAll(NewLEDMask);
            break;
         case REPORT_ID_LINES_GPIO1: ;
            GPIO1_ChangeLines(Data[0] | (Data[1] << 8), Data[2] | (Data[3] << 8));
            break;
         case REPORT_ID_DIRS_GPIO1: ;
            GPIO1_ChangeDirections(Data[0] | (Data[1] << 8), Data[2] | (Data[3] << 8));
            break;
         case REPORT_ID_LINES_GPIO2: ;
            GPIO2_ChangeLines(Data[0] | (Data[1] << 8), Data[2] | (Data[3] << 8));
            break;
         case REPORT_ID_DIRS_GPIO2: ;
            GPIO2_ChangeDirections(Data[0] | (Data[1] << 8), Data[2] | (Data[3] << 8));
            break;
         case REPORT_ID_IRQEN_GPIO: ;
            IRQmaskGPIO1 = Data[0] | (Data[1] << 8);
            IRQmaskGPIO2 = Data[2] | (Data[3] << 8);
            break;
         case REPORT_ID_SET_ADC: ;
            ADC_Configure(Data[0], Data[1]);
            break;
         case REPORT_ID_SERVO_PWM: ;
            if (Data[0] < 251)
               OCR1A = (uint16_t) Data[0] + 250;
            else
               if (Data[0] == 253)
               {
                  GPIO1_ChangeLines(0x0000, 0x0020);
                  GPIO1_ChangeDirections(0x0060, 0x0020);
                  ICR1 = (5000 - 1); // Servo pulse repetition = 20000 us
                  TCCR1A |= (0b10 << COM1A0) | (0b10 << WGM10);
                  TCCR1B = (0b11 << WGM12) | (0b011 << CS10);
               }
            if (Data[1] < 251)
               OCR1B = (uint16_t) Data[1] + 250;
            else
               if (Data[1] == 253)
               {
                  GPIO1_ChangeLines(0x0000, 0x0040);
                  GPIO1_ChangeDirections(0x0060, 0x0040);
                  ICR1 = (5000 - 1); // Servo pulse repetition = 20000 us
                  TCCR1A |= (0b10 << COM1B0) | (0b10 << WGM10);
                  TCCR1B = (0b11 << WGM12) | (0b011 << CS10);
               }
               // HW takes 20 ms before 1st pulse gets out
            break;
         case REPORT_ID_MEM_ADR: ;
            memAddress = Data[0] | (Data[1] << 8);
            break;
         case REPORT_ID_MEM_ACCESS: ;
            io_write((uint16_t *)memAddress, Data[0]);
            break;
         case REPORT_ID_ftOUTP:
            if (ft66843_IsEnabled())
            {
               ftGuiConWD = ft66843_CON_TIMEOUT_s / ft66843_REPETITION_s;
               ft66843_OutputsSetSlave(Data[1]);
               ft66843_OutputsSetMaster(Data[0]);
            }
            break;
         case REPORT_ID_ftSTPCFG: ;
            if (ft66843_IsEnabled())
            {
               ftGuiConWD = ft66843_CON_TIMEOUT_s / ft66843_REPETITION_s;
               ft66843_StopEnableSet(Data[0], Data[2]);     // master, slave
               ft66843_StopInversionSet(Data[1], Data[3]);  // master, slave
            }
            break;
         case REPORT_ID_ftSTPRESPONSE: ;
            if (ft66843_IsEnabled())
            {
               ftGuiConWD = ft66843_CON_TIMEOUT_s / ft66843_REPETITION_s;
               ft66843_StopResponseFwdSet(Data[0], Data[2]);// master, slave
               ft66843_StopResponseBwdSet(Data[1], Data[3]);// master, slave
            }
            break;
      }
   }
   else // According to the docu it's just the HID_REPORT_ITEM_Feature left
   {
      switch(ReportID)
      {
         case FEATURE_ID_REFLASH: ; // 42 ;)
            // Stop device and jump to bootlader if the key matches!
            // "Reflash" = 0x52 0x65 0x66 0x6c 0x61 0x73 0x68
            // This indicates the expressed wish to start the bootloader!
            if ((Data[0] == 'R') &&
                (Data[1] == 'e') &&
                (Data[2] == 'f') &&
                (Data[3] == 'l') &&
                (Data[4] == 'a') &&
                (Data[5] == 's') &&
                (Data[6] == 'h'))
            {
               ft66843_Leave();
               // https://blog.fsck.com/2014/08/how-to-reboot-an-arduino-leonardo-micro-into-the-bootloader.html
               // Detach from the bus
               USB_Disable();
               // Disable all interrupts
               cli();
               // Stash the magic bootloader key
               uint16_t *const bootKeyPtr = (uint16_t *)0x0800;
               *bootKeyPtr = (uint16_t)0x7777;
               // Let the WDT do a full HW reset
               wdt_enable(WDTO_250MS);
               for (;;);
               break;
            }
         case FEATURE_ID_ft66843:
            if (Data[0] & 0x01)
            {
               if (!ft66843_IsEnabled())
                  ft66843_Init();
               if (ft66843_IsEnabled())
                  ftGuiConWD = ft66843_CON_TIMEOUT_s / ft66843_REPETITION_s;
            }
            else
            {
               ftGuiConWD = 0;
               ft66843_Leave();
            }
            break;
      }
   }
}
/**<
 * \~English
 *  HID class driver callback function for the processing of HID
 *  reports from the host. This is where the USB-OUT-reports
 *  (host to device) are processed, depending on @c ReportType and
 *  @c ReportID.
 *  LUFA's HID class implementation also ends up here when the
 *  host sends a Feature-OUT-report.
 *
 *  @param[in] HIDInterfaceInfo  Pointer to the HID class interface configuration structure being referenced.
 *  @param[in] ReportID    Report ID of the received report from the host.
 *  @param[in] ReportType  The type of report that the host has sent, either @c HID_REPORT_ITEM_Out or @c HID_REPORT_ITEM_Feature.
 *  @param[in] ReportData  Pointer to a buffer where the received report has been stored.
 *  @param[in] ReportSize  Size in bytes of the received HID report.
 *
 * \~German
 *  Funktion die vom LUFA HID-Klassentreiber aufgerufen wird, wenn
 *  der Host einen USB-OUT-Report gesendet hat. Der übermittelte
 *  Report wird, abhängig von @c ReportType und @c ReportID,
 *  verarbeitet.
 *  Die Funktion wird auch aufgerufen wenn der Host einen USB-
 *  Feature-Report gesendet hat.
 *
 *  @param[in] HIDInterfaceInfo  zeigt auf die angewählte @c HID @c Class @c Treiber @c Schnittstellenkonfiguration @c und @c Status Struktur.
 *  @param[in] ReportID    übergibt die Report ID vom Host.
 *  @param[in] ReportType  ist der Typ des Reports, entweder @c HID_REPORT_ITEM_Out oder @c HID_REPORT_ITEM_Feature.
 *  @param[in] ReportData  zeigt auf den Puffer in dem die Datenbytes zu finden sind.
 *  @param[in] ReportSize  definiert die Reportlänge in Bytes.
 */


int main(void)
{
   SetupHardware();
   ftGuiConWD = 0;
   GlobalInterruptEnable();

   for (;;)
   {
      HID_Device_USBTask(&Generic_HID_Interface);
      USB_USBTask();
   }
}
/**<
 * \~English
 *  Main program. First it initializes some hardware.
 *  The complete activity then is performed by an endless loop
 *  calling some tasks.
 *
 * \~German
 *  Das Hauptprogramm. Zuerst initialisiert es diverse Hardware.
 *  Die komplette Steuerung und Kontrolle des USB-USerport wird
 *  dann durch Aufruf diverser Unterprogramme in der Endlosschleife
 *  dargestellt.
 */


ISR(TIMER3_COMPA_vect, ISR_NOBLOCK)
{
   ft66843_CoreTask();
   if (ftGuiConWD > 0)
      ftGuiConWD -= 1;
   else
      ft66843_Leave();
}
/**<
 * \~English
 *  Interrupt service when timer3 overflows. Used as evenly timed
 *  real time performer for the ft Computing Interface mode. See
 *  timer3 set up for the grid used.
 *
 * \~German
 *  Interrupt Routine für Timer3 Überlauf. Wird für den Echtzeitkern
 *  des ft Computing-Interface-Modus verwendet. Siehe die Einstellungen
 *  des Timer3 für das wirksame Zeitraster.
 */
