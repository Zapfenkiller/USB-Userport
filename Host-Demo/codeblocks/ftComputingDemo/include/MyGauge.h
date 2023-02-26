/***************************************************************
 * Name:      MyGauge.h
 * Purpose:   Defines a gauge
 * Author:    R. Trapp
 * Created:   2023-02-25
 * Copyright: R. Trapp
 * License:   GPLv3 - see License.txt
 **************************************************************/


#ifndef MYGAUGE_H
#define MYGAUGE_H


#ifdef WX_PRECOMP
   #include "wx_pch.h"
#else
   #include <wx/wx.h>
#endif


class MyGauge : public wxWindow
{
   public:
      MyGauge(wxWindow *parent,
                  int id,
                  int minVal = 0,
                  int maxVal = 100,
                  const wxString& label = wxEmptyString,
                  const wxString& tooltip = wxEmptyString);
      virtual ~MyGauge();

      void Disable(void);
      void Enable(void);
      void SetValue(int value);

   private:
      wxWindow* pFrame;
      int gaugeMin;
      int gaugeMax;
      int gaugeRange;
      volatile bool gaugeEnabled;
      wxStaticText* gLabel;
      wxStaticText* gRecentAsText;
      wxRect gaugeFrame;
      wxRect gauge;
      wxColour gaugeFramePenColour;
      wxColour gaugeFrameBrushColour;
      wxColour gaugeEnableColour;
      wxColour gaugeDisableColour;

      void OnPaint(wxPaintEvent& event);
};


#endif // MYGAUGE_H
