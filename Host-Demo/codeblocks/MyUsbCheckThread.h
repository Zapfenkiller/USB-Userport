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
