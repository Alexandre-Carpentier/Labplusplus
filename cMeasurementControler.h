#pragma once

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
//#include "cSingleton.h"
/*
class cCycle;
class cCycleControler;
class cPlot;
class cFooter;
class cObjectmanager;
class cMeasurementmanager;
class cUSB6001;
class cMeasurement;
class cTick;
*/

class cCycleControler;


class cMeasurementControler
{
private:
	std::jthread measurement_controler_thread;
public:
	std::shared_ptr<cCycleControler> m_cyclecontroler_;

	//std::thread control_thread;
	double freq_s_ = 0.0;

	cObjectmanager* obj_manager = nullptr;
	cMeasurementmanager* meas_manager = nullptr;
	cPlot* m_plot_ = nullptr;
	cFooter* m_footer_ = nullptr;

	std::vector<cMeasurement*> meas_pool;

public:
	cMeasurementControler(std::shared_ptr<cCycleControler> m_cyclecontroler)
	{
		m_cyclecontroler_ = m_cyclecontroler;
	}
	~cMeasurementControler()
	{
		std::cout << "[*] cMeasurementControler Dtor called.\n";
	}

	void poll();
	void start();
	void stop();
	double get_aquisition_rate();
	void set_aquisition_rate(double freq_s);
};

