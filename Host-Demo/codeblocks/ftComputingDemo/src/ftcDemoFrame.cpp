/***************************************************************
 * Name:      ftcDemoFrame.cpp
 * Purpose:   Implements Application Frame
 * Author:    R. Trapp (rene <dot> trapp [ät] web <dot> de)
 * Created:   2023-01-19
 * Copyright: R. Trapp ()
 * License:   GPLv3 - see License.txt
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

//#include "../AppConfig.h"
#include "../include/ftcDemoFrame.h"
//#include <cmath>


// helper function to retrieve wx version info
wxString wxbuildinfo()
{
   wxString wxbuild(wxVERSION_STRING);
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
   return wxbuild;
}


ftcDemoFrame::ftcDemoFrame(wxFrame *frame, const wxString& title)
    : wxFrame(frame, -1, title)
{
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
   my_status = new MyStatusBar(this, 0 /*wxST_SIZEGRIP*/);
   SetStatusBar(my_status);
   if (my_status->GetFieldsCount() > 0)
      SetStatusBarPane(1);

   // create buttons and other controls
   e1LedButton = new MyLedButton(this, idE1LedButton, _("E1"), _("Status of E1 input"));
// e1LedButton->SetMinSize(wxSize(this->GetClientSize().GetWidth()/3-24, 32));
// e1LedButton->SetMaxSize(wxSize(this->GetClientSize().GetWidth()/3-24, 32));
   e2LedButton = new MyLedButton(this, idE2LedButton, _("E2"), _("Status of E2 input"));
   e3LedButton = new MyLedButton(this, idE3LedButton, _("E3"), _("Status of E3 input"));
   e4LedButton = new MyLedButton(this, idE4LedButton, _("E4"), _("Status of E4 input"));
   e5LedButton = new MyLedButton(this, idE5LedButton, _("E5"), _("Status of E5 input"));
   e6LedButton = new MyLedButton(this, idE6LedButton, _("E6"), _("Status of E6 input"));
   e7LedButton = new MyLedButton(this, idE7LedButton, _("E7"), _("Status of E7 input"));
   e8LedButton = new MyLedButton(this, idE8LedButton, _("E8"), _("Status of E8 input"));
   o1LedButton = new MyLedButton(this, idO1LedButton, _("O1"), _("Status of O1 output"));
   o2LedButton = new MyLedButton(this, idO2LedButton, _("O2"), _("Status of O2 output"));
   o3LedButton = new MyLedButton(this, idO3LedButton, _("O3"), _("Status of O3 output"));
   o4LedButton = new MyLedButton(this, idO4LedButton, _("O4"), _("Status of O4 output"));
   o5LedButton = new MyLedButton(this, idO5LedButton, _("O5"), _("Status of O5 output"));
   o6LedButton = new MyLedButton(this, idO6LedButton, _("O6"), _("Status of O6 output"));
   o7LedButton = new MyLedButton(this, idO7LedButton, _("O7"), _("Status of O7 output"));
   o8LedButton = new MyLedButton(this, idO8LedButton, _("O8"), _("Status of O8 output"));

   m1CtrlBox = new MyMotorCtrlButton(this, idM1ControlBox, _("M1"));
   m1CtrlBox->SetBackgroundColour(0xA0A0A0);
   m2CtrlBox = new MyMotorCtrlButton(this, idM2ControlBox, _("M2"));
   m2CtrlBox->SetBackgroundColour(0xA0A0A0);
   m3CtrlBox = new MyMotorCtrlButton(this, idM3ControlBox, _("M3"));
   m3CtrlBox->SetBackgroundColour(0xA0A0A0);
   m4CtrlBox = new MyMotorCtrlButton(this, idM4ControlBox, _("M4"));
   m4CtrlBox->SetBackgroundColour(0xA0A0A0);

   s1FwdCtrlBox = new MyStopCtrlButton(this, idStop1FwdCtrlBox, _("Stop M1 FWD"), _("@ E1 ON"), _("NEVER"), _("@ E1 OFF"));
   s1FwdCtrlBox->SetBackgroundColour(0xA0F0A0);
   s2FwdCtrlBox = new MyStopCtrlButton(this, idStop2FwdCtrlBox, _("Stop M2 FWD"), _("@ E3 ON"), _("NEVER"), _("@ E3 OFF"));
   s2FwdCtrlBox->SetBackgroundColour(0xA0F0A0);
   s3FwdCtrlBox = new MyStopCtrlButton(this, idStop3FwdCtrlBox, _("Stop M3 FWD"), _("@ E5 ON"), _("NEVER"), _("@ E5 OFF"));
   s3FwdCtrlBox->SetBackgroundColour(0xA0F0A0);
   s4FwdCtrlBox = new MyStopCtrlButton(this, idStop4FwdCtrlBox, _("Stop M4 FWD"), _("@ E7 ON"), _("NEVER"), _("@ E7 OFF"));
   s4FwdCtrlBox->SetBackgroundColour(0xA0F0A0);

   s1BwdCtrlBox = new MyStopCtrlButton(this, idStop1BwdCtrlBox, _("Stop M1 BWD"), _("@ E2 ON"), _("NEVER"), _("@ E2 OFF"));
   s1BwdCtrlBox->SetBackgroundColour(0xA0A0F0);
   s2BwdCtrlBox = new MyStopCtrlButton(this, idStop2BwdCtrlBox, _("Stop M2 BWD"), _("@ E4 ON"), _("NEVER"), _("@ E4 OFF"));
   s2BwdCtrlBox->SetBackgroundColour(0xA0A0F0);
   s3BwdCtrlBox = new MyStopCtrlButton(this, idStop3BwdCtrlBox, _("Stop M3 BWD"), _("@ E6 ON"), _("NEVER"), _("@ E6 OFF"));
   s3BwdCtrlBox->SetBackgroundColour(0xA0A0F0);
   s4BwdCtrlBox = new MyStopCtrlButton(this, idStop4BwdCtrlBox, _("Stop M4 BWD"), _("@ E8 ON"), _("NEVER"), _("@ E8 OFF"));
   s4BwdCtrlBox->SetBackgroundColour(0xA0A0F0);

   exInputGauge = new MyGauge(this, idExButton, 0, 255, "EX", "Analog Input EX");
   exInputGauge->SetBackgroundColour(0xA0A0A0);
   eyInputGauge = new MyGauge(this, idEyButton, 0, 255, "EY", "Analog Input EY");
   eyInputGauge->SetBackgroundColour(0xA0A0A0);

   // Motor 1 controls
   wxBoxSizer *m1InputLedsBrick = new wxBoxSizer(wxVERTICAL);
   m1InputLedsBrick->Add(e1LedButton, 0, wxALL, 1);
   m1InputLedsBrick->Add(e2LedButton, 0, wxALL, 1);
   wxBoxSizer *m1FwdStopCtrlBrick = new wxBoxSizer(wxVERTICAL);
   m1FwdStopCtrlBrick->Add(s1FwdCtrlBox, 0, wxALL, 1);
   wxBoxSizer *m1BwdStopCtrlBrick = new wxBoxSizer(wxVERTICAL);
   m1BwdStopCtrlBrick->Add(s1BwdCtrlBox, 0, wxALL, 1);
   wxBoxSizer *m1CtrlBrick = new wxBoxSizer(wxVERTICAL);
   m1CtrlBrick->Add(m1CtrlBox, 0, wxALL, 1);
   wxBoxSizer *m1OutputLedsBrick = new wxBoxSizer(wxVERTICAL);
   m1OutputLedsBrick->Add(o1LedButton, 0, wxALL, 1);
   m1OutputLedsBrick->Add(o2LedButton, 0, wxALL, 1);
   wxBoxSizer *mot1Brick = new wxBoxSizer(wxHORIZONTAL);
   mot1Brick->Add(m1InputLedsBrick, 0, wxALIGN_CENTER | wxALL, 1);
   mot1Brick->Add(m1FwdStopCtrlBrick, 0, wxALIGN_TOP | wxALL, 1);
   mot1Brick->Add(m1BwdStopCtrlBrick, 0, wxALIGN_TOP | wxALL, 1);
   mot1Brick->Add(m1CtrlBrick, 0, wxALIGN_TOP | wxALL, 1);
   mot1Brick->Add(m1OutputLedsBrick, 0, wxALIGN_CENTER | wxALL, 1);

   // Motor 2 controls
   wxBoxSizer *m2InputLedsBrick = new wxBoxSizer(wxVERTICAL);
   m2InputLedsBrick->Add(e3LedButton, 0, wxALL, 1);
   m2InputLedsBrick->Add(e4LedButton, 0, wxALL, 1);
   wxBoxSizer *m2FwdStopCtrlBrick = new wxBoxSizer(wxVERTICAL);
   m2FwdStopCtrlBrick->Add(s2FwdCtrlBox, 0, wxALL, 1);
   wxBoxSizer *m2BwdStopCtrlBrick = new wxBoxSizer(wxVERTICAL);
   m2BwdStopCtrlBrick->Add(s2BwdCtrlBox, 0, wxALL, 1);
   wxBoxSizer *m2CtrlBrick = new wxBoxSizer(wxVERTICAL);
   m2CtrlBrick->Add(m2CtrlBox, 0, wxALL, 1);
   wxBoxSizer *m2OutputLedsBrick = new wxBoxSizer(wxVERTICAL);
   m2OutputLedsBrick->Add(o3LedButton, 0, wxALL, 1);
   m2OutputLedsBrick->Add(o4LedButton, 0, wxALL, 1);
   wxBoxSizer *mot2Brick = new wxBoxSizer(wxHORIZONTAL);
   mot2Brick->Add(m2InputLedsBrick, 0, wxALIGN_CENTER | wxALL, 1);
   mot2Brick->Add(m2FwdStopCtrlBrick, 0, wxALIGN_TOP | wxALL, 1);
   mot2Brick->Add(m2BwdStopCtrlBrick, 0, wxALIGN_TOP | wxALL, 1);
   mot2Brick->Add(m2CtrlBrick, 0, wxALIGN_TOP | wxALL, 1);
   mot2Brick->Add(m2OutputLedsBrick, 0, wxALIGN_CENTER | wxALL, 1);

   // Motor 3 controls
   wxBoxSizer *m3InputLedsBrick = new wxBoxSizer(wxVERTICAL);
   m3InputLedsBrick->Add(e5LedButton, 0, wxALL, 1);
   m3InputLedsBrick->Add(e6LedButton, 0, wxALL, 1);
   wxBoxSizer *m3FwdStopCtrlBrick = new wxBoxSizer(wxVERTICAL);
   m3FwdStopCtrlBrick->Add(s3FwdCtrlBox, 0, wxALL, 1);
   wxBoxSizer *m3BwdStopCtrlBrick = new wxBoxSizer(wxVERTICAL);
   m3BwdStopCtrlBrick->Add(s3BwdCtrlBox, 0, wxALL, 1);
   wxBoxSizer *m3CtrlBrick = new wxBoxSizer(wxVERTICAL);
   m3CtrlBrick->Add(m3CtrlBox, 0, wxALL, 1);
   wxBoxSizer *m3OutputLedsBrick = new wxBoxSizer(wxVERTICAL);
   m3OutputLedsBrick->Add(o5LedButton, 0, wxALL, 1);
   m3OutputLedsBrick->Add(o6LedButton, 0, wxALL, 1);
   wxBoxSizer *mot3Brick = new wxBoxSizer(wxHORIZONTAL);
   mot3Brick->Add(m3InputLedsBrick, 0, wxALIGN_CENTER | wxALL, 1);
   mot3Brick->Add(m3FwdStopCtrlBrick, 0, wxALIGN_TOP | wxALL, 1);
   mot3Brick->Add(m3BwdStopCtrlBrick, 0, wxALIGN_TOP | wxALL, 1);
   mot3Brick->Add(m3CtrlBrick, 0, wxALIGN_TOP | wxALL, 1);
   mot3Brick->Add(m3OutputLedsBrick, 0, wxALIGN_CENTER | wxALL, 1);

   // Motor 4 controls
   wxBoxSizer *m4InputLedsBrick = new wxBoxSizer(wxVERTICAL);
   m4InputLedsBrick->Add(e7LedButton, 0, wxALL, 1);
   m4InputLedsBrick->Add(e8LedButton, 0, wxALL, 1);
   wxBoxSizer *m4FwdStopCtrlBrick = new wxBoxSizer(wxVERTICAL);
   m4FwdStopCtrlBrick->Add(s4FwdCtrlBox, 0, wxALL, 1);
   wxBoxSizer *m4BwdStopCtrlBrick = new wxBoxSizer(wxVERTICAL);
   m4BwdStopCtrlBrick->Add(s4BwdCtrlBox, 0, wxALL, 1);
   wxBoxSizer *m4CtrlBrick = new wxBoxSizer(wxVERTICAL);
   m4CtrlBrick->Add(m4CtrlBox, 0, wxALL, 1);
   wxBoxSizer *m4OutputLedsBrick = new wxBoxSizer(wxVERTICAL);
   m4OutputLedsBrick->Add(o7LedButton, 0, wxALL, 1);
   m4OutputLedsBrick->Add(o8LedButton, 0, wxALL, 1);
   wxBoxSizer *mot4Brick = new wxBoxSizer(wxHORIZONTAL);
   mot4Brick->Add(m4InputLedsBrick, 0, wxALIGN_CENTER | wxALL, 1);
   mot4Brick->Add(m4FwdStopCtrlBrick, 0, wxALIGN_TOP | wxALL, 1);
   mot4Brick->Add(m4BwdStopCtrlBrick, 0, wxALIGN_TOP | wxALL, 1);
   mot4Brick->Add(m4CtrlBrick, 0, wxALIGN_TOP | wxALL, 1);
   mot4Brick->Add(m4OutputLedsBrick, 0, wxALIGN_CENTER | wxALL, 1);

   // Analog Inputs
   wxBoxSizer *analogBrick = new wxBoxSizer(wxHORIZONTAL);
   analogBrick->Add(exInputGauge, 0, wxALL, 1);
   analogBrick->Add(8, 1, 0, wxALL, 1);
   analogBrick->Add(eyInputGauge, 0, wxALL, 1);

   // assemble client area
   wxBoxSizer *allBricks = new wxBoxSizer(wxVERTICAL);
   allBricks->Add(mot1Brick, 0, wxALL, 0);
   allBricks->Add(mot2Brick, 0, wxALL, 0);
   allBricks->Add(mot3Brick, 0, wxALL, 0);
   allBricks->Add(mot4Brick, 0, wxALL, 0);
   allBricks->Add(analogBrick, 0, wxALL, 0);
   SetSizer(allBricks);

   // adjust size(s)
   int minFrmWidth = 0;
   int minFrmHeight = 0;
   int widthE;
   int heightE;
   e1LedButton->GetSize(&widthE, &heightE);
   minFrmWidth += (widthE+2);
   int widthS;
   int heightS;
   s1FwdCtrlBox->GetSize(&widthS, &heightS);
   minFrmWidth += (widthS+2);
   minFrmHeight += 4 * (heightS+2);
   s1BwdCtrlBox->GetSize(&widthS, &heightS);
   minFrmWidth += (widthS+2);
   int widthM;
   int heightM;
   m1CtrlBox->GetSize(&widthM, &heightM);
   minFrmWidth += (widthM+2);
   int widthO;
   int heightO;
   o1LedButton->GetSize(&widthO, &heightO);
   minFrmWidth += (widthO+2);
   int widthA;
   int heightA;
   exInputGauge->GetSize(&widthA, &heightA);
   minFrmHeight += (heightA+2);
   SetMinSize(wxSize(minFrmWidth+28, minFrmHeight+110));
   SetMaxSize(wxSize(minFrmWidth+28, minFrmHeight+110));

   SetBackgroundColour(wxColour(0xA0, 0xA0, 0xA0, wxALPHA_OPAQUE));
   DisableAllButtons();

   // bind the event handlers for this application
   Bind(wxEVT_COMMAND_MENU_SELECTED, &ftcDemoFrame::OnAbout, this, idMenuAbout);
   Bind(wxEVT_COMMAND_MENU_SELECTED, &ftcDemoFrame::OnQuit, this, idMenuQuit);
   Bind(wxEVT_CLOSE_WINDOW, &ftcDemoFrame::OnClose, this);

