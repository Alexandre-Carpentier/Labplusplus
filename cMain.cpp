#include "cMain.h"

#include"cOpen.h"
#include"cPlay.h"
#include"cPlot.h"
#include"cFooter.h"
#include"cRender.h"
#include"cTable.h"
#include"cCycleControler.h"
#include"cConfig.h"
#include"cObjectmanager.h"
#include"cInicfg.h"
#include "LoadBitmapFromRessource.h"

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

	//__asm {nop};

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

	wxMenu* menu = new wxMenu();

	this->Bind(wxEVT_MENU, &cMain::settingsButtonClicked, this,
		menu->Append(wxID_ANY, "&Instruments configuration\t1")->GetId());
	this->Bind(wxEVT_MENU, &cMain::editButtonClicked, this,
		menu->Append(wxID_ANY, "&Cycle configuration\t2")->GetId());
	this->Bind(wxEVT_MENU, &cMain::plotButtonClicked, this,
		menu->Append(wxID_ANY, "&Graph window\t3")->GetId());
	menu->AppendSeparator();
	this->Bind(wxEVT_MENU, &cMain::exitButtonClicked, this,
		menu->Append(wxID_ANY, "&Exit\tESC")->GetId());

	main_menu->Append(menu, "&File");

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

	m_config = new cConfig(this);
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
	m_table = new cTable(this);
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
	m_plot = new cPlot(this, GRAPH_NBPOINTS);
	manager->set_plot(m_plot);
	m_config->set_graph(m_plot);

	plot_leftpanel = m_plot->Getleftpan();
	plot_rightpanel = m_plot->Getrightpan();
	plot_hsizer = m_plot->Get_hsizer();
	panelTable.push_back(plot_leftpanel);
	panelTable.push_back(plot_rightpanel);
	plot_hsizer->Show(false);

	main_vsizer->Add(plot_hsizer, 1, wxEXPAND);
	m_render = new cRender(m_plot, DISP_FREQ);

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
	m_footer = new cFooter(this, m_plot, m_table, m_config);
	manager->set_footer(m_footer);
	wxBoxSizer* hfootersier = m_footer->GetSizer();

	main_vsizer->Add(hfootersier, 0, wxEXPAND);
	m_render->add_freq_footer(m_footer);

	////////////////////////////////////////////////////////////////////////////////
	// INI CONFIG LOADER
	////////////////////////////////////////////////////////////////////////////////
	cfg_saver = new cInicfg;


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

cMain::~cMain()
{
	std::cout << "cfg_saver, m_table, m_config, m_plot, m_render, m_footer deleted in cMain.cpp\n";
	delete cfg_saver;
	delete m_table;
	delete m_config;
	delete m_plot;
	delete m_render;
	delete m_footer;

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

		cMain::Close();
		m_render->Stop();
	}
	else
	{

	}
	evt.Skip();
}

void cMain::Paintevt(wxPaintEvent& evt)
{
	if (table_rightpanel != nullptr)
	{
		m_table->GridResize(m_table->grid);
	}

	//this->config_rightpanel->Refresh();
	//this->Refresh();
	evt.Skip();
}

void cMain::Sizeevt(wxSizeEvent& evt)
{
	this->Refresh();//////////////////////////////////////remove graph lag but degrade picture displayed in setting
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


