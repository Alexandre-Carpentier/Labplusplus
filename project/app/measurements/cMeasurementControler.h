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
#include "cMeasurement.h"

class cObjectmanager;
class cMeasurementmanager;
class cPlot;
class cFooter;

void zero_instrument(std::vector<cMeasurement*> meas_pool);
bool get_instr_setpoint(cMeasurement* meas, STEPSTRUCT step, double values[MAX_CHAN], size_t* read);

class cMeasurementControler : public currentValueObserved
{
private:
	std::jthread measurement_controler_thread;
	CURRENT_VALUE_STRUCT currentValues;
	std::shared_ptr<cCycleControler> m_cyclecontroler;
	size_t m_frame_size = 1;
public:

	double freq_s_ = 0.0;

	cObjectmanager* obj_manager = nullptr;
	cMeasurementmanager* meas_manager = nullptr;
	cPlot* m_plot_ = nullptr;
	cFooter* m_footer_ = nullptr;

public:
	cMeasurementControler(std::shared_ptr<cCycleControler> cyclecontroler, size_t frame_size)
	{
		m_cyclecontroler = cyclecontroler;
		m_frame_size = frame_size;
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

