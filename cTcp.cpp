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
	device_name_ = addr;
}

err_struct cTcp::init()
{
	std::wcout << L"[*] cSerial init() called\n";

	if (device_name_.compare(L"magic") == 0)
	{
		//TCPIP0::169.254.254.001::inst0::INSTR
		ViPFindList list = 0;
		ViPUInt32 count = 0;
		ViChar instrument_name[260] = "";
		if (VI_SUCCESS != viFindRsrc(ressource_manager, (char*)"TCPIP*?::*INSTR", list, count, instrument_name))
		{
			return { std::wstring(L"[!] viOpen() failled."), -1 };
		}
		// Convert to wide string
		std::string utf8_str = instrument_name;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> converter;
		device_name_ = converter.from_bytes(utf8_str);
	}

	status = viOpen(ressource_manager, (ViRsrc)this->device_name_.c_str(), VI_NO_LOCK, 0, &device_);
	if (status != VI_SUCCESS)
	{
		return { std::wstring(L"[!] viOpen() failled."), -2 };
	}

	// For Serial and TCP/IP socket connections enable the read Termination Character, or read's will timeout
	ViChar fullAddress[100] = "";
	viGetAttribute(device_, VI_ATTR_RSRC_NAME, fullAddress);
	if (strncmp("TCPIP", fullAddress, 5) != 0)
	{
		return { std::wstring(L"[!] viGetAttribute() failled. It is not a ASRL device"), -3 };
	}

	viSetAttribute(device_, VI_ATTR_TERMCHAR_EN, VI_TRUE);

	// Set timeout value to 500 ms
	status = viSetAttribute(device_, VI_ATTR_TMO_VALUE, 500);
	last_error.err_msg = std::wstring(L"OK");
	last_error.err_code = 0;
	return last_error;
}

err_struct cTcp::close()
{
	std::wcout << L"[*] cSerial close() called\n";
	viClose(device_);
	last_error.err_msg = std::wstring(L"OK");
	last_error.err_code = 0;
	return last_error;
}
