/***************************************************************
 * Name:      DemoMain.h
 * Purpose:   Defines the Application Frame
 * Author:    R. Trapp
 * Created:   2021-12-03
 * Copyright: R. Trapp
 * License:   GPLv3 - see License.txt
 **************************************************************/
// worker thread taken from wxWidgets samples/thread.cpp

#ifndef DEMOMAIN_H
#define DEMOMAIN_H


#ifdef WX_PRECOMP
   #include "wx_pch.h"
#else
   #include <wx/wx.h>
#endif


#include "DemoApp.h"
#include "MyStatusBar.h"


class DemoMain: public wxFrame
{
   public:
      DemoMain(wxFrame *frame, const wxString& title);
      ~DemoMain();

      // accessors for MyWorkerThread (called in its context!)
      bool Cancelled();

   private:
      MyStatusBar*   my_status;

      enum
      {
         idMenuQuit = wxID_HIGHEST+1,
         idMenuAbout
      };

      void OnClose(wxCloseEvent& event);
      void OnQuit(wxCommandEvent& event);
      void OnAbout(wxCommandEvent& event);
      void OnConnection(wxThreadEvent& event);

      bool connected; // USB-Userport connect state

      // was the worker thread cancelled by user?
      bool m_cancelled;
      wxCriticalSection m_csCancelled; // protects m_cancelled

      DECLARE_EVENT_TABLE()
};


#endif // DEMOMAIN_H
