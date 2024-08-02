#pragma once
#include <wx/wx.h>
#include <wx/timer.h>

#include <vector>
#include <string>
#include <format>

#include "encoding.h"
//#include "cSingleton.h"
#include "cSingletonSafe.h"
#include "cVisa.h"
#include "cVisatcp.h"
#include "cVisaserial.h"
#include "cProtocolFactory.h"
#include "ListUsbDevice.h"

const int refresh_rate = 10000;

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

class cDeviceMonitor /*: public wxTimer, public cSingletonSafe*/
{

public:
	cDeviceMonitor()
	{
		//Start(refresh_rate);
		
	}
	virtual ~cDeviceMonitor() {}

	void Notify();
	std::vector<cDev> get_device_vec();
	std::wstring get_first_available() { return dev_list.at(0).get_addr(); };
	void lookup_start();
	void lookup_stop();

private:
	std::vector<cDev> dev_list;
};

