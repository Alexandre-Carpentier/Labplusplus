#pragma once
#include <vector>
#include <format>
#include <bitset>
class cTick;
class cMeasurement;

#include "cMeasurement.h"

#ifdef _WIN64
#pragma comment (lib, "C:\\Program Files (x86)\\National Instruments\\Shared\\ExternalCompilerSupport\\C\\lib64\\msvc\\NIDAQmx.lib")
#else
#pragma comment (lib, "C:\\Program Files (x86)\\National Instruments\\Shared\\ExternalCompilerSupport\\C\\lib32\\msvc\\NIDAQmx.lib")
#endif

// Must Install Nationnal Instrument DAQmx Lib freely available here:
// https://www.ni.com/fr-fr/support/downloads/drivers/download.ni-daqmx.html#460239
// Default instalation path is located at :
// C:\Program Files (x86)\National Instruments\Shared\ExternalCompilerSupport\C\include
#include "C:\\Program Files (x86)\\National Instruments\\Shared\\ExternalCompilerSupport\\C\\include\\NIDAQmx.h"


class cUsb6001 : public cMeasurement {
private:
	CURRENT_DEVICE_CONFIG_STRUCT config_struct_;


	TaskHandle analog_taskHandle;
	TaskHandle digital_taskHandle;
	int32 DAQret;
	int sample_number = 2;
	float64 multiple_data[1200];
	DATAS result;
public:

	cUsb6001();
	std::string device_name() override;
	MEAS_TYPE device_type() override;
	size_t chan_count() override;
	size_t chan_read_count() override;
	size_t chan_write_count() override;

	int launch_device(CURRENT_DEVICE_CONFIG_STRUCT config_struct) override;

	DATAS read() override;
	void set(double* value, size_t length) override;

	void stop_device()override;


	~cUsb6001();

};
