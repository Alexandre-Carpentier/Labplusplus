#pragma once
#include <wx/wx.h>
#include <wx/grid.h>
#include <vector>
#include <Windows.h>
#include "enum.h"
#include <string>
#include <memory>
#include "cObjectmanager.h"

#include "cConfig.h"
#include"cPlot.h"

#include "cCycle.h"
#include "cTick.h"
#include <format>

class cDurationStatisticCtrl;

class cTable
{
public:
	wxGrid* grid = nullptr;
	cTable(wxWindow* inst, cConfig* m_config);
	int get_step_number();
	int get_loop_number();
	double get_total_step_duration();
	std::vector<STEPSTRUCT> get_step_table();
	void set_line_highlight(const int line);
	void set_lines_white();
	void set_loop_count(int count);
	int get_last_active_line();
	int get_last_active_col();
	bool IsActiveLine(const int line);
	void start_statistic(std::shared_ptr<cCycle>);
	void stop_statistic();
	// cMain access
	wxPanel* Getleftpan();
	wxPanel* Getrightpan();
	wxBoxSizer* Get_hsizer();
	void GridResize(wxGrid* grid);
	~cTable();
private:
	wxWindow* inst_ = nullptr;
	wxPanel* table_leftpanel_ = nullptr;
	wxPanel* table_rightpanel_ = nullptr;
	wxBoxSizer* table_hsizer_ = nullptr;
	const int COL_NB = 4; // Whitout plugin 
	const int LINE_NB = 200;
	size_t plugin_number = 0;

	cDurationStatisticCtrl* stat = nullptr;

	cConfig* m_config_ = nullptr;
	cCycle* m_cycle_ = nullptr;

	wxTextCtrl* loop = nullptr;

	void clearButtonClicked(wxCommandEvent& evt);
};

class cDurationStatisticCtrl : public wxWindow, wxTimer
{
public:
	cDurationStatisticCtrl(wxWindow* parent);
	~cDurationStatisticCtrl();
	void start(std::shared_ptr<cCycle>  m_cycle);
	void stop();
	void reset();


	void paintEvent(wxPaintEvent& evt);
	void paintNow();
	void render(wxDC& dc);
private:
	std::string get_current_timestamp();
	std::string add_to_current_timestamp(size_t duration_s);
	std::string compute_elapsed_timestamp();
	void Notify();

	std::shared_ptr<cCycle> m_cycle_;

	cTick* tick = nullptr;
	wxStaticText* started = nullptr;
	wxStaticText* end = nullptr;
	wxStaticText* elapsed = nullptr;
	wxStaticText* cycle_step_state = nullptr;

	// Elapsed timestamp computing
	SYSTEMTIME savedst = { 0 };
	FILETIME savedft = { 0 };

	// Update step performed
	size_t saved_total_cycle = 0;
	size_t saved_total_step = 0;
};
