/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#include "cGnuplot.h"

cGnuplot::cGnuplot(wxWindow* parent, wxWindowID id,
	const wxString& title,
	const wxPoint& pos,
	const wxSize& size,
	long style,
	const wxString& name) :
	wxDialog(parent, id, title, pos, size, style, name)
{
	wxBoxSizer* main_box = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* filename_box = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* static_file_name = new wxStaticText(this, wxID_ANY, L"Save to PDF?", wxDefaultPosition, this->FromDIP(wxSize(150, 30)), wxNO_BORDER );
	static_file_name->SetFont(static_file_name->GetFont().Scale(1.5f));

	file_name = new wxTextCtrl(this, wxID_ANY, L"IDXXXX-X-X-Testfile", wxDefaultPosition, this->FromDIP(wxSize(300,30)), wxSUNKEN_BORDER );
	file_name->SetFont(file_name->GetFont().Scale(2.0f));
	file_name->SetBackgroundColour(wxColour(255, 250, 250));

	wxStaticLine* split1 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition,
		this->FromDIP(wxDefaultSize),
		wxLI_HORIZONTAL,
		"");

	wxSizer* wxBtnsizer = CreateButtonSizer(wxOK | wxCANCEL);

	filename_box->Add(static_file_name, 0, wxEXPAND);
	filename_box->Add(file_name, 1, wxEXPAND | wxALL, this->FromDIP(4));

	main_box->Add(filename_box, 1, wxEXPAND |wxALL, this->FromDIP(20));
	main_box->Add(split1, 0, wxEXPAND);
	main_box->Add(wxBtnsizer, 0, wxALIGN_RIGHT | wxALL, this->FromDIP(5));

	SetSizer(main_box);
	SetMinSize(this->FromDIP(wxSize(200, 200)));
	Fit();
}

std::string cGnuplot::get_filename()
{
	return file_name->GetValue().ToStdString();
}

cGnuplot::~cGnuplot() {}