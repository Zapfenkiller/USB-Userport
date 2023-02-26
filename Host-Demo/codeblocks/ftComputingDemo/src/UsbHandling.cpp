/***************************************************************
 * Name:      UsbHandling.cpp
 * Purpose:   Implements class for USB communication to the
 *            ft Interface connection device
 * Author:    R. Trapp (rene <dot> trapp [ät] web <dot> de)
 * Created:   2023-01-20
 * Copyright: R. Trapp ()
 * License:   GPLv3 - see License.txt
 **************************************************************/

#ifdef WX_PRECOMP
   #include "wx_pch.h"
#else
   #include <wx/wx.h>
#endif

#ifdef __BORLANDC__
   #pragma hdrstop
#endif //__BORLANDC__

#include "../include/UsbHandling.h"


/* -------------------------------------------------------------------------- */
/* Identifier(s) of custom USB-HID-class device (PRIVATE)                     */
/* -------------------------------------------------------------------------- */


#define usbVID                  0x04D8   // Microchip Inc.
#define usbPID                  0xEBC7   // USB-Userport
#define usbSNR                  "01"     // Device serial number

// USB-Userport Report IDs and some convenience definitions
#define REPORT_ID_DEVICE_LEDS   0x01     // Report ID of on-board LED control
#define RX_LED_BITMASK          0x01     // Bitmask to access yellow RX-LED
#define TX_LED_BITMASK          0x02     // Bitmask to access green TX-LED

#define REPORT_ID_ftOUTP        0x21     // Motor driver set request
#define REPORT_ID_ftSTPCFG      0x22     // Set stop switch configuration
#define REPORT_ID_ftSTPRESPONSE 0x23     // Set motor response on stop switch
#define REPORT_ID_ftINP         0x24     // Retrieve full interface status
#define FEATURE_ID_ft66843      0x2B     // Set operation mode


/* -------------------------------------------------------------------------- */
/* Some variables that need to keep available during runtime                  */
/* -------------------------------------------------------------------------- */


const unsigned char init_rpt[2]     = {FEATURE_ID_ft66843, 0x01};
const unsigned char exit_rpt[2]     = {FEATURE_ID_ft66843, 0x00};
unsigned char setStpSwCfg_rpt[5]    = {REPORT_ID_ftSTPCFG, 0, 0, 0, 0};
unsigned char setStpResponse_rpt[5] = {REPORT_ID_ftSTPCFG, 0, 0, 0, 0};
unsigned char putOutpVec_rpt[3]     = {REPORT_ID_ftOUTP,   0, 0};
unsigned char setLed_rpt[2]         = {REPORT_ID_DEVICE_LEDS, 0};


/* -------------------------------------------------------------------------- */
/* Public interface functions                                                 */
/* -------------------------------------------------------------------------- */


UsbHandling::UsbHandling()
{
   devHandle = nullptr;
   libReady = false;
}


UsbHandling::~UsbHandling()
{
   ExitLib();
}


wxString UsbHandling::GetVersion(void)
{
   wxString about = _("hidapi ");
   about << hid_version_str();
   return about;
}


bool UsbHandling::InitializeLib(void)
{
   if (hid_init() == 0)
      libReady = true;
   return libReady;
}


void UsbHandling::ExitLib(void)
{
   CloseCon(false);
   libReady = false;
   hid_exit();
}


bool UsbHandling::LibIsReady(void)
{
   return libReady;
}


bool UsbHandling::ConnectToDevice(void)
{
   wxCriticalSectionLocker lock(accessLock);
   if (libReady)
   {
      const wchar_t snr[9] = _T(usbSNR);
      devHandle = hid_open(usbVID, usbPID, snr);
      if (devHandle)
      {
         IndiLedLit(true);
         return true;
      }
      return devHandle ? true : false;
   }
   else
      return false;
}


void UsbHandling::DisconnectFromDevice(void)
{
   IndiLedLit(false);
   CloseCon(false);
}


bool UsbHandling::DeviceIsConnected()
{
   if (devHandle)
   {
      unsigned char rpt[8] = {REPORT_ID_ftINP, 0, 0, 0, 0, 0, 0, 0};
      if (hid_get_input_report(devHandle, rpt, sizeof(rpt)) == -1)
         CloseCon(true);
   }
   return devHandle ? true : false;
}


bool UsbHandling::SendDeviceInitRequest(void)
{
   wxCriticalSectionLocker lock(accessLock);
   if (devHandle)
   {
      if (hid_send_feature_report(devHandle, init_rpt, sizeof(init_rpt)) == -1)
         CloseCon(true);
   }
   return devHandle ? true : false;
}


bool UsbHandling::DeviceIsInitialized(void)
{
   wxCriticalSectionLocker lock(accessLock);
   unsigned char rpt[8] = {REPORT_ID_ftINP, 0, 0, 0, 0, 0, 0, 0};
   if (devHandle)
   {
      if (hid_get_input_report(devHandle, rpt, sizeof(rpt)) == -1)
         CloseCon(true);
   }
   return (devHandle && (rpt[1] & 0x80)) ? true : false;
}


