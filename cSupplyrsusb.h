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
#include <format>
#include <cassert>
#include <thread>

#include "cUsb.h"
#include "cVisa.h"
#include "cVisatcp.h"
#include "cVisaserial.h"
#include "cProtocolFactory.h"

#include "encoding.h"
#include "cTick.h"
#include "cMeasurement.h"
//class cTick;
//class cMeasurement;

class cSupplyrsusb : public cMeasurement {
private:
	CURRENT_DEVICE_CONFIG_STRUCT config_struct_;
	double current_fake_value = 5.0;
	DATAS result;

	cProtocolFactory factory;
	std::unique_ptr<IProtocol> device;
	err_struct err;

	std::jthread acquireloop;
public:

	cSupplyrsusb();
	std::string device_name() override;
	MEAS_TYPE device_type() override;
	size_t chan_count() override;
	size_t chan_read_count() override;
	size_t chan_write_count() override;

	int launch_device() override;

	void acquire();
	DATAS read() override;
	void set(double* value, size_t length) override;

	void set_configuration_struct(CURRENT_DEVICE_CONFIG_STRUCT config_struct)override;
	void set_device_name(std::string name)override;

	void stop_device()override;


	virtual ~cSupplyrsusb();

};