// Bind(wxEVT_COMMAND_BUTTON_CLICKED, &ftcDemoFrame::OnO1LedButton, this, idO1LedButton);
// Bind(wxEVT_COMMAND_BUTTON_CLICKED, &ftcDemoFrame::OnO2LedButton, this, idO2LedButton);
// Bind(wxEVT_COMMAND_BUTTON_CLICKED, &ftcDemoFrame::OnO3LedButton, this, idO3LedButton);
// Bind(wxEVT_COMMAND_BUTTON_CLICKED, &ftcDemoFrame::OnO4LedButton, this, idO4LedButton);
// Bind(wxEVT_COMMAND_BUTTON_CLICKED, &ftcDemoFrame::OnO5LedButton, this, idO5LedButton);
// Bind(wxEVT_COMMAND_BUTTON_CLICKED, &ftcDemoFrame::OnO6LedButton, this, idO6LedButton);
// Bind(wxEVT_COMMAND_BUTTON_CLICKED, &ftcDemoFrame::OnO7LedButton, this, idO7LedButton);
// Bind(wxEVT_COMMAND_BUTTON_CLICKED, &ftcDemoFrame::OnO8LedButton, this, idO8LedButton);

   Bind(wxEVT_RADIOBOX, &ftcDemoFrame::OnM1CtrlBox, this, idM1ControlBox);
   Bind(wxEVT_RADIOBOX, &ftcDemoFrame::OnM2CtrlBox, this, idM2ControlBox);
   Bind(wxEVT_RADIOBOX, &ftcDemoFrame::OnM3CtrlBox, this, idM3ControlBox);
   Bind(wxEVT_RADIOBOX, &ftcDemoFrame::OnM4CtrlBox, this, idM4ControlBox);
   Bind(wxEVT_RADIOBOX, &ftcDemoFrame::OnS1FwdCtrlBox, this, idStop1FwdCtrlBox);
   Bind(wxEVT_RADIOBOX, &ftcDemoFrame::OnS2FwdCtrlBox, this, idStop2FwdCtrlBox);
   Bind(wxEVT_RADIOBOX, &ftcDemoFrame::OnS3FwdCtrlBox, this, idStop3FwdCtrlBox);
   Bind(wxEVT_RADIOBOX, &ftcDemoFrame::OnS4FwdCtrlBox, this, idStop4FwdCtrlBox);
   Bind(wxEVT_RADIOBOX, &ftcDemoFrame::OnS1BwdCtrlBox, this, idStop1BwdCtrlBox);
   Bind(wxEVT_RADIOBOX, &ftcDemoFrame::OnS2BwdCtrlBox, this, idStop2BwdCtrlBox);
   Bind(wxEVT_RADIOBOX, &ftcDemoFrame::OnS3BwdCtrlBox, this, idStop3BwdCtrlBox);
   Bind(wxEVT_RADIOBOX, &ftcDemoFrame::OnS4BwdCtrlBox, this, idStop4BwdCtrlBox);

   Bind(wxEVT_THREAD, &ftcDemoFrame::OnConnection, this, ftConnection::idEvtConnect);
   Bind(wxEVT_THREAD, &ftcDemoFrame::OnDisconnection, this, ftConnection::idEvtDisconnect);
   Bind(wxEVT_THREAD, &ftcDemoFrame::OnTick, this, ftConnection::idEvtTick);
   Bind(wxEVT_THREAD, &ftcDemoFrame::OnSwitchFeedback, this, ftConnection::idEvtInputsChg);
   Bind(wxEVT_THREAD, &ftcDemoFrame::OnExChange, this, ftConnection::idEvtResiXChg);
   Bind(wxEVT_THREAD, &ftcDemoFrame::OnEyChange, this, ftConnection::idEvtResiYChg);

   Bind(myMOTOR_EVENT, &ftcDemoFrame::OnM1Change, this, ftConnection::idEvtM1StatChg);
   Bind(myMOTOR_EVENT, &ftcDemoFrame::OnM2Change, this, ftConnection::idEvtM2StatChg);
   Bind(myMOTOR_EVENT, &ftcDemoFrame::OnM3Change, this, ftConnection::idEvtM3StatChg);
   Bind(myMOTOR_EVENT, &ftcDemoFrame::OnM4Change, this, ftConnection::idEvtM4StatChg);

   // try to get connection to the interface(s), no issue if device still detached
   // will automatically fire events on device arrival and departure
   ifBox = new ftConnection(this);
   ftConnection::failureCode_t fcode = ifBox->OpenConnection();
   switch(fcode)
   {
      case ftConnection::fcNTF:
         break;
      case ftConnection::fcNoDevice:
         break;
      case ftConnection::fcThreadIssue:
         wxLogError("Can't create interface communication thread!\nProbably system resources are exhausted.");
         break;
      case ftConnection::fcNoComLib:
         wxLogError("Issue when opening library '%s'.", ifBox->GetLibVersion());
         break;
      case ftConnection::fcNotInit:
         wxLogError("Interface device does not initialize.");
         break;
      default:
         wxLogError("Unknown failure code returns when trying to establish communication " \
                    "with the interface device!\nFailure code returned: %i", fcode);
   }
}


