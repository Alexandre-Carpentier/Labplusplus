#pragma once
#include <winsock2.h> 
#include <vector>
#include <format>
#include <cassert>
#include <thread>
#include "data_types.h"
#include "cMeasurement.h"
#include "cUsb.h"
#include "cVisa.h"
#include "cVisatcp.h"
#include "cVisaserial.h"
#include "cProtocolFactory.h"

class c6510usb : public cMeasurement {
private:
	CURRENT_DEVICE_CONFIG_STRUCT config_struct_;
	double current_fake_value = 5.0;
	DATAS result;

	cProtocolFactory factory;
	std::unique_ptr<IProtocol> device;
	err_struct err;

	std::jthread acquireloop;
public:

	c6510usb();
	std::string device_name() override;
	MEAS_TYPE device_type() override;
	size_t chan_count() override;
	size_t chan_read_count() override;
	size_t chan_write_count() override;

	int launch_device() override;

	void acquire();
	DATAS read() override;
	void set(double* value, size_t length) override;

	void set_configuration_struct(CURRENT_DEVICE_CONFIG_STRUCT *config_struct)override;
	void set_device_name(std::string name)override;

	void stop_device()override;


	virtual ~c6510usb();

};
