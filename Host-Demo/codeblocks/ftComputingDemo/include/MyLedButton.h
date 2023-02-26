/***************************************************************
 * Name:      MyLedButton.h
 * Purpose:   Defines a LED button control
 * Author:    R. Trapp
 * Created:   2023-01-14
 * Copyright: R. Trapp
 * License:   GPLv3 - see License.txt
 **************************************************************/


#ifndef MYLEDBUTTON_H
#define MYLEDBUTTON_H


#ifdef WX_PRECOMP
   #include "wx_pch.h"
#else
   #include <wx/wx.h>
#endif


class MyLedButton : public wxButton
{
   public:
      typedef enum
      {
         LED_Off = 0,
         LED_Red,
         LED_Orange,
         LED_Yellow,
         LED_Green,
         LED_Blue,
         LED_Pink,
         LED_White
      } MyLedState_t;
      MyLedButton(wxWindow *parent,
                  int id,
                  const wxString& label = wxEmptyString,
                  const wxString& tooltip = wxEmptyString);
      virtual ~MyLedButton();

      void SetState(MyLedState_t state);
      MyLedState_t GetState(void);


   private:
      wxBitmap bmap_ledOff;
      wxBitmap bmap_ledRed;
      wxBitmap bmap_ledOrange;
      wxBitmap bmap_ledYellow;
      wxBitmap bmap_ledGreen;
      wxBitmap bmap_ledBlue;
      wxBitmap bmap_ledPink;
      wxBitmap bmap_ledWhite;
      MyLedState_t myState;

      void NewBitmap(wxBitmap bitmap);
};


#endif // MYLEDBUTTON_H
