/***************************************************************
 * Name:      ftConnection.cpp
 * Purpose:   Wrapper of a ft Computing Interface Chain to form
 *            a common API to any application
 * Author:    R. Trapp (rene <dot> trapp [ät] web <dot> de)
 * Created:   2023-01-22
 * Copyright: R. Trapp
 * License:   GPLv3 - see License.txt
 **************************************************************/
// worker thread taken from:
//    wxWidgets samples/thread.cpp
// detection of device arrival / removal:
//    https://forums.wxwidgets.org/viewtopic.php?t=12138
//    https://stackoverflow.com/questions/38430587/any-windows-api-for-detecting-mtp-devices/68851782
// modified to get it independent from specific application class


#ifdef WX_PRECOMP
   #include "wx_pch.h"
#else
   #include <wx/wx.h>
#endif

#ifdef __BORLANDC__
   #pragma hdrstop
#endif //__BORLANDC__

#include "../include/ftConnection.h"


/* -------------------------------------------------------------------------- */
/* Constructor / Destructor                                                   */
/* -------------------------------------------------------------------------- */


ftConnection::ftConnection(wxFrame *frame)
   : wxThread()
{
   myFrame = frame;
   stopThread = false;
   connector = new UsbHandling();
   connectState = false;
   prevConnectState = false;
   triggerTransmission = triggerNothing;

   // instantiate the logical component controls
   mot1 = new MyMotor(myFrame, ftConnection::idEvtM1StatChg, MyMotor::STOP_ON_NONE, MyMotor::IDLE, MyMotor::STOP_ON_NONE, MyMotor::IDLE);
   mot2 = new MyMotor(myFrame, ftConnection::idEvtM2StatChg, MyMotor::STOP_ON_NONE, MyMotor::IDLE, MyMotor::STOP_ON_NONE, MyMotor::IDLE);
   mot3 = new MyMotor(myFrame, ftConnection::idEvtM3StatChg, MyMotor::STOP_ON_NONE, MyMotor::IDLE, MyMotor::STOP_ON_NONE, MyMotor::IDLE);
   mot4 = new MyMotor(myFrame, ftConnection::idEvtM4StatChg, MyMotor::STOP_ON_NONE, MyMotor::IDLE, MyMotor::STOP_ON_NONE, MyMotor::IDLE);
   mot5 = new MyMotor(myFrame, ftConnection::idEvtM5StatChg, MyMotor::STOP_ON_NONE, MyMotor::IDLE, MyMotor::STOP_ON_NONE, MyMotor::IDLE);
   mot6 = new MyMotor(myFrame, ftConnection::idEvtM6StatChg, MyMotor::STOP_ON_NONE, MyMotor::IDLE, MyMotor::STOP_ON_NONE, MyMotor::IDLE);
   mot7 = new MyMotor(myFrame, ftConnection::idEvtM7StatChg, MyMotor::STOP_ON_NONE, MyMotor::IDLE, MyMotor::STOP_ON_NONE, MyMotor::IDLE);
   mot8 = new MyMotor(myFrame, ftConnection::idEvtM8StatChg, MyMotor::STOP_ON_NONE, MyMotor::IDLE, MyMotor::STOP_ON_NONE, MyMotor::IDLE);
   // preset interface exchange data packet
   interfaceControl.outputsToMaster = \
        mot1->GetDrivePattern() << 0 \
      | mot2->GetDrivePattern() << 2 \
      | mot3->GetDrivePattern() << 4 \
      | mot4->GetDrivePattern() << 6;
   interfaceControl.outputsFromMaster = interfaceControl.outputsToMaster;
   interfaceControl.outputsToSlave = \
        mot5->GetDrivePattern() << 0 \
      | mot6->GetDrivePattern() << 2 \
      | mot7->GetDrivePattern() << 4 \
      | mot8->GetDrivePattern() << 6;
   interfaceControl.outputsFromSlave = interfaceControl.outputsToSlave;
   interfaceControl.stopInversionMaster = \
      mot1->GetStopSwitchActiveDef() << 0 \
    | mot2->GetStopSwitchActiveDef() << 2 \
    | mot3->GetStopSwitchActiveDef() << 4 \
    | mot4->GetStopSwitchActiveDef() << 6;
   interfaceControl.stopInversionSlave = \
      mot5->GetStopSwitchActiveDef() << 0 \
    | mot6->GetStopSwitchActiveDef() << 2 \
    | mot7->GetStopSwitchActiveDef() << 4 \
    | mot8->GetStopSwitchActiveDef() << 6;
   interfaceControl.stopEnableMaster = \
      mot1->GetStopSwitchRule() << 0 \
    | mot2->GetStopSwitchRule() << 2 \
    | mot3->GetStopSwitchRule() << 4 \
    | mot4->GetStopSwitchRule() << 6;
   interfaceControl.stopEnableSlave = \
      mot5->GetStopSwitchRule() << 0 \
    | mot6->GetStopSwitchRule() << 2 \
    | mot7->GetStopSwitchRule() << 4 \
    | mot8->GetStopSwitchRule() << 6;
   interfaceControl.stopResponsePatFwdMaster = \
      mot1->GetStopFwdPattern() << 0 \
    | mot2->GetStopFwdPattern() << 2 \
    | mot3->GetStopFwdPattern() << 4 \
    | mot4->GetStopFwdPattern() << 6;
   interfaceControl.stopResponsePatBwdMaster = \
      mot1->GetStopBwdPattern() << 0 \
    | mot2->GetStopBwdPattern() << 2 \
    | mot3->GetStopBwdPattern() << 4 \
    | mot4->GetStopBwdPattern() << 6;
   interfaceControl.stopResponsePatFwdMaster = \
      mot5->GetStopFwdPattern() << 0 \
    | mot6->GetStopFwdPattern() << 2 \
    | mot7->GetStopFwdPattern() << 4 \
    | mot8->GetStopFwdPattern() << 6;
   interfaceControl.stopResponsePatBwdSlave = \
      mot5->GetStopBwdPattern() << 0 \
    | mot6->GetStopBwdPattern() << 2 \
    | mot7->GetStopBwdPattern() << 4 \
    | mot8->GetStopBwdPattern() << 6;
// interfaceControlPrev = interfaceControl;
}


