#pragma once
#include <winsock2.h> 
#include <string>
#include <deque>
#include <iostream>
#include <vector>
#include <memory>
		
#include "cTick.h"
#include "WinGraph.h"
#include "data_types.h"

#define MAX_CHAN 64

struct DATAS
{
	size_t buffer_size;
	double buffer[MAX_CHAN];
	std::string unit;
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
	cMeasurement();

	virtual void set_configuration_struct(CURRENT_DEVICE_CONFIG_STRUCT config_struct);
	virtual int launch_device();

	virtual std::string device_name();
	virtual MEAS_TYPE device_type();
	virtual size_t chan_count();
	virtual size_t chan_read_count();
	virtual size_t chan_write_count();

	virtual DATAS read();
	virtual void set(double *value, size_t length);

	
	virtual void set_device_name(std::string name);
	virtual void set_device_addr(std::string addr);

	void start_device();
	virtual void stop_device();
	virtual ~cMeasurement() = default;
};





