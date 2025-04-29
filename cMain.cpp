/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#include "cMain.h"
#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/grid.h>
#include <wx/dcbuffer.h>
#include <vector>
#include <memory>

#include <shellapi.h>
#include "Shlobj.h"
#include "Shlobj_core.h"
#pragma comment(lib, "shell32")

#include <fstream>

#include "cDeviceMonitor.h"
#include "cConfig.h"
#include "cTable.h"


#include "resource.h"

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
EVT_TOOL(IDTOOL_OPENBTN, cMain::openButtonClicked)
EVT_TOOL(IDTOOL_SAVEBTN, cMain::saveButtonClicked)
EVT_TOOL(IDTOOL_SETTINGS, cMain::settingsButtonClicked)
EVT_TOOL(IDTOOL_EDITBTN, cMain::editButtonClicked)
EVT_TOOL(IDTOOL_PLAYBTN, cMain::playButtonClicked)
EVT_TOOL(IDTOOL_PLOT, cMain::plotButtonClicked)
EVT_TOOL(IDTOOL_EXIT, cMain::exitButtonClicked)

EVT_PAINT(cMain::Paintevt)
EVT_SIZE(cMain::Sizeevt)
EVT_SIZING(cMain::Moveevt)
EVT_MAXIMIZE(cMain::Maximizeevt)
wxEND_EVENT_TABLE()

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Lab++", wxPoint(200, 100), wxSize(1200, 600))
{
#ifdef _DEBUG
	//_crtBreakAlloc = 248;	
#endif
	if (this->GetHWND())
	{
		ShutdownBlockReasonCreate(this->GetHWND(), L"Do not stop before end of recording.");
	}


	/////////////////////////////////////////////////////////////
	//
	//	Called when close button is pressed
	//

	Bind(wxEVT_CLOSE_WINDOW, &cMain::OnCloseWindow, this);

	/////////////////////////////////////////////////////////////
	//
	//	Launch the device monitor tool
	//
	devmon->Notify();

	std::cout << "Current scale factor: " << this->GetDPIScaleFactor() << "\n";
	////////////////////////////////////////////////////////////////////////////////
	// OBJECT MANAGER
	////////////////////////////////////////////////////////////////////////////////
	std::cout << "cObjectmanager->getInstance()\n";
	cObjectmanager* manager = manager->getInstance();

	////////////////////////////////////////////////////////////////////////////////
	// MENUBAR
	////////////////////////////////////////////////////////////////////////////////
	wxMenuBar* main_menu = new wxMenuBar();

	wxMenu* file_menu = new wxMenu();
	wxMenu* edit_menu = new wxMenu();

	this->Bind(wxEVT_MENU, &cMain::settingsButtonClicked, this,
		file_menu->Append(wxID_ANY, "&Instruments configuration\t")->GetId());
	this->Bind(wxEVT_MENU, &cMain::editButtonClicked, this,
		file_menu->Append(wxID_ANY, "&Cycle configuration\t")->GetId());
	this->Bind(wxEVT_MENU, &cMain::plotButtonClicked, this,
		file_menu->Append(wxID_ANY, "&Graph window\t")->GetId());
	this->Bind(wxEVT_MENU, &cMain::MeasurementFolderButtonClicked, this,
		file_menu->Append(wxID_ANY, "&Open measurement folder\t")->GetId());
	file_menu->AppendSeparator();
	this->Bind(wxEVT_MENU, &cMain::exitButtonClicked, this,
		file_menu->Append(wxID_ANY, "&Exit\tESC")->GetId());

	main_menu->Append(file_menu, "&File");

	this->Bind(wxEVT_MENU, &cMain::OpenconfigFolderButtonClicked, this,
		edit_menu->Append(wxID_ANY, "&Open configuration folder\t")->GetId());
		this->Bind(wxEVT_MENU, &cMain::CreateDefaultScalesButtonClicked, this,
			edit_menu->Append(wxID_ANY, "&Reset custom scale to default\t")->GetId());
	this->Bind(wxEVT_MENU, &cMain::DeleteconfigButtonClicked, this,
		edit_menu->Append(wxID_ANY, "&Reset serializer (delete files)\t")->GetId());

	edit_menu->AppendSeparator();

	main_menu->Append(edit_menu, "&Configuration");

	SetMenuBar(main_menu);

	////////////////////////////////////////////////////////////////////////////////
	// TOOLBAR
	////////////////////////////////////////////////////////////////////////////////
	wxImage::AddHandler(new wxBMPHandler);
	//wxBitmap open(wxT("Open.bmp"), wxBITMAP_TYPE_BMP);
	//wxBitmap save(wxT("Save.bmp"), wxBITMAP_TYPE_BMP);
	//wxBitmap settings(wxT("Settings.bmp"), wxBITMAP_TYPE_BMP);
	wxBitmap settings("BMP0", wxBITMAP_TYPE_PNG_RESOURCE);
	//wxBitmap edit(wxT("Pixel editor.bmp"), wxBITMAP_TYPE_BMP);	
	wxBitmap edit("BMP1", wxBITMAP_TYPE_PNG_RESOURCE);
	//wxBitmap plot(wxT("Chart xy.bmp"), wxBITMAP_TYPE_BMP);
	wxBitmap plot("BMP2", wxBITMAP_TYPE_PNG_RESOURCE);
	//wxBitmap play(wxT("Play.bmp"), wxBITMAP_TYPE_BMP);
	//wxBitmap exit(wxT("Exit.bmp"), wxBITMAP_TYPE_BMP);
	wxBitmap exit("BMP3", wxBITMAP_TYPE_PNG_RESOURCE);

	wxToolBar* toolbar = CreateToolBar();
	//toolbar->AddTool(IDTOOL_OPENBTN, wxT("Open"), open);
	//toolbar->AddTool(IDTOOL_SAVEBTN, wxT("Save"), save);
	toolbar->AddTool(IDTOOL_SETTINGS, wxT("Conf"), settings);
	toolbar->AddTool(IDTOOL_EDITBTN, wxT("Edit"), edit);
	toolbar->AddTool(IDTOOL_PLOT, wxT("Plot"), plot);
	//toolbar->AddTool(IDTOOL_PLAYBTN, wxT("Play"), play);
	toolbar->AddTool(IDTOOL_EXIT, wxT("Exit"), exit);
	toolbar->SetToolBitmapSize(this->FromDIP(wxSize(64, 64)));
	toolbar->SetBackgroundColour(*wxWHITE);
	toolbar->Realize();

	////////////////////////////////////////////////////////////////////////////////
	// STATUS BAR
	////////////////////////////////////////////////////////////////////////////////
	statusBar = CreateStatusBar();
	statusBar->SetStatusText("Welcome to Lab++, your raw low level c++ acquizition software.");
	manager->set_status_bar(statusBar);

	////////////////////////////////////////////////////////////////////////////////
	// VERTICAL GLOBAL SIZER WND
	////////////////////////////////////////////////////////////////////////////////
	main_vsizer = new wxBoxSizer(wxVERTICAL);

	/*
	////////////////////////////////////////////////////////////////////////////////
	// OPEN WND
	////////////////////////////////////////////////////////////////////////////////
	cOpen* m_open = new cOpen(this);

	open_leftpanel = m_open->Getleftpan();
	panelTable.push_back(open_leftpanel); // save

	open_rightpanel = m_open->Getrightpan();
	panelTable.push_back(open_rightpanel); // save

	open_hsizer = m_open->Get_hsizer();
	main_vsizer->Add(open_hsizer, 1, wxEXPAND);
	open_hsizer->Show(false);

	////////////////////////////////////////////////////////////////////////////////
	// SAVE WND
	////////////////////////////////////////////////////////////////////////////////
	save_leftpanel = new wxPanel(this, 11005, wxDefaultPosition, wxSize(300, 600));
	save_leftpanel->SetBackgroundColour(wxColor(10, 60, 70));
	panelTable.push_back(save_leftpanel); // save

	save_rightpanel = new wxPanel(this, 11006, wxDefaultPosition, wxSize(600, 600));
	save_rightpanel->SetBackgroundColour(wxColor(00, 50, 60));
	panelTable.push_back(save_rightpanel); // save

	save_hsizer = new wxBoxSizer(wxHORIZONTAL);
	save_hsizer->Add(save_leftpanel, 0, wxEXPAND);
	save_hsizer->Add(save_rightpanel, 1, wxEXPAND);

	main_vsizer->Add(save_hsizer, 1, wxEXPAND);
	save_hsizer->Show(false);
	*/




	////////////////////////////////////////////////////////////////////////////////
	// CONFIG WND
	////////////////////////////////////////////////////////////////////////////////

	m_config = new cConfig(this, devmon, &signal_table);
	manager->set_config(m_config);
	config_leftpanel = m_config->Getleftpan();
	config_rightpanel = m_config->Getrightpan();
	config_hsizer = m_config->Get_hsizer();
	panelTable.push_back(config_leftpanel); // save
	panelTable.push_back(config_rightpanel); // save
	config_hsizer->Show(true);

	main_vsizer->Add(config_hsizer, 1, wxEXPAND);

	////////////////////////////////////////////////////////////////////////////////
	// EDIT WND
	////////////////////////////////////////////////////////////////////////////////
	m_table = new cTable (this, m_config);
	manager->set_table(m_table);
	table_leftpanel = m_table->Getleftpan();
	table_rightpanel = m_table->Getrightpan();
	wxSize sizep = table_rightpanel->GetSize();
	table_hsizer = m_table->Get_hsizer();
	panelTable.push_back(table_leftpanel); // save
	panelTable.push_back(table_rightpanel); // save
	table_hsizer->Show(false);

	main_vsizer->Add(table_hsizer, 1, wxEXPAND);

	////////////////////////////////////////////////////////////////////////////////
	// PLOT WND
	////////////////////////////////////////////////////////////////////////////////
	m_plot = new cPlot(this, GRAPH_NBPOINTS, &signal_table);
	manager->set_plot(m_plot);
	m_config->set_graph(m_plot);

	plot_leftpanel = m_plot->Getleftpan();
	plot_rightpanel = m_plot->Getrightpan();
	plot_hsizer = m_plot->Get_hsizer();
	panelTable.push_back(plot_leftpanel);
	panelTable.push_back(plot_rightpanel);
	plot_hsizer->Show(false);

	main_vsizer->Add(plot_hsizer, 1, wxEXPAND);
	m_graphrender = std::make_unique<cGraphrender>(m_plot, DISP_FREQ);
	m_statrender = std::make_unique<cStatrender>(m_plot, 1000, &signal_table);

	////////////////////////////////////////////////////////////////////////////////
	// PLAY WND
	////////////////////////////////////////////////////////////////////////////////
	/*
	cPlay* m_play = new cPlay(this);

	play_leftpanel = m_play->Getleftpan();
	panelTable.push_back(play_leftpanel); // save

	play_rightpanel = m_play->Getrightpan();
	panelTable.push_back(play_rightpanel); // save

	play_hsizer = m_play->Get_hsizer();
	main_vsizer->Add(play_hsizer, 1, wxEXPAND);
	play_hsizer->Show(false);
	*/
	////////////////////////////////////////////////////////////////////////////////
	// FOOTER
	////////////////////////////////////////////////////////////////////////////////
	m_footer = new cFooter(this, m_plot, m_table, m_config, devmon);
	manager->set_footer(m_footer);
	wxBoxSizer* hfootersier = m_footer->GetSizer();

	main_vsizer->Add(hfootersier, 0, wxEXPAND);
	m_graphrender->add_freq_footer(m_footer);

	////////////////////////////////////////////////////////////////////////////////
	// INI CONFIG LOADER
	////////////////////////////////////////////////////////////////////////////////

	// must load row in the table
	cfg_saver = std::make_unique<cInicfg>();


	////////////////////////////////////////////////////////////////////////////////
	// MAIN SIZER
	////////////////////////////////////////////////////////////////////////////////
	this->SetSizerAndFit(main_vsizer);
	this->SetSize(this->FromDIP(wxSize(900, 600)));

	//this->SetSizer(main_vsizer);
	//this->Layout();
	//wxSize s; s.SetWidth(600); s.SetHeight(600);
	//this->SetMinSize(s);
	//this->SetClientSize(this->FromDIP(wxSize(400, 300)));
}

