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

  ATTENTION:
  The above license expressly does **not** include the VID/PID
  used. This has been donated to me, René Trapp, by Microchip
  Technology Inc. for sole use with this project.
*/


/** @file
 *  USB Device Descriptors, for library use when in USB device mode.
 *  Descriptors are special computer-readable structures which the
 *  host requests upon device enumeration, to determine the device's
 *  capabilities and functions.

 @todo
   Eine deutsche Version der Dokumentation hinzufügen.
 */

#include "Descriptors.h"

/** HID class report descriptor. This is a special descriptor constructed with values from the
 *  USBIF HID class specification to describe the reports and capabilities of the HID device. This
 *  descriptor is parsed by the host and its contents used to determine what data (and in what encoding)
 *  the device will send, and what it may be sent back from the host. Refer to the HID specification for
 *  more details on HID report descriptors.
 *  \n [USBIF HID Specification](https://www.usb.org/sites/default/files/documents/hid1_11.pdf)
 *  \n [USBIF HID Usage Tables](https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf) \n
 *  The descriptor is sparsely defined, just the basic necessities and nothing else.
 */
const USB_Descriptor_HIDReport_Datatype_t PROGMEM GenericReport[] =
{
   HID_RI_USAGE_PAGE(16, 0xFF00),                        // "Vendor specific"
   HID_RI_USAGE(8, 0x01),                                // Arbitrarily choosen
   HID_RI_COLLECTION(8, 0x01),                           // Arbitrarily choosen
      HID_RI_REPORT_SIZE(8, 0x08),                       // Generally 8 bits wide
      HID_RI_LOGICAL_MAXIMUM(8, 0xFF),                   // Allow all 256 ...
      HID_RI_LOGICAL_MINIMUM(8, 0x00),                   // ...  combinations

      /* On-Board LED control */
      HID_RI_USAGE_PAGE(8, 0x08),                        // "LEDs"
      HID_RI_REPORT_ID(8, REPORT_ID_DEVICE_LEDS),        // ./Config/AppConfig.h
      HID_RI_REPORT_COUNT(8, REPORT_SIZE_DEVICE_LEDS),   // ./Config/AppConfig.h
      /* IN report */
      HID_RI_USAGE(8, 0x00),                             // "Purpose Undefined"
      HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE),
      /* OUT report */
      HID_RI_USAGE(8, 0x00),                             // "Purpose Undefined"
      HID_RI_OUTPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE | HID_IOF_NON_VOLATILE),

      /* ... */
      HID_RI_USAGE_PAGE(8, 0x06),                        // "Generic Device Control ?
      
      /* GPIO1 */
      /* IN reports */
      HID_RI_REPORT_COUNT(8, REPORT_SIZE_GET_GPIO),      // ./Config/AppConfig.h
      HID_RI_REPORT_ID(8, REPORT_ID_LINES_GPIO1),        // ./Config/AppConfig.h
      HID_RI_USAGE(8, 0x00),                             // "Purpose Undefined"
      HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE),
      HID_RI_REPORT_ID(8, REPORT_ID_DIRS_GPIO1),         // ./Config/AppConfig.h
      HID_RI_USAGE(8, 0x00),                             // "Purpose Undefined"
      HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE),
      HID_RI_REPORT_ID(8, REPORT_ID_LINES_GPIO2),        // ./Config/AppConfig.h
      HID_RI_USAGE(8, 0x00),                             // "Purpose Undefined"
      HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE),
      HID_RI_REPORT_ID(8, REPORT_ID_DIRS_GPIO2),         // ./Config/AppConfig.h
      HID_RI_USAGE(8, 0x00),                             // "Purpose Undefined"
      HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE),
      /* OUT reports */
      HID_RI_REPORT_COUNT(8, REPORT_SIZE_SET_GPIO),      // ./Config/AppConfig.h
      HID_RI_REPORT_ID(8, REPORT_ID_LINES_GPIO1),        // ./Config/AppConfig.h
      HID_RI_USAGE(8, 0x00),                             // "Purpose Undefined"
      HID_RI_OUTPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE | HID_IOF_NON_VOLATILE),
      HID_RI_REPORT_ID(8, REPORT_ID_DIRS_GPIO1),         // ./Config/AppConfig.h
      HID_RI_USAGE(8, 0x00),                             // "Purpose Undefined"
      HID_RI_OUTPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE | HID_IOF_NON_VOLATILE),
      HID_RI_REPORT_ID(8, REPORT_ID_LINES_GPIO2),        // ./Config/AppConfig.h
      HID_RI_USAGE(8, 0x00),                             // "Purpose Undefined"
      HID_RI_OUTPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE | HID_IOF_NON_VOLATILE),
      HID_RI_REPORT_ID(8, REPORT_ID_DIRS_GPIO2),         // ./Config/AppConfig.h
      HID_RI_USAGE(8, 0x00),                             // "Purpose Undefined"
      HID_RI_OUTPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE | HID_IOF_NON_VOLATILE),

      /* GPIO IRQ handling */
      HID_RI_REPORT_COUNT(8, REPORT_SIZE_IRQEN_GPIO),    // ./Config/AppConfig.h
      HID_RI_REPORT_ID(8, REPORT_ID_IRQEN_GPIO),         // ./Config/AppConfig.h
      HID_RI_USAGE(8, 0x00),                             // "Purpose Undefined"
      HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE),
      HID_RI_USAGE(8, 0x00),                             // "Purpose Undefined"
      HID_RI_OUTPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE | HID_IOF_NON_VOLATILE),
      HID_RI_REPORT_COUNT(8, REPORT_SIZE_IRQ_GPIO),      // ./Config/AppConfig.h
      HID_RI_REPORT_ID(8, REPORT_ID_IRQ_GPIO),           // ./Config/AppConfig.h
      HID_RI_USAGE(8, 0x00),                             // "Purpose Undefined"
      HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE),

      /* Device configuration control */
      /* Feature Reflash */
      HID_RI_REPORT_ID(8, FEATURE_ID_REFLASH),           // ./Config/AppConfig.h
      HID_RI_REPORT_COUNT(8, FEATURE_SIZE_REFLASH),      // ./Config/AppConfig.h
      HID_RI_USAGE(8, 0x00),                             // "Purpose Undefined"
      HID_RI_FEATURE(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE | HID_IOF_NON_VOLATILE),

   HID_RI_END_COLLECTION(0)
};
/* Why are all the descriptors defined as full byte(s)?
   The whole thing is "vendor specific". I am free to define what
   I see fit. Here it is the USB that sends and receives packets
   consisting of full bytes. Less than 8 bits are sent as byte
   anyway, the unused bits do no impact anything.
   In case of the on-board LED control one _could_ define lots of
   descriptors telling which LED is on which bit position. Okay,
   it _might_ make sense if the USB-Userport shall support tons of
   premade applications that support such features. But there are no
   such applications around up to now. You still remember the usage
   page is set to "vendor specific"? Up to now the definition here
   sets the standard. The applications emerging from this would be
   simple and not parsing the descriptor string to adapt to dozens
   of variations. Those two facts lead to: Just simple descriptors
   and one convention what a certain report tells (this is why
   report IDs are used). The device software ignores unused bits
   anyway so it does not make sense to boost the report descriptor
   size for just no benefit.
*/


