/***************************************************************
 * Name:      MyUsbCheckThread.h
 * Purpose:   Implements the USB Check Thread
 * Author:    R. Trapp
 * Created:   2021-12-05
 * Copyright: R. Trapp
 * License:   GPLv3 - see License.txt
 **************************************************************/
// worker thread taken from wxWidgets samples/thread.cpp

#ifdef WX_PRECOMP
   #include "wx_pch.h"
#else
   #include <wx/wx.h>
#endif


#include "AppConfig.h"
#include "MyUsbCheckThread.h"
#include "hidapi.h"


MyUsbCheckThread::MyUsbCheckThread(DemoMain *frame)
   : wxThread()
{
   MyFrame = frame;
}


void MyUsbCheckThread::OnExit()
{
   wxThreadEvent event(wxEVT_THREAD, idThreadConnection);
   event.SetInt(0); // that's all
   wxQueueEvent(MyFrame, event.Clone());
}


wxThread::ExitCode MyUsbCheckThread::Entry()
{
   usbState = stateConnect2API;
   hid_device* devHandle;

   while (!MyFrame->Cancelled())
   {
      // check if this thread shall exit
      if (TestDestroy())
         break;

      switch (usbState)
      {
         case  stateConnect2API:
            if (hid_init() == 0)
            {  // hidapi is ready
               usbState = stateOpenDevice;
            }
            else
            {  // hidapi refuses
               wxMilliSleep(500);
            }
            break;
         case  stateOpenDevice:
            {
               const wchar_t snr[9] = _T(usbSNR);
               devHandle = hid_open(usbVID, usbPID, snr);
               if (devHandle)
               {
                  wxThreadEvent event(wxEVT_THREAD, idThreadConnection);
                  event.SetInt(1);
                  wxQueueEvent(MyFrame, event.Clone());
                  usbState = stateConnected;
               }
               else
               {  // no USB-Userport
                  wxMilliSleep(500);
               }
            }
            break;
         case  stateConnected:
            // check connectivity is still there, a bit q'n'd
            {
               #define  REPORT_ID_DEVICE_LEDS   0x01
               unsigned char led[2] = {REPORT_ID_DEVICE_LEDS, 0x00};
               int ret = hid_get_input_report(devHandle, led, sizeof(led));
               if (ret == -1)
               {
                  hid_close(devHandle);
                  wxThreadEvent event(wxEVT_THREAD, idThreadConnection);
                  event.SetInt(0);
                  wxQueueEvent(MyFrame, event.Clone());
                  usbState = stateOpenDevice;
               }
            }
            wxMilliSleep(50);
            break;
         default:
            hid_close(devHandle);
            hid_exit();
            wxThreadEvent event(wxEVT_THREAD, idThreadConnection);
            event.SetInt(0);
            wxQueueEvent(MyFrame, event.Clone());
            usbState = stateConnect2API;
      } // switch()
   }  // while()
   hid_close(devHandle);
   hid_exit();
   // create any type of command event here
   wxThreadEvent event(wxEVT_THREAD, idThreadConnection);
   event.SetInt(0);
   // send in a thread-safe way
   wxQueueEvent(MyFrame, event.Clone());
   return NULL;
}