void cMain::OnCloseWindow(wxCloseEvent& event)
{
	Destroy();
}

cMain::~cMain()
{
	if (this->GetHWND())
	{
		ShutdownBlockReasonDestroy(this->GetHWND());
	}

	std::cout << "cfg_saver, m_table, m_config, m_plot, m_graphrender, m_statrender, m_footer deleted in cMain.cpp\n";
	delete m_table;
	delete m_plot;
	delete m_footer;
	delete m_config;

	cfg_saver = nullptr; 
	m_config = nullptr;
	m_plot = nullptr;
	m_footer = nullptr;

	// destroy singleton
	std::cout << "cObjectmanager->getInstance()\n";
	cObjectmanager* manager = manager->getInstance();
	manager->kill();
#ifdef _DEBUG
	//_CrtDumpMemoryLeaks();
#endif

}

void cMain::openButtonClicked(wxCommandEvent& evt)
{
	if (!open_leftpanel->IsShown())
	{
		for (auto& i : panelTable) // Hide every panel reccorded in the table
			i->Hide();

		open_leftpanel->Show();
		open_rightpanel->Show();
		open_hsizer->Show(true);
		this->Layout();
	}
	evt.Skip();
}

void cMain::saveButtonClicked(wxCommandEvent& evt)
{
	if (!save_leftpanel->IsShown())
	{
		for (auto& i : panelTable) // Hide every panel reccorded in the table
			i->Hide();

		save_leftpanel->Show();
		save_rightpanel->Show();
		save_hsizer->Show(true);
		this->Layout();
	}
	evt.Skip();
}