ftcDemoFrame::~ftcDemoFrame()
{
   ifBox->CloseConnection();
   SetStatusBar(NULL);
   delete my_status;
}


void ftcDemoFrame::OnClose(wxCloseEvent &event)
{
   Destroy();
}


void ftcDemoFrame::OnQuit(wxCommandEvent &event)
{
   Destroy();
}


void ftcDemoFrame::OnAbout(wxCommandEvent &event)
{
   wxString wxWidgetsVersion = wxbuildinfo();
   wxString hidapiVersion = ifBox->GetLibVersion();
   wxString about;
   about << _("Hi out there!\n" \
              "I am an experimental C++ GUI application.\n" \
              "My purpose is to control a fischertechnik / CVK Computing " \
              "Interface by a dedicated USB-HID class device.\n" \
              "I have been made with the Code::Blocks IDE 20.03 using "
             );
   about << hidapiVersion;
   about << _(" and a ");
   about << wxWidgetsVersion;
   about << _(". My source is open to the public. I am under a GPLv3 license.");
   about << _(".");
   wxMessageBox(about, _("About This"));
}


void ftcDemoFrame::OnConnection(wxThreadEvent &event)
{
   // connected
   my_status->SetConnectStatus(true);
   EnableAllButtons();
}


void ftcDemoFrame::OnDisconnection(wxThreadEvent &event)
{
   // not connected / connection lost
   my_status->SetConnectStatus(false);
   DisableAllButtons();
}


