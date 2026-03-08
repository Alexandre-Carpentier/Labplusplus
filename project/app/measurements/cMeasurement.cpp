/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#include "cMeasurement.h"

std::string cMeasurement::device_name() { return std::string("Measurement super class"); };
MEAS_TYPE cMeasurement::device_type() { return SUPER_INSTR; }

size_t cMeasurement::chan_count() { return 1; }
size_t cMeasurement::chan_read_count() { return 1; }
size_t cMeasurement::chan_write_count() { return 0; }

DATAS cMeasurement::read() { 
	result_struct.buffer[0] = (-12.0 + rand() % 1); 
	result_struct.buffer_size = 1; 
	return result_struct; 
}

CHUNKS cMeasurement::read_multiple() 
{ 
	const size_t SIGNAL_NUMBERS = 3;
	CHUNKS chunks; 
	std::array<double, MAX_FRAME> frame;

	// Generate signals filled with random data
	for (size_t num = 0; num < SIGNAL_NUMBERS; ++num)
	{
		for (size_t i = 0; i < MAX_FRAME; i++)
		{
			frame[i] = 2 + (0.1 * (rand() % 4));
		}
		chunks.push_back(frame);
	}
	return chunks;
}

void cMeasurement::set(double* value, size_t length) { std::cout << "[*] Set instrument super class to: " << value << "\n"; };

void cMeasurement::set_configuration_struct(CURRENT_DEVICE_CONFIG_STRUCT *config_struct)
{
	config_struct_ = *config_struct;
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

void cMeasurement::start_device() {};
void cMeasurement::stop_device() {};

//cMeasurement::~cMeasurement();










