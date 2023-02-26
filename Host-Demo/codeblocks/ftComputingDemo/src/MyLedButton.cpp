/***************************************************************
 * Name:      MyLedButton.cpp
 * Purpose:   Implements a LED button control
 * Author:    R. Trapp
 * Created:   2023-01-14
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

#include "../include/MyLedButton.h"


MyLedButton::MyLedButton(
                         wxWindow *parent,
                         int id,
                         const wxString& label,
                         const wxString& tooltip)
   : wxButton(parent, id, label)
{
   static const char *ledOffBase_xpm[] =
   {
      /* columns rows count-of-colors chars-per-pixel */
      "24 24 5 1",
      "* c #808080", // #RRGGBB
      "+ c #A0A0A0",
      "x c #B0B0B0",
      "# c #C0C0C0",
      "  c None",
      "         ******         ",
      "      ************      ",
      "     **++++++++++**     ",
      "    **+++xx+++++++**    ",
      "   **++xxxxxx++++++**   ",
      "  **++xxxxxxxx++++++**  ",
      " **++xxxx##xxxx++++++** ",
      " **+xxxx####xxxx+++++** ",
      " **+xxxx####xxxx+++++** ",
      "**+++xxxx##xxxx+++++++**",
      "**++++xxxxxxxx++++++++**",
      "**+++++xxxxxx+++++++++**",
      "**+++++++xx+++++++++++**",
      "**++++++++++++++++++++**",
      "**++++++++++++++++++++**",
      " **++++++++++++++++++** ",
      " **++++++++++++++++++** ",
      " **++++++++++++++++++** ",
      "  **++++++++++++++++**  ",
      "   **++++++++++++++**   ",
      "    **++++++++++++**    ",
      "     **++++++++++**     ",
      "      ************      ",
      "         ******         "
   };
   bmap_ledOff = wxBitmap(ledOffBase_xpm);

   static const char *ledOnBase_xpm[] =
   {
      /* columns rows count-of-colors chars-per-pixel */
      "24 24 5 1",
      "* c #808080", // #RRGGBB
      "+ c #A0A0A0",
      "x c #B0B0B0",
      "# c #C0C0C0",
      "  c None",
      "         ******         ",
      "      ************      ",
      "     **++++++++++**     ",
      "    **++++++++++++**    ",
      "   **++++++++++++++**   ",
      "  **++++++++++++++++**  ",
      " **++++++++++++++++++** ",
      " **++++++++xx++++++++** ",
      " **++++++xxxxxx++++++** ",
      "**++++++xxxxxxxx++++++**",
      "**+++++xxxx##xxxx+++++**",
      "**++++xxxx####xxxx++++**",
      "**++++xxxx####xxxx++++**",
      "**+++++xxxx##xxxx+++++**",
      "**++++++xxxxxxxx++++++**",
      " **++++++xxxxxx++++++** ",
      " **++++++++xx++++++++** ",
      " **++++++++++++++++++** ",
      "  **++++++++++++++++**  ",
      "   **++++++++++++++**   ",
      "    **++++++++++++**    ",
      "     **++++++++++**     ",
      "      ************      ",
      "         ******         "
   };

   ledOnBase_xpm[1] = "* c #A04040";
   ledOnBase_xpm[2] = "+ c #E00000";
   ledOnBase_xpm[3] = "x c #FF0000";
   ledOnBase_xpm[4] = "# c #FF0000";
   bmap_ledRed = wxBitmap(ledOnBase_xpm);

   ledOnBase_xpm[1] = "* c #A08040";
   ledOnBase_xpm[2] = "+ c #E8A800";
   ledOnBase_xpm[3] = "x c #FFB000";
   ledOnBase_xpm[4] = "# c #FFB000";
   bmap_ledOrange = wxBitmap(ledOnBase_xpm);

   ledOnBase_xpm[1] = "* c #A0A040";
   ledOnBase_xpm[2] = "+ c #E8E800";
   ledOnBase_xpm[3] = "x c #FFFF00";
   ledOnBase_xpm[4] = "# c #FFFF00";
   bmap_ledYellow = wxBitmap(ledOnBase_xpm);

   ledOnBase_xpm[1] = "* c #40A040";
   ledOnBase_xpm[2] = "+ c #00E000";
   ledOnBase_xpm[3] = "x c #00FF00";
   ledOnBase_xpm[4] = "# c #00FF00";
   bmap_ledGreen = wxBitmap(ledOnBase_xpm);

   ledOnBase_xpm[1] = "* c #4040A0";
   ledOnBase_xpm[2] = "+ c #4040E0";
   ledOnBase_xpm[3] = "x c #4040FF";
   ledOnBase_xpm[4] = "# c #4040FF";
   bmap_ledBlue = wxBitmap(ledOnBase_xpm);

   ledOnBase_xpm[1] = "* c #A040A0";
   ledOnBase_xpm[2] = "+ c #F000F0";
   ledOnBase_xpm[3] = "x c #FF00FF";
   ledOnBase_xpm[4] = "# c #FF00FF";
   bmap_ledPink = wxBitmap(ledOnBase_xpm);

   ledOnBase_xpm[1] = "* c #A0A0A0";
   ledOnBase_xpm[2] = "+ c #E0E0E0";
   ledOnBase_xpm[3] = "x c #FFFFFF";
   ledOnBase_xpm[4] = "# c #FFFFFF";
   bmap_ledWhite = wxBitmap(ledOnBase_xpm);

   Disable();
   SetToolTip(tooltip);
   NewBitmap(bmap_ledOff);
   myState = LED_Off;
}


MyLedButton::~MyLedButton()
{
}


void MyLedButton::SetState(MyLedState_t state)
{
   myState = state;
   switch (state)
   {
      case LED_Red:
         NewBitmap(bmap_ledRed);
         break;
      case LED_Orange:
         NewBitmap(bmap_ledOrange);
         break;
      case LED_Yellow:
         NewBitmap(bmap_ledYellow);
         break;
      case LED_Green:
         NewBitmap(bmap_ledGreen);
         break;
      case LED_Blue:
         NewBitmap(bmap_ledBlue);
         break;
      case LED_Pink:
         NewBitmap(bmap_ledPink);
         break;
      case LED_White:
         NewBitmap(bmap_ledWhite);
         break;
      default:
         NewBitmap(bmap_ledOff);
         myState = LED_Off;
   }
}


MyLedButton::MyLedState_t MyLedButton::GetState(void)
{
   return myState;
}


void MyLedButton::NewBitmap(wxBitmap bitmap)
{
   SetBitmap(bitmap, wxLEFT);
   SetBitmapCurrent(bitmap);
   SetBitmapFocus(bitmap);
   SetBitmapHover(bitmap);
   SetBitmapPressed(bitmap);
   SetBitmapSelected(bitmap);
}
