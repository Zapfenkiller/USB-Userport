/***************************************************************
 * Name:      DemoMain.cpp
 * Purpose:   Implements the Application Frame
 * Author:    R. Trapp
 * Created:   2021-12-03
 * Copyright: R. Trapp
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


#include "AppConfig.h"
#include "DemoMain.h"
#include "MyUsbCheckThread.h"
#include "hidapi.h"


/*****************
TODO

 Klare und einfache Oberfläche, minimalistisches Menü

 Umschalten der RX-LED und der TX-LED per Mausklick
 Darstellung zweier GPIOs (fest vorgegeben) als digitale Eingänge (wie?)
 Darstellung eines ADC (fest vorgegeben) als analoger Wert (wie?)

 Nur als Demo und zum Abgucken wie die Dinge funktionieren.
 **NICHT** DIE EIERLEGENDE WOLLMILCHSAU MIT ALLEM HASTENICHGESEHN!

 LED-Darstellung in eigene Klasse auslagern?
 Ziel:
 Methode für Darstellung OFF, R, G, Y, B
 Hier werden nur OFF, G und Y benutzt


 Dran denken:
 Für internationalization die Strings mit _("") definieren.

*****************/


//#define  BTN_BITMAP_POS    wxRIGHT


wxDECLARE_APP(DemoApp);   // necessary because DemoApp uses separate files


// helper function to retrieve wx version info
enum wxbuildinfoformat {
   short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
   wxString wxbuild(wxVERSION_STRING);

   if (format == long_f )
   {
#if defined(__WXMSW__)
      wxbuild << _T("-Windows");
#elif defined(__WXMAC__)
      wxbuild << _T("-Mac");
#elif defined(__UNIX__)
      wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
      wxbuild << _T("-Unicode build");
#else
      wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
   }

   return wxbuild;
}


BEGIN_EVENT_TABLE(DemoMain, wxFrame)
   EVT_CLOSE(DemoMain::OnClose)
   EVT_MENU(idMenuQuit, DemoMain::OnQuit)
   EVT_MENU(idMenuAbout, DemoMain::OnAbout)
   EVT_THREAD(MyUsbCheckThread::idThreadConnection, DemoMain::OnConnection)
   EVT_BUTTON(idRxLedButton, DemoMain::OnRxLedButton)
   EVT_BUTTON(idTxLedButton, DemoMain::OnTxLedButton)
END_EVENT_TABLE()


MyUsbCheckThread *MyThread;
hid_device* deviceHandle;


DemoMain::DemoMain(wxFrame *frame, const wxString& title)
   : wxFrame(frame, -1, title)
{
   SetMinSize(wxSize(320, 240));
   SetSize(wxSize(800, 480));
   SetBackgroundColour(wxColour((unsigned char)0x80, (unsigned char)0x80, (unsigned char)0xFF, wxALPHA_OPAQUE));

   // create a menu bar
   wxMenuBar* mbar = new wxMenuBar();
   wxMenu* fileMenu = new wxMenu(_(""));
   fileMenu->Append(idMenuQuit, _("&Quit\tAlt-F4"), _("Quit the application"));
   mbar->Append(fileMenu, _("&File"));
   wxMenu* helpMenu = new wxMenu(_(""));
   helpMenu->Append(idMenuAbout, _("&About\tF1"), _("Show info about this application"));
   mbar->Append(helpMenu, _("&Help"));
   SetMenuBar(mbar);

   // create a status bar
   // status bar eats up frame client area, so be sure to construct this before
   // further frame content
   my_status = new MyStatusBar(this, wxST_SIZEGRIP);
   SetStatusBar(my_status);

   // create LED control buttons
   rxLedButton = new wxButton(this, idRxLedButton, _("RxLED"));
   rxLedButton->SetToolTip(_("Toggle Rx-LED"));
   rxLedButton->SetOwnForegroundColour(wxColour((unsigned char)0xFF, (unsigned char)0xFF, (unsigned char)0xFF, wxALPHA_OPAQUE));
   rxLedButton->SetBackgroundColour(wxColour((unsigned char)0x40, (unsigned char)0x40, (unsigned char)0x40, wxALPHA_OPAQUE));
   rxLedButton->Disable();
   txLedButton = new wxButton(this, idTxLedButton, _("TxLED"));
   txLedButton->SetToolTip(_("Toggle Tx-LED"));
   txLedButton->SetOwnForegroundColour(wxColour((unsigned char)0xFF, (unsigned char)0xFF, (unsigned char)0xFF, wxALPHA_OPAQUE));
   txLedButton->SetBackgroundColour(wxColour((unsigned char)0x40, (unsigned char)0x40, (unsigned char)0x40, wxALPHA_OPAQUE));
   txLedButton->Disable();
   // add LED control buttons to sizer element
   wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
   buttonSizer->Add
   (
      rxLedButton, 0, 0, 0
   );
   buttonSizer->Add
   (
      txLedButton, 0, 0, 0
   );

   // create and start connection state checker (background thread)
   connected = false;
   m_cancelled = false;
   MyThread = new MyUsbCheckThread(this);
   if (MyThread->Create() == wxTHREAD_NO_ERROR)
   {
      MyThread->Run();
   }
   else
   {
      wxLogError(wxT("Can't create connectivity checker thread!"));
   }

   // finalize frame construction
   wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);
   panelSizer->Add
   (
      buttonSizer, 0, wxEXPAND | wxALL, 0
   );
   SetSizer(panelSizer);
   Centre();
   Show();
}


