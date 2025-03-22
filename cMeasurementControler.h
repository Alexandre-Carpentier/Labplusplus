/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include <thread>
#include <vector>
#include <iostream>
#include "cObserver.h"

class cObjectmanager;
class cMeasurementmanager;
class cPlot;
class cFooter;
class cMeasurement;
class cCycleControler;

class cMeasurementControler : public currentValueObserved
{
private:
	std::jthread measurement_controler_thread;
	CURRENT_VALUE_STRUCT currentValues;
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

