#pragma once
#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/clrpicker.h>
#include <wx/statline.h>
#include "enum.h"

class cObjectmanager;
class cPlot;

class wxScaleButton : public wxWindow
{
    wxCheckBox* autoscale = nullptr;
    wxStaticText* staticscale = nullptr;
    int zoom = 0;
    wxSlider* slider = nullptr;

    static const int buttonWidth = 300;
    static const int buttonHeight = 100;

public:
    wxScaleButton(wxFrame* parent, wxWindowID id);


    void set_scale_factor(int factor);

    int get_scale_factor();

    void OnAutoscaleSignalChanged(wxEvent& event);
    void OnZoomSignalChanged(wxCommandEvent& event);

    void paintEvent(wxPaintEvent& evt);
    void paintNow();

    void render(wxDC& dc);

    DECLARE_EVENT_TABLE()
};