ftConnection::~ftConnection()
{
}


/* -------------------------------------------------------------------------- */
/* Connectivity control                                                       */
/* -------------------------------------------------------------------------- */


wxThread::ExitCode ftConnection::Entry()
{
   wxMilliSleep(120); // allow even worst case analog values to get sampled 1st
   while (!TestDestroy())
   {
      if (stopThread)
         break;
      // not only reads input data but also checks for connectivity
      connectState = connector->GetInputs(&interfaceControl);
      if (connectState && !(interfaceControl.flagsOfChange & UsbHandling::FLAG_DEVICE_UP))
         connectState = false; // connected but not up and running
      if (connectState)
      {  // device is connected (attached && initialized)
         // motor logic responds to stop switches
         mot1->FeedStopSwitches(interfaceControl.inputsFromMaster >> 0);
         mot2->FeedStopSwitches(interfaceControl.inputsFromMaster >> 2);
         mot3->FeedStopSwitches(interfaceControl.inputsFromMaster >> 4);
         mot4->FeedStopSwitches(interfaceControl.inputsFromMaster >> 6);
         mot5->FeedStopSwitches(interfaceControl.inputsFromSlave >> 0);
         mot6->FeedStopSwitches(interfaceControl.inputsFromSlave >> 2);
         mot7->FeedStopSwitches(interfaceControl.inputsFromSlave >> 4);
         mot8->FeedStopSwitches(interfaceControl.inputsFromSlave >> 6);
         if (connectState != prevConnectState)
         {
            wxThreadEvent event1(wxEVT_THREAD, idEvtConnect);
            wxQueueEvent(myFrame, event1.Clone());

            // special service: refresh all inputs, throw events
            // It seems some schematics interchanged EX / EY -> countermeasure
            wxThreadEvent event2(wxEVT_THREAD, idEvtResiXChg);
            event2.SetInt(interfaceControl.analogEY);
            wxQueueEvent(myFrame, event2.Clone());

            wxThreadEvent event3(wxEVT_THREAD, idEvtResiYChg);
            event3.SetInt(interfaceControl.analogEX);
            wxQueueEvent(myFrame, event3.Clone());

            wxThreadEvent event4(wxEVT_THREAD, idEvtInputsChg);
            event4.SetInt(interfaceControl.inputsFromMaster | (interfaceControl.inputsFromSlave << 8));
            wxQueueEvent(myFrame, event4.Clone());

            wxThreadEvent event5(wxEVT_THREAD, idEvtOutputsChg);
            event5.SetInt(interfaceControl.outputsFromMaster | (interfaceControl.outputsFromSlave << 8));
            wxQueueEvent(myFrame, event5.Clone());
            wxThreadEvent event6(wxEVT_THREAD, idEvtM1StatChg);
            wxQueueEvent(myFrame, event6.Clone());
            wxThreadEvent event7(wxEVT_THREAD, idEvtM2StatChg);
            wxQueueEvent(myFrame, event7.Clone());
            wxThreadEvent event8(wxEVT_THREAD, idEvtM3StatChg);
            wxQueueEvent(myFrame, event8.Clone());
            wxThreadEvent event9(wxEVT_THREAD, idEvtM4StatChg);
            wxQueueEvent(myFrame, event9.Clone());
            wxThreadEvent event10(wxEVT_THREAD, idEvtM5StatChg);
            wxQueueEvent(myFrame, event10.Clone());
            wxThreadEvent event11(wxEVT_THREAD, idEvtM6StatChg);
            wxQueueEvent(myFrame, event11.Clone());
            wxThreadEvent event12(wxEVT_THREAD, idEvtM7StatChg);
            wxQueueEvent(myFrame, event12.Clone());
            wxThreadEvent event13(wxEVT_THREAD, idEvtM8StatChg);
            wxQueueEvent(myFrame, event13.Clone());

            // special service: already send the stop switch control
            // but never ever send motor control to avoid unintended (re)start!
            PrepareStopSwitches();
         }
         else
         {
            if (interfaceControl.flagsOfChange & UsbHandling::FLAG_INPUT_CHANGE)
            {
               wxThreadEvent event(wxEVT_THREAD, idEvtInputsChg);
               event.SetInt(interfaceControl.inputsFromMaster | (interfaceControl.inputsFromSlave << 8));
               wxQueueEvent(myFrame, event.Clone());
            }
            if (interfaceControl.flagsOfChange & UsbHandling::FLAG_OUTPUT_CHANGE)
            {
               wxThreadEvent event1(wxEVT_THREAD, idEvtOutputsChg);
               event1.SetInt(interfaceControl.outputsFromMaster | (interfaceControl.outputsFromSlave << 8));
               wxQueueEvent(myFrame, event1.Clone());
               wxThreadEvent event2(wxEVT_THREAD, idEvtM1StatChg);
               wxQueueEvent(myFrame, event2.Clone());
               wxThreadEvent event3(wxEVT_THREAD, idEvtM2StatChg);
               wxQueueEvent(myFrame, event3.Clone());
               wxThreadEvent event4(wxEVT_THREAD, idEvtM3StatChg);
               wxQueueEvent(myFrame, event4.Clone());
               wxThreadEvent event5(wxEVT_THREAD, idEvtM4StatChg);
               wxQueueEvent(myFrame, event5.Clone());
               wxThreadEvent event6(wxEVT_THREAD, idEvtM5StatChg);
               wxQueueEvent(myFrame, event6.Clone());
               wxThreadEvent event7(wxEVT_THREAD, idEvtM6StatChg);
               wxQueueEvent(myFrame, event7.Clone());
               wxThreadEvent event8(wxEVT_THREAD, idEvtM7StatChg);
               wxQueueEvent(myFrame, event8.Clone());
               wxThreadEvent event9(wxEVT_THREAD, idEvtM8StatChg);
               wxQueueEvent(myFrame, event9.Clone());
            }
            // It seems some schematics interchanged EX / EY -> countermeasure
            if (interfaceControl.flagsOfChange & UsbHandling::FLAG_POTI_X_CHANGE)
            {
               wxThreadEvent event(wxEVT_THREAD, idEvtResiYChg);
               event.SetInt(interfaceControl.analogEX);
               wxQueueEvent(myFrame, event.Clone());
            }
            if (interfaceControl.flagsOfChange & UsbHandling::FLAG_POTI_Y_CHANGE)
            {
               wxThreadEvent event(wxEVT_THREAD, idEvtResiXChg);
               event.SetInt(interfaceControl.analogEY);
               wxQueueEvent(myFrame, event.Clone());
            }
            if (triggerTransmission & triggerMotorSetting)
            {
               connector->PutOutputs(interfaceControl);
               triggerTransmission &= ~triggerMotorSetting;
            }
            if (triggerTransmission & triggerStopSwitchSetting)
            {
               connector->SetStopSwitchConfig(interfaceControl);
               triggerTransmission &= ~triggerStopSwitchSetting;
            }
         }
      }
      else
      {  // device is not connected (!attached || !initalized)
         if (connectState != prevConnectState)
         {
            wxThreadEvent event(wxEVT_THREAD, idEvtDisconnect);
            wxQueueEvent(myFrame, event.Clone());
         }
         else
         {
            Connect();
         }
      }
      prevConnectState = connectState;
      wxThreadEvent event(wxEVT_THREAD, idEvtTick);
      wxQueueEvent(myFrame, event.Clone());
      wxMilliSleep(40); // wait a moment before sampling again
   }
   connector->DisconnectFromDevice();
   wxThreadEvent event(wxEVT_THREAD, idEvtDisconnect);
   wxQueueEvent(myFrame, event.Clone());
   return NULL;
}


