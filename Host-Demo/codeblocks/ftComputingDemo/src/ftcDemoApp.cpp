/***************************************************************
 * Name:      ftcDemoApp.cpp
 * Purpose:   Implements Application Class
 * Author:    R. Trapp (rene <dot> trapp [ät] web <dot> de)
 * Created:   2023-01-19
 * Copyright: R. Trapp ()
 * License:   GPLv3 - see License.txt
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "../include/ftcDemoApp.h"
#include "../include/ftcDemoFrame.h"


IMPLEMENT_APP(ftcDemoApp);


bool ftcDemoApp::OnInit()
{
   static const char *icon_xpm[] =
   {
      /* columns rows count-of-colors chars-per-pixel */
      "24 24 4 1",
      "# c #000000", // #RRGGBB
      "+ c #A0A0A0",
      ". c #FFFFFF",
      "  c None",
      "      +++  +++  +++     ",
      "      +++  +++  +++     ",
      "      +++  +++  +++     ",
      "   ##################   ",
      "   ##############..##   ",
      "+++######..#####...##+++",
      "+++######..####...###+++",
      "+++######..###...####+++",
      "   ######..##...#####   ",
      "   ######..#...######   ",
      "+++######.....#######+++",
      "+++##........########+++",
      "+++##........########+++",
      "   ######.....#######   ",
      "   ######..#...#.####   ",
      "+++######..##.....###+++",
      "+++######..#......###+++",
      "+++######..##......##+++",
      "   ######..####....##   ",
      "   ##############..##   ",
      "    #################   ",
      "      +++  +++  +++     ",
      "      +++  +++  +++     ",
      "      +++  +++  +++     ",
      "                        "
   };
   wxBitmap bmap_icon = wxBitmap(icon_xpm);
   wxIcon appIcon;
   appIcon.CopyFromBitmap(bmap_icon);

   ftcDemoFrame* frame = new ftcDemoFrame(0L, _("ft Computing Interface Demo"));
   frame->SetIcon(appIcon); // To Set App Icon
   frame->Show();
   return true;
}
