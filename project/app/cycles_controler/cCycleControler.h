/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include <wx/wx.h>
#include <thread>
#include <mutex>
#include <iostream>

#include "enum.h"
#include "cCycle.h"
class cTable;

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
	bool get_current_step_param(STEPSTRUCT currentStep);
	~cCycleControler();

private:
	void poll();
	
	// Helper functions for poll()
	bool wait_for_step_duration(const std::stop_source& stop_source);
	bool is_cycle_completed() const;
	bool handle_cycle_completion(const std::stop_source& stop_source);
	void update_status_bar_completion();
	void send_stop_event();
	void cleanup_and_exit();

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

