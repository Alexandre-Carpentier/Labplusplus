#pragma once
#include <vector>
class cTick;
class cMeasurement;

#include "cMeasurement.h"

class cSupplyrssim : public cMeasurement {
private:
	CURRENT_DEVICE_CONFIG_STRUCT config_struct_;
	double current_fake_value = 42.0;
	DATAS result;
public:

	cSupplyrssim();
	std::string device_name() override;
	MEAS_TYPE device_type() override;
	size_t chan_count() override;
	size_t chan_write_count() override;
	size_t chan_read_count() override;

	void set_configuration_struct(CURRENT_DEVICE_CONFIG_STRUCT config_struct) override;
	int launch_device() override;

	DATAS read() override;
	void set(double* value, size_t length) override;

	void stop_device()override;


	virtual ~cSupplyrssim();

};