void ftConnection::OnExit()
{
   wxThreadEvent event(wxEVT_THREAD, idEvtDisconnect);
   wxQueueEvent(myFrame, event.Clone());
   connector->ExitLib();
   stopThread = true;
}


ftConnection::failureCode_t ftConnection::OpenConnection()
{
   // create and start connection state checker (background thread)
   if (Create() == wxTHREAD_NO_ERROR)
   {
      if (Run() == wxTHREAD_NO_ERROR)
      {
// ToDo: Use mutex lock - really?
//wxMilliSleep(100); // allow the thread to startup before some access happens from here => q'n'd!
         return Connect();
      }
   }
   return fcThreadIssue;
}


void ftConnection::CloseConnection(void)
{
   stopThread = true;
   interfaceControl.outputsToMaster = 0;
   interfaceControl.outputsToSlave = 0;
   connector->PutOutputs(interfaceControl);
   connector->DisconnectFromDevice();
   wxThreadEvent event(wxEVT_THREAD, idEvtDisconnect);
   wxQueueEvent(myFrame, event.Clone());
}


wxString ftConnection::GetLibVersion(void)
{
   return connector->GetVersion();
}


/* -------------------------------------------------------------------------- */
/* API (GUI / application)                                                    */
/* -------------------------------------------------------------------------- */


