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

#ifdef __BORLANDC__
   #pragma hdrstop
#endif //__BORLANDC__

#include "../include/MyStatusBar.h"


MyStatusBar::MyStatusBar(wxWindow *parent, long style)
   : wxStatusBar(parent, wxID_ANY, style, _(""))
{
   static const char *disconnected_xpm[] =
   {
      /* columns rows count-of-colors chars-per-pixel */
      "30 24 5 1",
      "* c #FF0000", // #RRGGBB
      "+ c #F0A0A0",
      "x c #808080",
      "# c #808080",
      "  c None",
      "    **********************    ",
      "  **************************  ",
      " **+++++++*++++++++++++++++** ",
      " *++++++++*+++++++++++++++++* ",
      "**+++++++++*++++++x#++++++++**",
      "**+++++++++*+++++++x##++++++**",
      "**++++++++++*+++++++x###++++**",
      "**++#####x++*+++x#########++**",
      "**+++++++++++*++++++x###++++**",
      "**+++++++++++*+++++x##++++++**",
      "**++++++++++++*+++x#++++++++**",
      "**++++++++++++*+++++++++++++**",
      "**+++++++++++++*++++++++++++**",
      "**++++++++#x+++*++++++++++++**",
      "**++++++##x+++++*+++++++++++**",
      "**++++###x++++++*+++++++++++**",
      "**++#########x+++*++x#####++**",
      "**++++###x+++++++*++++++++++**",
      "**++++++##x+++++++*+++++++++**",
      "**++++++++#x++++++*+++++++++**",
      " *+++++++++++++++++*++++++++* ",
      " **++++++++++++++++*+++++++** ",
      "  **************************  ",
      "    **********************    "
   };
   bmap_disconnected = wxBitmap(disconnected_xpm);

   static const char *connected_xpm[] =
   {
      /* columns rows count-of-colors chars-per-pixel */
      "30 24 5 1",
      "* c #00FF00", // #RRGGBB
      "+ c #A0F0A0",
      "x c #808080",
      "# c #000000",
      "  c None",
      "    **********************    ",
      "  **************************  ",
      " **++++++++++++++++++++++++** ",
      " *++++++++++++++++++++++++++* ",
      "**++++++++++++++++x#++++++++**",
      "**+++++++++++++++++x##++++++**",
      "**++++++++++++++++++x###++++**",
      "**++######################++**",
      "**++++++++++++++++++x###++++**",
      "**+++++++++++++++++x##++++++**",
      "**++++++++++++++++x#++++++++**",
      "**++++++++++++++++++++++++++**",
      "**++++++++++++++++++++++++++**",
      "**++++++++#x++++++++++++++++**",
      "**++++++##x+++++++++++++++++**",
      "**++++###x++++++++++++++++++**",
      "**++######################++**",
      "**++++###x++++++++++++++++++**",
      "**++++++##x+++++++++++++++++**",
      "**++++++++#x++++++++++++++++**",
      " *++++++++++++++++++++++++++* ",
      " **++++++++++++++++++++++++** ",
      "  **************************  ",
      "    **********************    "
   };
   bmap_connected = wxBitmap(connected_xpm);

   static const int widths[2] = {this->GetSize().GetHeight(), -1};
   SetFieldsCount(2);
   SetStatusWidths(2, widths);
   m_statBitmap = new wxStaticBitmap(this, wxID_ANY, bmap_disconnected);
   Bind(wxEVT_SIZE, &MyStatusBar::OnSize, this);
}


MyStatusBar::~MyStatusBar()
{
}


void MyStatusBar::OnSize(wxSizeEvent& event)
{
   wxRect rect;

   if (m_statBitmap)
   {
      GetFieldRect(0, rect);
      m_statBitmap->SetPosition(wxPoint(rect.x + 4, rect.y));
   }

   event.Skip();
}


void MyStatusBar::SetConnectStatus(bool c)
{
   if (m_statBitmap)
      if (c)
         m_statBitmap->SetBitmap(bmap_connected);
      else
         m_statBitmap->SetBitmap(bmap_disconnected);
}
