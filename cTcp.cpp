#include "cTcp.h"

cTcp::cTcp()
	:cVisa()
{
	std::wcout << L"[*] cSerial default constructor called\n";
	device_name_ = L"magic";
}

cTcp::cTcp(std::wstring addr)
	:cVisa()
{
	std::wcout << L"[*] cSerial constructor called\n";
	assert(addr.size() > 0);
	device_name_ = addr;
}

err_struct cTcp::init()
{
	std::wcout << L"[*] cSerial init() called\n";
	assert(ressource_manager>0);

	if (device_name_.compare(L"magic") == 0)
	{
		//TCPIP0::169.254.254.001::inst0::INSTR
		ViPFindList list = 0;
		ViPUInt32 count = 0;
#define INSTR_SIZE 260
		ViChar instrument_name[INSTR_SIZE] = "";
		if (VI_SUCCESS != viFindRsrc(ressource_manager, ViString("TCPIP*?::*INSTR"), list, count, instrument_name))
		{
			return { std::wstring(L"[!] viOpen() failled."), -1 };
		}
		assert(strnlen(instrument_name, INSTR_SIZE) > 0);

		// Convert to wide string
		device_name_ = ConvertUtf8ToWide(instrument_name);
		assert(device_name_.size() > 0);
	}

	std::string device_utf8 = ConvertWideToUtf8(device_name_);
	device_ = 0;
	status = viOpen(ressource_manager, (ViRsrc)device_utf8.c_str(), VI_NO_LOCK, 0, &device_);
	if (status != VI_SUCCESS)
	{
		return { std::wstring(L"[!] viOpen() failled."), -2 };
	}
	assert(device_ > 0);

	// For Serial and TCP/IP socket connections enable the read Termination Character, or read's will timeout
	ViChar fullAddress[100] = "";
	viGetAttribute(device_, VI_ATTR_RSRC_NAME, fullAddress);
	if (strncmp("TCPIP", fullAddress, 5) != 0)
	{
		return { std::wstring(L"[!] viGetAttribute() failled. It is not a ASRL device"), -3 };
	}
	viClear(device_);

	viSetAttribute(device_, VI_ATTR_TERMCHAR_EN, VI_TRUE);
	viSetAttribute(device_, VI_ATTR_TERMCHAR, 0x0A); // CR=0x0D LF=0x0A

	// Set timeout value to 500 ms
	status = viSetAttribute(device_, VI_ATTR_TMO_VALUE, 100);
	last_error.err_msg = std::wstring(L"OK");
	last_error.err_code = 0;
	return last_error;
}

err_struct cTcp::close()
{
	std::wcout << L"[*] cSerial close() called\n";
	viClose(device_);
	viClose(ressource_manager);
	last_error.err_msg = std::wstring(L"OK");
	last_error.err_code = 0;
	return last_error;
}
