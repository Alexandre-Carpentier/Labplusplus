#ifndef _CGNUPLOT_H_
#define _CGNUPLOT_H_

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/statline.h>

class cGnuplot : public wxDialog
{
public:

    cGnuplot(wxWindow* parent, wxWindowID id,
        const wxString& title,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_DIALOG_STYLE,
        const wxString& name = wxASCII_STR(wxDialogNameStr));

    std::string get_filename();

    virtual ~cGnuplot();

private:
    wxTextCtrl* file_name = nullptr;
};

#endif

