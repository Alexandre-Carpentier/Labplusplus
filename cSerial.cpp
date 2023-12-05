#include "cSerial.h"

cSerial::cSerial()
	:cVisa()
{
	std::wcout << L"[*] cSerial default constructor called\n";
	device_name_ = L"magic";
}

cSerial::cSerial(std::wstring addr)
	:cVisa()
{
	std::wcout << L"[*] cSerial constructor called\n";
	device_name_ = addr;
}

err_struct cSerial::init()
{
	std::wcout << L"[*] cSerial init() called\n";

	if (device_name_.compare(L"magic") == 0)
	{
		//TCPIP0::169.254.254.001::inst0::INSTR
		ViPFindList list = 0;
		ViPUInt32 count = 0;
		ViChar instrument_name[260] = "";
		if (VI_SUCCESS != viFindRsrc(ressource_manager, "ASRL*?::*INSTR", list, count, instrument_name))
		{
			return { std::wstring(L"[!] viFindRsrc() failled."), -1 };
		}

		// Convert tu utf16 wide char (wchar_t)
		std::string str_utf8 = instrument_name;
		std::wstring_convert< std::codecvt_utf8_utf16<wchar_t>, wchar_t> converter;
		device_name_ = converter.from_bytes(str_utf8);
	}
	else
	{
		/*
		if (VI_SUCCESS != viOpenDefaultRM(&ressource_manager))
		{
			return { std::wstring(L"[!] viOpenDefaultRM() failled."), -1 };
		}
		*/
	}

	//status = viOpen(ressource_manager, (ViConstRsrc)this->device_name_.c_str(), VI_NO_LOCK, 0, &device_);

	status = viOpen(ressource_manager, (ViConstRsrc)"ASRL20::INSTR", VI_NULL, 0, &device_);
	//status = viOpen(ressource_manager, (ViConstRsrc)this->device_name_.c_str(), VI_NULL, 0, &device_);
	if (status != VI_SUCCESS)
	{
		return { std::wstring(L"[!] viOpen() failled."), -2 };
	}

	// For Serial and TCP/IP socket connections enable the read Termination Character, or read's will timeout
	ViChar fullAddress[100] = "";
	viGetAttribute(device_, VI_ATTR_RSRC_NAME, fullAddress);
	if (strncmp("ASRL", fullAddress, 4) != 0)
	{
		return { std::wstring(L"[!] viGetAttribute() failled. It is not a ASRL device"), -3 };
	}

	viSetAttribute(device_, VI_ATTR_TERMCHAR_EN, VI_TRUE);
	// If you've setup the serial port settings in Connection Expert, you can remove this section. 
	// Otherwise, set your connection parameters
	viSetAttribute(device_, VI_ATTR_ASRL_BAUD, 9600);
	viSetAttribute(device_, VI_ATTR_ASRL_DATA_BITS, 8);
	viSetAttribute(device_, VI_ATTR_TERMCHAR, 0x0A);
	viSetAttribute(device_, VI_ATTR_ASRL_PARITY, VI_ASRL_PAR_NONE);
	viSetAttribute(device_, VI_ATTR_ASRL_FLOW_CNTRL, VI_ASRL_FLOW_RTS_CTS);
	viSetAttribute(device_, VI_ATTR_ASRL_FLOW_CNTRL, VI_ASRL_FLOW_XON_XOFF);

	// Set timeout value to 5s
	status = viSetAttribute(device_, VI_ATTR_TMO_VALUE, 5000);

	last_error.err_msg = std::wstring(L"OK");
	last_error.err_code = 0;
	return last_error;
}

err_struct cSerial::close()
{
	std::wcout << L"[*] cSerial close() called\n";
	viClose(device_);
	last_error.err_msg = std::wstring(L"OK");
	last_error.err_code = 0;
	return last_error;
}
