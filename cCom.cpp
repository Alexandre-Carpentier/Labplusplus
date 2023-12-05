#include "cCom.h"

cCom::cCom()
{
	std::cout << "[*] cCom default constructor called\n";
	device_name_ = L"magic";

	last_error.err_msg = std::wstring(L"OK");
	last_error.err_code = 0;
}

cCom::cCom(std::wstring addr)
{
	std::cout << "[*] cCom constructor called\n";
	device_name_ = addr;

	last_error.err_msg = std::wstring(L"OK");
	last_error.err_code = 0;
}

err_struct cCom::init()
{
	std::cout << "[*] cVisa init() called\n";

	if (device_name_.compare(L"magic") == 0)
	{
		// TODO: autosearch first available addr
		device_name_ = L"COM1";
	}

	std::wcout << L"lpFileName" << device_name_;
	hCom = CreateFile(device_name_.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hCom == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			std::wcout << L"\nError: \nThe system cannot find the file specified (" << device_name_ << L")\n";
		}
		else if (GetLastError() == ERROR_INVALID_NAME)
		{
			std::wcout << L"\nError: \n" << hCom << L" 'filename, directory name, or volume label syntax is incorrect'\n";
		}
		else
		{
			std::wcout << L"Handle creation error code: " << GetLastError() << L"\n";
		}
		std::wcout << L"CreateFile returned an invalid handle value.";
	}

	if (!GetCommState(hCom, &dcbSerialParams))
	{
		std::wcout << L"Error: \ncould not get " << hCom << L" state!\n";

	}

	// Usefull link:
	//http://www.tetraedre.com/advanced/serial/msdn_serial.htm

	dcbSerialParams.BaudRate = CBR_9600;

	// Enable XON and XOFF

	dcbSerialParams.fOutX = TRUE;
	dcbSerialParams.fInX = TRUE;

	dcbSerialParams.StopBits = 0;
	dcbSerialParams.Parity = 0;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.EofChar = '\n';

	if (SetCommState(hCom, &dcbSerialParams) == 0)
	{
		last_error.err_msg = std::wstring(L"SetCommState() error.\n");
		last_error.err_code = -1;
		std::wcout << last_error.err_msg;
		return last_error;
	}

	// instance an object of COMMTIMEOUTS.
	COMMTIMEOUTS comTimeOut;
	// Specify time-out between charactor for receiving.
	comTimeOut.ReadIntervalTimeout = 3;
	// Specify value that is multiplied 
	// by the requested number of bytes to be read. 
	comTimeOut.ReadTotalTimeoutMultiplier = 3;
	// Specify value is added to the product of the 
	// ReadTotalTimeoutMultiplier member
	comTimeOut.ReadTotalTimeoutConstant = 2;
	// Specify value that is multiplied 
	// by the requested number of bytes to be sent. 
	comTimeOut.WriteTotalTimeoutMultiplier = 3;
	// Specify value is added to the product of the 
	// WriteTotalTimeoutMultiplier member
	comTimeOut.WriteTotalTimeoutConstant = 2;
	// set the time-out parameter into device control.
	SetCommTimeouts(hCom, &comTimeOut);

	last_error.err_msg = std::wstring(L"OK");
	last_error.err_code = 0;
	return last_error;
}

err_struct cCom::write(std::wstring scpi)
{
	DWORD dwBytes = 0x0;
	if (!WriteFile(hCom, scpi.c_str(), std::wcslen(scpi.c_str()), &dwBytes, nullptr))
	{
		last_error.err_msg = std::wstring(L"WriteFile() error");
		last_error.err_code = 0;
		return last_error;
	}

	last_error.err_msg = std::wstring(L"OK");
	last_error.err_code = 0;
	return last_error;
}

err_struct cCom::read(std::wstring& scpi)
{
	size_t buff_size = 256;
	std::vector<wchar_t> buffer;
	buffer.resize(buff_size);

	DWORD dwBytes = 0x0;

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


	last_error.err_msg = std::wstring(L"OK");
	last_error.err_code = 0;
	return last_error;
}

err_struct cCom::close()
{
	std::wcout << L"[*] cCom close() called\n";
	CloseHandle(hCom);
	last_error.err_msg = std::wstring(L"OK\n");
	last_error.err_code = 0;
	return last_error;
}