void ftcDemoFrame::OnTick(wxThreadEvent &event)
{
   // triggered more or less regularly from ifBox
   // perform here whatever needs to get performed on a regular base
   // without receiving other events
}


void ftcDemoFrame::OnO1LedButton(wxCommandEvent& event)
{
   if (o1LedButton->GetState() == MyLedButton::LED_Off)
      SetStateOfO1LedButton(true);
   else
      SetStateOfO1LedButton(false);
}


void ftcDemoFrame::OnO2LedButton(wxCommandEvent& event)
{
   if (o2LedButton->GetState() == MyLedButton::LED_Off)
      SetStateOfO2LedButton(true);
   else
      SetStateOfO2LedButton(false);
}


void ftcDemoFrame::OnO3LedButton(wxCommandEvent& event)
{
   if (o3LedButton->GetState() == MyLedButton::LED_Off)
      SetStateOfO3LedButton(true);
   else
      SetStateOfO3LedButton(false);
}


void ftcDemoFrame::OnO4LedButton(wxCommandEvent& event)
{
   if (o4LedButton->GetState() == MyLedButton::LED_Off)
      SetStateOfO4LedButton(true);
   else
      SetStateOfO4LedButton(false);
}


void ftcDemoFrame::OnO5LedButton(wxCommandEvent& event)
{
   if (o5LedButton->GetState() == MyLedButton::LED_Off)
      SetStateOfO5LedButton(true);
   else
      SetStateOfO5LedButton(false);
}


