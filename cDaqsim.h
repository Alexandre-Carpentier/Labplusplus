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
class cTick;
class cMeasurement;

#include "cMeasurement.h"
#include "C:\\Program Files (x86)\\National Instruments\\Shared\\ExternalCompilerSupport\\C\\include\\NIDAQmx.h"

class cDaqsim : public cMeasurement {
private:
	CURRENT_DEVICE_CONFIG_STRUCT config_struct_;
	DATAS result;
	double *current_fake_value=nullptr;
public:

	cDaqsim();
	std::string device_name() override;
	MEAS_TYPE device_type() override;
	size_t chan_count() override;
	size_t chan_read_count() override;
	size_t chan_write_count() override;

	void set_configuration_struct(CURRENT_DEVICE_CONFIG_STRUCT config_struct) override;
	int launch_device() override;

	DATAS read() override;
	void set(double* value, size_t length) override;

	void stop_device()override;


	virtual ~cDaqsim();

};
