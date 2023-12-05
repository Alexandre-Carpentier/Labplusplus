#pragma once
/*
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif
*/
#include "resource.h"
#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/grid.h>
#include <wx/dcbuffer.h>
#include <vector>
#include <memory>

#include "cDeviceMonitor.h"

#pragma comment (lib, "WinGraph.lib")

#pragma comment (lib, "Advapi32.lib")
#pragma comment (lib, "Gdi32.lib")
#pragma comment (lib, "Winspool.lib")
#pragma comment (lib, "Ole32.lib")
#pragma comment (lib, "Comctl32.lib")
#pragma comment (lib, "Comdlg32.lib")
#pragma comment (lib, "Rpcrt4.lib")

#pragma comment (lib, "Kernel32.lib")


class cOpen;
class cPlay;
class cPlot;
class cFooter;
class cRender;
class cTable;
class cCycleControler;
class cConfig;
class cObjectmanager;
class cInicfg;

class cMain :public wxFrame
{


public:

	cMain();
	~cMain();

public:

	enum { DISP_FREQ = 100 };						// ms to draw
	enum { GRAPH_NBPOINTS = 1000000 };				// pts

	std::unique_ptr<cDeviceMonitor> devmon;

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

	cRender* m_render = nullptr;

	cFooter* m_footer = nullptr;

	cInicfg* cfg_saver = nullptr;

	wxBoxSizer* main_vsizer = nullptr;

	wxDECLARE_EVENT_TABLE();

	void settingsButtonClicked(wxCommandEvent& evt);
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

};

