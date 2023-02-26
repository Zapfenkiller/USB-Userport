/***************************************************************
 * Name:      MyGauge.h
 * Purpose:   Implements a gauge
 * Author:    R. Trapp
 * Created:   2023-02-25
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

#include "../include/MyGauge.h"


#define GAUGE_UPPER 200 // pixel


MyGauge::MyGauge(
                 wxWindow *parent,
                 int id,
                 int minVal,
                 int maxVal,
                 const wxString& label,
                 const wxString& tooltip
                )
   : wxWindow(parent, id)
{
   pFrame = parent;
   gaugeMin = minVal;
   gaugeMax = maxVal;
   gaugeRange = maxVal - minVal;

   gLabel = new wxStaticText \
   ( \
      this, \
      wxID_ANY, \
      label, \
      wxPoint(8, 0), \
      wxSize(20, -1), \
      wxST_NO_AUTORESIZE \
   );
   gaugeFrame = wxRect\
   ( \
      gLabel->GetPosition().x + gLabel->GetSize().GetWidth() + 6, \
      0, \
      GAUGE_UPPER + 4, \
      gLabel->GetSize().GetHeight() \
   );
   gauge = wxRect\
   ( \
      gLabel->GetPosition().x + gLabel->GetSize().GetWidth() + 6 + 2, \
      2, \
      0, \
      gLabel->GetSize().GetHeight() - 4 \
   );
   gRecentAsText = new wxStaticText \
   ( \
      this, \
      wxID_ANY, \
      "---", \
      wxPoint(gLabel->GetPosition().x + gaugeFrame.x + gaugeFrame.width + 6, 0), \
      wxSize(32, -1), /* debug session: 8 px / digit, 3 we need! */ \
      wxST_NO_AUTORESIZE \
   );
   gaugeFramePenColour = wxColour(0xC0, 0xC0, 0xC0);
   gaugeFrameBrushColour = wxColour(0x80, 0x80, 0x80);
   gaugeDisableColour = wxColour(0xA0, 0xA0, 0xA0);
   gaugeEnableColour = wxColour(0x40, 0x60, 0xFF);
   SetToolTip(tooltip);
   Bind(wxEVT_PAINT, &MyGauge::OnPaint, this);
   gaugeEnabled = false;
   Disable();
}


MyGauge::~MyGauge()
{
}


void MyGauge::Disable(void)
{
   gRecentAsText->SetLabel("---");
   gaugeEnabled = false;
   Refresh();
}


void MyGauge::Enable(void)
{
   gaugeEnabled = true;
   Refresh();
}


void MyGauge::SetValue(int value)
{
   gRecentAsText->SetLabel(wxString::Format("%3i", value));
   int v = 0;
   if (value > gaugeMin)
   {
      if (value < gaugeMax)
         v = (int)((double)(value - gaugeMin) * (double)GAUGE_UPPER / (double)gaugeRange);
      else
         v = GAUGE_UPPER;
   }
   gauge.SetWidth(v);
   Refresh();
}


void MyGauge::OnPaint(wxPaintEvent& event)
{
   wxPaintDC dc(this);
   if (gaugeEnabled)
   {
      dc.SetPen(wxPen(gaugeFramePenColour));
      dc.SetBrush(wxBrush(gaugeFrameBrushColour));
      dc.DrawRectangle(gaugeFrame);
      dc.SetPen(wxPen(gaugeEnableColour));
      dc.SetBrush(wxBrush(gaugeEnableColour));
      dc.DrawRectangle(gauge);
   }
   else
   {
      dc.SetPen(wxPen(gaugeFramePenColour));
      dc.SetBrush(wxBrush(gaugeDisableColour));
      dc.DrawRectangle(gaugeFrame);
   }
}
