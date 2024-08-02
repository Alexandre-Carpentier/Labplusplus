#include "cDeviceIDSaver.h"
#include "format"

cDeviceIDSaver::cDeviceIDSaver(size_t signal_number, wxWindow* parent, wxWindowID id,
	const wxString& title,
	const wxPoint& pos,
	const wxSize& size,
	long style,
	const wxString& name) :
	wxDialog(parent, id, title, pos, size, style, name)
{
	wxBoxSizer* main_box = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* filename_box[128] = { 0 };
	signal_number_ =signal_number;
	assert(signal_number < 128);

	for (size_t i = 0; i < signal_number; i++)
	{
		filename_box[i] = new wxBoxSizer(wxHORIZONTAL);
		std::string number = std::format("{}", (i + 1));
		std::string text = "Instrument n°" + number;
		static_file_name[i] = new wxStaticText(this, wxID_ANY, text, wxDefaultPosition, this->FromDIP(wxSize(150, 30)), wxNO_BORDER);
		static_file_name[i]->SetFont(static_file_name[i]->GetFont().Scale(1.5f));

		header[i] = new wxTextCtrl(this, wxID_ANY, L"LE099", wxDefaultPosition, this->FromDIP(wxSize(300, 30)), wxSUNKEN_BORDER);
		header[i]->SetFont(header[i]->GetFont().Scale(2.0f));
		header[i]->SetBackgroundColour(wxColour(255, 250, 250));

		filename_box[i]->Add(static_file_name[i], 0, wxEXPAND);
		filename_box[i]->Add(header[i], 1, wxEXPAND | wxALL, this->FromDIP(4));
		main_box->Add(filename_box[i], 1, wxEXPAND | wxALL, this->FromDIP(20));
	}

	wxStaticLine* split1 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition,
		this->FromDIP(wxDefaultSize),
		wxLI_HORIZONTAL,
		"");

	wxSizer* wxBtnsizer = CreateButtonSizer(wxOK | wxCANCEL);
	
	main_box->Add(split1, 0, wxEXPAND);
	main_box->Add(wxBtnsizer, 0, wxALIGN_RIGHT | wxALL, this->FromDIP(5));

	SetSizer(main_box);
	SetMinSize(this->FromDIP(wxSize(200, 200)));
	Fit();
}

std::string cDeviceIDSaver::concat_instr()
{
	std::string concated;
	for (size_t i = 0; i < signal_number_; i++)
	{
		concated += header[i]->GetValue().ToStdString();
		concated += " ";
	}
	return concated;
}

std::string cDeviceIDSaver::get_header()
{
	return concat_instr();
}

cDeviceIDSaver::~cDeviceIDSaver() {}