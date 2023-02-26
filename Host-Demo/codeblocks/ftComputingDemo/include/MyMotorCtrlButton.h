/***************************************************************
 * Name:      MyMotorCtrlButton.h
 * Purpose:   Implements a motor radiobox control
 * Author:    R. Trapp
 * Created:   2023-02-21
 * Copyright: R. Trapp
 * License:   GPLv3 - see License.txt
 **************************************************************/


#ifndef MYMOTORCTRLBUTTON_H
#define MYMOTORCTRLBUTTON_H


#ifdef WX_PRECOMP
   #include "wx_pch.h"
#else
   #include <wx/wx.h>
#endif


class MyMotorCtrlButton : public wxRadioBox
{
   public:
      typedef enum
      {
         MOTOR_DRV_IDLE  = 0,
         MOTOR_DRV_BRAKE = 1,
         MOTOR_DRV_FWD   = 2,
         MOTOR_DRV_BWD   = 3
      } MyMotorCtrlState_t;

      MyMotorCtrlButton
         (
          wxWindow *parent,
          int id,
          const wxString& boxLabel    = "Motor",
          const wxString& idleBtnLbl  = "IDLE",
          const wxString& brakeBtnLbl = "BRK",
          const wxString& fwdBtnLbl   = "FWD",
          const wxString& bwdBtnLbl   = "BWD"
         );
      virtual ~MyMotorCtrlButton();

      void SetState(MyMotorCtrlState_t state);
      MyMotorCtrlState_t GetState(void);


   private:
};


#endif // MYMOTORCTRLBUTTON_H
