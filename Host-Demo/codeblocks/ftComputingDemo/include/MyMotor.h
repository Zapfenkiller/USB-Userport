/***************************************************************
 * Name:      MyMotor.h
 * Purpose:   Represent a logic BDC motor control, convert from
 *            logic interface to a physical representation.
 * Author:    R. Trapp (rene <dot> trapp [ät] web <dot> de)
 * Created:   2023-01-28
 * Copyright: R. Trapp
 * License:   GPLv3 - see License.txt
 **************************************************************/

#ifndef __MYMOTOR_H__
   #define __MYMOTOR_H__


#ifdef WX_PRECOMP
   #include "wx_pch.h"
#else
   #include <wx/wx.h>
#endif


wxDECLARE_EVENT(myMOTOR_EVENT, wxCommandEvent);


class MyMotor
{
   public:

      typedef enum
      {
         STOP_ON_NONE = 0b00,
         STOP_ON_LO   = 0b11,
         STOP_ON_HI   = 0b10
      } stopCtrlDef_t;

      typedef enum
      {
         IDLE  = 0b00,
         FWD   = 0b01,
         BWD   = 0b10,
         BRAKE = 0b11
      } driveState_t;

      typedef enum
      {
         canNothing              = 0,
         canDriveFwd             = 1 <<  0,
         canDriveBwd             = 1 <<  1,
         canIdle                 = 1 <<  2,
         canBrakeLoSide          = 1 <<  3,
         canBrakeHiSide          = 1 <<  4,
         canSenseFailures        = 1 <<  5,
         canSenseSpeed           = 1 <<  6,
         canControlSpeed         = 1 <<  7,
         canSenseDriveTorque     = 1 <<  8,
         canControlDriveTorque   = 1 <<  9,
         canSenseBrakeTorque     = 1 << 10,
         canControlBrakeTorque   = 1 << 11,
         canSenseDirection       = 1 << 12,
         canTrackPosition        = 1 << 13
      } capabilities_t;

      MyMotor(
         wxFrame *parent,
         int idOfEvent,
         stopCtrlDef_t stopFwd = STOP_ON_NONE,
         driveState_t stopStateFwd = IDLE,
         stopCtrlDef_t stopBwd = STOP_ON_NONE,
         driveState_t stopStateBwd = IDLE
      );
      virtual ~MyMotor();

      // API, dedicated to the GUI / application side
      virtual driveState_t SetState(driveState_t s);
      driveState_t GetState(void);
      bool IsRunning(void);
      virtual void SetFwdStopConfig(stopCtrlDef_t fwdStopWhen, driveState_t fwdStopCmd);
      virtual void SetBwdStopConfig(stopCtrlDef_t bwdStopWhen, driveState_t bwdStopCmd);
      virtual capabilities_t GetCapabilities(void);

      // physical abstraction, dedicated to communication side
      virtual int GetDrivePattern(void);
      int GetStopSwitchActiveDef(void);
      int GetStopSwitchRule(void);
      int GetStopFwdPattern(void);
      int GetStopBwdPattern(void);
      void FeedStopSwitches(int);


   private:

      typedef struct
      {
         stopCtrlDef_t when;
         driveState_t how;
         bool stopSwitch;
      } stopCtrl_t;

      volatile stopCtrl_t stopCtrlFwd;
      volatile stopCtrl_t stopCtrlBwd;
      volatile driveState_t pwrState;
      volatile driveState_t pwrStatePrev;

      wxFrame *myFrame;
      int eventID;

      bool FwdIsAllowed(void);
      bool BwdIsAllowed(void);
      int logic2physical(driveState_t drive);
};


#endif // __MYMOTOR_H__