void cMain::settingsButtonClicked(wxCommandEvent& evt)
{

	if (!config_leftpanel->IsShown())
	{
		for (auto& i : panelTable) // Hide every panel reccorded in the table
			i->Hide();

		config_leftpanel->Show();
		config_rightpanel->Show();
		config_hsizer->Show(true);
		this->Layout();
	}
	evt.Skip();
}

void cMain::CreateDefaultScalesButtonClicked(wxCommandEvent& evt)
{
	TCHAR szPath[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szPath)))
	{
		wcscat_s<MAX_PATH>(szPath, L"\\Lab++ScalePreset.ini");
		std::ifstream config_file(szPath);
		if (!config_file.is_open())
		{
			std::cout << "[*] File is missing, create new...\n";
			config_file.close();

			std::ofstream config_file(szPath);
			if (!config_file.is_open())
			{
				std::cout << "[!] Error creating file, something went wrong..., please verify your system right to access filesystem.\n";
				evt.Skip();
				return;
			}
			config_file << "[Citec 0-16 bar 0-10V sensor]\n";
			config_file << "name = Citec 0-16 bar 0-10V sensor\n";
			config_file << "slope = 1.6\n";
			config_file << "shift = 0\n";
			config_file << "unit = bar\n";
			config_file << "[0-5v 10sccm]\n";
			config_file << "name = 0-5v 10sccm\n";
			config_file << "slope = 2\n";
			config_file << "shift = 0\n";
			config_file << "unit = sccm\n";
			config_file << "[0 - 5v 1000sccm]\n";
			config_file << "name = 0-5v 1000sccm\n";
			config_file << "slope = 200\n";
			config_file << "shift = 0\n";
			config_file << "unit = sccm\n";

			config_file.close();

			MessageBox(0, L"Success. You must restart the software to use the new scale saved", L"Success", S_OK | MB_ICONINFORMATION);

		}
		else
		{
			MessageBox(0, L"Fail. File already exist, you must delete: \"Lab++ScalePreset.ini\" to write a new one.", L"Failure", S_OK| MB_ICONERROR);
		}
	}
	evt.Skip();
}