void ftcDemoFrame::OnO6LedButton(wxCommandEvent& event)
{
   if (o6LedButton->GetState() == MyLedButton::LED_Off)
      SetStateOfO6LedButton(true);
   else
      SetStateOfO6LedButton(false);
}


void ftcDemoFrame::OnO7LedButton(wxCommandEvent& event)
{
   if (o7LedButton->GetState() == MyLedButton::LED_Off)
      SetStateOfO7LedButton(true);
   else
      SetStateOfO7LedButton(false);
}


void ftcDemoFrame::OnO8LedButton(wxCommandEvent& event)
{
   if (o8LedButton->GetState() == MyLedButton::LED_Off)
      SetStateOfO8LedButton(true);
   else
      SetStateOfO8LedButton(false);
}


void ftcDemoFrame::OnM1CtrlBox(wxCommandEvent& event)
{
   MyMotor::driveState_t drv = CtrlBox2Motor(m1CtrlBox->GetState());
   drv = ifBox->mot1->SetState(drv);
   m1CtrlBox->SetState(Motor2CtrlBox(drv));
   ifBox->SetMotor(ftConnection::MOT1);
   switch (drv)
   {
      case MyMotor::IDLE: ;
         SetStateOfO1LedButton(false);
         SetStateOfO2LedButton(false);
         break;
      case MyMotor::FWD: ;
         SetStateOfO1LedButton(true);
         SetStateOfO2LedButton(false);
         break;
      case MyMotor::BWD: ;
         SetStateOfO1LedButton(false);
         SetStateOfO2LedButton(true);
         break;
      case MyMotor::BRAKE: ;
         SetStateOfO1LedButton(true);
         SetStateOfO2LedButton(true);
         break;
      default: ;
         SetStateOfO1LedButton(false);
         SetStateOfO2LedButton(false);
   }
}


void ftcDemoFrame::OnM2CtrlBox(wxCommandEvent& event)
{
   MyMotor::driveState_t drv = CtrlBox2Motor(m2CtrlBox->GetState());
   drv = ifBox->mot2->SetState(drv);
   m2CtrlBox->SetState(Motor2CtrlBox(drv));
   ifBox->SetMotor(ftConnection::MOT2);
   switch (drv)
   {
      case MyMotor::IDLE: ;
         SetStateOfO3LedButton(false);
         SetStateOfO4LedButton(false);
         break;
      case MyMotor::FWD: ;
         SetStateOfO3LedButton(true);
         SetStateOfO4LedButton(false);
         break;
      case MyMotor::BWD: ;
         SetStateOfO3LedButton(false);
         SetStateOfO4LedButton(true);
         break;
      case MyMotor::BRAKE: ;
         SetStateOfO3LedButton(true);
         SetStateOfO4LedButton(true);
         break;
      default: ;
         SetStateOfO3LedButton(false);
         SetStateOfO4LedButton(false);
   }
}


void ftcDemoFrame::OnM3CtrlBox(wxCommandEvent& event)
{
   MyMotor::driveState_t drv = CtrlBox2Motor(m3CtrlBox->GetState());
   drv = ifBox->mot3->SetState(drv);
   m3CtrlBox->SetState(Motor2CtrlBox(drv));
   ifBox->SetMotor(ftConnection::MOT3);
   switch (drv)
   {
      case MyMotor::IDLE: ;
         SetStateOfO5LedButton(false);
         SetStateOfO6LedButton(false);
         break;
      case MyMotor::FWD: ;
         SetStateOfO5LedButton(true);
         SetStateOfO6LedButton(false);
         break;
      case MyMotor::BWD: ;
         SetStateOfO5LedButton(false);
         SetStateOfO6LedButton(true);
         break;
      case MyMotor::BRAKE: ;
         SetStateOfO5LedButton(true);
         SetStateOfO6LedButton(true);
         break;
      default: ;
         SetStateOfO5LedButton(false);
         SetStateOfO6LedButton(false);
   }
}


void ftcDemoFrame::OnM4CtrlBox(wxCommandEvent& event)
{
   MyMotor::driveState_t drv = CtrlBox2Motor(m4CtrlBox->GetState());
   drv = ifBox->mot4->SetState(drv);
   m4CtrlBox->SetState(Motor2CtrlBox(drv));
   ifBox->SetMotor(ftConnection::MOT4);
   switch (drv)
   {
      case MyMotor::IDLE: ;
         SetStateOfO7LedButton(false);
         SetStateOfO8LedButton(false);
         break;
      case MyMotor::FWD: ;
         SetStateOfO7LedButton(true);
         SetStateOfO8LedButton(false);
         break;
      case MyMotor::BWD: ;
         SetStateOfO7LedButton(false);
         SetStateOfO8LedButton(true);
         break;
      case MyMotor::BRAKE: ;
         SetStateOfO7LedButton(true);
         SetStateOfO8LedButton(true);
         break;
      default: ;
         SetStateOfO7LedButton(false);
         SetStateOfO8LedButton(false);
   }
}


