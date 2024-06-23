#include "cDaqmxScaleDlg.h"

ScaleDialog::ScaleDialog(wxWindow* parent, wxWindowID id,
	const wxString& title,
	const wxPoint& pos,
	const wxSize& size,
	long style,
	const wxString& name) :
	wxDialog(parent, id, title, pos, size, style, name)
{
	wxBoxSizer* main_box = new wxBoxSizer(wxVERTICAL);

	wxStaticText* static_lineartxt = new wxStaticText(this, IDCSTATICLINEAR_, L"Linearize: ax + b = f(x)", wxDefaultPosition, this->FromDIP(wxDefaultSize), wxSUNKEN_BORDER);
	static_lineartxt->SetFont(static_lineartxt->GetFont().Scale(text_size * 1.5));
	static_lineartxt->SetBackgroundColour(*bgcolor);

	wxFlexGridSizer* flex = new wxFlexGridSizer(2, wxSize(200, 20));

	wxStaticText* static_chan_name = new wxStaticText(this, IDCSTATICCHANNAME_, L"Preset name:", wxDefaultPosition, this->FromDIP(wxDefaultSize), wxSUNKEN_BORDER);
	static_chan_name->SetFont(static_chan_name->GetFont().Scale(text_size));
	static_chan_name->SetBackgroundColour(*bgcolor);

	std::string str_name;
	str_name.append("Citec 0-16 bar sensor");
	chan_name = new wxTextCtrl(this, IDCCHANNAME_, str_name, wxDefaultPosition, this->FromDIP(wxSize(300, 40)), wxSUNKEN_BORDER);
	chan_name->SetFont(chan_name->GetFont().Scale(text_size));
	chan_name->SetBackgroundColour(wxColor(250, 250, 250));

	wxStaticText* static_chan_slope = new wxStaticText(this, IDCSTATICCHANSLOPE_, L"Slope (a):", wxDefaultPosition, this->FromDIP(wxDefaultSize), wxSUNKEN_BORDER);
	static_chan_slope->SetFont(static_chan_slope->GetFont().Scale(text_size));
	static_chan_slope->SetBackgroundColour(*bgcolor);

	std::string str_slope;
	str_slope.append("1");
	chan_slope = new wxTextCtrl(this, IDCCHANSLOPE_, str_slope, wxDefaultPosition, this->FromDIP(wxDefaultSize), wxSUNKEN_BORDER);
	chan_slope->SetFont(chan_slope->GetFont().Scale(text_size));
	chan_slope->SetBackgroundColour(wxColor(250, 250, 250));


	wxStaticText* static_chan_shift = new wxStaticText(this, IDCSTATICCHANSHIFT_, L"Shift (b):", wxDefaultPosition, this->FromDIP(wxDefaultSize), wxSUNKEN_BORDER);
	static_chan_shift->SetFont(static_chan_shift->GetFont().Scale(text_size));
	static_chan_shift->SetBackgroundColour(*bgcolor);

	std::string str_shift;
	str_shift.append("0");
	chan_shift = new wxTextCtrl(this, IDCCHANSHIFT_, str_shift, wxDefaultPosition, this->FromDIP(wxDefaultSize), wxSUNKEN_BORDER);
	chan_shift->SetFont(chan_shift->GetFont().Scale(text_size));
	chan_shift->SetBackgroundColour(wxColor(250, 250, 250));


	wxStaticText* static_chan_unit = new wxStaticText(this, IDCSTATICCHANUNIT_, L"Unit:", wxDefaultPosition, this->FromDIP(wxDefaultSize), wxSUNKEN_BORDER);
	static_chan_unit->SetFont(static_chan_unit->GetFont().Scale(text_size));
	static_chan_unit->SetBackgroundColour(*bgcolor);

	std::string str_unit;
	str_unit.append("bar");
	chan_unit = new wxTextCtrl(this, IDCCHANUNIT_, str_unit, wxDefaultPosition, this->FromDIP(wxDefaultSize), wxSUNKEN_BORDER);
	chan_unit->SetFont(chan_unit->GetFont().Scale(text_size));
	chan_unit->SetBackgroundColour(wxColor(250, 250, 250));

	flex->Add(static_chan_name, 0, wxLEFT | wxRIGHT, this->FromDIP(5));
	flex->Add(chan_name, 1, wxEXPAND | wxLEFT | wxRIGHT, this->FromDIP(5));
	flex->Add(static_chan_slope, 0, wxLEFT | wxRIGHT, this->FromDIP(5));
	flex->Add(chan_slope, 1, wxEXPAND | wxLEFT | wxRIGHT, this->FromDIP(5));
	flex->Add(static_chan_shift, 0, wxLEFT | wxRIGHT, this->FromDIP(5));
	flex->Add(chan_shift, 1, wxEXPAND | wxLEFT | wxRIGHT, this->FromDIP(5));
	flex->Add(static_chan_unit, 0, wxLEFT | wxRIGHT, this->FromDIP(5));
	flex->Add(chan_unit, 1, wxEXPAND | wxLEFT | wxRIGHT, this->FromDIP(5));

	wxStaticLine* split = new wxStaticLine(this, wxID_ANY, wxDefaultPosition,
		this->FromDIP(wxDefaultSize),
		wxLI_HORIZONTAL,
		"");

	wxSizer* wxBtnsizer = CreateButtonSizer(wxOK | wxCANCEL);

	main_box->Add(static_lineartxt, 1, wxEXPAND | wxALL, this->FromDIP(10));
	main_box->Add(flex, 1, wxEXPAND | wxALL, this->FromDIP(20));
	main_box->Add(split, 0, wxEXPAND);
	main_box->Add(wxBtnsizer, 0, wxALIGN_RIGHT | wxALL, this->FromDIP(5));

	SetSizer(main_box);
	SetMinSize(this->FromDIP(wxSize(200, 200)));
	Fit();
}

ScaleDialog::~ScaleDialog() {}