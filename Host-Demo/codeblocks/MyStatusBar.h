/***************************************************************
 * Name:      MyStatusBar.h
 * Purpose:   Defines a more sophisticated status bar
 * Author:    R. Trapp
 * Created:   2021-12-05
 * Copyright: R. Trapp
 * License:   GPLv3 - see License.txt
 **************************************************************/
// Foundation found at https://wiki.wxwidgets.org/Non-Static_Status_Bar_Attempt

#ifndef MYSTATUSBAR_H
#define MYSTATUSBAR_H


#ifdef WX_PRECOMP
   #include "wx_pch.h"
#else
   #include <wx/wx.h>
#endif


class MyStatusBar : public wxStatusBar
{
   public:
      MyStatusBar(wxWindow *parent, long style);
      virtual ~MyStatusBar();

      // public functions
      void SetConnectStatus(bool);

      // event handlers
      void OnSize(wxSizeEvent& event);


   private:
      wxStaticBitmap*   m_statBitmap;

      DECLARE_EVENT_TABLE()
};


#endif // MYSTATUSBAR_H