void ftcDemoFrame::OnS1FwdCtrlBox(wxCommandEvent& event)
{
   MyMotor::stopCtrlDef_t fwd = StopBox2Motor(s1FwdCtrlBox->GetState());
// ToDo: Auf Auswahlfeld umstellen und Sicherung gegen unerlaubtes CMD
   ifBox->mot1->SetFwdStopConfig(fwd, MyMotor::IDLE);
   ifBox->PrepareStopSwitches();
}


void ftcDemoFrame::OnS2FwdCtrlBox(wxCommandEvent& event)
{
   MyMotor::stopCtrlDef_t fwd = StopBox2Motor(s2FwdCtrlBox->GetState());
// ToDo: Auf Auswahlfeld umstellen und Sicherung gegen unerlaubtes CMD
   ifBox->mot2->SetFwdStopConfig(fwd, MyMotor::IDLE);
   ifBox->PrepareStopSwitches();
}


void ftcDemoFrame::OnS3FwdCtrlBox(wxCommandEvent& event)
{
   MyMotor::stopCtrlDef_t fwd = StopBox2Motor(s3FwdCtrlBox->GetState());
// ToDo: Auf Auswahlfeld umstellen und Sicherung gegen unerlaubtes CMD
   ifBox->mot3->SetFwdStopConfig(fwd, MyMotor::IDLE);
   ifBox->PrepareStopSwitches();
}


void ftcDemoFrame::OnS4FwdCtrlBox(wxCommandEvent& event)
{
   MyMotor::stopCtrlDef_t fwd = StopBox2Motor(s4FwdCtrlBox->GetState());
// ToDo: Auf Auswahlfeld umstellen und Sicherung gegen unerlaubtes CMD
   ifBox->mot4->SetFwdStopConfig(fwd, MyMotor::IDLE);
   ifBox->PrepareStopSwitches();
}


void ftcDemoFrame::OnS1BwdCtrlBox(wxCommandEvent& event)
{
   MyMotor::stopCtrlDef_t bwd = StopBox2Motor(s1BwdCtrlBox->GetState());
// ToDo: Auf Auswahlfeld umstellen und Sicherung gegen unerlaubtes CMD
   ifBox->mot1->SetBwdStopConfig(bwd, MyMotor::IDLE);
   ifBox->PrepareStopSwitches();
}


void ftcDemoFrame::OnS2BwdCtrlBox(wxCommandEvent& event)
{
   MyMotor::stopCtrlDef_t bwd = StopBox2Motor(s2BwdCtrlBox->GetState());
// ToDo: Auf Auswahlfeld umstellen und Sicherung gegen unerlaubtes CMD
   ifBox->mot2->SetBwdStopConfig(bwd, MyMotor::IDLE);
   ifBox->PrepareStopSwitches();
}


void ftcDemoFrame::OnS3BwdCtrlBox(wxCommandEvent& event)
{
   MyMotor::stopCtrlDef_t bwd = StopBox2Motor(s3BwdCtrlBox->GetState());
// ToDo: Auf Auswahlfeld umstellen und Sicherung gegen unerlaubtes CMD
   ifBox->mot3->SetBwdStopConfig(bwd, MyMotor::IDLE);
   ifBox->PrepareStopSwitches();
}


void ftcDemoFrame::OnS4BwdCtrlBox(wxCommandEvent& event)
{
   MyMotor::stopCtrlDef_t bwd = StopBox2Motor(s4BwdCtrlBox->GetState());
// ToDo: Auf Auswahlfeld umstellen und Sicherung gegen unerlaubtes CMD
   ifBox->mot4->SetBwdStopConfig(bwd, MyMotor::IDLE);
   ifBox->PrepareStopSwitches();
}


void ftcDemoFrame::OnM1Change(wxCommandEvent& event)
{
   MyMotor::driveState_t drv = ifBox->mot1->GetState();
   m1CtrlBox->SetState(Motor2CtrlBox(drv));
   switch (drv)
   {
      case MyMotor::IDLE: ;
         SetStateOfO1LedButton(false);
         SetStateOfO2LedButton(false);
         break;
      case MyMotor::FWD: ;
         SetStateOfO1LedButton(true);
         SetStateOfO2LedButton(false);
         break;
      case MyMotor::BWD: ;
         SetStateOfO1LedButton(false);
         SetStateOfO2LedButton(true);
         break;
      case MyMotor::BRAKE: ;
         SetStateOfO1LedButton(true);
         SetStateOfO2LedButton(true);
         break;
      default: ;
         SetStateOfO1LedButton(false);
         SetStateOfO2LedButton(false);
   }
}


void ftcDemoFrame::OnM2Change(wxCommandEvent& event)
{
   MyMotor::driveState_t drv = ifBox->mot2->GetState();
   m2CtrlBox->SetState(Motor2CtrlBox(drv));
   switch (drv)
   {
      case MyMotor::IDLE: ;
         SetStateOfO3LedButton(false);
         SetStateOfO4LedButton(false);
         break;
      case MyMotor::FWD: ;
         SetStateOfO3LedButton(true);
         SetStateOfO4LedButton(false);
         break;
      case MyMotor::BWD: ;
         SetStateOfO3LedButton(false);
         SetStateOfO4LedButton(true);
         break;
      case MyMotor::BRAKE: ;
         SetStateOfO3LedButton(true);
         SetStateOfO4LedButton(true);
         break;
      default: ;
         SetStateOfO3LedButton(false);
         SetStateOfO4LedButton(false);
   }
}


