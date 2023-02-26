/***************************************************************
 * Name:      UsbHandling.h
 * Purpose:   Defines class for USB communication to the
 *            ft Interface connection device
 * Author:    R. Trapp (rene <dot> trapp [ät] web <dot> de)
 * Created:   2023-01-20
 * Copyright: R. Trapp ()
 * License:   GPLv3 - see License.txt
 **************************************************************/
// worker thread taken from:
//    wxWidgets samples/thread.cpp
// detection of device arrival / removal:
//    https://forums.wxwidgets.org/viewtopic.php?t=12138
//    https://stackoverflow.com/questions/38430587/any-windows-api-for-detecting-mtp-devices/68851782
// modified to get it independent from specific application class

#ifndef USBHANDLING_H
#define USBHANDLING_H


#ifdef WX_PRECOMP
   #include "wx_pch.h"
#else
   #include <wx/wx.h>
#endif

#include "hidapi.h"


class UsbHandling
{
   public:
      typedef struct
      {
         int stopEnableMaster;      // '1' = stop switch active
         int stopEnableSlave;       // '1' = stop switch active
         int stopInversionMaster;   // '1' = stop switch active when '0'
         int stopInversionSlave;    // '1' = stop switch active when '0'
         int stopResponsePatFwdMaster; // IDLE or BRAKE
         int stopResponsePatBwdMaster; // IDLE or BRAKE
         int stopResponsePatFwdSlave;  // IDLE or BRAKE
         int stopResponsePatBwdSlave;  // IDLE or BRAKE
         int outputsToMaster;       // O8..O1 Desired IDLE, FWD, BWD or BRAKE
         int outputsToSlave;        // O8..O1 Desired IDLE, FWD, BWD or BRAKE
         int outputsFromMaster;     // O8..O1 Effective IDLE, FWD, BWD or BRAKE
         int outputsFromSlave;      // O8..O1 Effective IDLE, FWD, BWD or BRAKE
         int inputsFromMaster;      // E8..E1 switches status '0' or '1'
         int inputsFromSlave;       // E8..E1 switches status '0' or '1'
         int analogEX;              // resistance reading EX
         int analogEY;              // resistance reading EY
         int flagsOfChange;         // some data has been changed
      } usbDataXchg_t;

      enum
      {
         FLAG_OUTPUT_CHANGE   = (1 << 0),
         FLAG_INPUT_CHANGE    = (1 << 1),
         FLAG_POTI_X_CHANGE   = (1 << 2),
         FLAG_POTI_Y_CHANGE   = (1 << 3),
         FLAG_DEVICE_UP       = (1 << 7)
      };

      UsbHandling();
      virtual ~UsbHandling();
      wxString GetVersion(void);
      bool InitializeLib(void);                    // returns TRUE if success, FALSE else
      void ExitLib(void);
      bool LibIsReady(void);                       // returns TRUE if success, FALSE else
      bool ConnectToDevice(void);                  // returns TRUE if success, FALSE else
      void DisconnectFromDevice(void);
      bool DeviceIsConnected(void);                // returns TRUE if success, FALSE else
      bool SendDeviceInitRequest(void);            // returns TRUE if success, FALSE else
      bool DeviceIsInitialized(void);              // returns TRUE if success, FALSE else
      bool SetStopSwitchConfig(usbDataXchg_t data);// returns TRUE if success, FALSE else
      bool PutOutputs(usbDataXchg_t data);         // returns TRUE if success, FALSE else
      bool GetInputs(usbDataXchg_t *data);         // returns TRUE if success, FALSE else


   private:
      wxFrame           *myFrame;
      wxCriticalSection accessLock;                // protect hidapi transfers
      hid_device*       devHandle;
      bool              libReady;

      void CloseCon(bool panic);
      bool IndiLedLit(bool on);                    // returns TRUE if success, FALSE else
      bool GetLedStates(unsigned char* leds);      // returns TRUE if success, FALSE else
      bool SetLedStates(unsigned char leds);       // returns TRUE if success, FALSE else
};

/*
   Except for DeviceIsInitialized() a FALSE return indicates a loss of the
   physical connection.
   In case of DeviceIsInitialized() == FALSE, this indicates that the
   interface lost the connection to the GUI, but still might be attached
   physically. Device departure also is a reason here.
   Use DeviceIsConnected() to check just for physical presence.
*/
#endif // USBHANDLING_H
