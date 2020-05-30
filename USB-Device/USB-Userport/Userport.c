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
 *
 *  Main source file for the USB-Userport. This file contains the
 *  main tasks and the control logic.

 @todo
   Eine deutsche Version der Dokumentation hinzufügen.
 */


#include "Userport.h"


/** LUFA HID Class driver interface configuration and state information. This structure is
 *  passed to all HID Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
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
         },
   };


static uint16_t IRQmaskGPIO1;    /**< Contains the change monitor control bitmask for GPIO1. */
static uint16_t IRQmaskGPIO2;    /**< Contains the change monitor control bitmask for GPIO2. */
static uint16_t previousGPIO1;   /**< Tracks the previous line state for change detection on GPIO1. */
static uint16_t previousGPIO2;   /**< Tracks the previous line state for change detection on GPIO2. */


/** Main program.
 *  First it initializes some hardware.
 *  The complete activity then is performed by an endless loop
 *  calling some tasks. */
int main(void)
{
   SetupHardware();

   GlobalInterruptEnable();

   for (;;)
   {
      HID_Device_USBTask(&Generic_HID_Interface);
      USB_USBTask();
   }
}


/** Initializes the USB-Userport core. */
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

   LEDs_Init();

   USB_Init();
}


/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
}


/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
}


/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
   bool ConfigSuccess = true;

   ConfigSuccess &= HID_Device_ConfigureEndpoints(&Generic_HID_Interface);

   USB_Device_EnableSOFEvents();
}


/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
   HID_Device_ProcessControlRequest(&Generic_HID_Interface);
}


/** Event handler for the USB device Start Of Frame event. */
void EVENT_USB_Device_StartOfFrame(void)
{
   HID_Device_MillisecondElapsed(&Generic_HID_Interface);
}


/** HID class driver callback function for the creation of HID reports to the host.
 *  This is where the USB-IN-reports are created.
 *  Depending on the @c ReportID the respective answer is compiled.
 *
 *  @param[in]     HIDInterfaceInfo  Pointer to the HID class interface configuration structure being referenced.
 *  @param[in,out] ReportID    Report ID requested by the host if non-zero, otherwise its the INTERRUPT IN poll.
 *  @param[in]     ReportType  Type of the report to create, either @c HID_REPORT_ITEM_In or @c ID_REPORT_ITEM_Feature.
 *  @param[out]    ReportData  Pointer to a buffer where the created report should be stored.
 *  @param[out]    ReportSize  Number of bytes written in the report (or zero if no report is to be sent).
 *
 *  @return Boolean @c true to force the sending of the report, @c false to not send it. Library automatics are overriden.
 */
bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                         uint8_t* const ReportID,
                                         const uint8_t ReportType,
                                         void* ReportData,
                                         uint16_t* const ReportSize)
{

   uint8_t* Data = (uint8_t*)ReportData;
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
      }
   }
   else // According to the docu it's just the HID_REPORT_ITEM_Feature left
   {
      switch(*ReportID)
      {
         case FEATURE_ID_REFLASH: ; // 42 ;)
            *ReportSize = FEATURE_SIZE_REFLASH;
            return true;
            break;
      }
   }
   return false;
}


/** HID class driver callback function for the processing of HID reports from the host.
 *  This is where the USB-OUT-reports are created.
 *  Depending on the @c ReportID the respective answer is compiled.
 *
 *  @param[in] HIDInterfaceInfo  Pointer to the HID class interface configuration structure being referenced.
 *  @param[in] ReportID    Report ID of the received report from the host.
 *  @param[in] ReportType  The type of report that the host has sent, either @c HID_REPORT_ITEM_Out or Qc HID_REPORT_ITEM_Feature.
 *  @param[in] ReportData  Pointer to a buffer where the received report has been stored.
 *  @param[in] ReportSize  Size in bytes of the received HID report.
 */
void CALLBACK_HID_Device_ProcessHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                          const uint8_t ReportID,
                                          const uint8_t ReportType,
                                          const void* ReportData,
                                          const uint16_t ReportSize)
{
   uint8_t* Data       = (uint8_t*)ReportData;

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
      }
   }
   else // According to the docu it's just the HID_REPORT_ITEM_Feature left
   {
      switch(ReportID)
      {
         case FEATURE_ID_REFLASH: ; // 42 ;)
            // Stop device and jump to bootlader if the key mates!
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
               //  https://blog.fsck.com/2014/08/how-to-reboot-an-arduino-leonardo-micro-into-the-bootloader.html
               // Detach from the bus and reset it
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
      }
   }
}
