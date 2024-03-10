#include "cMeasurement.h"
#include "cTick.h"


void cCommon::add_value(double X, double Y) {
	ring_buffer.X.push_back(X); ring_buffer.Y.push_back(Y);
}

void cCommon::remove_value() {
	ring_buffer.X.pop_front(); ring_buffer.Y.pop_front();
}

cMeasurement::cMeasurement()
{
	
}

int cMeasurement::launch_device(CURRENT_DEVICE_CONFIG_STRUCT config_struct) { return -1; }

std::string cMeasurement::device_name() { return std::string("Measurement super class"); };
MEAS_TYPE cMeasurement::device_type() { return SUPER_INSTR; }

size_t cMeasurement::chan_count() { return 1; }

DATAS cMeasurement::read() { result_struct.buffer[0] = (-12.0 + rand() % 1); result_struct.buffer_size = 1;  return result_struct; }
void cMeasurement::set(double value) { std::cout << "[*] Set instrument super class to: " << value << "\n"; };

void cMeasurement::set_configuration_struct(CURRENT_DEVICE_CONFIG_STRUCT config_struct)
{
	config_struct_ = config_struct;
	return;
}

void cMeasurement::set_device_name(std::string name)
{
	config_struct_.device_name = name;
}

void cMeasurement::set_device_addr(std::string addr)
{
	config_struct_.device_addr = addr;
}

void cMeasurement::stop_device() {};

//cMeasurement::~cMeasurement();