DemoMain::~DemoMain()
{
   m_cancelled = true;
   SetStatusBar(NULL);
   delete my_status;
}


void DemoMain::OnClose(wxCloseEvent &event)
{
   Destroy();
}


void DemoMain::OnQuit(wxCommandEvent &event)
{
   Destroy();
}


void DemoMain::OnAbout(wxCommandEvent &event)
{
   wxString wxWidgetsVersion = wxbuildinfo(long_f);
   wxString hidapiVersion = hid_version_str();
   wxString about;
   about << _("Hi out there!\n" \
              "I am a demonstrator how to handle the USB-Userport by a " \
              "C++ GUI application. I have been build with the Code::Blocks "
              "IDE 20.03 using hidapi ");
   about << hidapiVersion;
   about << _(" and a ");
   about << wxWidgetsVersion;
   about << _(". My source is open to the public. I am under a GPLv3 license.");
   wxMessageBox(about, _("About This"));
}


void DemoMain::OnConnection(wxThreadEvent &event)
{
   int n = event.GetInt();

   if (n == 0)
   {
      deviceHandle = 0;
      connected = false;
      this->my_status->SetConnectStatus(false);
      rxLedButton->Disable();
      txLedButton->Disable();
   }
   else
   {
      deviceHandle = MyThread->GetDeviceHandle();
      connected = true;
      this->my_status->SetConnectStatus(true);
      rxLedButton->Enable();
      txLedButton->Enable();
   }
}


bool DemoMain::Cancelled()
{
    wxCriticalSectionLocker lock(m_csCancelled);

    return m_cancelled;
}


void DemoMain::OnRxLedButton(wxCommandEvent &event)
{
   unsigned char led[2] = {REPORT_ID_DEVICE_LEDS, RX_LED_BITMASK};
   if (connected)
   {
      if (hid_get_input_report(deviceHandle, led, sizeof(led)) == -1)
      {
         connected = false;
         return;
      }
      led[1] ^= RX_LED_BITMASK;
      if (hid_write(deviceHandle, led, sizeof(led)) == -1)
      {
         connected = false;
         return;
      }
   }
}


void DemoMain::OnTxLedButton(wxCommandEvent &event)
{
   unsigned char led[2] = {REPORT_ID_DEVICE_LEDS, TX_LED_BITMASK};
   if (connected)
   {
      if (hid_get_input_report(deviceHandle, led, sizeof(led)) == -1)
      {
         connected = false;
         return;
      }
      led[1] ^= TX_LED_BITMASK;
      if (hid_write(deviceHandle, led, sizeof(led)) == -1)
      {
         connected = false;
      }
   }
}
