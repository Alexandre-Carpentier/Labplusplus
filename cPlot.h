/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include <wx/scrolwin.h>
#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/grid.h>
#include <wx/bannerwindow.h>

#include <list>
#include <vector>
#include <Windows.h>
#include <process.h>

#pragma comment(lib, "LogLibrary.lib")
#include "LogLibrary\log.h"

#include "cMeasurementmanager.h"
#include "cSignalBtn.h"
#include "cTick.h"

#include "PlotWindow.h"

class cSignalTable;

class cPlot
{
private:
	// Use to log data on disk
	cLog logger;
	cTick tick;

	wxPanel* plot_leftpanel_ = nullptr;
	wxPanel* plot_rightpanel_ = nullptr;
	wxBoxSizer* plot_hsizer_ = nullptr;
	wxWindow* inst_ = nullptr;
	wxSizer* main_vsizer_ = nullptr;
	cMeasurementmanager* meas_manager = nullptr;

	std::unique_ptr<PlotWindow> plot;

	wxBoxSizer* left_vsizer = nullptr;

	const static int MAX_SIG = 64;
	int CURRENT_SIG;
	wxBoxSizer* legend_vsizer = nullptr;

	wxCustomButton* chan_info_btn_pool[MAX_SIG];
	cSignalTable* sig_ = nullptr;

public:
	cPlot(wxWindow* inst, int nbPoints, cSignalTable* signal_table);
	~cPlot();

	void update_gui();

	void update_chan_name_to_gui(MEAS_TYPE type, std::string name, size_t position); // change the signal name in the channel btn class
	void update_chan_physical_name_to_gui(std::string name, size_t position); // change the signal physical name in the channel btn class
	void update_chan_physical_unit_to_gui(std::string unit, size_t position); // change the signal physical unit in the channel btn class
	void update_chan_statistic_labels();
	int GetButtonBaseID();
	int GuiPositionToWingraphPosition(wxWindowID id);
	void update_chan_color(wxColor col, wxWindowID id);
	void reset_chan_statistic_labels(wxWindowID id);
	void reset_chan_statistic_by_signal_position(size_t signal_position);

	bool get_graph_state();
	std::string get_graph_filename();
	LOGTYPE get_graph_logger_mode();
	int get_graph_signal_count();

	void set_signal_filter(FILTER_M FilteringType, int position);
	void set_signal_filter_threshold(float intensity, int position);
	void set_signal_name(std::string signame, int position);  // change the signal name in the WinGraph module
	void show_all_signals(bool isDisplayed);
	void start_graph(LOGTYPE ReccordingType, int SignalNumber, std::string opt_header);
	void stop_graph();
	void graph_addpoint(const int signb, double val[]);
	void graph_addpoints(const int signb, double* val[], int chunk_size);

	double get_signal_min_value(size_t id, int SignalNumber);
	double get_signal_average_value(int SignalNumber);
	double get_signal_max_value(int SignalNumber);
	void set_signal_visible(bool bShow, int SignalNumber);

	void set_autoscale(bool enable);
	void set_zoom(int factor);
	int get_zoom();

	void SizeGraph();
	void RenderGraph();
	void Refresh();

	// wxWidget panel
	wxPanel* Getleftpan();
	wxPanel* Getrightpan();
	wxBoxSizer* Get_hsizer();
};

