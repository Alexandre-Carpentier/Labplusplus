#include "cOpen.h"

cOpen::cOpen(wxWindow* inst)
{
	std::cout << "cOpen ctor...\n";
	inst_ = inst;
	open_leftpanel_ = new wxPanel(inst, 11014, wxDefaultPosition, wxSize(300, 600));
	open_leftpanel_->SetBackgroundColour(wxColor(00, 50, 60));


	open_rightpanel_ = new wxPanel(inst, 11015, wxDefaultPosition, wxSize(600, 600), wxSUNKEN_BORDER);

	wxBoxSizer* wall_vsizer = new wxBoxSizer(wxHORIZONTAL);
	wxButton* wallpaper = new wxButton(open_rightpanel_, wxID_ANY, "Not implemented yet", wxPoint(0, 0), wxSize(200, 20), wxNO_BORDER);
	wallpaper->SetBitmap(wxBitmap(wxT("Japan.png"), wxBITMAP_TYPE_PNG));
	wall_vsizer->Add(wallpaper, 1, wxEXPAND);
	open_rightpanel_->SetBackgroundColour(wxColor(00, 60, 60));
	open_rightpanel_->SetSizerAndFit(wall_vsizer);
	// Load filesystem

	open_hsizer_ = new wxBoxSizer(wxHORIZONTAL);
	open_hsizer_->Add(open_leftpanel_, 0, wxEXPAND);
	open_hsizer_->Add(open_rightpanel_, 1, wxEXPAND);
	open_hsizer_->Show(false);
}

cOpen::~cOpen()
{
	std::cout << "cOpen dtor...\n";
}

wxPanel* cOpen::Getleftpan()
{
	return open_leftpanel_;
}

wxPanel* cOpen::Getrightpan()
{
	return open_rightpanel_;
}

wxBoxSizer* cOpen::Get_hsizer()
{
	return open_hsizer_;
}



