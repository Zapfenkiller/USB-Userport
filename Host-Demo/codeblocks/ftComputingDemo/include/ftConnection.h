/***************************************************************
 * Name:      ftConnection.h
 * Purpose:   Wrapper of a ft Computing Interface Chain to form
 *            a common API controlling an electro-mechanic
 *            contraption.
 * Author:    R. Trapp (rene <dot> trapp [ät] web <dot> de)
 * Created:   2023-01-22
 * Copyright: R. Trapp
 * License:   GPLv3 - see License.txt
 **************************************************************/

#ifndef FTCONNECTION_H
#define FTCONNECTION_H


#ifdef WX_PRECOMP
   #include "wx_pch.h"
#else
   #include <wx/wx.h>
#endif

#include "UsbHandling.h"
#include "MyMotor.h"


class ftConnection : public wxThread
{
   public:

      /* -------------------------------------------------------------------- */
      /* Constructor / Destructor                                             */
      /* -------------------------------------------------------------------- */
      ftConnection(wxFrame *frame);
      virtual ~ftConnection();

      /* -------------------------------------------------------------------- */
      /* All events emitted to the GUI / application level                    */
      /* -------------------------------------------------------------------- */
      enum
      {
         idEvtConnect = wxID_HIGHEST+1,
         idEvtDisconnect,
         idEvtTick,
         idEvtInputsChg,
         idEvtOutputsChg,
         idEvtResiXChg,
         idEvtResiYChg,
         idEvtM1StatChg,
         idEvtM2StatChg,
         idEvtM3StatChg,
         idEvtM4StatChg,
         idEvtM5StatChg,
         idEvtM6StatChg,
         idEvtM7StatChg,
         idEvtM8StatChg,
         idEvtftConnectionHighest
      };

      /* -------------------------------------------------------------------- */
      /* Connectivity control                                                 */
      /* -------------------------------------------------------------------- */
      typedef enum
      {
         fcNTF = 0,           // No Trouble Found
         fcThreadIssue = -1,  // thread not created
         fcNoDevice = -2,     // device detached
         fcNoComLib = -3,     // hidapi not loaded
         fcNotInit = -4       // device present bit not initialized
      } failureCode_t;

      failureCode_t OpenConnection(void);
      void CloseConnection(void);
      wxString GetLibVersion(void);

      /* -------------------------------------------------------------------- */
      /* API (GUI / application)                                              */
      /* -------------------------------------------------------------------- */
      enum
      {
         MOT1 = 1,
         MOT2,
         MOT3,
         MOT4,
         MOT5,
         MOT6,
         MOT7,
         MOT8
      };

      MyMotor* mot1;
      MyMotor* mot2;
      MyMotor* mot3;
      MyMotor* mot4;
      MyMotor* mot5;
      MyMotor* mot6;
      MyMotor* mot7;
      MyMotor* mot8;

      void PrepareStopSwitches(void);
      void SetMotor(int mot);
      int GetSwitches(void);
      int GetAnalog(int pot);

   private:

      wxFrame *myFrame;

      /* -------------------------------------------------------------------- */
      /* Connectivity control                                                 */
      /* -------------------------------------------------------------------- */
      enum
      {
         triggerNothing             = 0,
         triggerReconnect           = 1 << 0,
         triggerMotorSetting        = 1 << 1,
         triggerStopSwitchSetting   = 1 << 2,
      };
      bool stopThread;
      bool connectState;
      bool prevConnectState;
      int  triggerTransmission;
      UsbHandling *connector;

      // thread execution starts here
      virtual void *Entry(void);
      // called when the thread exits - whether it terminates normally or is
      // stopped with Delete() (but not when it is Kill()ed!)
      virtual void OnExit(void);

      /* -------------------------------------------------------------------- */
      /* API (GUI / application)                                              */
      /* -------------------------------------------------------------------- */
      UsbHandling::usbDataXchg_t interfaceControl;


      /* -------------------------------------------------------------------- */
      /* Internal helpers                                                     */
      /* -------------------------------------------------------------------- */
      int mux (int a, int b, int sel);

      failureCode_t Connect(void);
};

#endif // FTCONNECTION_H
