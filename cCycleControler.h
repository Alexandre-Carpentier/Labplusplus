#pragma once
#include <wx/wx.h>
#include <thread>

class cCycle;
class cTable;

class cCycleControler : public wxWindow
{
private:
	cCycle* m_cycle_ = nullptr;
	cTable* m_table_ = nullptr;
	wxWindow* inst_ = nullptr;

	std::thread* thread = nullptr;

	long long start = PerformanceCounter();
	long long end = PerformanceCounter();
	long long delta;
	long long freq = PerformanceFrequency();
public:


	bool bRunning = false;


	cCycleControler(cCycle* m_cycle, cTable* m_table, wxWindow* inst);
	void poll(cCycleControler* self);
	void stop();
	inline long long PerformanceFrequency();
	inline long long PerformanceCounter();

	~cCycleControler();

};

