/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#include "cUsb.h"
#if _WIN64
extern "C" void initusb_asm();
#else 

#endif


cUsb::cUsb()
{
	std::cout << "[*] cUsb default constructor called\n";
	device_name_ = L"magic";

	last_error.err_msg = std::wstring(L"OK");
	last_error.err_code = 0;
}

cUsb::cUsb(std::wstring addr)
{
	std::cout << "[*] cUsb constructor called\n";
	device_name_ = addr;

	last_error.err_msg = std::wstring(L"OK");
	last_error.err_code = 0;
}

err_struct cUsb::init()
{
	std::cout << "[*] cUsb init() called\n";
#if _WIN64
	initusb_asm();
#endif

	if (device_name_.compare(L"magic") == 0)
	{
		device_name_ = L"\\\\?\\usb#vid_2a8d&pid_0397#cn62154267#{a5dcbf10-6530-11d2-901f-00c04fb951ed}";
	}

	std::wcout << L"lpFileName" << device_name_ << "\n";
	hUsb = CreateFile(device_name_.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);

	if (hUsb == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			std::wcout << L"\nError: \nThe system cannot find the file specified (" << device_name_ << L")\n";
		}
		else if (GetLastError() == ERROR_INVALID_NAME)
		{
			std::wcout << L"\nError: \n" << hUsb << L" 'filename, directory name, or volume label syntax is incorrect'\n";
		}
		else if (GetLastError() == ERROR_GEN_FAILURE)
		{
			std::wcout << L"\nError: \n" << hUsb << L" A device attached to the system is not functioning.\n";
		}
		else
		{
			std::wcout << L"Handle creation error code: " << GetLastError() << L"\n";
		}
		std::wcout << L"CreateFile returned an invalid handle value.";
		last_error.err_msg = std::wstring(L"CreateFile failed");
		last_error.err_code = -1;
		return last_error;
	}

	// CANCELIO
	OVERLAPPED overlapped = { 0 };
	//ZeroMemory(&overlapped, sizeof(OVERLAPPED));
	overlapped.hEvent = CreateEvent(NULL, false, false, NULL);

	USBTMC_PIPE_TYPE pipeType = USBTMC_ALL_PIPES;

	DWORD retVal = 0;
	if (DeviceIoControl(hUsb, (DWORD)IOCTL_USBTMC_CANCEL_IO, (LPVOID)&pipeType, sizeof(USBTMC_PIPE_TYPE), NULL, 0, &retVal, &overlapped)) {
		int ret = WaitForSingleObject(overlapped.hEvent, 1000);
		if (ret == WAIT_TIMEOUT)
		{
			std::cout << "Cancel timeout\n";
			std::wcout << L"Cancel timeout.";
			last_error.err_msg = std::wstring(L"DeviceIoControl IOCTL_USBTMC_CANCEL_IO timeout");
			last_error.err_code = -2;
			return last_error;
		}			
		else if (ret == 0)
			std::cout << "Cancel succeeded\n";
		else
		{
			std::cout << "Cancel failed with code" << GetLastError() << "\n";
			last_error.err_msg = std::wstring(L"DeviceIoControl IOCTL_USBTMC_CANCEL_IO failed");
			last_error.err_code = -3;
			return last_error;
		}
			
	}
	CloseHandle(overlapped.hEvent);

	// RESETPIPE
	//overlapped;
	ZeroMemory(&overlapped, sizeof(OVERLAPPED));
	overlapped.hEvent = CreateEvent(NULL, false, false, NULL);

	pipeType = USBTMC_ALL_PIPES;

	retVal = 0;
	if (DeviceIoControl(hUsb, (DWORD)IOCTL_USBTMC_RESET_PIPE, (LPVOID)&pipeType, sizeof(USBTMC_PIPE_TYPE), NULL, 0, &retVal, &overlapped)) {
		int ret = WaitForSingleObject(overlapped.hEvent, 1000);
		if (ret == WAIT_TIMEOUT)
		{
			std::cout << "Reset timeout\n";
			last_error.err_msg = std::wstring(L"DeviceIoControl IOCTL_USBTMC_RESET_PIPE failed");
			last_error.err_code = -4;
			return last_error;
		}		
		else if (ret == 0)
		{
			std::cout << "Reset succeeded\n";
		}			
		else
		{
			std::cout << "Reset failed with code" << GetLastError() << "\n";
			last_error.err_msg = std::wstring(L"DeviceIoControl IOCTL_USBTMC_RESET_PIPE failed");
			last_error.err_code = -5;
			return last_error;
		}		
	}
	CloseHandle(overlapped.hEvent);

	// GETINFO
	//overlapped;
	ZeroMemory(&overlapped, sizeof(OVERLAPPED));
	overlapped.hEvent = CreateEvent(NULL, false, false, NULL);

	USBTMC_DRV_INFO drvInfo;
	ZeroMemory(&drvInfo, sizeof(USBTMC_DRV_INFO));

	retVal = 0;
	if (DeviceIoControl(hUsb, (DWORD)IOCTL_USBTMC_GETINFO, NULL, 0, &drvInfo, sizeof(USBTMC_DRV_INFO), &retVal, &overlapped)) {
		WaitForSingleObject(overlapped.hEvent, 1000);
		if (drvInfo.major > 0)
		{
			std::cout << "IOCTL_USBTMC_GETINFO returned:" << drvInfo.major << drvInfo.minor << drvInfo.build << ConvertWideToUtf8(drvInfo.manufacturer) << "\n";
		}
		else
		{
			last_error.err_msg = std::wstring(L"DeviceIoControl IOCTL_USBTMC_GETINFO failed");
			last_error.err_code = -6;
			return last_error;
		}
	}
	CloseHandle(overlapped.hEvent);

	// SENDREQUEST
	//overlapped;
	ZeroMemory(&overlapped, sizeof(OVERLAPPED));
	overlapped.hEvent = 0;
	overlapped.hEvent = CreateEvent(NULL, false, false, NULL);
	if (overlapped.hEvent == 0)
	{
		std::cout << "CreateEvent failed with code " << GetLastError() << "\n";
		last_error.err_msg = std::wstring(L"CreateEvent failed");
		last_error.err_code = -6;
		return last_error;
	}

	const int bufferSize = 0x12;
	unsigned char buffer[bufferSize];
	ZeroMemory(buffer, bufferSize);

	USBTMC_IO_BLOCK ioBlock;
	ZeroMemory(&ioBlock, sizeof(USBTMC_IO_BLOCK));

	ioBlock.bmRequestType = TGT_DEVICE | REQ_STD | DIR_FROM_DEVICE;
	ioBlock.bRequest = URB_GET_DESCRIPTOR;
	ioBlock.wValue = URB_INTERFACE << 8;
	ioBlock.wIndex = 0;
	ioBlock.wLength = bufferSize;
	ioBlock.fTransferDirectionIn = 1;

	retVal = 0;
	if (DeviceIoControl(hUsb, (DWORD)IOCTL_USBTMC_SEND_REQUEST, (LPVOID)&ioBlock, sizeof(USBTMC_IO_BLOCK), buffer, bufferSize, &retVal, &overlapped)) {
		int ret = WaitForSingleObject(overlapped.hEvent, 1000);
		if (ret == WAIT_TIMEOUT)
		{
			std::cout << "SendRequest timeout\n";
			last_error.err_msg = std::wstring(L"DeviceIoControl IOCTL_USBTMC_SEND_REQUEST failed");
			last_error.err_code = -7;
			return last_error;
		}		
		else if (ret == 0) {
			std::cout << "SendRequest succeeded\n";
		}
		else
		{
			std::cout << "SendRequest failed with code " << GetLastError() << "\n";
			last_error.err_msg = std::wstring(L"DeviceIoControl IOCTL_USBTMC_SEND_REQUEST failed");
			last_error.err_code = -8;
			return last_error;
		}
			
	}
	else {
		std::cout << "DeviceIO SendRequest failed with code " << GetLastError() << "\n";
		last_error.err_msg = std::wstring(L"DeviceIoControl failed");
		last_error.err_code = -9;
		return last_error;
	}
	CloseHandle(overlapped.hEvent);

	// WRITE
	overlapped;
	ZeroMemory(&overlapped, sizeof(OVERLAPPED));
	overlapped.hEvent = CreateEvent(NULL, false, false, NULL);

	const char* cmd = "*RST\n";
	retVal = 0;
	if (WriteFile(hUsb, cmd, static_cast<DWORD>(strlen(cmd)), &retVal, &overlapped)) {
		std::cout << "WriteFile wrote " << retVal << " bytes\n";
	}
	else if (GetLastError() == ERROR_IO_PENDING) {
		int ret = WaitForSingleObject(overlapped.hEvent, 1000);
		std::cout << "WriteFile WaitForSingleObject returned " << ret <<"\n";
		if (ret == 0)
		{
			std::cout << "WriteFile complete\n";
		}		
		else if (ret == WAIT_TIMEOUT)
		{
			std::cout << "WriteFile timeout\n";
			last_error.err_msg = std::wstring(L"WriteFile failed");
			last_error.err_code = -10;
			return last_error;
		}			
		else
		{
			std::cout << "WriteFile failed with code " << GetLastError() << "\n";
			last_error.err_msg = std::wstring(L"WriteFile failed");
			last_error.err_code = -11;
			return last_error;
		}			
	}
	if (overlapped.hEvent != 0)
	{
		CloseHandle(overlapped.hEvent);
	}

	// READ
	overlapped;
	ZeroMemory(&overlapped, sizeof(OVERLAPPED));
	overlapped.hEvent = CreateEvent(NULL, false, false, NULL);

	DWORD dwResult = 0;
	char recv[1024] = "";
	retVal = 0;
	if (ReadFile(hUsb, &recv, sizeof(recv), &retVal, &overlapped)) {
		std::cout << "ReadFile read " << retVal << " bytes\n";
	}
	else if (GetLastError() == ERROR_IO_PENDING) {
		int ret = WaitForSingleObject(overlapped.hEvent, 2000);
		std::cout << "ReadFile WaitForSingleObject returned " << ret << "\n";
		if (ret == 0)
		{
			std::cout << "ReadFile complete\n";
			GetOverlappedResult(hUsb, &overlapped, &dwResult, true);
		}	
		else if (ret == WAIT_TIMEOUT)
		{
			std::cout << "ReadFile timeout\n";
			last_error.err_msg = std::wstring(L"ReadFile timeout failed");
			last_error.err_code = -12;
			return last_error;
		}			
		else
		{
			std::cout << "ReadFile failed with code " << GetLastError() << "\n";
			last_error.err_msg = std::wstring(L"ReadFile failed");
			last_error.err_code = -13;
			return last_error;
		}
			
	}
	else 
	{
		dwResult = GetLastError();
		std::cout << "[!] Err:" << dwResult << "\n";
		last_error.err_msg = std::wstring(L"ReadFile failed");
		last_error.err_code = -14;
		return last_error;
	}
	if (overlapped.hEvent != 0)
	{
		CloseHandle(overlapped.hEvent);
	}
	// WinUsb_Initialize
	/*
	WINUSB_INTERFACE_HANDLE usb_interface = INVALID_HANDLE_VALUE;
	BOOL bRes = WinUsb_Initialize(hUsb, &usb_interface);
	if (bRes == false)
	{
		if (GetLastError() == ERROR_BAD_DEVICE)
		{
			std::wcout << L"\nError WinUsb_Initialize: \nThe system cannot find the device specified (" << device_name_ << L")\n";
		}
		else if (GetLastError() == ERROR_NOT_ENOUGH_MEMORY)
		{
			std::wcout << L"\nError WinUsb_Initialize: \n" << hUsb << L" 'System out of memory'\n";
		}
		else if (GetLastError() == ERROR_INVALID_HANDLE)
		{
			std::wcout << L"\nError WinUsb_Initialize: \n" << hUsb << L" 'DeviceHandle hUsb is null or FILE_FLAG_OVERLAPPED not defined in CreateFile(). '\n";
		}
		else
		{
			std::wcout << L"WinUsb_Initialize handle creation error code: " << GetLastError() << L"\n";
		}
		std::wcout << L"WinUsb_Initialize returned an invalid handle value.";
	}

	// WinUsb_QueryDeviceInformation
	unsigned long id = 0;
	unsigned long* length = NULL;
	void* buffer = NULL;
	WinUsb_QueryDeviceInformation(usb_interface, id, length, buffer);

	// WinUsb_QueryInterfaceSettings
	// WinUsb_FlushPipe
	// WinUsb_QueryPipe
	// WinUsb_ReadPipe
	// WinUsb_WritePipe
	// WinUsb_Free
	WinUsb_Free(usb_interface);
	*/


	last_error.err_msg = std::wstring(L"OK");
	last_error.err_code = 0;
	return last_error;
}