bool UsbHandling::SetStopSwitchConfig(usbDataXchg_t data)
{
   wxCriticalSectionLocker lock(accessLock);
   if (devHandle)
   {
      setStpSwCfg_rpt[0] = REPORT_ID_ftSTPCFG;
      setStpSwCfg_rpt[1] = (unsigned char)(data.stopEnableMaster);
      setStpSwCfg_rpt[2] = (unsigned char)(data.stopInversionMaster);
      setStpSwCfg_rpt[3] = (unsigned char)(data.stopEnableSlave);
      setStpSwCfg_rpt[4] = (unsigned char)(data.stopInversionSlave);
      if (hid_write(devHandle, setStpSwCfg_rpt, sizeof(setStpSwCfg_rpt)) == -1)
      {
         CloseCon(true);
         return false;
      }
      setStpResponse_rpt[0] = REPORT_ID_ftSTPRESPONSE;
      setStpResponse_rpt[1] = (unsigned char)(data.stopResponsePatFwdMaster);
      setStpResponse_rpt[2] = (unsigned char)(data.stopResponsePatBwdMaster);
      setStpResponse_rpt[3] = (unsigned char)(data.stopResponsePatFwdSlave);
      setStpResponse_rpt[4] = (unsigned char)(data.stopResponsePatBwdSlave);
      if (hid_write(devHandle, setStpResponse_rpt, sizeof(setStpResponse_rpt)) == -1)
      {
         CloseCon(true);
         return false;
      }
   }
   return devHandle ? true : false;
}


bool UsbHandling::PutOutputs(usbDataXchg_t data)
{
   wxCriticalSectionLocker lock(accessLock);
   if (devHandle)
   {
      putOutpVec_rpt[0] = REPORT_ID_ftOUTP;
      putOutpVec_rpt[1] = (unsigned char)(data.outputsToMaster);
      putOutpVec_rpt[2] = (unsigned char)(data.outputsToSlave);
      if (hid_write(devHandle, putOutpVec_rpt, sizeof(putOutpVec_rpt)) == -1)
         CloseCon(true);
   }
   return devHandle ? true : false;
}


bool UsbHandling::GetInputs(usbDataXchg_t *data)
{
   wxCriticalSectionLocker lock(accessLock);
   if (devHandle)
   {
      unsigned char getInpVec_rpt[8] = {REPORT_ID_ftINP, 0, 0, 0, 0, 0, 0, 0};
      if (hid_get_input_report(devHandle, getInpVec_rpt, sizeof(getInpVec_rpt)) == -1)
         CloseCon(true);
      else
      {
         data->flagsOfChange = getInpVec_rpt[1];
         data->outputsFromMaster = getInpVec_rpt[2];
         data->outputsFromSlave = getInpVec_rpt[3];
         data->inputsFromMaster = getInpVec_rpt[4];
         data->inputsFromSlave = getInpVec_rpt[5];
         data->analogEX = getInpVec_rpt[6];
         data->analogEY = getInpVec_rpt[7];
      }
   }
   return devHandle ? true : false;
}


/* -------------------------------------------------------------------------- */
/* Private handling stuff                                                     */
/* -------------------------------------------------------------------------- */


void UsbHandling::CloseCon(bool panic)
{
   wxCriticalSectionLocker lock(accessLock);
   if (devHandle)
   {
      if (!panic)
         hid_send_feature_report(devHandle, exit_rpt, sizeof(exit_rpt));
      hid_close(devHandle);
      devHandle = nullptr;
   }
}


bool UsbHandling::IndiLedLit(bool on)
{
   unsigned char led;
   if (GetLedStates(&led))
   {
      if (on)
         led |= RX_LED_BITMASK;
      else
         led &= ~RX_LED_BITMASK;
      return SetLedStates(led);
   }
   else
      return false;
}


bool UsbHandling::GetLedStates(unsigned char* leds)
{
   wxCriticalSectionLocker lock(accessLock);
   if (devHandle)
   {
      unsigned char getLed_rpt[2] = {REPORT_ID_DEVICE_LEDS, 0};
      if (hid_get_input_report(devHandle, getLed_rpt, sizeof(getLed_rpt)) == -1)
         CloseCon(true);
      else
         *leds = getLed_rpt[1];
   }
   return devHandle ? true : false;
}


bool UsbHandling::SetLedStates(unsigned char leds)
{
   wxCriticalSectionLocker lock(accessLock);
   if (devHandle)
   {
      setLed_rpt[0] = REPORT_ID_DEVICE_LEDS;
      setLed_rpt[1] = leds;
      if (hid_write(devHandle, setLed_rpt, sizeof(setLed_rpt)) == -1)
         CloseCon(true);
   }
   return devHandle ? true : false;
}
