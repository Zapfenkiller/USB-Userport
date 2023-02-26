/***************************************************************
 * Name:      DemoApp.cpp
 * Purpose:   Implements the Application Class
 * Author:    R. Trapp
 * Created:   2021-12-03
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


#include "DemoApp.h"
#include "DemoMain.h"


IMPLEMENT_APP(DemoApp);


bool DemoApp::OnInit()
{
   wxImage::AddHandler(new wxPNGHandler);


   DemoMain* frame = new DemoMain(0L, _("USB-Userport Host Demo"));
   frame->SetIcon(wxICON(appIcon));
   frame->Show();


   return true;
}