/** Device descriptor structure. This descriptor, located in FLASH memory, describes the overall
 *  device characteristics, including the supported USB version, control endpoint size and the
 *  number of device configurations. The descriptor is read out by the USB host when the enumeration
 *  process begins.
 *  => hid1_11.pdf, page 76
 */
const USB_Descriptor_Device_t PROGMEM DeviceDescriptor =
{
   .Header                 = {.Size = sizeof(USB_Descriptor_Device_t), .Type = DTYPE_Device}, // 0x12, 0x01

   .USBSpecification       = VERSION_BCD(1,1,0),
   .Class                  = USB_CSCP_NoDeviceClass,        // 0x00  'None'
   .SubClass               = USB_CSCP_NoDeviceSubclass,     // 0x00  'None'
   .Protocol               = USB_CSCP_NoDeviceProtocol,     // 0x00  'None'

   .Endpoint0Size          = FIXED_CONTROL_ENDPOINT_SIZE,   // ./Config/LUFAConfig.h

   .VendorID               = 0x04D8,                        // Microchip Technology Inc
   .ProductID              = 0xEBC7,                        // "USB-Userport"
   .ReleaseNumber          = VERSION_BCD(                   // ./Config/AppConfig.h
                              RELEASE_MAJOR,                // ./Config/AppConfig.h
                              RELEASE_MINOR,                // ./Config/AppConfig.h
                              RELEASE_REVISION),            // ./Config/AppConfig.h

   .ManufacturerStrIndex   = STRING_ID_Manufacturer,        // 0x01 ./Descriptors.h
   .ProductStrIndex        = STRING_ID_Product,             // 0x02 ./Descriptors.h
   .SerialNumStrIndex      = STRING_ID_SerialNo,            // 0x03 ./Descriptors.h

   .NumberOfConfigurations = FIXED_NUM_CONFIGURATIONS       // ./Config/LUFAConfig.h
};

