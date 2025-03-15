/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#include "cVisaserial.h"

cSerial::cSerial()
	:cVisa()
{
	//std::wcout << L"[*] cSerial default constructor called\n";
	device_name_ = L"magic";
}

cSerial::cSerial(std::wstring addr)
	:cVisa()
{
	//std::wcout << L"[*] cSerial constructor called\n";
	assert(addr.size() > 0);
	device_name_ = addr;
}

err_struct cSerial::init()
{
	//std::wcout << L"[*] cSerial init() called\n";
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
		if (VI_SUCCESS != viFindRsrc(ressource_manager, ViString("ASRL*?::*INSTR"), list, count, instr))
		{
			return { std::wstring(L"[!] viFindRsrc() failled."), -1 };
		}

		assert(count != nullptr);
		assert(*count > 0 );
		assert(list != nullptr);
		assert(*list > 0 );

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
	assert(device_utf8.size() > 0 );
	assert(device_utf8.size() <  20 );

	//status = viOpen(ressource_manager, (ViConstRsrc)this->device_name_.c_str(), VI_NO_LOCK, 0, &device_);
	/*
	// build VISA ressource address -> ex:\\.\COM34
	// C Style
	wchar_t* addr = nullptr;
	int addr_length = (device_name_.size()+1);
	addr = (wchar_t*) malloc(addr_length);
	if(addr == nullptr)
	{
		MessageBox(0, L"Fail", L"Fail to allocate addr buffer in cSerial::init()", S_OK);
		return { std::wstring(L"[!] Fail to allocate addr buffer in cSerial::init()."), -2 };
	}

	// copy std::string
	wchar_t com_id[4] = L"";
	wcsncpy(addr, device_name_.c_str(), device_name_.size() + 1);
	addr[device_name_.size() + 1] = L'\0';
	for (int i = 0; i < wcslen(addr); i++)
	{
		int max_digit = 3;
		if (addr[i] >= L'0' && addr[i] <= L'9')
		{
			// Sanitize
			max_digit--;
			if(max_digit==0)
			{
				return { std::wstring(L"[!] Fail to resolve addr com ID is > 3 digit."), -3 };
			}
			
			com_id[i]=addr[i]; // One unicode char
		}
		i++;
	}

	// Sanitize
	if (wcscmp(com_id, L"") == 0)
	{
		wcscpy(com_id, L"*");
	}

	free(addr);
	addr = nullptr;

	// C style end
	*/

	char* p = nullptr;
	std::string utf8_com = device_utf8.substr(7);
	long com_numb = strtol(utf8_com.c_str(), &p, 10);

	if (p == utf8_com.c_str())
	{
		std::cout << "[!] error\n";
	}
	assert(com_numb > 0);
	assert(com_numb < 255);

	//long com_numb = 4;
	if (com_numb < 0)
	{
		return { std::wstring(L"[!] strtol() failled."), -4 };
	}
	if (com_numb == 0)
	{
		return { std::wstring(L"[!] strtol() failled."), -5 };
	}
	if (com_numb > 255)
	{
		return { std::wstring(L"[!] strtol() failled."), -6 };
	}

	assert(com_numb >= 1 );
	assert(com_numb < 255 );

	std::string utf8_com_number = std::to_string(com_numb); // convert long to string
	std::string visa_device = std::format("ASRL{}::INSTR", utf8_com_number);

	assert(visa_device.size() <  14);

	// TODO: check VI_NULL vs VI_NOLOCK
	//status = viOpen(ressource_manager, (ViRsrc)device_name_.c_str(), VI_NULL, 0, &device_);
	//status = viOpen(ressource_manager, (ViRsrc)"ASRL4::INSTR", VI_NULL, 0, &device_);
	device_ = 0;
	status = viOpen(ressource_manager,  (ViRsrc)visa_device.c_str(), VI_NULL, 0, &device_);
	if (status != VI_SUCCESS)
	{
		return { std::wstring(L"[!] viOpen() failled."), -4 };
	}

	assert(device_ > 0);

	// Clear line
	status = viClear(device_);

	// For Serial and TCP/IP socket connections enable the read Termination Character, or read's will timeout
	ViChar fullAddress[100] = "";
	viGetAttribute(device_, VI_ATTR_RSRC_NAME, fullAddress);
	if (strncmp("ASRL", fullAddress, 4) != 0)
	{
		return { std::wstring(L"[!] viGetAttribute() failled. It is not a ASRL device"), -5 };
	}

	// Set timeout value to X s
	status = viSetAttribute(device_, VI_ATTR_TMO_VALUE, 1000);

	/*
	#define VI_ATTR_ASRL_AVAIL_NUM                (0x3FFF00ACUL
	#define VI_ATTR_ASRL_CTS_STATE                (0x3FFF00AEUL)
	#define VI_ATTR_ASRL_DCD_STATE                (0x3FFF00AFUL)
	#define VI_ATTR_ASRL_DSR_STATE                (0x3FFF00B1UL)
	#define VI_ATTR_ASRL_DTR_STATE                (0x3FFF00B2UL)
	#define VI_ATTR_ASRL_REPLACE_CHAR             (0x3FFF00BEUL)
	#define VI_ATTR_ASRL_RI_STATE                 (0x3FFF00BFUL)
	#define VI_ATTR_ASRL_RTS_STATE                (0x3FFF00C0UL)
	#define VI_ATTR_ASRL_XON_CHAR                 (0x3FFF00C1UL)
	#define VI_ATTR_ASRL_XOFF_CHAR                (0x3FFF00C2UL)
	*/

	viSetAttribute(device_, VI_ATTR_TERMCHAR_EN, VI_FALSE);

	viSetAttribute(device_, VI_ATTR_SEND_END_EN, VI_TRUE);
	viSetAttribute(device_, VI_ATTR_SUPPRESS_END_EN, VI_FALSE);

	viSetAttribute(device_, VI_ATTR_ASRL_END_IN, VI_TRUE);
	viSetAttribute(device_, VI_ATTR_ASRL_END_OUT, VI_TRUE);
	
	viSetAttribute(device_, VI_ATTR_ASRL_BAUD, 9600);
	
	//viSetAttribute(device_, VI_ATTR_ASRL_FLOW_CNTRL, VI_ASRL_FLOW_NONE);
	//viSetAttribute(device_, VI_ATTR_ASRL_FLOW_CNTRL, VI_ASRL_FLOW_XON_XOFF);
	viSetAttribute(device_, VI_ATTR_ASRL_FLOW_CNTRL, VI_ASRL_FLOW_RTS_CTS);

	viSetAttribute(device_, VI_ATTR_ASRL_PARITY, VI_ASRL_PAR_NONE);
	viSetAttribute(device_, VI_ATTR_ASRL_DATA_BITS, 8);
	viSetAttribute(device_, VI_ATTR_ASRL_STOP_BITS, VI_ASRL_STOP_ONE);
	

	viSetAttribute(device_, VI_ATTR_TERMCHAR, 0xD); // CR=0x0D LF=0x0A
	viSetAttribute(device_, VI_ATTR_IO_PROT, VI_PROT_4882_STRS);

	viFlush(device_, 0xC0);
	viSetBuf(device_, 0x30, 4096);

	last_error.err_msg = std::wstring(L"OK");
	last_error.err_code = 0;
	return last_error;
}

err_struct cSerial::close()
{
	//std::wcout << L"[*] cSerial close() called\n";
	viClose(device_);
	viClose(ressource_manager);
	/*
	'Lab++.exe' (Win32) : Déchargé 'C:\Program Files\IVI Foundation\VISA\Win64\NIvisa\PxiPlugins\NiViPpiP.dll'
'Lab++.exe' (Win32) : Déchargé 'C:\Program Files\Keysight\IO Libraries Suite\bin\ioModInstPpi.dll'
Le thread 0x73d4 s'est arrêté avec le code 0 (0x0).
'Lab++.exe' (Win32) : Déchargé 'C:\Program Files\IVI Foundation\VISA\Win64\NIvisa\PxiPlugins\NiViPpiD.dll'
*/
	last_error.err_msg = std::wstring(L"OK");
	last_error.err_code = 0;
	return last_error;
}