err_struct cUsb::write(std::wstring scpi)
{
	assert(scpi.size() > 0);

	DWORD dwBytes = 0x0;
	/*if (!WriteFile(hCom, scpi.c_str(), std::wcslen(scpi.c_str()), &dwBytes, nullptr))
	{
		last_error.err_msg = std::wstring(L"WriteFile() error");
		last_error.err_code = 0;
		return last_error;
	}

	assert(dwBytes > 0);
	*/
	last_error.err_msg = std::wstring(L"OK");
	last_error.err_code = 0;
	return last_error;
}

err_struct cUsb::read(std::wstring& scpi)
{
	size_t buff_size = 256;
	std::vector<wchar_t> buffer;
	buffer.resize(buff_size);
	scpi.clear();
	DWORD dwBytes = 0x0;
	/*
	do
	{
		if (!ReadFile(hCom, &buffer[0] + dwBytes, buff_size, &dwBytes, NULL))
		{
			last_error.err_msg = std::wstring(L"Undeterminate error at ReadFile().\n");
			last_error.err_code = -1;
			std::wcout << last_error.err_msg;
			return last_error;
		}

		// Add 256 bytes memory if buffer is full
		if (dwBytes == buff_size)
			buffer.resize(buff_size);

	} while (dwBytes > 0);

	*/
	last_error.err_msg = std::wstring(L"OK");
	last_error.err_code = 0;
	return last_error;
}

err_struct cUsb::close()
{
	std::wcout << L"[*] cUsb close() called\n";
	CloseHandle(hUsb);
	last_error.err_msg = std::wstring(L"OK\n");
	last_error.err_code = 0;
	return last_error;
}
