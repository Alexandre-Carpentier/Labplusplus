#pragma once
#include <wx/wx.h>
#include <thread>
#include <mutex>
#include <iostream>

#include "enum.h"
#include "cCycle.h"
#include "cTable.h"

class cCycleControler /* : public wxWindow*/
{
public:
	// Create a lock shared by cCycleControler and cMeasurementControler
	std::mutex cycle_mutex;

	cCycleControler(cTable* m_table, wxWindow* inst);
	void start();
	void stop();
	int get_current_step();
	int get_current_loop();
	int get_total_step();
	int get_total_loop();
	STEPSTRUCT get_current_step_param();
	~cCycleControler();
private:
	void poll();
	std::shared_ptr<cCycle> m_cycle = std::make_shared<cCycle>();

	cTable* m_table_ = nullptr;
	wxWindow* inst_ = nullptr;

	std::jthread thread;

	inline long long PerformanceFrequency();
	inline long long PerformanceCounter();

	long long start_tick = PerformanceCounter();
	long long end_tick = PerformanceCounter();
	double delta_tick;
	long long freq = PerformanceFrequency();
};

