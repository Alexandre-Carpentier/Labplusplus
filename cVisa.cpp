#include "cVisa.h"

cVisa::cVisa()
{
	//std::wcout << "[*] cVisa default constructor called\n";
	device_name_ = L"magic";

	if (viOpenDefaultRM(&ressource_manager) != VI_SUCCESS)
	{
		last_error.err_msg = std::wstring(L"[!] viOpen() failed.");
		last_error.err_code = -1;
	}
	else
	{
		assert(ressource_manager > 0);
		last_error.err_msg = std::wstring(L"OK");
		last_error.err_code = 0;
	}
}

err_struct cVisa::init()
{
	//std::wcout << L"[*] cVisa init() called\n";

	assert(device_name_.size() < 15);
	assert(ressource_manager > 0);

	if (device_name_.compare(L"magic") == 0)
	{
		device_name_.clear();

		ViPFindList list = 0;
		ViPUInt32 count = 0;

#define INSTR_SIZE 512
		char instr[INSTR_SIZE] = "";
		if (VI_SUCCESS != viFindRsrc(ressource_manager, ViString("?::*INSTR"), list, count, instr))
		{
			return { std::wstring(L"[!] viOpen() failed."), -1 };
		}
		assert(count == nullptr);
		assert(*count > 0);
		assert(list == nullptr);
		assert(*list > 0 );
		assert(strnlen(instr, INSTR_SIZE) > 0);

		if (strnlen(instr, INSTR_SIZE) > 0)
		{
			std::string utf8_str = instr; // load value into a standard string
			device_name_ = ConvertUtf8ToWide(utf8_str); // convert to wide
		}
		else
		{
			return { std::wstring(L"[!] viFindRsrc() failed. instrument returned is 0 characters"), -2 };
		}
	}

	assert(device_name_.size() > 0 );
	assert(device_name_.size() < 64 );

	std::string dev_name_utf8 = ConvertWideToUtf8(device_name_);

	assert(dev_name_utf8.size() >= 0 );
	assert(dev_name_utf8.size() < 20);

	device_ = 0;
	status = viOpen(ressource_manager, (ViString)dev_name_utf8.c_str(), VI_NO_LOCK, 0, &device_);
	if (status != VI_SUCCESS)
	{
		return { std::wstring(L"[!] viOpen() failed."), -3 };
	}

	assert(device_ > 0);

	// Set timeout value to 500 ms
	status = viSetAttribute(device_, VI_ATTR_TMO_VALUE, 500);

	last_error.err_msg = std::wstring(L"OK");
	last_error.err_code = 0;
	return last_error;
}

