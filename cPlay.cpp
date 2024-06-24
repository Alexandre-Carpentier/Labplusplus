#include "cPlay.h"

cPlay::cPlay(wxWindow* inst)
{
	std::cout << "cOpen ctor...\n";
	inst_ = inst;
	play_leftpanel_ = new wxPanel(inst, 11014, wxDefaultPosition, wxSize(500, 600));
	play_leftpanel_->SetBackgroundColour(wxColor(00, 50, 60));

	wxButton* meas_folder_btn = new wxButton(play_leftpanel_, IDCMEASFOLDER, "Open measurement folder", wxPoint(0, 0), wxSize(500, 40), wxSUNKEN_BORDER);
	inst->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cPlay::MeasFolderButtonClicked, this, IDCMEASFOLDER);
	meas_folder_btn->SetFont(meas_folder_btn->GetFont().Scale(1.5));

	wxButton* origin_export_btn = new wxButton(play_leftpanel_, IDCORIGINEXPORT, "Open with Origin Graphing & Analysis", wxPoint(0, 0), wxSize(500, 40), wxSUNKEN_BORDER);
	inst->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cPlay::OriginExportButtonClicked, this, IDCORIGINEXPORT);
	origin_export_btn->SetFont(origin_export_btn->GetFont().Scale(1.5));

	wxButton* jpg_export_btn = new wxButton(play_leftpanel_, IDCJPGEXPORT, "Export to jpg", wxPoint(0, 0), wxSize(500, 40), wxSUNKEN_BORDER);
	inst->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cPlay::JpgExportButtonClicked, this, IDCJPGEXPORT);
	jpg_export_btn->SetFont(jpg_export_btn->GetFont().Scale(1.5));

	wxButton* excel_export_btn = new wxButton(play_leftpanel_, IDCEXCELEXPORT, "Export to xlsx", wxPoint(0, 0), wxSize(500, 40), wxSUNKEN_BORDER);
	inst->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cPlay::ExcelExportButtonClicked, this, IDCEXCELEXPORT);
	excel_export_btn->SetFont(excel_export_btn->GetFont().Scale(1.5));

	left_vsizer = new wxBoxSizer(wxVERTICAL);

	left_vsizer->Add(meas_folder_btn, 0, wxTOP | wxLEFT | wxRIGHT | wxEXPAND, 3);
	left_vsizer->Add(origin_export_btn, 0, wxTOP | wxLEFT | wxRIGHT | wxEXPAND, 3);
	left_vsizer->Add(jpg_export_btn, 0, wxTOP | wxLEFT | wxRIGHT | wxEXPAND, 3);
	left_vsizer->Add(excel_export_btn, 0, wxTOP | wxLEFT | wxRIGHT | wxBOTTOM | wxEXPAND, 3);
	play_leftpanel_->SetSizerAndFit(left_vsizer);


	play_rightpanel_ = new wxPanel(inst, 11015, wxDefaultPosition, wxSize(600, 600), wxSUNKEN_BORDER);

	wxBoxSizer* wall_vsizer = new wxBoxSizer(wxHORIZONTAL);
	wxButton* wallpaper = new wxButton(play_rightpanel_, wxID_ANY, "Not implemented yet", wxPoint(0, 0), wxSize(200, 20), wxNO_BORDER);
	wallpaper->SetBitmap(wxBitmap(wxT("Japan.png"), wxBITMAP_TYPE_PNG));
	wall_vsizer->Add(wallpaper, 1, wxEXPAND);
	play_rightpanel_->SetBackgroundColour(wxColor(00, 60, 60));
	play_rightpanel_->SetSizerAndFit(wall_vsizer);
	// Load filesystem

	play_hsizer_ = new wxBoxSizer(wxHORIZONTAL);
	play_hsizer_->Add(play_leftpanel_, 0, wxEXPAND);
	play_hsizer_->Add(play_rightpanel_, 1, wxEXPAND);
	play_hsizer_->Show(false);
}

void cPlay::MeasFolderButtonClicked(wxEvent& evt)
{
	wchar_t current_dir[MAX_PATH];
	memset(current_dir, 0, sizeof(current_dir));
	DWORD length = sizeof(current_dir) / 4; // TODO: change /4 hardcoded value to prevent the BOF
	GetCurrentDirectory(length, current_dir);
	ShellExecute(NULL, L"explore", current_dir, NULL, NULL, SW_SHOWNORMAL);
	evt.Skip();
}

void cPlay::OriginExportButtonClicked(wxEvent& evt)
{
	MessageBox(GetFocus(), L"Not yet implemented", L"Sorry", S_OK);
	evt.Skip();
}

void cPlay::JpgExportButtonClicked(wxEvent& evt)
{
	MessageBox(GetFocus(), L"Not yet implemented", L"Sorry", S_OK);
	//Create a DC for the whole screen area
	//wxScreenDC dcScreen;

	//Get the size of the screen/DC
	//wxCoord screenWidth, screenHeight;
	//dcScreen.GetSize(&screenWidth, &screenHeight);

	//Create a Bitmap that will later on hold the screenshot image
	//Note that the Bitmap must have a size big enough to hold the screenshot
	//-1 means using the current default colour depth
	//wxBitmap screenshot(screenWidth, screenHeight, -1);

	//Create a memory DC that will be used for actually taking the screenshot
	//wxMemoryDC memDC;
	//Tell the memory DC to use our Bitmap
	//all drawing action on the memory DC will go to the Bitmap now
	//memDC.SelectObject(screenshot);
	//Blit (in this case copy) the actual screen on the memory DC
	//and thus the Bitmap
	//memDC.Blit(0, //Copy to this X coordinate
		//0, //Copy to this Y coordinate
		//screenWidth, //Copy this width
		//screenHeight, //Copy this height
		//&dcScreen, //From where do we copy?
		//0, //What's the X offset in the original DC?
		//0  //What's the Y offset in the original DC?
	//);
	//Select the Bitmap out of the memory DC by selecting a new
	//uninitialized Bitmap
	//memDC.SelectObject(wxNullBitmap);

	//Our Bitmap now has the screenshot, so let's save it :-)
	//screenshot.SaveFile("screenshot.jpg", wxBITMAP_TYPE_JPEG);

	evt.Skip();
}

void cPlay::ExcelExportButtonClicked(wxEvent& evt)
{
	//MessageBox(GetFocus(), L"Not yet implemented", L"Sorry", S_OK);

	wxFileDialog
		openFileDialog(this->inst_, _("Open *.lab file"), "", "",
			"LAB files (*.lab)|*.lab", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;     // the user changed idea...

	// proceed loading the file chosen by the user;
	// this can be done with e.g. wxWidgets input streams:
	wxFileInputStream input_stream(openFileDialog.GetPath());
	if (!input_stream.IsOk())
	{
		wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
		return;
	}

	std::thread task(&create_xlsx_from_saved_data, openFileDialog.GetPath());

	task.detach();

	evt.Skip();
}


cPlay::~cPlay()
{
	std::cout << "cPlay dtor...\n";
}

wxPanel* cPlay::Getleftpan()
{
	return play_leftpanel_;
}

wxPanel* cPlay::Getrightpan()
{
	return play_rightpanel_;
}

wxBoxSizer* cPlay::Get_hsizer()
{
	return play_hsizer_;
}



