/***************************************************************
 * Name:      MyMotor.cpp
 * Purpose:   Represent a logic BDC motor control, convert from
 *            logic interface to a physical representation.
 * Author:    R. Trapp (rene <dot> trapp [ät] web <dot> de)
 * Created:   2023-01-28
 * Copyright: R. Trapp
 * License:   GPLv3 - see License.txt
 **************************************************************/


#include "../include/MyMotor.h"


wxDEFINE_EVENT(myMOTOR_EVENT, wxCommandEvent);


/* -------------------------------------------------------------------------- */
/* Constructor / Destructor                                                   */
/* -------------------------------------------------------------------------- */


MyMotor::MyMotor(
   wxFrame *parent,
   int idOfEvent,
   stopCtrlDef_t stopFwd,
   driveState_t stopStateFwd,
   stopCtrlDef_t stopBwd,
   driveState_t stopStateBwd
)
{
   eventID = idOfEvent;
   myFrame = parent;
   pwrState = IDLE;
   pwrStatePrev = pwrState;
   stopCtrlFwd.when = stopFwd;
   stopCtrlFwd.how = stopStateFwd;
   stopCtrlFwd.stopSwitch = false; // ToDo: true ?
   stopCtrlBwd.when = stopBwd;
   stopCtrlBwd.how = stopStateBwd;
   stopCtrlBwd.stopSwitch = false; // ToDo: true ?
}


MyMotor::~MyMotor()
{
}


/* -------------------------------------------------------------------------- */
/* API, dedicated to the GUI / application side                               */
/* -------------------------------------------------------------------------- */


MyMotor::driveState_t MyMotor::SetState(driveState_t s)
{
   /*******************************************************************
    * Depending on HW one of IDLE or BRAKE might not be possible, this
    * gets replaced by the available one already on logical level.
    * The configuration below is for fischertechnik Computing Interface
    * series 3056*, applying discrete H-brige drivers.
    * NOTE: Also adapt \c logic2physical() below!
    *******************************************************************/
   switch (s)
   {
      case FWD:
         if (FwdIsAllowed())
            pwrState = FWD;
         else
            pwrState = stopCtrlFwd.how;
         break;
      case BWD:
         if (BwdIsAllowed())
            pwrState = BWD;
         else
            pwrState = stopCtrlBwd.how;
         break;
      case IDLE:
         pwrState = IDLE;
         break;
      case BRAKE:
         pwrState = IDLE;
         break;
      default:
         pwrState = IDLE;
   }
   pwrStatePrev = pwrState;
   return pwrState;
}


MyMotor::driveState_t MyMotor::GetState(void)
{
   return pwrState;
}


bool MyMotor::IsRunning(void)
{
   if ((pwrState == FWD) || (pwrState == BWD))
      return true;
   else
      return false;
}


void MyMotor::SetFwdStopConfig(stopCtrlDef_t fwdStopWhen, driveState_t fwdStopCmd)
{
   stopCtrlFwd.when  = fwdStopWhen;
   stopCtrlFwd.how   = fwdStopCmd;
}


void MyMotor::SetBwdStopConfig(stopCtrlDef_t bwdStopWhen, driveState_t bwdStopCmd)
{
   stopCtrlBwd.when  = bwdStopWhen;
   stopCtrlBwd.how   = bwdStopCmd;
}


MyMotor::capabilities_t MyMotor::GetCapabilities(void)
{
   return (MyMotor::capabilities_t)(MyMotor::canIdle | MyMotor::canDriveFwd | MyMotor::canDriveBwd);
}


/* -------------------------------------------------------------------------- */
/* Physical abstraction, dedicated to communication side                      */
/* -------------------------------------------------------------------------- */


int  MyMotor::GetDrivePattern(void)
{
   return logic2physical(pwrState);
}


int MyMotor::GetStopSwitchActiveDef(void)
{
   int pattern = 0b00;
   if (stopCtrlFwd.when == STOP_ON_LO)
      pattern |= 0b01;
   if (stopCtrlBwd.when == STOP_ON_LO)
      pattern |= 0b10;
   return pattern;
}


int MyMotor::GetStopSwitchRule(void)
{
   int pattern = 0b00;
   if ((stopCtrlFwd.when == STOP_ON_LO) || (stopCtrlFwd.when == STOP_ON_HI))
      pattern |= 0b01;
   if ((stopCtrlBwd.when == STOP_ON_LO) || (stopCtrlBwd.when == STOP_ON_HI))
      pattern |= 0b10;
   return pattern;
}


int MyMotor::GetStopFwdPattern(void)
{
   return logic2physical(stopCtrlFwd.how);
}


int MyMotor::GetStopBwdPattern(void)
{
   return logic2physical(stopCtrlBwd.how);
}


void MyMotor::FeedStopSwitches(int sw)
{
   if (sw & (1 << 0))
      stopCtrlFwd.stopSwitch = true;
   else
      stopCtrlFwd.stopSwitch = false;
   if (sw & (1 << 1))
      stopCtrlBwd.stopSwitch = true;
   else
      stopCtrlBwd.stopSwitch = false;

   switch (pwrState)
   {
      case FWD:
         if (!FwdIsAllowed())
            pwrState = stopCtrlFwd.how;
         break;
      case BWD:
         if (!BwdIsAllowed())
            pwrState = stopCtrlBwd.how;
         break;
      default:
         ;
   }
   if (pwrState != pwrStatePrev)
   {
      wxCommandEvent event(myMOTOR_EVENT, eventID);
      wxPostEvent(myFrame, *event.Clone());
   }
   pwrStatePrev = pwrState;
}


/* -------------------------------------------------------------------------- */
/* Internal helpers                                                           */
/* -------------------------------------------------------------------------- */


bool MyMotor::FwdIsAllowed(void)
{
   switch (stopCtrlFwd.when)
   {
      case STOP_ON_HI: ;
         return !stopCtrlFwd.stopSwitch;
         break;
      case STOP_ON_LO: ;
         return stopCtrlFwd.stopSwitch;
         break;
      default: ;
         return true;
   }
}


bool MyMotor::BwdIsAllowed(void)
{
   switch (stopCtrlBwd.when)
   {
      case STOP_ON_HI: ;
         return !stopCtrlBwd.stopSwitch;
         break;
      case STOP_ON_LO: ;
         return stopCtrlBwd.stopSwitch;
         break;
      default: ;
         return true;
   }
}


int MyMotor::logic2physical(driveState_t drive)
{
   /*******************************************************************
    * This is the place to map logic states to HW control bit patterns.
    * Check carefully with your setup for the correct assignments.
    * Doublecheck to not force the bridge into a shoot-through mode!
    * The configuration below is for fischertechnik Computing Interface
    * series 3056*, applying discrete H-brige drivers.
    * NOTE: Needs to match \c SetState() above!
    *******************************************************************/
   int pattern;
   switch (drive)
   {
      case FWD:
         pattern = 0b01;
         break;
      case BWD:
         pattern = 0b10;
         break;
      case BRAKE:
         pattern = 0b00;
         break;
      case IDLE:
         pattern = 0b00;
         break;
      default:
         pattern = 0b00;
   }
   return pattern;
}
