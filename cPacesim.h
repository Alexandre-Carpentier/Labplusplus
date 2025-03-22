/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
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

	int launch_device() override;

	DATAS read() override;
	void set(double* value, size_t length) override;

	void set_configuration_struct(CURRENT_DEVICE_CONFIG_STRUCT config_struct)override;
	void set_device_name(std::string name)override;

	void stop_device()override;


	virtual ~cPacesim();

};
