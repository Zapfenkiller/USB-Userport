/***************************************************************
 * Name:      MyUsbCheckThread.h
 * Purpose:   Defines the USB Check Thread
 * Author:    R. Trapp
 * Created:   2021-12-05
 * Copyright: R. Trapp
 * License:   GPLv3 - see License.txt
 **************************************************************/
// worker thread taken from wxWidgets samples/thread.cpp

#ifndef MYUSBCHECKTHREAD_H
#define MYUSBCHECKTHREAD_H


#ifdef WX_PRECOMP
   #include "wx_pch.h"
#else
   #include <wx/wx.h>
#endif


#include "DemoMain.h"
#include "hidapi.h"


class MyUsbCheckThread: public wxThread
{
   public:
      MyUsbCheckThread(DemoMain *frame);
      DemoMain *MyFrame;

      enum
      {
         idThreadConnection = wxID_HIGHEST+1
      };

      // report device handle, returns 0 if no device open
      hid_device* GetDeviceHandle();

      // thread execution starts here
      virtual void *Entry();

      // called when the thread exits - whether it terminates normally or is
      // stopped with Delete() (but not when it is Kill()ed!)
      virtual void OnExit();


   private:
      hid_device* devHandle;

      enum
      {
         stateConnect2API = 0,
         stateOpenDevice,
         stateConnected
      } usbState;
};


#endif // MYUSBCHECKTHREAD_H

/***************************************************************
 Rather short documentation:
 MyUsbCheckThread::Entry() repeats until the application is
 stopped. Its purpose is to connect to the hidapi, open the
 default USB-Userport and then regularly checks the connectivity
 to be there. It throws an event to the main thread in case the
 connectivity state changes. It works completely in the
 background.
 If "offline" a connection attempt is made every 500 ms.
 If "online" the connectivity check performs every 50 ms.
 **************************************************************/
