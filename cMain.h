/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#pragma once
#if _WIN64
//#pragma comment (lib, "WinGraph.lib")
#else 
//#pragma comment (lib, "WinGraph32.lib")
#endif
#pragma comment (lib, "Advapi32.lib")
#pragma comment (lib, "Gdi32.lib")
#pragma comment (lib, "Winspool.lib")
#pragma comment (lib, "Ole32.lib")

#pragma comment (lib, "Comctl32.lib")
#pragma comment (lib, "Comdlg32.lib")
#pragma comment (lib, "Rpcrt4.lib")
#pragma comment (lib, "Kernel32.lib")
#pragma comment(lib, "SetupAPI.lib")
#pragma comment(lib, "Winusb.lib")

///////////////////////////////////////////////////////////////////////////////////////




class cDeviceMonitor;

#include"cObjectmanager.h"
#include"cGraphrender.h"
#include"cStatrender.h"
#include"cInicfg.h"
#include "cSignalTable.h"

/*
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif
*/
//#include <vld.h>

/*
#include"cOpen.h"
#include"cPlay.h"
#include"cPlot.h"


#include"cStatrender.h"

#include"cCycleControler.h"


#include"cInicfg.h"
#include "LoadBitmapFromRessource.h"


class cOpen;
class cPlay;
class cPlot;
class cFooter;
class cObjectmanager;
class cRender;
class cTable;
class cCycleControler;
class cConfig;
class cInicfg;

*/
class cMain :public wxFrame
{
public:
	const enum { DISP_FREQ = 250 };						// ms to draw
	const enum { GRAPH_NBPOINTS = 1000 };					// pts

	HMODULE hVisa = nullptr;
	////////////////////////////////////////////////////////////////////////////////
	// DEV MONITOR
	////////////////////////////////////////////////////////////////////////////////
	std::shared_ptr<cDeviceMonitor> devmon = std::make_shared<cDeviceMonitor>();
	////////////////////////////////////////////////////////////////////////////////
	// SIGNAL TABLE
	////////////////////////////////////////////////////////////////////////////////
	cSignalTable signal_table;

	wxStatusBar* statusBar = nullptr;
	wxToolBar* toolbar = nullptr;

	std::vector <wxPanel*> panelTable;

	wxPanel* open_leftpanel = nullptr;
	wxPanel* open_rightpanel = nullptr;
	wxBoxSizer* open_hsizer = nullptr;

	wxPanel* save_leftpanel = nullptr;
	wxPanel* save_rightpanel = nullptr;
	wxBoxSizer* save_hsizer = nullptr;

	cConfig* m_config = nullptr;
	wxPanel* config_leftpanel = nullptr;
	wxPanel* config_rightpanel = nullptr;
	wxBoxSizer* config_hsizer = nullptr;

	cTable* m_table = nullptr;
	wxPanel* table_leftpanel = nullptr;
	wxPanel* table_rightpanel = nullptr;
	wxBoxSizer* table_hsizer = nullptr;

	wxPanel* play_leftpanel = nullptr;
	wxPanel* play_rightpanel = nullptr;
	wxBoxSizer* play_hsizer = nullptr;

	cPlot* m_plot = nullptr;
	wxPanel* plot_leftpanel = nullptr;
	wxPanel* plot_rightpanel = nullptr;
	wxBoxSizer* plot_hsizer = nullptr;

	std::unique_ptr<cGraphrender> m_graphrender;
	std::unique_ptr <cStatrender> m_statrender;

	cFooter* m_footer = nullptr;

	std::unique_ptr<cInicfg> cfg_saver;

	wxBoxSizer* main_vsizer = nullptr;

	wxDECLARE_EVENT_TABLE();

public:
	cMain();
	void OnCloseWindow(wxCloseEvent& event);
	~cMain();

	void settingsButtonClicked(wxCommandEvent& evt);

	void CreateDefaultScalesButtonClicked(wxCommandEvent& evt);
	void DeleteconfigButtonClicked(wxCommandEvent& evt);
	void OpenconfigFolderButtonClicked(wxCommandEvent& evt);
	
	void openButtonClicked(wxCommandEvent& evt);
	void saveButtonClicked(wxCommandEvent& evt);
	void editButtonClicked(wxCommandEvent& evt);
	void playButtonClicked(wxCommandEvent& evt);
	void plotButtonClicked(wxCommandEvent& evt);
	void exitButtonClicked(wxCommandEvent& evt);

	void PaintJapan(wxPaintEvent& evt);
	void Paintevt(wxPaintEvent& evt);
	void Sizeevt(wxSizeEvent& evt);
	void Moveevt(wxSizeEvent& evt);
	void Maximizeevt(wxMaximizeEvent& evt);

	wxPanel Getplotleftpanel(wxPanel* plot_leftpanel_);
	void MeasurementFolderButtonClicked(wxCommandEvent& evt);
	void StopDiscoverDeviceTimer();
};

