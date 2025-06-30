/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include <wx/wx.h>
#include <wx/combobox.h>
#include <wx/dialog.h>
#include <wx/statline.h>

enum {
    IDCSTATICLINEAR_ = 8000,
    IDCSTATICCHANNAME_,
    IDCCHANNAME_,
    IDCSTATICCHANSLOPE_,
    IDCCHANSLOPE_,
    IDCSTATICCHANSHIFT_,
    IDCCHANSHIFT_,
    IDCSTATICCHANUNIT_,
    IDCCHANUNIT_
};



class ScaleDialog : public wxDialog
{
public:

    ScaleDialog(wxWindow* parent, wxWindowID id,
        const wxString& title,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_DIALOG_STYLE,
        const wxString& name = wxASCII_STR(wxDialogNameStr));

    virtual ~ScaleDialog();

    std::string  get_scale_name() const { return chan_name->GetValue().ToStdString(); }
    std::string  get_scale_slope() const { return chan_slope->GetValue().ToStdString(); }
    std::string  get_scale_shift() const { return chan_shift->GetValue().ToStdString(); }
    std::string  get_scale_unit() const { return chan_unit->GetValue().ToStdString(); }

private:
    const float text_size = 1.5;
    wxColor* bgcolor = new wxColor(235, 235, 235);

    wxTextCtrl* chan_name = nullptr;
    wxTextCtrl* chan_slope = nullptr;
    wxTextCtrl* chan_shift = nullptr;
    wxTextCtrl* chan_unit = nullptr;

    std::string scale_name = "My custom scale name";
    std::string scale_slope = "1";
    std::string scale_shift = "0";
    std::string scale_unit = "kpa";

    //void OnOk(wxCommandEvent& event);

    //DECLARE_EVENT_TABLE()
};