void ftcDemoFrame::OnM3Change(wxCommandEvent& event)
{
   MyMotor::driveState_t drv = ifBox->mot3->GetState();
   m3CtrlBox->SetState(Motor2CtrlBox(drv));
   switch (drv)
   {
      case MyMotor::IDLE: ;
         SetStateOfO5LedButton(false);
         SetStateOfO6LedButton(false);
         break;
      case MyMotor::FWD: ;
         SetStateOfO5LedButton(true);
         SetStateOfO6LedButton(false);
         break;
      case MyMotor::BWD: ;
         SetStateOfO5LedButton(false);
         SetStateOfO6LedButton(true);
         break;
      case MyMotor::BRAKE: ;
         SetStateOfO5LedButton(true);
         SetStateOfO6LedButton(true);
         break;
      default: ;
         SetStateOfO5LedButton(false);
         SetStateOfO6LedButton(false);
   }
}


void ftcDemoFrame::OnM4Change(wxCommandEvent& event)
{
   MyMotor::driveState_t drv = ifBox->mot4->GetState();
   m4CtrlBox->SetState(Motor2CtrlBox(drv));
   switch (drv)
   {
      case MyMotor::IDLE: ;
         SetStateOfO7LedButton(false);
         SetStateOfO8LedButton(false);
         break;
      case MyMotor::FWD: ;
         SetStateOfO7LedButton(true);
         SetStateOfO8LedButton(false);
         break;
      case MyMotor::BWD: ;
         SetStateOfO7LedButton(false);
         SetStateOfO8LedButton(true);
         break;
      case MyMotor::BRAKE: ;
         SetStateOfO7LedButton(true);
         SetStateOfO8LedButton(true);
         break;
      default: ;
         SetStateOfO7LedButton(false);
         SetStateOfO8LedButton(false);
   }
}


void ftcDemoFrame::OnSwitchFeedback(wxThreadEvent& event)
{
   SetStateOfInLedButtons(event.GetInt() & 0xFF);
}


void ftcDemoFrame::OnExChange(wxThreadEvent& event)
{
   exInputGauge->SetValue(event.GetInt());
}


void ftcDemoFrame::OnEyChange(wxThreadEvent& event)
{
   eyInputGauge->SetValue(event.GetInt());
}


void ftcDemoFrame::DisableAllButtons()
{
   e1LedButton->Disable();
   e2LedButton->Disable();
   e3LedButton->Disable();
   e4LedButton->Disable();
   e5LedButton->Disable();
   e6LedButton->Disable();
   e7LedButton->Disable();
   e8LedButton->Disable();
   exInputGauge->Disable();
   eyInputGauge->Disable();
   m1CtrlBox->Disable();
   m2CtrlBox->Disable();
   m3CtrlBox->Disable();
   m4CtrlBox->Disable();
   o1LedButton->Disable();
   o2LedButton->Disable();
   o3LedButton->Disable();
   o4LedButton->Disable();
   o5LedButton->Disable();
   o6LedButton->Disable();
   o7LedButton->Disable();
   o8LedButton->Disable();
   s1FwdCtrlBox->Disable();
   s2FwdCtrlBox->Disable();
   s3FwdCtrlBox->Disable();
   s4FwdCtrlBox->Disable();
   s1BwdCtrlBox->Disable();
   s2BwdCtrlBox->Disable();
   s3BwdCtrlBox->Disable();
   s4BwdCtrlBox->Disable();
}


void ftcDemoFrame::EnableAllButtons()
{
   e1LedButton->Enable();
   e2LedButton->Enable();
   e3LedButton->Enable();
   e4LedButton->Enable();
   e5LedButton->Enable();
   e6LedButton->Enable();
   e7LedButton->Enable();
   e8LedButton->Enable();
   exInputGauge->Enable();
   eyInputGauge->Enable();
   m1CtrlBox->Enable();
   m2CtrlBox->Enable();
   m3CtrlBox->Enable();
   m4CtrlBox->Enable();
   o1LedButton->Enable();
   o2LedButton->Enable();
   o3LedButton->Enable();
   o4LedButton->Enable();
   o5LedButton->Enable();
   o6LedButton->Enable();
   o7LedButton->Enable();
   o8LedButton->Enable();
   s1FwdCtrlBox->Enable();
   s2FwdCtrlBox->Enable();
   s3FwdCtrlBox->Enable();
   s4FwdCtrlBox->Enable();
   s1BwdCtrlBox->Enable();
   s2BwdCtrlBox->Enable();
   s3BwdCtrlBox->Enable();
   s4BwdCtrlBox->Enable();
}


void ftcDemoFrame::SetStateOfInLedButtons(int state)
{
   SetStateOfE1LedButton(state & (1 << 0));
   SetStateOfE2LedButton(state & (1 << 1));
   SetStateOfE3LedButton(state & (1 << 2));
   SetStateOfE4LedButton(state & (1 << 3));
   SetStateOfE5LedButton(state & (1 << 4));
   SetStateOfE6LedButton(state & (1 << 5));
   SetStateOfE7LedButton(state & (1 << 6));
   SetStateOfE8LedButton(state & (1 << 7));
}


void ftcDemoFrame::SetStateOfE1LedButton(bool state)
{
   e1LedButton->SetState(state ? MyLedButton::LED_Red : MyLedButton::LED_Off);
}


void ftcDemoFrame::SetStateOfE2LedButton(bool state)
{
   e2LedButton->SetState(state ? MyLedButton::LED_Red : MyLedButton::LED_Off);
}


void ftcDemoFrame::SetStateOfE3LedButton(bool state)
{
   e3LedButton->SetState(state ? MyLedButton::LED_Red : MyLedButton::LED_Off);
}


void ftcDemoFrame::SetStateOfE4LedButton(bool state)
{
   e4LedButton->SetState(state ? MyLedButton::LED_Red : MyLedButton::LED_Off);
}


