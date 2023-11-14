#pragma once

#include <wx/wx.h>
#include <thread>
#include <Windows.h>
#include <mutex>

#include <vector>

//#include "cSingleton.h"

class cCycle;
class cCycleControler;
class cPlot;
class cFooter;
class cObjectmanager;
class cMeasurementmanager;
class cUSB6001;
class cMeasurement;
class cTick;

class cMeasurementControler
{
private:
	cCycle* m_cycle_ = nullptr; 
	cCycleControler* m_cyclecontroler_ = nullptr;
	std::jthread measurement_controler_thread;

public:
	std::thread control_thread;
	double freq_s_ = 0.0;
	std::atomic<bool> bRunning = false;
	cObjectmanager* obj_manager = nullptr;
	cMeasurementmanager* meas_manager = nullptr;
	cPlot* m_plot_ = nullptr;
	cFooter* m_footer_ = nullptr;

	std::vector<cMeasurement*> meas_pool;

public:
	cMeasurementControler(cCycle* m_cycle, cCycleControler* m_cyclecontroler)
	{
		m_cycle_ = m_cycle; // save ptr of cycle class to update step command
		m_cyclecontroler_ = m_cyclecontroler;
	}

	void poll();
	void start();
	void stop();
	double get_aquisition_rate();
	void set_aquisition_rate(double freq_s);
};

