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
#include "cNiDaq.h"
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

bool cCycleControler::get_current_step_param(STEPSTRUCT currentStep)
{
	std::atomic <int> step = m_cycle->get_current_step();
	std::vector<STEPSTRUCT> step_struct_vec = m_cycle->get_step_table();
	//assert(step_struct_vec.size() > 0);
	if (step_struct_vec.size() == 0)
	{
		std::cout << "[!] Error in cCycleControler: std::vector<STEPSTRUCT> step_struct_vec = m_cycle->get_step_table() return 0\n"; 
		return false;
	}
	currentStep = step_struct_vec[step];
	return true;
}

void cCycleControler::poll()
{
	auto stop_source = thread.get_stop_source();
	
	std::cout << "[*] Cycle controler daemon started\n";
	
	end_tick = PerformanceCounter();

	while (!stop_source.stop_requested())
	{
		// Highlight current step
		auto stepPos = m_cycle->get_current_step();
		m_table_->set_line_highlight(stepPos);
		
		// Wait for step duration
		if (!wait_for_step_duration(stop_source))
		{
			break; // Stop requested during wait
		}

		// Execute cycle step logic
		{
			std::lock_guard<std::mutex> lock(cycle_mutex);
			
			bool jumped = m_cycle->next();
			
			// Update highlight immediately if a jump occurred
			if (jumped)
			{
				m_table_->set_line_highlight(m_cycle->get_current_step());
			}
		}

		// Check if all steps completed
		if (is_cycle_completed())
		{
			if (!handle_cycle_completion(stop_source))
			{
				break; // Cycle finished, exit loop
			}
		}
	}

	cleanup_and_exit();
}

bool cCycleControler::wait_for_step_duration(const std::stop_source& stop_source)
{
	start_tick = PerformanceCounter();
	
	std::lock_guard<std::mutex> lock(cycle_mutex);
	
	delta_tick = static_cast<double>((end_tick - start_tick) / freq);
	
	while (delta_tick < (m_cycle->get_duration() * 10) && !stop_source.stop_requested())
	{
		end_tick = PerformanceCounter();
		delta_tick = static_cast<double>((end_tick - start_tick) / (freq / 10));
	}
	
	return !stop_source.stop_requested();
}

bool cCycleControler::is_cycle_completed() const
{
	int total = m_cycle->get_total_step_number();
	int current = m_cycle->get_current_step();

	// continue
	if (current <= total)
	{
		return false;
	}
	
	// cycle completed
	return true;
}

bool cCycleControler::handle_cycle_completion(const std::stop_source& stop_source)
{
	std::cout << "[*] Cycle controler step end\n";

	// Reset to first step for next loop
	m_cycle->set_current_step(0);

	m_table_->set_lines_white();
	
	std::atomic<int> loop_count = m_cycle->get_current_loop();

	if (loop_count > 0)
	{
		// Start next loop
		std::cout << "[*] Cycle controler next cycle loaded\n";
		loop_count--;
		m_cycle->set_current_loop(loop_count);
		m_table_->set_loop_count(loop_count);
		
		// Réinitialiser les compteurs de saut pour le nouveau tour
		m_cycle->reset_jump_counters();
		
		return true; // Continue polling
	}
	
	// All loops completed
	update_status_bar_completion();
	send_stop_event();
	return false; // Stop polling
}

void cCycleControler::update_status_bar_completion()
{
	cObjectmanager* object_manager = cObjectmanager::getInstance();
	wxStatusBar* statusbar = object_manager->get_status_bar();
	wxString statusstr = wxString::Format("100 %% performed...");
	statusbar->SetLabelText(statusstr);
}

void cCycleControler::send_stop_event()
{
	wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, IDC_STARTBTN);
	wxPostEvent(inst_, evt);
}

void cCycleControler::cleanup_and_exit()
{
	std::cout << "[*] Cycle controler exitting daemon\n";
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

	size_t loop_number = m_table_->get_loop_number(); // loop max-1 -> 0 to XXX

	m_cycle->clear_cycles();
	m_cycle->set_current_step(0);
	m_cycle->set_current_loop(loop_number); // loop max-1 -> 0 to XXX
	m_cycle->set_total_step_number(m_table_->get_step_number());  // 0 to XXX...
	m_cycle->set_total_loop_number(loop_number); // 0 to XXX...
	auto tab = m_table_->get_step_table();
	m_cycle->set_step_table(tab); // Get steps vector

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


