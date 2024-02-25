#pragma once
#include <winsock2.h> 
#include <vector>
#include <format>
#include <cassert>

#include "cVisa.h"
#include "cTcp.h"
#include "cSerial.h"
#include "cProtocolFactory.h"

#include "encoding.h"

class cTick;
class cMeasurement;

#include "cMeasurement.h"

class cPacecom : public cMeasurement {
private:
	CURRENT_DEVICE_CONFIG_STRUCT config_struct_;
	double current_fake_value = 42.0;
	DATAS result;

	cProtocolFactory factory;
	std::unique_ptr<cProtocol> device;
	err_struct err;

	std::jthread acquireloop;
public:

	cPacecom();
	std::string device_name() override;
	MEAS_TYPE device_type() override;
	size_t chan_count() override;

	int launch_device(CURRENT_DEVICE_CONFIG_STRUCT config_struct) override;

	void acquire();
	DATAS read() override;
	void set(double value) override;

	void set_configuration_struct(CURRENT_DEVICE_CONFIG_STRUCT config_struct)override;
	void set_device_name(std::string name)override;

	void stop_device()override;


	virtual ~cPacecom();

};
