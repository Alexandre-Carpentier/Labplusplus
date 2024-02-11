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

class cPlot;
class cCycle;
class cTick;
class cDurationStatisticCtrl;

class cTable
{
public:
	wxGrid* grid = nullptr;
private:
	wxWindow* inst_ = nullptr;
	wxPanel* table_leftpanel_ = nullptr;
	wxPanel* table_rightpanel_ = nullptr;
	wxBoxSizer* table_hsizer_ = nullptr;
	const int COL_NB = 4; // Whitout plugin 
	const int LINE_NB = 200;
	size_t pugin_number = 0;

	cDurationStatisticCtrl* stat = nullptr;
	cCycle* m_cycle = nullptr;

	cConfig* m_config_ = nullptr;

	wxTextCtrl* loop = nullptr;

public:
	cTable(wxWindow* inst, cConfig* m_config);

	~cTable();

	void destroy_cycle();

	cCycle* load_cycle();

	void set_line_highlight(const int line);

	void set_lines_white();

	int get_last_active_line();

	bool IsActiveLine(const int line);

	void GridResize(wxGrid* grid);

	void set_loop_count(int count);

	wxPanel* Getleftpan();

	wxPanel* Getrightpan();

	wxBoxSizer* Get_hsizer();

	void clearButtonClicked(wxCommandEvent& evt);
};

class cDurationStatisticCtrl : public wxWindow, wxTimer
{
public:
	cDurationStatisticCtrl(wxWindow* parent);
	~cDurationStatisticCtrl();
	void start(cCycle* m_cycle, cTable* m_table);
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

	cCycle* m_cycle_ = nullptr;
	cTable* m_table_ = nullptr;

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
