/***************************************************************
 * Name:      MyStopCtrlButton.c
 * Purpose:   Implements a motor stop radiobox control
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

#include "../include/MyStopCtrlButton.h"


MyStopCtrlButton::MyStopCtrlButton
         (
          wxWindow *parent,
          int id,
          const wxString& boxLabel,
          const wxString& onBtnLbl,
          const wxString& nvrBtnLbl,
          const wxString& offBtnLbl
         )
   : wxRadioBox()
{
   wxString buttonLabels[3] = {onBtnLbl, nvrBtnLbl, offBtnLbl};
   Create(parent, id, boxLabel, wxDefaultPosition, wxDefaultSize, 3, buttonLabels, 1, wxRA_SPECIFY_COLS);
   Disable();
   SetSelection(1);
   SetBackgroundColour(0xD0D0D0);
   SetToolTip(_("Define motor automatic stopping"));
}


MyStopCtrlButton::~MyStopCtrlButton()
{
}


void MyStopCtrlButton::SetState(MyStopCtrlState_t state)
{
   switch (state)
   {
      case STOP_SW_ON_HIGH:
         SetSelection(0);
         break;
      case STOP_SW_ON_LOW:
         SetSelection(2);
         break;
      case STOP_SW_NEVER:
      default:
         SetSelection(1);
   }
}


MyStopCtrlButton::MyStopCtrlState_t MyStopCtrlButton::GetState(void)
{
   switch (GetSelection())
   {
      case 0:
         return STOP_SW_ON_HIGH;
         break;
      case 2:
         return STOP_SW_ON_LOW;
         break;
      case 1:
      default:
         return STOP_SW_NEVER;
   }
}
