/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////

#include "cCycle.h"

	cCycle::cCycle()
	{
		std::cout << "cCycle ctor...\n";
		cycle = std::make_shared<CYCLESTRUCT>();
		const std::lock_guard<std::mutex> lock(step_mutex);
		cycle->current_step = 0;
		cycle->total_step = 0;
		cycle->current_loop = 0;
		cycle->total_loop = 0;
	}

	cCycle::~cCycle()
	{
		std::cout << "cCycle dtor...\n";
	}

	// clear old cycles 
	void cCycle::clear_cycles()
	{
		assert(cycle != nullptr);
		const std::lock_guard<std::mutex> lock(step_mutex);
		cycle->current_step = 0;
		cycle->total_step = 0;
		cycle->current_loop = 0;
		cycle->total_loop = 0;
		for (size_t x = 0; x < cycle->step_table.size(); x++)
		{
			cycle->step_table[x].controler_vec.clear();
		}
		cycle->step_table.clear();
	}

	std::shared_ptr<cCycle::CYCLESTRUCT> cCycle::get_cycles()
	{
		const std::lock_guard<std::mutex> lock(step_mutex);
		assert(cycle != nullptr);
		return cycle;
	}

	void cCycle::set_step_table(std::vector<STEPSTRUCT> step_table)
	{
		assert(cycle != nullptr);
		assert(step_table.size() > 0);
		const std::lock_guard<std::mutex> lock(step_mutex);

		// Sauvegarder les valeurs initiales de jumpcount
		for (auto& step : step_table)
		{
			step.jumpcount_initial = step.jumpcount;
		}

		cycle->step_table = step_table;
	}

	std::vector<STEPSTRUCT> cCycle::get_step_table()
	{
		//assert(cycle->step_table.size() > 0);
		const std::lock_guard<std::mutex> lock(step_mutex);
		return cycle->step_table;
	}

	int cCycle::get_current_loop()
	{
		assert(cycle != nullptr);
		const std::lock_guard<std::mutex> lock(step_mutex);
		return cycle->current_loop;
	}

	void cCycle::set_current_loop(int count)
	{
		assert(cycle != nullptr);
		const std::lock_guard<std::mutex> lock(step_mutex);
		cycle->current_loop = count;
	}

	int cCycle::get_current_step()
	{
		assert(cycle != nullptr);
		const std::lock_guard<std::mutex> lock(step_mutex);
		return cycle->current_step;
	}

	void cCycle::set_current_step(int count)
	{
		assert(cycle != nullptr);
		const std::lock_guard<std::mutex> lock(step_mutex);
		cycle->current_step = count;
	}

	int cCycle::get_total_step_number()
	{
		assert(cycle != nullptr);
		const std::lock_guard<std::mutex> lock(step_mutex);
		return cycle->total_step;
	}

	void cCycle::set_total_step_number(int count)
	{
		assert(cycle != nullptr);
		const std::lock_guard<std::mutex> lock(step_mutex);
		cycle->total_step = count;
	}

	int cCycle::get_total_loop_number()
	{
		assert(cycle != nullptr);
		const std::lock_guard<std::mutex> lock(step_mutex);
		return cycle->total_loop;
	}

	void cCycle::set_total_loop_number(int count)
	{
		assert(cycle != nullptr);
		const std::lock_guard<std::mutex> lock(step_mutex);
		cycle->total_loop = count;
	}

	void cCycle::reset_jump_counters()
	{
		assert(cycle != nullptr);
		const std::lock_guard<std::mutex> lock(step_mutex);
		
		std::cout << "[*] cCycle::reset_jump_counters() - Resetting all jump counters\n";
		
		for (auto& step : cycle->step_table)
		{
			step.jumpcount = step.jumpcount_initial;
		}
	}

	void cCycle::previous()
	{
		assert(cycle->current_step >= 0);
		const std::lock_guard<std::mutex> lock(step_mutex);
		cycle->current_step--;
	}

	bool cCycle::next()
	{
		assert(cycle != nullptr);
		assert(cycle->current_step <= cycle->total_step);
		
		const std::lock_guard<std::mutex> lock(step_mutex);
		
		// Get current step information
		int current_step_index = cycle->current_step;

		int total_steps = cycle->total_step;
		
		// Check if current step has a jump configured
		if (current_step_index < static_cast<int>(cycle->step_table.size()))
		{
			STEPSTRUCT& current_step_struct = cycle->step_table[current_step_index];
			
			if (current_step_struct.jumpcount > 0)
			{
				// Decrement jump count
				current_step_struct.jumpcount--;
				
				// Jump to specified step
				int jump_target = current_step_struct.jumpto - 1; // Start at 0
				
				// Validate jump target
				if (jump_target >= 0 && jump_target < cycle->total_step)
				{

					cycle->current_step = jump_target;
					std::cout << "[*] cCycle::next() - Jumping to step " << jump_target 
					          << " (jumpcount remaining: " << current_step_struct.jumpcount << ")\n";
					return true; // Saut effectué
				}


				std::cout << "[!] cCycle::next() - Invalid jump target " << jump_target
					<< ", proceeding to next step\n";
			}
		}
		
		// No jump or jump exhausted, proceed to next step normally
		cycle->current_step++;
		return false; // Pas de saut
	}

	int cCycle::get_jumpto() { return cycle->step_table.at(cycle->current_step).jumpto; }
	int cCycle::get_jumpto(const int index) { return cycle->step_table.at(index).jumpto; }
	int cCycle::get_jumpcount() { return cycle->step_table.at(cycle->current_step).jumpcount; }
	int cCycle::get_jumpcount(const int index) { return cycle->step_table.at(index).jumpcount; }

	double cCycle::get_duration() {
		if (cycle->current_step > cycle->total_step)
		{
			std::cout << "[!] cCycle::get_duration() - Current step (" << cycle->current_step << ") exceeds total steps (" << cycle->total_step << "), returning 0\n";
			return 0;		
		}

		const std::lock_guard<std::mutex> lock(step_mutex);
		auto duration = cycle->step_table.at(cycle->current_step).duration;
		if(duration <0)
		{
			std::cout << "[!] cCycle::get_duration() - Invalid duration (" << duration << ") at step " << cycle->current_step << ", returning 0\n";
			return 0;
		}
		// OK
		return duration;
	}
	double cCycle::get_duration(const int index) {
		const std::lock_guard<std::mutex> lock(step_mutex);
		return cycle->step_table.at(index).duration;
	}

	void cCycle::set_jumpto(const int jt) { cycle->step_table.at(cycle->current_step).jumpto = jt; }
	void cCycle::set_jumpto(const int jt, const int index) { cycle->step_table.at(index).jumpto = jt; }
	void cCycle::set_jumpcount(const int jc) { cycle->step_table.at(cycle->current_step).jumpcount = jc; }
	void cCycle::set_jumpcount(const int jc, const int index) { cycle->step_table.at(index).jumpcount = jc; }

	void cCycle::set_duration(const double d) { const std::lock_guard<std::mutex> lock(step_mutex); cycle->step_table.at(cycle->current_step).duration = d; }
	void cCycle::set_duration(const double d, const int index) { const std::lock_guard<std::mutex> lock(step_mutex); cycle->step_table.at(index).duration = d; }


