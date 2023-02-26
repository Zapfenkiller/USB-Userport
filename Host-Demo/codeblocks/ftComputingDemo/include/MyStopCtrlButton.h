/***************************************************************
 * Name:      MyStopCtrlButton.h
 * Purpose:   Implements a motor stop radiobox control
 * Author:    R. Trapp
 * Created:   2023-02-21
 * Copyright: R. Trapp
 * License:   GPLv3 - see License.txt
 **************************************************************/


#ifndef MYSTOPCTRLBUTTON_H
#define MYSTOPCTRLBUTTON_H


#ifdef WX_PRECOMP
   #include "wx_pch.h"
#else
   #include <wx/wx.h>
#endif


class MyStopCtrlButton : public wxRadioBox
{
   public:
      typedef enum
      {
         STOP_SW_ON_LOW  = 0,
         STOP_SW_NEVER   = 1,
         STOP_SW_ON_HIGH = 2
      } MyStopCtrlState_t;

      MyStopCtrlButton
         (
          wxWindow *parent,
          int id,
          const wxString& boxLabel    = "Stop",
          const wxString& onBtnLbl  = "@ ON",
          const wxString& nvrBtnLbl = "NEVER",
          const wxString& offBtnLbl   = "@ OFF"
         );
      virtual ~MyStopCtrlButton();

      void SetState(MyStopCtrlState_t state);
      MyStopCtrlState_t GetState(void);


   private:
};


#endif // MYSTOPCTRLBUTTON_H
