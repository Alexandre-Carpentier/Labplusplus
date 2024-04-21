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

	int launch_device(CURRENT_DEVICE_CONFIG_STRUCT config_struct) override;

	DATAS read() override;
	void set(double* value, size_t length) override;

	void stop_device()override;


	virtual ~cDaqsim();

};
