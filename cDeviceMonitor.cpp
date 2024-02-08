#include "cDeviceMonitor.h"

std::vector<int> get_rs232port_list()
{
	HANDLE hCom = nullptr;
	std::wstring comaddrbase = L"\\\\.\\COM";
	std::wstring comaddr = L"";

	std::vector<int> com_list;

	for (int iComNb = 0; iComNb < 64; iComNb++)
	{
		comaddr = comaddrbase;
		comaddr.append(std::to_wstring(iComNb));
		//std::cout << "[*] - Trying opening :" << comaddr << " with CreateFile() \n";

		hCom = CreateFile(comaddr.c_str(), GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, NULL, NULL);
		if (hCom != INVALID_HANDLE_VALUE && hCom != NULL)
		{
			com_list.push_back(iComNb);
			CloseHandle(hCom);
		}
	}
	return com_list;
}

void cDeviceMonitor::Notify()
{
	dev_list.clear();
	std::wstring comaddrbase = L"\\\\.\\COM8";
	cDev dev;
	dev.set_addr(comaddrbase);

	dev.set_type(L"VISASERIAL");
	dev.set_name(comaddrbase);
	dev_list.push_back(dev);

	
	/*
	// Com lookup
	dev_list.clear();
	std::vector<int> com = get_rs232port_list();
	cProtocolFactory factory;
	std::wstring comaddrbase = L"\\\\.\\COM";
	std::wstring comaddr = L"";

	for (auto& item : com)
	{
		std::cout << item << "\n";

		comaddr = comaddrbase;
		comaddr.append(std::to_wstring(item));

		// Create the proper implementation of cProtocol with a factory method
		std::unique_ptr<cProtocol> device = factory.make(PROTOCOLENUM::SERIAL, comaddr.c_str());

		std::wstring instr_name = L"";
		device->write(L"*IDN?\n");
		device->read(instr_name);

		cDev dev;	
		dev.set_addr(comaddr);	

		if (instr_name.compare(L"") != 0)
		{
			dev.set_type(L"VISASERIAL");
			dev.set_name(instr_name);
		}
		else
		{
			dev.set_type(L"SERIAL");
			dev.set_name(L"Unknown");		
		}
		std::cout << "[*] Found: " << dev.get_type() << " " << dev.get_name() << " at COM" << item << "\n";
		dev_list.push_back(dev);
	}
	*/
}

std::vector<cDev> cDeviceMonitor::get_device_vec()
{
	return dev_list;
}

void cDeviceMonitor::lookup_stop()
{
	wxTimer::Stop();
}

void cDeviceMonitor::lookup_start()
{
	wxTimer::Start();
}
