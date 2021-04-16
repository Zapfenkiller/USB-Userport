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
 *   @brief Header file for Descriptors.c.
 *
 *  The header gets included also by the application.
 *
 *  \~German
 *   @brief Datei mit dem Kopfteil zu Descriptors.c.
 *
 *  Diese Datei wird ebenfalls von der Applikation benutzt.
 */

#ifndef _DESCRIPTORS_H_
#define _DESCRIPTORS_H_

   /* Includes: */
      #include <avr/pgmspace.h>

      #include <LUFA/Drivers/USB/USB.h>

      #include "Config/AppConfig.h"

   /* Type Defines: */
      typedef struct
      {
         USB_Descriptor_Configuration_Header_t Config;

         // Generic HID Interface
         USB_Descriptor_Interface_t            HID_Interface;
         USB_HID_Descriptor_HID_t              HID_GenericHID;
         USB_Descriptor_Endpoint_t             HID_ReportINEndpoint;
      } USB_Descriptor_Configuration_t;
      /**<
       * \~English
       *  Type define for the device configuration descriptor
       *  structure. This must be defined in the application code,
       *  as the configuration descriptor contains several
       *  sub-descriptors which vary between devices, and which
       *  describe the device's usage to the host.
       *
       * \~German
       *  Typdefinition für den Device Configuration Descriptor.
       *  Dieser muss im Code der Applikation mit Werten gefüllt
       *  werden da er mehrere Unterdeskriptoren enthält, die mit
       *  dem Gerät variieren. Dieser Deskriptor beschreibt dem
       *  Host die Nutzung des Gerätes.
       */


      /**
       * \~English
       *  Enum for the device interface descriptor IDs within the
       *  device. Each interface descriptor shall have a unique ID
       *  index associated with it, for referencing purposes.
       *
       * \~German
       *  Aufzählung der Interface Descriptor IDs des Gerätes.
       *  Jeder Interface Deskriptor muss eine einmalige ID haben
       *  mit der er referenziert wird.
       */
      enum InterfaceDescriptors_t
      {
         INTERFACE_ID_GenericHID = 0, /**< GenericHID interface descriptor ID */
      };


      /**
       * \~English
       *  Enum for the device string descriptor IDs within the
       *  device. Each string descriptor shall have a unique ID
       *  index associated with it, for referencing purposes.
       *
       * \~German
       *  Aufzählung der String Descriptor IDs des Gerätes. Jeder
       *  String Deskriptor muss eine einmalige ID haben mit der
       *  er referenziert wird.
       */
      enum StringDescriptors_t
      {
         STRING_ID_Language     = 0, /**< Supported Languages string descriptor ID (must be zero) */
         STRING_ID_Manufacturer = 1, /**< Manufacturer string ID */
         STRING_ID_Product      = 2, /**< Product string ID */
         STRING_ID_SerialNo     = 3, /**< Serial number string ID */
      };


   /* Macros: */
      #define GENERIC_IN_EPADDR         (ENDPOINT_DIR_IN | 1)
      /**<
       * \~English Endpoint address of the report IN endpoint (µC internals).
       * \~German  Adresse des Report IN Endpunktes (µC interna).
       */


      #define GENERIC_EPSIZE            8
      /**<
       * \~English Size in bytes of the Report IN endpoint (µC internals).
       * \~German  Größe des Report IN Endpunktes in Byte (µC interna).
       */


   /* Function Prototypes: */
      uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue,
                                          const uint16_t wIndex,
                                          const void** const DescriptorAddress)
                                          ATTR_WARN_UNUSED_RESULT ATTR_NON_NULL_PTR_ARG(3);

#endif
