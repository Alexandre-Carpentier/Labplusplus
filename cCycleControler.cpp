/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#include "cCycleControler.h"
#include <wx/wx.h>
#include <thread>
#include <Windows.h>
#include <mutex>
#include <memory>
#include <vector>

#include "cCycle.h"
#include "cCycleControler.h"
#include "cPlot.h"
#include "cFooter.h"
#include "cObjectmanager.h"
#include "cMeasurementmanager.h"
#include "cUSB6001.h"
#include "cMeasurement.h"
#include "cTick.h"

int cCycleControler::get_current_step()
{
	assert(m_cycle != nullptr);
	return m_cycle->get_current_step();
}

int cCycleControler::get_current_loop()
{
	assert(m_cycle != nullptr);
	return m_cycle->get_current_loop();
}

int cCycleControler::get_total_step()
{
	assert(m_cycle != nullptr);
	return m_cycle->get_total_step_number();
}

int cCycleControler::get_total_loop()
{
	assert(m_cycle != nullptr);
	return m_cycle->get_total_loop_number();
}

STEPSTRUCT cCycleControler::get_current_step_param()
{
	std::atomic <int> step = m_cycle->get_current_step();
	std::vector<STEPSTRUCT> step_struct_vec = m_cycle->get_step_table();
	//assert(step_struct_vec.size() > 0);
	if (step_struct_vec.size() == 0)
	{
		std::cout << "[!] Error in cCycleControler: std::vector<STEPSTRUCT> step_struct_vec = m_cycle->get_step_table() return 0\n"; 
	}
	return step_struct_vec[step];
}

void cCycleControler::poll()
{
	auto st = thread.get_stop_source();

	end_tick = PerformanceCounter();

	while (!st.stop_requested())
	{
		m_table_->set_line_highlight(m_cycle->get_current_step());
		start_tick = PerformanceCounter();

		delta_tick = (double)((end_tick - start_tick) / freq);
		while (delta_tick < (m_cycle->get_duration()*10) and st.stop_requested() == false)
		{
			end_tick = PerformanceCounter();
			delta_tick = (double)((end_tick - start_tick) / (freq/10));
		}

		if (st.stop_requested())
		{
			goto kill;
		}

		// protect
		cycle_mutex.lock();

		m_cycle->next();

		int total = m_cycle->get_total_step_number();
		int current = m_cycle->get_current_step();
		if (total - current == 0)
		{
			std::cout << "[*] Cycle controler step end\n";
			m_table_->set_lines_white(); // Set all lines to white colour
			std::atomic<int> loopcount = m_cycle->get_current_loop();

			if (loopcount > 0)
			{
				std::cout << "[*] Cycle controler next cycle loaded\n";
				loopcount--;
				m_cycle->set_current_step(0);
				m_cycle->set_current_loop(loopcount);
				m_table_->set_loop_count(loopcount);
			}
			if (loopcount <= 0)
			{
				// Update % in status bar
				cObjectmanager* object_manager = object_manager->getInstance();
				wxStatusBar* statusbar = object_manager->get_status_bar();
				wxString statusstr = wxString::Format("100 %% performed...");
				statusbar->SetLabelText(statusstr);

				// Send virtual click on STOP btn
				wxCommandEvent evt = wxCommandEvent(wxEVT_COMMAND_BUTTON_CLICKED, IDC_STARTBTN);
				wxPostEvent(inst_, evt);
				cycle_mutex.unlock();
				goto kill;

			}
		}
		// unprotect
		cycle_mutex.unlock();
	}
kill:

	//MessageBox(GetFocus(), L"End of cycle", L"Success", MB_OK);

	std::cout << "[*] Cycle controler exitting daemon\n";
	//MessageBox(GetFocus(), L"Exit control daemon", L"Success", MB_OK);

}


cCycleControler::cCycleControler(cTable* m_table, wxWindow* inst)
{
	std::cout << "cCycleControler ctor...\n";
	assert(m_table != nullptr);
	m_table_ = m_table;
	inst_ = inst;
}

void cCycleControler::start()
{
	std::cout << "[*] cCycleControler start called\n";

		// build a new cycle based on cTable

	assert(m_cycle != nullptr);
	assert(m_table_ != nullptr);
	m_cycle->clear_cycles();
	m_cycle->set_current_step(0);
	m_cycle->set_current_loop(m_table_->get_loop_number());
	m_cycle->set_total_step_number(m_table_->get_step_number());
	m_cycle->set_total_loop_number(m_table_->get_loop_number());

	m_cycle->set_step_table(m_table_->get_step_table());

		// Send the cycle to statistic module 
	
	m_table_->start_statistic(m_cycle);

		// start the polling thread

	//assert(!thread.joinable()); // Thread must be stopped 
	thread = std::jthread(&cCycleControler::poll, this);
	std::cout << "[*] cCycleControler start success\n";
}

void cCycleControler::stop()
{
	assert(m_cycle != nullptr);
	assert(m_table_ != nullptr);
	std::cout << "[*] cCycleControler stop called\n";
	m_table_->stop_statistic();
	thread.request_stop();
	//thread.join();
	m_cycle->clear_cycles();
	std::cout << "[*] cCycleControler stopped\n";
}

inline long long cCycleControler::PerformanceFrequency()
{
	LARGE_INTEGER li;
	QueryPerformanceFrequency(&li);
	return li.QuadPart;
}

inline long long cCycleControler::PerformanceCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return li.QuadPart;
}

cCycleControler::~cCycleControler()
{
	assert(m_table_ != nullptr);
	std::cout << "cCycleControler dtor...\n";
}


