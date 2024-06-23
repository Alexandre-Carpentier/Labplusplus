#pragma once
#include <winsock2.h> 
#include <vector>
/*
class cTick;
class cMeasurement;
*/
#include "cMeasurement.h"

class cPacesim : public cMeasurement {
private:
	CURRENT_DEVICE_CONFIG_STRUCT config_struct_;
	double current_fake_value = 42.0;
	DATAS result;
public:

	cPacesim();
	std::string device_name() override;
	MEAS_TYPE device_type() override;
	size_t chan_count() override;
	size_t chan_write_count() override;
	size_t chan_read_count() override;

	int launch_device(CURRENT_DEVICE_CONFIG_STRUCT config_struct) override;

	DATAS read() override;
	void set(double* value, size_t length) override;

	void stop_device()override;


	virtual ~cPacesim();

};