bool delete_all_conf_files(wchar_t* path)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind=0;

	hFind = FindFirstFile(path, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		printf("FindFirstFile failed (%d)\n", GetLastError());
		FindClose(hFind);
		return false;
	}
	else
	{
		if (FindFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
		{
			_tprintf(TEXT("found: %s\n"),FindFileData.cFileName);
		}
		do
		{
			FindNextFile(hFind, &FindFileData);
			if (hFind == INVALID_HANDLE_VALUE)
			{
				printf("FindNextFile failed (%d)\n", GetLastError());
				FindClose(hFind);
				return false;
			}
			if (FindFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
			{
				_tprintf(TEXT("found: %s\n"), FindFileData.cFileName);
				std::wstring filename = FindFileData.cFileName;
				// Compare if it is a config file to delete
				if (filename.compare(0,11, L"Lab++Config") == 0)
				{
					// build full path
					std::wstring full_path = path;
					size_t pos = full_path.find(L"*");
					full_path = full_path.substr(0, pos);
					full_path += filename;
					std::cout << "Erasing file:" << full_path << "\n";
					//DeleteFile
					DeleteFile(full_path.c_str());
				}

			}
		} while (hFind != INVALID_HANDLE_VALUE);
		
	}
	FindClose(hFind);
	return true;
}
void cMain::DeleteconfigButtonClicked(wxCommandEvent& evt)
{
	TCHAR szPath[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szPath)))
	{
		wcscat_s<MAX_PATH>(szPath, L"\\*");
		if (!delete_all_conf_files(szPath))
		{
			MessageBox(0, L"Fail to deleteconfig files.", L"Failure", S_OK | MB_ICONERROR);
		}
		MessageBox(0, L"Success. All config file \"Lab++Config*.ini\" has been deleted.", L"Success", S_OK | MB_ICONINFORMATION);
	}
	evt.Skip();
}

