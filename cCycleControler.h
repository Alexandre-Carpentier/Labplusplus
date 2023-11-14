#pragma once
#include <wx/wx.h>
#include <thread>
#include <mutex>

class cCycle;
class cTable;

class cCycleControler : public wxWindow
{
private:
	cCycle* m_cycle_ = nullptr;
	cTable* m_table_ = nullptr;
	wxWindow* inst_ = nullptr;

	std::jthread thread;

	long long start = PerformanceCounter();
	long long end = PerformanceCounter();
	double delta;
	long long freq = PerformanceFrequency();
public:
	std::mutex critical_section; // used to synchronize cycle and measurement controler when read/write

	bool bRunning = false;


	cCycleControler(cCycle* m_cycle, cTable* m_table, wxWindow* inst);
	void poll();
	void stop();
	inline long long PerformanceFrequency();
	inline long long PerformanceCounter();

	//std::mutex get_mutex_reference();

	~cCycleControler();

};