err_struct cVisa::write(std::wstring scpi)
{
	// Convert to multibytes string
	//std::wstring utf16_str = scpi;
	//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> converter;
	//std::string utf8_str = converter.to_bytes(utf16_str);

	assert(scpi.size() > 0);
	assert(scpi.size() < 256);


	std::string utf8 = ConvertWideToUtf8(scpi);

	assert(utf8.size() > 0);
	assert(utf8.size() < 256);

	ViStatus res = viPrintf(device_, (char *)utf8.c_str());

	// TODO: check this part
	if (res != VI_SUCCESS)
	{
		// Get extended error msg

		// Note  The size of the desc parameter should be at least 256 bytes.
		char msg[512] = "";

		res = viStatusDesc(device_, res, msg);
		assert(strlen(msg) < 300);
		assert(strlen(msg) > 0);

		if (res == VI_SUCCESS)
		{		
			// Convert to widestring string
			if (strlen(msg)>0)
			{
				utf8.clear();
				utf8 = msg;
			}
			else
			{
				utf8.clear();
				utf8 = "viStatusDesc error";
			}
			//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> converter;
			//last_error.err_msg = converter.from_bytes(utf8_str);

			last_error.err_msg = ConvertUtf8ToWide(utf8);
			assert(last_error.err_msg.size() < 300);
			assert(last_error.err_msg.size() >= 0);

			last_error.err_code = -2;

			std::wcout << last_error.err_msg;
			return last_error;
		}
		else
		{
			msg[0] = '\0'; // Sanitize
			last_error.err_msg = L"impossible to retrieve viRead error with viStatusDesc()\n";
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
#define BUFFER_SIZE 512
	unsigned char msg[BUFFER_SIZE]= "";
	ViUInt32 iRead = 0;

	ZeroMemory(msg, sizeof(msg));
	ViStatus res = viRead(device_, msg, BUFFER_SIZE, &iRead);
	last_error.err_code = res;

	assert(iRead < BUFFER_SIZE);
	assert(iRead >= 0 );

	msg[iRead] = '\0'; // Sanitize

	// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // 
	// RECV
	// 
	// CONVERT BYTES TO CHAR: Assuming char* (C string style) == unsigned char* (bytes) do a cast
	// 
	// 
	scpi.clear();
	if (strnlen(reinterpret_cast<const char*>(msg), BUFFER_SIZE) > 0)
	{	
		// Cast
		std::string msg_str = reinterpret_cast<const char*>(msg);
		assert(msg_str.size() > 0);
	
		// remove forbiden character or corrupted bytes
		int old_length = msg_str.size();
		msg_str.erase(remove_if(msg_str.begin(), msg_str.end(), [](char c) {return !(c >= 0 && c < 128); }), msg_str.end());
		int new_length = msg_str.size();
		if (new_length < old_length)
		{
			std::cout << "[!] Corrupted data received at cVisa::read()\n";
		}
		assert(msg_str.size() > 0);

		// convert to Wide
		scpi = ConvertUtf8ToWide(msg_str);
		assert(scpi.size() > 0);
	}
	// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // 
	
	// TODO: check this part
	/*
	if (res != VI_SUCCESS)
	{


		//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> converter;
		//scpi = converter.from_bytes(msg);


		char err_msg[100] = "";
		res = viStatusDesc(device_, res, err_msg);
		if (res == VI_SUCCESS)
		{
			printf("viRead error description: ");
			printf(err_msg);
			printf("\n");
		}
		last_error.err_msg = L"viRead error\n";
		last_error.err_code = -2;

		std::wcout << last_error.err_msg;
		return last_error;
		
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

			//todo: check this part
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> converter;
			last_error.err_msg = L"Error at Visa read!\n"; //converter.from_bytes(utf8_str);
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
			//wchar_t* wide = nullptr;
			
			std::string msg_str = msg;
			scpi = ConvertUtf8ToWide(msg_str);

			//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> converter;
			//scpi = converter.from_bytes(msg);
		}
		return last_error;
	}
	*/

	if (res == VI_ERROR_INV_OBJECT)
	{
		last_error.err_msg = std::wstring(L"The given session reference is invalid.\n");
	}
	else if (res == VI_ERROR_RSRC_LOCKED)
	{
		last_error.err_msg = std::wstring(L"Specified operation could not be performed because the resource identified by vi has been locked for this kind of access.\n");
	}
	else if (res == VI_ERROR_IO)
	{
		last_error.err_msg = std::wstring(L"Could not perform read operation because of I / O error.");
	}
	else if (res == VI_ERROR_TMO)
	{
		last_error.err_msg = std::wstring(L"Timeout expired before read operation completed.\n");
	}
	else if (res == VI_ERROR_INV_FMT)
	{
		last_error.err_msg = std::wstring(L"A format specifier in the readFmt string is invalid.\n");
	}
	else if (res == VI_ERROR_NSUP_FMT)
	{
		last_error.err_msg = std::wstring(L"A format specifier in the readFmt string is not supported.\n");
	}
	else if (res == VI_ERROR_ALLOC)
	{
		last_error.err_msg = std::wstring(L"The system could not allocate a formatted I / O buffer because of insufficient resources.\n");
	}
	else if (res == VI_SUCCESS)
	{
		last_error.err_msg = std::wstring(L"The operation completed successfully and the END indicator was received(for interfaces that have END indicators).This completion code is returned regardless of whether the termination character is received or the number of bytes read is equal to count.\n");
	}
	else if (res == VI_SUCCESS_TERM_CHAR)
	{
		last_error.err_msg = std::wstring(L"The specified termination character was read but no END indicator was received.This completion code is returned regardless of whether the number of bytes read is equal to count.\n");
	}
	else if (res == VI_SUCCESS_MAX_CNT)
	{		
		last_error.err_msg = std::wstring(L"The number of bytes read is equal to count.No END indicator was received and no termination character was read.\n");
	}
	else
	{
		last_error.err_msg = std::wstring(L"Undeterminate error at visa read.\n");	
	}

	// Display viRead success
	std::wcout << last_error.err_msg;
	
	// Display buffer received
	//std::wcout << "[******************BUFFER*******************]\n[";
	//std::wcout << scpi << "]\n";
	//std::wcout << "[******************BUFFER*******************]\n";

	return last_error;
}

err_struct cVisa::close()
{
	//std::wcout << L"[*] cVisa close() called\n";
	viClear(device_);
	viClose(device_);
	last_error.err_msg = std::wstring(L"OK\n");
	last_error.err_code = 0;
	return last_error;
}
