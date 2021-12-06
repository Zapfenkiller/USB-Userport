/***************************************************************
 * Name:      MyStatusBar.cpp
 * Purpose:   Implements a more sophisticated status bar
 * Author:    R. Trapp
 * Created:   2021-12-05
 * Copyright: R. Trapp
 * License:   GPLv3 - see License.txt
 **************************************************************/
// Foundation found at https://wiki.wxwidgets.org/Non-Static_Status_Bar_Attempt

#ifdef WX_PRECOMP
   #include "wx_pch.h"
#else
   #include <wx/wx.h>
#endif
#include "MyStatusBar.h"


BEGIN_EVENT_TABLE(MyStatusBar, wxStatusBar)
   EVT_SIZE(MyStatusBar::OnSize)
END_EVENT_TABLE()


MyStatusBar::MyStatusBar(wxWindow *parent, long style)
   : wxStatusBar(parent, wxID_ANY, style, _(""))
{

   static const int widths[2] = {-1, this->GetSize().GetHeight()};
   SetFieldsCount(2);
   SetStatusWidths(2, widths);

   m_statBitmap = new wxStaticBitmap(this, wxID_ANY, wxBITMAP_PNG(usbNo));
}


MyStatusBar::~MyStatusBar()
{
}


void MyStatusBar::OnSize(wxSizeEvent& event)
{
   wxRect rect;

   if (m_statBitmap)
   {
      GetFieldRect(1, rect);
      m_statBitmap->SetPosition(wxPoint(rect.x + 4, rect.y + 2));
   }

   event.Skip();
}


void MyStatusBar::SetConnectStatus(bool c)
{
   if (m_statBitmap)
   {
      if (c)
      {
         m_statBitmap->SetBitmap(wxBITMAP_PNG(usbYes));
      }
      else
      {
         m_statBitmap->SetBitmap(wxBITMAP_PNG(usbNo));
      }
   }
}