void ftConnection::PrepareStopSwitches(void)
{
   while (triggerTransmission & triggerStopSwitchSetting)
   {
      // wait for thread to finish the previous request
   }
   interfaceControl.stopInversionMaster = \
      mot1->GetStopSwitchActiveDef() << 0 \
    | mot2->GetStopSwitchActiveDef() << 2 \
    | mot3->GetStopSwitchActiveDef() << 4 \
    | mot4->GetStopSwitchActiveDef() << 6;
   interfaceControl.stopInversionSlave = \
      mot5->GetStopSwitchActiveDef() << 0 \
    | mot6->GetStopSwitchActiveDef() << 2 \
    | mot7->GetStopSwitchActiveDef() << 4 \
    | mot8->GetStopSwitchActiveDef() << 6;
   interfaceControl.stopEnableMaster = \
      mot1->GetStopSwitchRule() << 0 \
    | mot2->GetStopSwitchRule() << 2 \
    | mot3->GetStopSwitchRule() << 4 \
    | mot4->GetStopSwitchRule() << 6;
   interfaceControl.stopEnableSlave = \
      mot5->GetStopSwitchRule() << 0 \
    | mot6->GetStopSwitchRule() << 2 \
    | mot7->GetStopSwitchRule() << 4 \
    | mot8->GetStopSwitchRule() << 6;
   interfaceControl.stopResponsePatFwdMaster = \
      mot1->GetStopFwdPattern() << 0 \
    | mot2->GetStopFwdPattern() << 2 \
    | mot3->GetStopFwdPattern() << 4 \
    | mot4->GetStopFwdPattern() << 6;
   interfaceControl.stopResponsePatBwdMaster = \
      mot1->GetStopBwdPattern() << 0 \
    | mot2->GetStopBwdPattern() << 2 \
    | mot3->GetStopBwdPattern() << 4 \
    | mot4->GetStopBwdPattern() << 6;
   interfaceControl.stopResponsePatFwdMaster = \
      mot5->GetStopFwdPattern() << 0 \
    | mot6->GetStopFwdPattern() << 2 \
    | mot7->GetStopFwdPattern() << 4 \
    | mot8->GetStopFwdPattern() << 6;
   interfaceControl.stopResponsePatBwdSlave = \
      mot5->GetStopBwdPattern() << 0 \
    | mot6->GetStopBwdPattern() << 2 \
    | mot7->GetStopBwdPattern() << 4 \
    | mot8->GetStopBwdPattern() << 6;
   triggerTransmission |= triggerStopSwitchSetting;
}


