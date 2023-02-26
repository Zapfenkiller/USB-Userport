/***************************************************************
 * Name:      MyMotorCtrlButton.c
 * Purpose:   Implements a motor radiobox control
 * Author:    R. Trapp
 * Created:   2023-02-21
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

#include "../include/MyMotorCtrlButton.h"


MyMotorCtrlButton::MyMotorCtrlButton
         (
          wxWindow *parent,
          int id,
          const wxString& boxLabel,
          const wxString& idleBtnLbl,
          const wxString& brakeBtnLbl,
          const wxString& fwdBtnLbl,
          const wxString& bwdBtnLbl
         )
   : wxRadioBox()
{
   wxString buttonLabels[4] = {idleBtnLbl, brakeBtnLbl, fwdBtnLbl, bwdBtnLbl};
   Create(parent, id, boxLabel, wxDefaultPosition, wxDefaultSize, 4, buttonLabels, 2, wxRA_SPECIFY_ROWS);
   Disable();
   SetSelection(0);
   SetBackgroundColour(0xD0D0D0);
   SetToolTip(_("Control motor driving"));
}


MyMotorCtrlButton::~MyMotorCtrlButton()
{
}


void MyMotorCtrlButton::SetState(MyMotorCtrlState_t state)
{
   switch (state)
   {
      case MOTOR_DRV_BWD:
         SetSelection(3);
         break;
      case MOTOR_DRV_FWD:
         SetSelection(2);
         break;
      case MOTOR_DRV_BRAKE:
         SetSelection(1);
         break;
      case MOTOR_DRV_IDLE:
      default:
         SetSelection(0);
   }
}


MyMotorCtrlButton::MyMotorCtrlState_t MyMotorCtrlButton::GetState(void)
{
   switch (GetSelection())
   {
      case 3:
         return MOTOR_DRV_BWD;
         break;
      case 2:
         return MOTOR_DRV_FWD;
         break;
      case 1:
         return MOTOR_DRV_BRAKE;
         break;
      case 0:
      default:
         return MOTOR_DRV_IDLE;
   }
}