void ftcDemoFrame::SetStateOfE5LedButton(bool state)
{
   e5LedButton->SetState(state ? MyLedButton::LED_Red : MyLedButton::LED_Off);
}


void ftcDemoFrame::SetStateOfE6LedButton(bool state)
{
   e6LedButton->SetState(state ? MyLedButton::LED_Red : MyLedButton::LED_Off);
}


void ftcDemoFrame::SetStateOfE7LedButton(bool state)
{
   e7LedButton->SetState(state ? MyLedButton::LED_Red : MyLedButton::LED_Off);
}


void ftcDemoFrame::SetStateOfE8LedButton(bool state)
{
   e8LedButton->SetState(state ? MyLedButton::LED_Red : MyLedButton::LED_Off);
}


void ftcDemoFrame::SetStateOfOutLedButtons(int state)
{
   SetStateOfO1LedButton(state & (1 << 0));
   SetStateOfO2LedButton(state & (1 << 1));
   SetStateOfO3LedButton(state & (1 << 2));
   SetStateOfO4LedButton(state & (1 << 3));
   SetStateOfO5LedButton(state & (1 << 4));
   SetStateOfO6LedButton(state & (1 << 5));
   SetStateOfO7LedButton(state & (1 << 6));
   SetStateOfO8LedButton(state & (1 << 7));
}


void ftcDemoFrame::SetStateOfO1LedButton(bool state)
{
   o1LedButton->SetState(state ? MyLedButton::LED_Green : MyLedButton::LED_Off);
}


void ftcDemoFrame::SetStateOfO2LedButton(bool state)
{
   o2LedButton->SetState(state ? MyLedButton::LED_Red : MyLedButton::LED_Off);
}


void ftcDemoFrame::SetStateOfO3LedButton(bool state)
{
   o3LedButton->SetState(state ? MyLedButton::LED_Green : MyLedButton::LED_Off);
}


void ftcDemoFrame::SetStateOfO4LedButton(bool state)
{
   o4LedButton->SetState(state ? MyLedButton::LED_Red : MyLedButton::LED_Off);
}


void ftcDemoFrame::SetStateOfO5LedButton(bool state)
{
   o5LedButton->SetState(state ? MyLedButton::LED_Green : MyLedButton::LED_Off);
}


void ftcDemoFrame::SetStateOfO6LedButton(bool state)
{
   o6LedButton->SetState(state ? MyLedButton::LED_Red : MyLedButton::LED_Off);
}


void ftcDemoFrame::SetStateOfO7LedButton(bool state)
{
   o7LedButton->SetState(state ? MyLedButton::LED_Green : MyLedButton::LED_Off);
}


void ftcDemoFrame::SetStateOfO8LedButton(bool state)
{
   o8LedButton->SetState(state ? MyLedButton::LED_Red : MyLedButton::LED_Off);
}


MyMotor::driveState_t ftcDemoFrame::CtrlBox2Motor(MyMotorCtrlButton::MyMotorCtrlState_t sel)
{
   MyMotor::driveState_t drv;
   switch(sel)
   {
      case MyMotorCtrlButton::MOTOR_DRV_IDLE:
         drv = MyMotor::IDLE;
         break;
      case MyMotorCtrlButton::MOTOR_DRV_BRAKE:
         drv = MyMotor::BRAKE;
         break;
      case MyMotorCtrlButton::MOTOR_DRV_FWD:
         drv = MyMotor::FWD;
         break;
      case MyMotorCtrlButton::MOTOR_DRV_BWD:
         drv = MyMotor::BWD;
         break;
      default:
         drv = MyMotor::IDLE;
   }
   return drv;
}


MyMotorCtrlButton::MyMotorCtrlState_t ftcDemoFrame::Motor2CtrlBox(MyMotor::driveState_t drv)
{
   MyMotorCtrlButton::MyMotorCtrlState_t sel;
   switch(drv)
   {
      case MyMotor::IDLE:
         sel = MyMotorCtrlButton::MOTOR_DRV_IDLE;
         break;
      case MyMotor::BRAKE:
         sel = MyMotorCtrlButton::MOTOR_DRV_BRAKE;
         break;
      case MyMotor::FWD:
         sel = MyMotorCtrlButton::MOTOR_DRV_FWD;
         break;
      case MyMotor::BWD:
         sel = MyMotorCtrlButton::MOTOR_DRV_BWD;
         break;
      default:
         sel = MyMotorCtrlButton::MOTOR_DRV_IDLE;
   }
   return sel;
}


MyMotor::stopCtrlDef_t ftcDemoFrame::StopBox2Motor(MyStopCtrlButton::MyStopCtrlState_t sel)
{
   MyMotor::stopCtrlDef_t stop;
   switch(sel)
   {
      case MyStopCtrlButton::STOP_SW_ON_HIGH:
         stop = MyMotor::STOP_ON_HI;
         break;
      case MyStopCtrlButton::STOP_SW_NEVER:
         stop = MyMotor::STOP_ON_NONE;
         break;
      case MyStopCtrlButton::STOP_SW_ON_LOW:
         stop = MyMotor::STOP_ON_LO;
         break;
      default:
         stop = MyMotor::STOP_ON_NONE;
   }
   return stop;
}


MyStopCtrlButton::MyStopCtrlState_t ftcDemoFrame::Motor2StopBox(MyMotor::stopCtrlDef_t stop)
{
   MyStopCtrlButton::MyStopCtrlState_t sel;
   switch(stop)
   {
      case MyMotor::STOP_ON_HI:
         sel = MyStopCtrlButton::STOP_SW_ON_HIGH;
         break;
      case MyMotor::STOP_ON_NONE:
         sel = MyStopCtrlButton::STOP_SW_NEVER;
         break;
      case MyMotor::STOP_ON_LO:
         sel = MyStopCtrlButton::STOP_SW_ON_LOW;
         break;
      default:
         sel = MyStopCtrlButton::STOP_SW_NEVER;
   }
   return sel;
}
