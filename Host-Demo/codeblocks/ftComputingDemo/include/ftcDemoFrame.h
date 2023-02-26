/***************************************************************
 * Name:      ftcDemoFrame.h
 * Purpose:   Defines Application Frame
 * Author:    R. Trapp (rene <dot> trapp [ät] web <dot> de)
 * Created:   2023-01-19
 * Copyright: R. Trapp ()
 * License:   GPLv3 - see License.txt
 **************************************************************/
// worker thread taken from:
//    wxWidgets samples/thread.cpp
// detection of device arrival / removal:
//    https://forums.wxwidgets.org/viewtopic.php?t=12138
//    https://stackoverflow.com/questions/38430587/any-windows-api-for-detecting-mtp-devices/68851782

#ifndef FTCDEMOFRAME_H
#define FTCDEMOFRAME_H

#ifdef WX_PRECOMP
   #include "wx_pch.h"
#else
   #include <wx/wx.h>
#endif

#include "ftcDemoApp.h"
#include "ftConnection.h"
#include "MyGauge.h"
#include "MyLedButton.h"
#include "MyMotor.h"
#include "MyMotorCtrlButton.h"
#include "MyStatusBar.h"
#include "MyStopCtrlButton.h"


class ftcDemoFrame: public wxFrame
{
   public:
      ftcDemoFrame(wxFrame *frame, const wxString& title);
      virtual ~ftcDemoFrame();

   private:
      enum
      {
         idMenuQuit = ftConnection::idEvtftConnectionHighest+1,
         idMenuAbout,

         idE1LedButton,
         idE2LedButton,
         idE3LedButton,
         idE4LedButton,
         idE5LedButton,
         idE6LedButton,
         idE7LedButton,
         idE8LedButton,

         idExButton,
         idEyButton,

         idO1LedButton,
         idO2LedButton,
         idO3LedButton,
         idO4LedButton,
         idO5LedButton,
         idO6LedButton,
         idO7LedButton,
         idO8LedButton,

         idM1ControlBox,
         idM2ControlBox,
         idM3ControlBox,
         idM4ControlBox,

         idStop1FwdCtrlBox,
         idStop2FwdCtrlBox,
         idStop3FwdCtrlBox,
         idStop4FwdCtrlBox,

         idStop1BwdCtrlBox,
         idStop2BwdCtrlBox,
         idStop3BwdCtrlBox,
         idStop4BwdCtrlBox
      };

      ftConnection*        ifBox;

      MyGauge*             exInputGauge;
      MyGauge*             eyInputGauge;
      MyLedButton*         e1LedButton;
      MyLedButton*         e2LedButton;
      MyLedButton*         e3LedButton;
      MyLedButton*         e4LedButton;
      MyLedButton*         e5LedButton;
      MyLedButton*         e6LedButton;
      MyLedButton*         e7LedButton;
      MyLedButton*         e8LedButton;
      MyLedButton*         o1LedButton;
      MyLedButton*         o2LedButton;
      MyLedButton*         o3LedButton;
      MyLedButton*         o4LedButton;
      MyLedButton*         o5LedButton;
      MyLedButton*         o6LedButton;
      MyLedButton*         o7LedButton;
      MyLedButton*         o8LedButton;
      MyMotorCtrlButton*   m1CtrlBox;
      MyMotorCtrlButton*   m2CtrlBox;
      MyMotorCtrlButton*   m3CtrlBox;
      MyMotorCtrlButton*   m4CtrlBox;
      MyStatusBar*         my_status;
      MyStopCtrlButton*    s1FwdCtrlBox;
      MyStopCtrlButton*    s2FwdCtrlBox;
      MyStopCtrlButton*    s3FwdCtrlBox;
      MyStopCtrlButton*    s4FwdCtrlBox;
      MyStopCtrlButton*    s1BwdCtrlBox;
      MyStopCtrlButton*    s2BwdCtrlBox;
      MyStopCtrlButton*    s3BwdCtrlBox;
      MyStopCtrlButton*    s4BwdCtrlBox;

      void OnClose(wxCloseEvent& event);
      void OnQuit(wxCommandEvent& event);
      void OnAbout(wxCommandEvent& event);
      void OnConnection(wxThreadEvent& event);
      void OnDisconnection(wxThreadEvent& event);
      void OnTick(wxThreadEvent& event);

      void OnO1LedButton(wxCommandEvent& event);
      void OnO2LedButton(wxCommandEvent& event);
      void OnO3LedButton(wxCommandEvent& event);
      void OnO4LedButton(wxCommandEvent& event);
      void OnO5LedButton(wxCommandEvent& event);
      void OnO6LedButton(wxCommandEvent& event);
      void OnO7LedButton(wxCommandEvent& event);
      void OnO8LedButton(wxCommandEvent& event);

      void OnM1CtrlBox(wxCommandEvent& event);
      void OnM2CtrlBox(wxCommandEvent& event);
      void OnM3CtrlBox(wxCommandEvent& event);
      void OnM4CtrlBox(wxCommandEvent& event);

      void OnS1FwdCtrlBox(wxCommandEvent& event);
      void OnS2FwdCtrlBox(wxCommandEvent& event);
      void OnS3FwdCtrlBox(wxCommandEvent& event);
      void OnS4FwdCtrlBox(wxCommandEvent& event);

      void OnS1BwdCtrlBox(wxCommandEvent& event);
      void OnS2BwdCtrlBox(wxCommandEvent& event);
      void OnS3BwdCtrlBox(wxCommandEvent& event);
      void OnS4BwdCtrlBox(wxCommandEvent& event);

      void OnM1Change(wxCommandEvent& event);
      void OnM2Change(wxCommandEvent& event);
      void OnM3Change(wxCommandEvent& event);
      void OnM4Change(wxCommandEvent& event);

      void OnSwitchFeedback(wxThreadEvent& event);

      void OnExChange(wxThreadEvent& event);
      void OnEyChange(wxThreadEvent& event);

      // helpers & converters
      void DisableAllButtons(void);
      void EnableAllButtons(void);

      void SetStateOfInLedButtons(int state);
      void SetStateOfE1LedButton(bool state);
      void SetStateOfE2LedButton(bool state);
      void SetStateOfE3LedButton(bool state);
      void SetStateOfE4LedButton(bool state);
      void SetStateOfE5LedButton(bool state);
      void SetStateOfE6LedButton(bool state);
      void SetStateOfE7LedButton(bool state);
      void SetStateOfE8LedButton(bool state);

      void SetStateOfOutLedButtons(int state);
      void SetStateOfO1LedButton(bool state);
      void SetStateOfO2LedButton(bool state);
      void SetStateOfO3LedButton(bool state);
      void SetStateOfO4LedButton(bool state);
      void SetStateOfO5LedButton(bool state);
      void SetStateOfO6LedButton(bool state);
      void SetStateOfO7LedButton(bool state);
      void SetStateOfO8LedButton(bool state);

      MyMotor::driveState_t CtrlBox2Motor(MyMotorCtrlButton::MyMotorCtrlState_t sel);
      MyMotorCtrlButton::MyMotorCtrlState_t Motor2CtrlBox(MyMotor::driveState_t drv);

      MyMotor::stopCtrlDef_t StopBox2Motor(MyStopCtrlButton::MyStopCtrlState_t sel);
      MyStopCtrlButton::MyStopCtrlState_t Motor2StopBox(MyMotor::stopCtrlDef_t stop);
};

#endif // FTCDEMOFRAME_H