/** Configuration descriptor structure. This descriptor, located in FLASH memory, describes the usage
 *  of the device in one of its supported configurations, including information about any device interfaces
 *  and endpoints. The descriptor is read out by the USB host during the enumeration process when selecting
 *  a configuration so that the host may correctly communicate with the USB device.
 */
const USB_Descriptor_Configuration_t PROGMEM ConfigurationDescriptor =
{
   .Config =
      {
         .Header                 = {.Size = sizeof(USB_Descriptor_Configuration_Header_t), .Type = DTYPE_Configuration},

         .TotalConfigurationSize = sizeof(USB_Descriptor_Configuration_t),
         .TotalInterfaces        = 1,

         .ConfigurationNumber    = 1,
         .ConfigurationStrIndex  = NO_DESCRIPTOR,

         .ConfigAttributes       = (USB_CONFIG_ATTR_RESERVED | USB_CONFIG_ATTR_SELFPOWERED),

         .MaxPowerConsumption    = USB_CONFIG_POWER_MA(MAX_CURRENT_DRAW) // ./Config/AppConfig.h
      },

   .HID_Interface =
      {
         .Header                 = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},

         .InterfaceNumber        = INTERFACE_ID_GenericHID,
         .AlternateSetting       = 0x00,

         .TotalEndpoints         = 1,

         .Class                  = HID_CSCP_HIDClass,
         .SubClass               = HID_CSCP_NonBootSubclass,
         .Protocol               = HID_CSCP_NonBootProtocol,

         .InterfaceStrIndex      = NO_DESCRIPTOR
      },

   .HID_GenericHID =
      {
         .Header                 = {.Size = sizeof(USB_HID_Descriptor_HID_t), .Type = HID_DTYPE_HID},

         .HIDSpec                = VERSION_BCD(1,1,1),
         .CountryCode            = 0x00,
         .TotalReportDescriptors = 1,
         .HIDReportType          = HID_DTYPE_Report,
         .HIDReportLength        = sizeof(GenericReport)
      },

   .HID_ReportINEndpoint =
      {
         .Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

         .EndpointAddress        = GENERIC_IN_EPADDR,
         .Attributes             = (EP_TYPE_INTERRUPT | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
         .EndpointSize           = GENERIC_EPSIZE,
         .PollingIntervalMS      = POLLING_INTERVAL // ./Config/AppConfig.h
      },
};

/** Language descriptor structure. This descriptor, located in FLASH memory, is returned when the host requests
 *  the string descriptor with index 0 (the first index). It is actually an array of 16-bit integers, which indicate
 *  via the language ID table available at USB.org what languages the device supports for its string descriptors.
 */
