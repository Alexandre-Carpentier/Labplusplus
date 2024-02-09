#include "cVisa.h"

cVisa::cVisa()
{
	std::wcout << "[*] cVisa default constructor called\n";
	device_name_ = L"magic";

	if (viOpenDefaultRM(&ressource_manager) != VI_SUCCESS)
	{
		last_error.err_msg = std::wstring(L"[!] viOpen() failed.");
		last_error.err_code = -1;
	}
	else
	{
		last_error.err_msg = std::wstring(L"OK");
		last_error.err_code = 0;
	}
}

err_struct cVisa::init()
{
	std::wcout << L"[*] cVisa init() called\n";

	if (device_name_.compare(L"magic") == 0)
	{
		ViPFindList list = 0;
		ViPUInt32 count = 0;

		char* instr = nullptr;
		if (VI_SUCCESS != viFindRsrc(ressource_manager, (char*)"?::*INSTR", list, count, instr))
		{
			return { std::wstring(L"[!] viOpen() failed."), -1 };
		}

		std::string utf8_str = instr; // load value into a standard string
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> converter;
		device_name_ = converter.from_bytes(utf8_str);
	}

	status = viOpen(ressource_manager, (ViRsrc)this->device_name_.c_str(), VI_NO_LOCK, 0, &device_);
	if (status != VI_SUCCESS)
	{
		return { std::wstring(L"[!] viOpen() failed."), -2 };
	}

	// Set timeout value to 500 ms
	status = viSetAttribute(device_, VI_ATTR_TMO_VALUE, 500);

	last_error.err_msg = std::wstring(L"OK");
	last_error.err_code = 0;
	return last_error;
}

err_struct cVisa::write(std::wstring scpi)
{
	// Convert to multibytes string
	std::wstring utf16_str = scpi;
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> converter;
	std::string utf8_str = converter.to_bytes(utf16_str);

	ViStatus res = viPrintf(device_, (char *)utf8_str.c_str());

	// TODO: check this part
	if (res != VI_SUCCESS)
	{
		char msg[100];
		res = viStatusDesc(device_, res, msg);
		if (res == VI_SUCCESS)
		{		// Convert to widestring string
			std::string utf8_str;
			if (msg[0] != '\0')
			{
				utf8_str = msg;
			}
			else
			{
				utf8_str = "viStatusDesc error";
			}
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> converter;
			last_error.err_msg = converter.from_bytes(utf8_str);
			last_error.err_code = -2;

			std::wcout << last_error.err_msg;
			return last_error;
		}
		else
		{
			last_error.err_msg = L"impossible to retrieve viRead error\n";
			last_error.err_code = -3;

			std::wcout << last_error.err_msg;
			return last_error;
		}
	}

	last_error.err_msg = std::wstring(L"OK");
	last_error.err_code = 0;
	return last_error;
}

err_struct cVisa::read(std::wstring& scpi)
{

	char msg[100];
	ViUInt32 iRead = 0;
	ViStatus res = viRead(device_, (ViBuf)msg, sizeof(msg), &iRead);
	msg[iRead] = '\0';

	// TODO: check this part
	if (res != VI_SUCCESS)
	{
		char err_msg[100] = "";
		res = viStatusDesc(device_, res, err_msg);
		if (res == VI_SUCCESS)
		{		// Convert to widestring string
			std::string utf8_str;
			if (err_msg[0] != '\0')
			{
				utf8_str = err_msg;
			}
			else
			{
				utf8_str = "viStatusDesc error";
			}
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> converter;
			last_error.err_msg = converter.from_bytes(utf8_str);
			last_error.err_code = -2;

			// If characters present in the buffer
			if (iRead > 0)
			{
				std::wstring wmsg = converter.from_bytes(msg);
				scpi = wmsg;
			}

			std::wcout << last_error.err_msg;
			return last_error;
		}
		else
		{
			last_error.err_msg = L"impossible to retrieve viRead error\n";
			last_error.err_code = -3;

			std::wcout << last_error.err_msg;
			return last_error;
		}
	}
	else
	{
		// VI_SUCCESS

		last_error.err_msg = std::wstring(L"OK\n");
		last_error.err_code = 0;

		// If characters present in the buffer
		if (iRead > 0)
		{
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> converter;
			scpi = converter.from_bytes(msg);
		}
		return last_error;
	}


	/*
	if (res == VI_ERROR_INV_OBJECT)
	{
		last_error.err_msg = std::wstring(L"The given session reference is invalid.\n");
		std::wcout << last_error.err_msg;
		return last_error;

	}
	if (res == VI_ERROR_RSRC_LOCKED)
	{
		last_error.err_msg = std::wstring(L"Specified operation could not be performed because the resource identified by vi has been locked for this kind of access.\n");
		std::wcout << last_error.err_msg;
		return last_error;
	}
	if (res == VI_ERROR_IO)
	{
		last_error.err_msg = std::wstring(L"Could not perform read operation because of I / O error.");
		std::wcout << last_error.err_msg;
		return last_error;
	}
	if (res == VI_ERROR_TMO)
	{
		last_error.err_msg = std::wstring(L"Timeout expired before read operation completed.\n");
		std::wcout << last_error.err_msg;
		return last_error;
	}
	if (res == VI_ERROR_INV_FMT)
	{
		last_error.err_msg = std::wstring(L"A format specifier in the readFmt string is invalid.\n");
		std::wcout << last_error.err_msg;
		return last_error;
	}
	if (res == VI_ERROR_NSUP_FMT)
	{
		last_error.err_msg = std::wstring(L"A format specifier in the readFmt string is not supported.\n");
		std::wcout << last_error.err_msg;
		return last_error;
	}
	if (res == VI_ERROR_ALLOC)
	{
		last_error.err_msg = std::wstring(L"The system could not allocate a formatted I / O buffer because of insufficient resources.\n");
		std::wcout << last_error.err_msg;
		return last_error;
	}
	if (res == VI_SUCCESS)
	{
		last_error.err_msg = std::wstring(L"OK\n");
		std::wcout << msg << std::endl;
		//std::cout << buffer << std::endl;
		std::string str = msg;

		//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> converter;
		//last_error.err_msg = converter.from_bytes(utf8_str);
		return last_error;
	}
	last_error.err_msg = std::wstring(L"Undeterminate error at visa read.\n");
	std::wcout << last_error.err_msg;
	return last_error;
	*/
}

err_struct cVisa::close()
{
	std::wcout << L"[*] cVisa close() called\n";
	viClear(device_);
	viClose(device_);
	last_error.err_msg = std::wstring(L"OK\n");
	last_error.err_code = 0;
	return last_error;
}
