#pragma once

#include <wx/wx.h>
#include <thread>
#include <Windows.h>

//#include "cSingleton.h"

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


public:
	std::thread control_thread;
	double freq_s_ = 0.0;
	std::atomic<bool> bRunning = false;
	cObjectmanager* obj_manager = nullptr;
	cMeasurementmanager* meas_manager = nullptr;
	cPlot* m_plot_ = nullptr;
	cFooter* m_footer_ = nullptr;
public:
	std::vector<cMeasurement*> meas_pool;

public:
	void poll();
	void start();
	void stop();
	double get_aquisition_rate();
	void set_aquisition_rate(double freq_s);
};

