/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#ifndef _CDEVICEIDSAVER_H_
#define _CDEVICEIDSAVER_H_

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/statline.h>

class cDeviceIDSaver : public wxDialog
{
public:

    cDeviceIDSaver(size_t signal_number, wxWindow* parent, wxWindowID id,
        const wxString& title,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_DIALOG_STYLE,
        const wxString& name = wxASCII_STR(wxDialogNameStr));

    std::string get_header();

    virtual ~cDeviceIDSaver();

private:
    std::string concat_instr();
    size_t signal_number_ = 0;
    wxStaticText* static_file_name[128] = { 0 };
    wxTextCtrl* header[128] = { 0 };
};

#endif

