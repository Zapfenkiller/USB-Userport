/*
  * The USB-Userport *
  Copyright 2020  René Trapp (rene [dot] trapp (-at-) web [dot] de)

  * LUFA Library *
  Copyright 2017  Dean Camera (dean [at] fourwalledcubicle [dot] com)
*/


/** @file
 *  \~English
 *   @brief Header file for Userport.c.
 *
 *  \~German
 *   @brief  Userport.c bindet diese Datei ein.
 */

#ifndef _USERPORT_H_
#define _USERPORT_H_

   /* Includes: */
// #include <avr/io.h>
   #include <avr/wdt.h>
   #include <avr/power.h>
// #include <avr/interrupt.h>
// #include <string.h>

   #include "Descriptors.h"
   #include "Board/ADC.h"
   #include "Board/GPIOs.h"
   #include "Board/LEDs.h"
   #include "Board/IOREGs.h"
   #include "Config/AppConfig.h"

   #include <LUFA/Drivers/USB/USB.h>
   #include <LUFA/Platform/Platform.h>

   /* Function Prototypes: */
   void SetupHardware(void);

   void EVENT_USB_Device_Connect(void);
   void EVENT_USB_Device_Disconnect(void);
   void EVENT_USB_Device_ConfigurationChanged(void);
   void EVENT_USB_Device_ControlRequest(void);
   void EVENT_USB_Device_StartOfFrame(void);

   bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                            uint8_t* const ReportID,
                                            const uint8_t ReportType,
                                            void* ReportData,
                                            uint16_t* const ReportSize);
   void CALLBACK_HID_Device_ProcessHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                             const uint8_t ReportID,
                                             const uint8_t ReportType,
                                             const void* ReportData,
                                             const uint16_t ReportSize);

#endif
