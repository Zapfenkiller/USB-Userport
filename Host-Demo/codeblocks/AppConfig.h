/***************************************************************
 * Name:      AppConfig.h
 * Purpose:   Defines Application Settings
 * Author:    R. Trapp
 * Created:   2021-12-06
 * Copyright: R. Trapp
 * License:   GPLv3 - see License.txt
 *            =================== ATTENTION! ===================
 *            This license does not cover the VID / PID usage!
 **************************************************************/


#define  usbVID   0x04D8   // Microchip Inc.
#define  usbPID   0xEBC7   // USB-Userport
#define  usbSNR   "01"     // Device serial number

#define  REPORT_ID_DEVICE_LEDS   0x01  // Report ID of onboard LED control

#define  RX_LED_BITMASK          0x01  // Bitmask to access yellow LED
#define  TX_LED_BITMASK          0x02  // Bitmask to access green LED
