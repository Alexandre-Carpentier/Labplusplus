/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include <winsock2.h> 
#include <string>
#include <deque>
#include <iostream>
#include <vector>
#include <memory>
		
#include "cTick.h"
#include "data_types.h"

const short MAX_CHAN = 64;
const short MAX_FRAME = 1024;

struct DATAS
{
	size_t buffer_size =0;
	double buffer[MAX_CHAN] = { 0 };
	std::string unit = "Unit";
};

struct CHUNKS {
	size_t buffer_numbers = 0;
	std::vector<std::vector<double>> buffer; // buffer[channel][frame]
	size_t rate_per_s = 1000;
	std::string unit = "Unit";

	CHUNKS(size_t channels = 0, size_t frames = MAX_FRAME)
		: buffer(channels, std::vector<double>(frames, 0.0)),
		buffer_numbers(0),
		rate_per_s(1000)
	{
	}
};

class cCommon : public cTick
{
private:
	typedef struct {
		std::deque<double> X;
		std::deque<double> Y;
	}DATA, * PDATA;

	DATA ring_buffer;

public:
	void add_value(double X, double Y);
	void remove_value();

	~cCommon() {};
};


class cMeasurement : public cCommon {
private:
	DATAS result_struct;
	CURRENT_DEVICE_CONFIG_STRUCT config_struct_;
public:

	virtual void set_configuration_struct(CURRENT_DEVICE_CONFIG_STRUCT *config_struct) = 0;
	virtual int launch_device() = 0;

	virtual std::string device_name() = 0;
	virtual MEAS_TYPE device_type() = 0;
	virtual size_t chan_count() = 0;
	virtual size_t chan_read_count() = 0;
	virtual size_t chan_write_count() = 0;

	virtual DATAS read()=0;
	virtual CHUNKS read_multiple();
	virtual void set(double *value, size_t length) = 0;

	
	virtual void set_device_name(std::string name);
	virtual void set_device_addr(std::string addr);

	void start_device();
	virtual void stop_device() = 0;
};





