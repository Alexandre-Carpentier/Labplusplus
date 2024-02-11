#include "cCycleControler.h"
#include <wx/wx.h>
#include <thread>
#include <mutex>
#include <iostream>
#include "enum.h"

#include "cCycle.h"
#include "cTable.h"

void cCycleControler::poll()
{
	auto st = thread.get_stop_source();

	end = PerformanceCounter();

	while (!st.stop_requested())
	{
		m_table_->set_line_highlight(m_cycle_->get_current_step());
		start = PerformanceCounter();

		delta = (double)((end - start) / freq);
		while (delta < (m_cycle_->get_duration()*10) and st.stop_requested() == false)
		{
			end = PerformanceCounter();
			delta = (double)((end - start) / (freq/10));
			//Sleep(10);
		}

		if (st.stop_requested())
		{
			goto kill;
		}
		if (m_cycle_ == nullptr)
		{
			goto kill;
		}
		critical_section.lock(); ////////////////////////////////////////////////////CRITICAL_SECTION//////////////
		m_cycle_->next();

		int total = m_cycle_->get_total_step_number();
		int current = m_cycle_->get_current_step();
		if (total - current == 0)
		{
			std::cout << "[*] Cycle controler step end\n";
			m_table_->set_lines_white();
			int loopcount = m_cycle_->get_current_loop();

			if (loopcount > 0)
			{
				std::cout << "[*] Cycle controler next cycle loaded\n";
				loopcount--;
				m_cycle_->set_current_step(0);
				m_cycle_->set_current_loop(loopcount);
				m_table_->set_loop_count(loopcount);
			}
			if (loopcount <= 0)
			{

				// Update % in status bar
				cObjectmanager* object_manager = object_manager->getInstance();
				wxStatusBar* statusbar = object_manager->get_status_bar();
				wxString statusstr = wxString::Format("100 %% performed...");
				statusbar->SetLabelText(statusstr);
				critical_section.unlock(); ////////////////////////////////////////////////////CRITICAL_SECTION//////////////

				// Send virtual click on STOP btn
				wxCommandEvent evt = wxCommandEvent(wxEVT_COMMAND_BUTTON_CLICKED, IDC_STARTBTN);
				wxPostEvent(inst_, evt);

				MessageBox(GetFocus(), L"End of cycle", L"Success", MB_OK);
				break;
			}
		}
		critical_section.unlock(); ////////////////////////////////////////////////////CRITICAL_SECTION//////////////
	}
kill:
	std::cout << "[*] Cycle controler exitting daemon\n";
	MessageBox(GetFocus(), L"Exit control daemon", L"Success", MB_OK);
}


cCycleControler::cCycleControler(cCycle* m_cycle, cTable* m_table, wxWindow* inst)
{
	std::cout << "cCycleControler ctor...\n";
	m_cycle_ = m_cycle;
	m_table_ = m_table;
	inst_ = inst;

	thread = std::jthread(&cCycleControler::poll, this);

	bRunning = true;
}

void cCycleControler::stop()
{
	bRunning = false;
	thread.request_stop();
	std::cout << "[*] cCycleControler has joined\n";
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
	std::cout << "cCycleControler dtor...\n";

}
/*
std::mutex cCycleControler::get_mutex_reference()
{
	return nullptr;
}
*/

