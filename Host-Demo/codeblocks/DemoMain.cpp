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


#include "DemoMain.h"
//#include "MyWorkerThread.h"
#include "hidapi.h"


/*****************
TODO

 Verbindungserkennung per Hintergrund-Thread
 L�uft alle 50 bis 100 ms und schaltet das Status-Icon um, je nachdem ob ein
 USB-Userport verbunden ist oder nicht. Setzt au�erdem die LEDs und die
 anderen Anzeigen synchron zur HW!

 Klare und einfache Oberfl�che, minimalistisches Men�

 Umschalten der RX-LED und der TX-LED per Mausklick
 Darstellung zweier GPIOs (fest vorgegeben) als digitale Eing�nge (wie?)
 Darstellung eines ADC (fest vorgegeben) als analoger Wert (wie?)

 Nur als Demo und zum Abgucken wie die Dinge funktionieren.
 **NICHT** DIE EIERLEGENDE WOLLMILCHSAU MIT ALLEM HASTENICHGESEHN!

 LED-Darstellung in eigene Klasse auslagern?
 Ziel:
 Methode f�r Darstellung OFF, R, G, Y, B
 Hier werden nur OFF, G und Y benutzt


 Dran denken:
 F�r internationalization die Strings mit _("") definieren.

*****************/


#define  BTN_BITMAP_POS    wxRIGHT


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
END_EVENT_TABLE()


DemoMain::DemoMain(wxFrame *frame, const wxString& title)
   : wxFrame(frame, -1, title)
{
   SetMinSize(wxSize(320, 240));
   SetSize(wxSize(800, 480));
   SetBackgroundColour(wxColour((unsigned char)0x80, (unsigned char)0x80, (unsigned char)0xFF, wxALPHA_OPAQUE));

   // create a menu bar
   wxMenuBar* mbar = new wxMenuBar();
   wxMenu* fileMenu = new wxMenu(_T(""));
   fileMenu->Append(idMenuQuit, _("&Quit\tAlt-F4"), _("Quit the application"));
   mbar->Append(fileMenu, _("&File"));
   wxMenu* helpMenu = new wxMenu(_T(""));
   helpMenu->Append(idMenuAbout, _("&About\tF1"), _("Show info about this application"));
   mbar->Append(helpMenu, _("&Help"));
   SetMenuBar(mbar);

   // create a status bar
   // status bar eats frame client area, so be sure to construct his before
   // further frame content
   my_status = new MyStatusBar(this, wxST_SIZEGRIP);
   SetStatusBar(my_status);

   Centre();
   Show();
}


DemoMain::~DemoMain()
{
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
              "C++ GUI application. I have been build with the CodeBlocks::IDE "\
              "20.03 using hidapi ");
   about << hidapiVersion;
   about << _(" and a ");
   about << wxWidgetsVersion;
   about << _(". My source is open to the public. I am under a GPLv3 license.");
   wxMessageBox(about, _("About This"));
}