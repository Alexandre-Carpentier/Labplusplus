#pragma once
#include <wx/scrolwin.h>
#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/grid.h>

#include <list>
#include <vector>
#include <Windows.h>
#include <process.h>
#include "WinGraph.h"
#include "cSignalTable.h"

static HGRAPH hGraph;

class cMeasurementControler;
class cObjectmanager;
class cMeasurementmanager;

#include "cSignalBtn.h"
#include "cMeasurement.h"

class cPlot
{

private:
	wxPanel* plot_leftpanel_ = nullptr;;
	wxPanel* plot_rightpanel_ = nullptr;;
	wxBoxSizer* plot_hsizer_ = nullptr;;
	wxWindow* inst_ = nullptr;;
	wxSizer* main_vsizer_ = nullptr;
	cMeasurementmanager* meas_manager = nullptr;


	wxBoxSizer* left_vsizer = nullptr;

	const static int MAX_SIG = 64; // Must add new IDCCHANINFO0+i in enum.h
	int CURRENT_SIG;
	wxBoxSizer* legend_vsizer = nullptr;

	//std::list<CHAN_LEGEND_STRUCT> chan_legend_struct_list;
	wxCustomButton* chan_info_btn_pool[MAX_SIG];

public:
	cPlot(wxWindow* inst, int nbPoints);
	~cPlot();

	void update_gui();

	// Channel btn class
	//void init_chan_to_gui(std::string chan_name, std::string chan_addr, std::string chan_unit, wxColor chan_color);
	//void resize_chan_number_to_gui(size_t max_item);
	//int gui_get_last_active_channel_number();
	//int get_chan_number_to_gui();
	//void add_chan_to_gui(std::string chan_name, std::string chan_addr, std::string chan_unit, wxColor chan_color, size_t position);
	//void draw_chan_to_gui();
	//void remove_chan_to_gui(size_t position);
	void update_chan_name_to_gui(std::string name, size_t position); // change the signal name in the channel btn class
	void update_chan_physical_name_to_gui(std::string name, size_t position); // change the signal physical name in the channel btn class
	void update_chan_physical_unit_to_gui(std::string unit, size_t position); // change the signal physical unit in the channel btn class
	void update_chan_statistic_labels();
	int GuiPositionToWingraphPosition(wxWindowID id);
	void update_chan_color(wxColor col, wxWindowID id);
	void reset_chan_statistic_labels(wxWindowID id);

	// Wingraph
	HGRAPH get_graph_handle();
	bool get_graph_state();
	int get_graph_signal_count();
	void set_graph_filter(FILTER_M FilteringType);
	void set_signal_name(std::string signame, int position);  // change the signal name in the WinGraph module
	void show_all_signals(bool isDisplayed);
	void start_graph(FILTER_M FilteringType, LOGGER_M ReccordingType, int SignalNumber);
	void stop_graph();
	void graph_addpoint(const int signb, double val[]);

	double get_signal_min_value(int SignalNumber);
	double get_signal_average_value(int SignalNumber);
	double get_signal_max_value(int SignalNumber);
	void set_signal_visible(bool bShow, int SignalNumber);

	void set_autoscale(bool enable);
	void set_zoom(int factor);
	int get_zoom();

	void SizeGraph();
	void RenderGraph();

	// wxWidget panel
	wxPanel* Getleftpan();
	wxPanel* Getrightpan();
	wxBoxSizer* Get_hsizer();
};

