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
#include "cCycleControler.h"

class cObjectmanager;
class cMeasurementmanager;
class cPlot;
class cFooter;
class cMeasurement;

void zero_instrument(std::vector<cMeasurement*> meas_pool);
bool get_instr_setpoint(cMeasurement* meas, STEPSTRUCT step, double* values, size_t buffer_length, size_t* read);

class cMeasurementControler : public currentValueObserved
{
private:
	std::jthread measurement_controler_thread;
	CURRENT_VALUE_STRUCT currentValues;
	std::shared_ptr<cCycleControler> m_cyclecontroler;
public:

	double freq_s_ = 0.0;

	cObjectmanager* obj_manager = nullptr;
	cMeasurementmanager* meas_manager = nullptr;
	cPlot* m_plot_ = nullptr;
	cFooter* m_footer_ = nullptr;

	//std::vector<cMeasurement*> meas_pool;

public:
	cMeasurementControler(std::shared_ptr<cCycleControler> cyclecontroler)
	{
		m_cyclecontroler = cyclecontroler;
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