void cMain::OpenconfigFolderButtonClicked(wxCommandEvent& evt)
{
	TCHAR szPath[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szPath)))
	{
		wcscat_s<MAX_PATH>(szPath, L"\\");
		ShellExecute(0, L"open", szPath, NULL, NULL, 1);
	}
	evt.Skip();
}

void cMain::editButtonClicked(wxCommandEvent& evt)
{
	if (!table_leftpanel->IsShown())
	{
		for (auto& i : panelTable) // Hide every panel reccorded in the table
			i->Hide();

		table_leftpanel->Show();
		table_rightpanel->Show();
		table_hsizer->Show(true);
		this->Layout();
	}

	evt.Skip();
}

void cMain::playButtonClicked(wxCommandEvent& evt)
{
	if (!play_leftpanel->IsShown())
	{
		for (auto& i : panelTable) // Hide every panel reccorded in the play
			i->Hide();

		play_leftpanel->Show();
		play_rightpanel->Show();
		play_hsizer->Show(true);
		this->Layout();
	}

	evt.Skip();
}

void cMain::plotButtonClicked(wxCommandEvent& evt)
{
	if (!plot_leftpanel->IsShown())
	{
		for (auto& i : panelTable) // Hide every panel reccorded in the play
			i->Hide();

		plot_leftpanel->Show();
		plot_rightpanel->Show();
		plot_hsizer->Show(true);
		this->Layout();
	}
	evt.Skip();
}

void cMain::exitButtonClicked(wxCommandEvent& evt)
{
	MSGBOXPARAMS mbp;
	mbp.cbSize = sizeof(MSGBOXPARAMS);
	mbp.hwndOwner = NULL;// GetFocus();
	mbp.hInstance = GetModuleHandle(NULL);
	mbp.lpszText = L"Are you sure to Exit ?";
	mbp.lpszCaption = L"Quit ? ";
	mbp.dwStyle = MB_YESNO | MB_SYSTEMMODAL | MB_USERICON;
	mbp.dwLanguageId = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT);
	mbp.lpfnMsgBoxCallback = NULL;
	mbp.dwContextHelpId = 0;
	mbp.lpszIcon = MAKEINTRESOURCE(IDI_ICON1);

	INT iResult = MessageBoxIndirect(&mbp);//MessageBoxIndirect(GetFocus(), L"Are you sure to Exit ?", L"Quit ? ", MB_ICONQUESTION | MB_YESNO  | MB_SYSTEMMODAL);
	if (iResult == IDYES)
	{
		// TODO: Check if measurements are running and close it properly
		// TODO: Save current table config	

		// Send click on STOP btn
		bool on = m_plot->get_graph_state();
		if (on)
		{
			wxCommandEvent evt = wxCommandEvent(wxEVT_COMMAND_BUTTON_CLICKED, IDC_STARTBTN);
			wxPostEvent(this, evt);
		}
		m_graphrender->Stop();
		m_statrender->Stop();
		cMain::Close();
	}
	else
	{

	}
}

void cMain::Paintevt(wxPaintEvent& evt)
{
	if (table_rightpanel != nullptr)
	{
		m_table->GridResize(m_table->grid);
	}
	if (m_graphrender)
	{
		m_graphrender->QuickNotify(); // Redraw graph
	}
	//this->Refresh();
	evt.Skip();
}

void cMain::Sizeevt(wxSizeEvent& evt)
{
	if (m_graphrender)
	{
		m_graphrender->Notify(); // Redraw graph
	}
	this->Refresh();//////////////////////////////////////remove this line -> graph lag disapear but degrade picture displayed in setting	
	evt.Skip();
}

void cMain::Moveevt(wxSizeEvent& evt)
{
	//this->Refresh();
	evt.Skip();
}

void cMain::Maximizeevt(wxMaximizeEvent& evt)
{

	this->Refresh();
	evt.Skip();
}

wxPanel cMain::Getplotleftpanel(wxPanel* plot_leftpanel_)
{
	return plot_leftpanel;
}

void cMain::MeasurementFolderButtonClicked(wxCommandEvent& evt)
{
	// Open current application directory
	wchar_t current_dir[MAX_PATH];
	memset(current_dir, 0, sizeof(current_dir));
	DWORD length = sizeof(current_dir) / 4; // TODO: change /4 hardcoded value to prevent the BOF
	GetCurrentDirectory(length, current_dir);
	ShellExecute(NULL, L"explore", current_dir, NULL, NULL, SW_SHOWNORMAL);
	evt.Skip();
}

void cMain::StopDiscoverDeviceTimer()
{
	devmon->lookup_stop();
}