const USB_Descriptor_String_t PROGMEM LanguageString = USB_STRING_DESCRIPTOR_ARRAY(LANGUAGE_ID_ENG);

/** Manufacturer descriptor string. This is a Unicode string containing the manufacturer's details in human readable
 *  form, and is read out upon request by the host when the appropriate string ID is requested, listed in the Device
 *  Descriptor.
 */
const USB_Descriptor_String_t PROGMEM ManufacturerString = USB_STRING_DESCRIPTOR(L"René Trapp");

/** Product descriptor string. This is a Unicode string containing the product's details in human readable form,
 *  and is read out upon request by the host when the appropriate string ID is requested, listed in the Device
 *  Descriptor.
 */
const USB_Descriptor_String_t PROGMEM ProductString = USB_STRING_DESCRIPTOR(L"USB-Userport");

/** Serial number descriptor string. This is a Unicode string containing the serial number in human readable form,
 *  and is read out upon request by the host when the appropriate string ID is requested, listed in the Device
 *  Descriptor.
 *  With the USB-Userport project this serial number is (ab)used to address a certain device in case there are
 *  more than one attached to the computer. If you have such usage scenarios consider to compile the whole project
 *  as often as needed and adjust this string for each compile run.
 *  Since the serial number literally is a string you are not restricted to just digits '0' to '9'. Everything is
 *  possible, even chinese runes have been seen out there in the wild. And as the thing drops to the host just as
 *  data <i>bytes</i> you are free to do what you see fit. But be aware, there are claims about windows to not
 *  enumerate the device if digits are others than '0'..'9' or 'A' to 'F'.
 */
const USB_Descriptor_String_t PROGMEM SerialString = USB_STRING_DESCRIPTOR(L"01");

/** This function is called by the library when in device mode, and must be overridden (see library "USB Descriptors"
 *  documentation) by the application code so that the address and size of a requested descriptor can be given
 *  to the USB library. When the device receives a Get Descriptor request on the control endpoint, this function
 *  is called so that the descriptor details can be passed back and the appropriate descriptor sent back to the
 *  USB host.
 */
uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue,
                                    const uint16_t wIndex,
                                    const void** const DescriptorAddress)
{
   const uint8_t  DescriptorType   = (wValue >> 8);
   const uint8_t  DescriptorNumber = (wValue & 0xFF);

   const void* Address = NULL;
   uint16_t    Size    = NO_DESCRIPTOR;

   switch (DescriptorType)
   {
      case DTYPE_Device:
         Address = &DeviceDescriptor;
         Size    = sizeof(USB_Descriptor_Device_t);
         break;
      case DTYPE_Configuration:
         Address = &ConfigurationDescriptor;
         Size    = sizeof(USB_Descriptor_Configuration_t);
         break;
      case DTYPE_String:
         switch (DescriptorNumber)
         {
            case STRING_ID_Language:
               Address = &LanguageString;
               Size    = pgm_read_byte(&LanguageString.Header.Size);
               break;
            case STRING_ID_Manufacturer:
               Address = &ManufacturerString;
               Size    = pgm_read_byte(&ManufacturerString.Header.Size);
               break;
            case STRING_ID_Product:
               Address = &ProductString;
               Size    = pgm_read_byte(&ProductString.Header.Size);
               break;
            case STRING_ID_SerialNo:
               Address = &SerialString;
               Size    = pgm_read_byte(&SerialString.Header.Size);
               break;
         }

         break;
      case HID_DTYPE_HID:
         Address = &ConfigurationDescriptor.HID_GenericHID;
         Size    = sizeof(USB_HID_Descriptor_HID_t);
         break;
      case HID_DTYPE_Report:
         Address = &GenericReport;
         Size    = sizeof(GenericReport);
         break;
   }

   *DescriptorAddress = Address;
   return Size;
}
