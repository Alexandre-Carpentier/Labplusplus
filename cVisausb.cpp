/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#include "cVisausb.h"

cVisausb::cVisausb()
	:cVisa()
{
	//std::wcout << L"[*] cVisausb default constructor called\n";
	device_name_ = L"magic";
}

cVisausb::cVisausb(std::wstring addr)
	:cVisa()
{
	//std::wcout << L"[*] cVisausb constructor called\n";
	assert(addr.size() > 0);
	device_name_ = addr;
}

err_struct cVisausb::init()
{
	//std::wcout << L"[*] cVisausb init() called\n";
	assert(device_name_.size() > 0);
	assert(ressource_manager > 0);

	std::string device_utf8 = ConvertWideToUtf8(device_name_);

	if (device_utf8.compare("magic") == 0) // if magic used. No address is specified. Searching for the first one available
	{
		device_utf8.clear();

		//TCPIP0::169.254.254.001::inst0::INSTR
		ViPFindList list = 0;
		ViPUInt32 count = 0;

#define INSTR_SIZE 512
		char instr[INSTR_SIZE] = "";
		if (VI_SUCCESS != viFindRsrc(ressource_manager, ViString("USB*INSTR"), list, count, instr))
		{
			return { std::wstring(L"[!] viFindRsrc() failled."), -1 };
		}

		assert(count != nullptr);
		assert(*count > 0);
		assert(list != nullptr);
		assert(*list > 0);

		assert(strnlen(instr, INSTR_SIZE) > 0);

		if (strnlen(instr, INSTR_SIZE) > 0)
		{
			device_utf8 = instr; // load value into a standard string
		}
		else
		{
			return { std::wstring(L"[!] viFindRsrc() failed. instrument returned is 0 characters"), -2 };
		}

	}
	assert(device_utf8.size() > 0);
	assert(device_utf8.size() < 260);

	// TODO: check VI_NULL vs VI_NOLOCK
	//status = viOpen(ressource_manager, (ViRsrc)device_name_.c_str(), VI_NULL, 0, &device_);
	//status = viOpen(ressource_manager, (ViRsrc)"ASRL4::INSTR", VI_NULL, 0, &device_);
	device_ = 0;
	status = viOpen(ressource_manager, (ViRsrc)device_utf8.c_str(), VI_NULL, 0, &device_);
	if (status != VI_SUCCESS)
	{
		if (status == -1073807343) 
		{
			std::cout << "Les informations concernant la position sont insuffisantes ou bien soit le périphérique soit la ressource n'est pas présent dans le système.\n";
		}
		return { std::wstring(L"[!] viOpen() failled."), -4 };
	}

	assert(device_ > 0);

	// Clear line
	status = viClear(device_);

	// For Serial and TCP/IP socket connections enable the read Termination Character, or read's will timeout
	ViChar fullAddress[100] = "";
	viGetAttribute(device_, VI_ATTR_RSRC_NAME, fullAddress);
	if (strncmp("USB", fullAddress, 3) != 0)
	{
		return { std::wstring(L"[!] viGetAttribute() failled. It is not a USB device"), -5 };
	}

	// Set timeout value to X s
	status = viSetAttribute(device_, VI_ATTR_TMO_VALUE, 500);

	/*
#define VI_ATTR_4882_COMPLIANT                (0x3FFF019FUL)
#define VI_ATTR_USB_SERIAL_NUM                (0xBFFF01A0UL)
#define VI_ATTR_USB_INTFC_NUM                 (0x3FFF01A1UL)
#define VI_ATTR_USB_PROTOCOL                  (0x3FFF01A7UL)
#define VI_ATTR_USB_MAX_INTR_SIZE             (0x3FFF01AFUL)
	*/

	viSetAttribute(device_, VI_ATTR_TERMCHAR, 0xA); // CR=0x0D LF=0x0A


	viFlush(device_, 0xC0);
	viSetBuf(device_, 0x30, 4096);

	last_error.err_msg = std::wstring(L"OK");
	last_error.err_code = 0;
	return last_error;
}

err_struct cVisausb::close()
{
	//std::wcout << L"[*] cVisausb close() called\n";
	viClose(device_);
	viClose(ressource_manager);

	last_error.err_msg = std::wstring(L"OK");
	last_error.err_code = 0;
	return last_error;
}
