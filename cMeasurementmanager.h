#pragma once
#include <wx/wx.h>
#include "cSingleton.h"
#include "cMeasurement.h"

class cDaqmx;
class cMeasurementControler;
class cMeasurement;

class cMeasurementmanager :public cSingleton<cMeasurementmanager>
{
private:
	friend class cSingleton<cMeasurementmanager>;

	cMeasurementControler* m_meas_controler_ = nullptr;
	std::vector <cMeasurement*> m_meas_pool;
	size_t signal_number = 0;
public:

	void set_measurement_controler(cMeasurementControler* m_controler);
	void set_measurement(cMeasurement* measurement);

	cMeasurementControler* get_measurement_controler();
	std::vector<cMeasurement*> get_measurement_pool();
	size_t get_measurement_pool_size();
	cMeasurement* get_measurement(size_t position);
	MEAS_TYPE get_measurement_type(cMeasurement*);
	size_t get_measurement_channel_number(cMeasurement*);
	size_t get_measurement_total_channel_number(void);

	void destroy_measurement_controler();
	void destroy_measurement_pool();
	void stop_all_devices();
	bool destroy_subsystem(MEAS_TYPE type);
};


