#pragma once
#include <wx/wx.h>
#include <wx/timer.h>

#include <vector>
#include <string>

#include "cSingleton.h"
#include "cVisa.h"
#include "cTcp.h"
#include "cSerial.h"
#include "cProtocolFactory.h"

const int refresh_rate = 20000;

class cDev
{
public:
	void set_type(std::wstring type) { type_ = type; };
	void set_addr(std::wstring addr) { addr_ = addr; };
	void set_name(std::wstring name) { name_ = name; };

	std::wstring get_type() { return type_; };
	std::wstring get_addr() { return addr_; };
	std::wstring get_name() { return name_; };

private:
	std::wstring type_;
	std::wstring addr_;
	std::wstring name_;
};

class cDeviceMonitor : public wxTimer, public cSingleton<cDeviceMonitor>
{
public:
	cDeviceMonitor()
	{
		Start(refresh_rate);
	}

	void Notify();
	std::vector<cDev> get_device_vec();
	void lookup_start();
	void lookup_stop();

	virtual ~cDeviceMonitor()
	{

	}

private:

	std::vector<cDev> dev_list;
};


