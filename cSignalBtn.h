#pragma once


#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/clrpicker.h>
#include <wx/statline.h>
#include "enum.h"

class cObjectmanager;
class cPlot;

class wxCustomButton : public wxWindow
{
    wxStaticText* txt = nullptr;
    wxStaticText* staticunit = nullptr;
    wxStaticText* unit = nullptr;
    wxStaticText* staticaddr = nullptr;
    wxStaticText* addr = nullptr;
    wxColourPickerCtrl* picker = nullptr;
    wxStaticText* min = nullptr;
    wxStaticText* avg = nullptr;
    wxStaticText* max = nullptr;
    wxButton* reset = nullptr;
    wxCheckBox* show = nullptr;

    bool pressedDown;
    wxString name_;
    wxString address_;
    wxString unit_;
    double min_;
    double avg_;
    double max_;
    wxColor color_;

    static const int buttonWidth = 300;
    static const int buttonHeight = 100;

public:
    wxCustomButton(wxFrame* parent, wxWindowID id, wxString name, wxString address, wxColor color);

    void set_name(wxString name);
    void set_address(wxString address);
    void set_unit(wxString unit);
    void set_min(double val);
    void set_average(double val);
    void set_max(double val);
    void set_color(wxColor color);
    void set_visible(bool isDisplayed);

    wxString get_name();
    wxString get_address();
    double get_min();
    double get_average();
    double get_max();
    wxColor get_color();

    void paintEvent(wxPaintEvent& evt);
    void paintNow();

    void render(wxDC& dc);

    // some useful events
    void mouseMoved(wxMouseEvent& event);
    void OnPickerChanged(wxColourPickerEvent& event);
    void OnShowSignalChanged(wxEvent& event);
    void mouseDown(wxMouseEvent& event);
    void mouseWheelMoved(wxMouseEvent& event);
    void mouseReleased(wxMouseEvent& event);
    void rightClick(wxMouseEvent& event);
    void mouseLeftWindow(wxMouseEvent& event);
    void keyPressed(wxKeyEvent& event);
    void keyReleased(wxKeyEvent& event);

    void reset_stat(wxEvent& event);

    DECLARE_EVENT_TABLE()
};