void ftConnection::SetMotor(int mot)
{
   while (triggerTransmission & triggerMotorSetting)
   {
      // wait for thread to finish the previous request
   }
   int outputVectorMaster = interfaceControl.outputsToMaster;
   int outputVectorSlave = interfaceControl.outputsToSlave;
   switch (mot)
   {
      case MOT1:
         outputVectorMaster = mux(outputVectorMaster, mot1->GetDrivePattern() << 0, 0b00000011);
         break;
      case MOT2:
         outputVectorMaster = mux(outputVectorMaster, mot2->GetDrivePattern() << 2, 0b00001100);
         break;
      case MOT3:
         outputVectorMaster = mux(outputVectorMaster, mot3->GetDrivePattern() << 4, 0b00110000);
         break;
      case MOT4:
         outputVectorMaster = mux(outputVectorMaster, mot4->GetDrivePattern() << 6, 0b11000000);
         break;
      case MOT5:
         outputVectorSlave = mux(outputVectorSlave, mot5->GetDrivePattern() << 0, 0b00000011);
         break;
      case MOT6:
         outputVectorSlave = mux(outputVectorSlave, mot6->GetDrivePattern() << 2, 0b00001100);
         break;
      case MOT7:
         outputVectorSlave = mux(outputVectorSlave, mot7->GetDrivePattern() << 4, 0b00110000);
         break;
      case MOT8:
         outputVectorSlave = mux(outputVectorSlave, mot8->GetDrivePattern() << 6, 0b11000000);
         break;
      default:
         ;
   }
   interfaceControl.outputsToMaster = outputVectorMaster;
   interfaceControl.outputsToSlave = outputVectorSlave;
   triggerTransmission |= triggerMotorSetting;
}


int ftConnection::GetSwitches(void)
{
   return interfaceControl.inputsFromMaster | (interfaceControl.inputsFromSlave << 8);
}


int ftConnection::GetAnalog(int pot)
{
   switch (pot)
   {
      case 0:
         return interfaceControl.analogEX;
      case 1:
         return interfaceControl.analogEY;
      default: ;
         return -1;
   }
}


/* -------------------------------------------------------------------------- */
/* Internal helpers                                                           */
/* -------------------------------------------------------------------------- */


int ftConnection::mux(int a, int b, int sel)
{
   b &=  sel;
   a &= ~sel;
   return a | b;
}


ftConnection::failureCode_t ftConnection::Connect(void)
{
   if (!connector->LibIsReady())
      if (!connector->InitializeLib())
         return fcNoComLib;
   if (!connector->DeviceIsConnected())
      if (!connector->ConnectToDevice())
         return fcNoDevice;
   if(!connector->DeviceIsInitialized())
      if (!connector->SendDeviceInitRequest())
         return fcNotInit;
   return fcNTF; // No Trouble Found
}
