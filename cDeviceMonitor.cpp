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
	// Com lookup
	dev_list.clear();
	std::vector<int> com = get_rs232port_list();
	std::vector<std::string> usb = get_USBTMCport_list();

	cProtocolFactory factory;
	//COM
	std::wstring comaddrbase = L"\\\\.\\COM";
	std::wstring comaddr = L"";
	//USB
	std::wstring usbaddr = L"";

	for (auto& item : com)
	{
		//std::cout << item << "\n";

		comaddr = comaddrbase;
		comaddr.append(std::to_wstring(item));

		// Create the proper implementation of cProtocol with a factory method
		std::unique_ptr<IProtocol> device = factory.make(PROTOCOLENUM::VISASERIAL, comaddr.c_str());

		std::wstring instr_name = L"";
		device->init();
		device->write(L"*IDN?\n");
		device->read(instr_name);
		if (instr_name.compare(L"") != 0)
		{
			device->write(L"*IDN?\r");
			device->read(instr_name);
		}
		device->close();

		cDev dev;	
		dev.set_addr(comaddr);	

		if (instr_name.compare(L"") != 0)
		{
			dev.set_type(L"VISASERIAL");
			dev.set_name(instr_name);
		}
		else
		{
			dev.set_type(L"RS232");
			dev.set_name(L"Unknown");		
		}
		std::cout << "[*] cDeviceMonitor found: " << dev.get_type() << " " << dev.get_name() << " at COM" << item << "\n";
		dev_list.push_back(dev);
	}
	for (auto& item : usb)
	{
		//extract info
		//\\ ? \usb#vid_2a8d&pid_0397#cn62154267#{a5dcbf10-6530-11d2-901f-00c04fb951ed}"
		item = item.substr(item.find("vid_")+4);
		//2a8d&pid_0397#cn62154267#{a5dcbf10-6530-11d2-901f-00c04fb951ed}"
		size_t last = item.find("&pid_");
		std::string vid = item.substr(0, last);
		item= item.substr(last + 5);
		//0397#cn62154267#{a5dcbf10 - 6530 - 11d2 - 901f - 00c04fb951ed}"
		last = item.find("#");
		std::string pid = item.substr(0, last);
		item = item.substr(last + 1);
		std::string SN = item.substr(0, item.find("#{"));

		std::string addr = std::format("USB0::0x{}::0x{}::{}::INSTR", vid,pid,SN);
		usbaddr = ConvertUtf8ToWide(addr);
		//usbaddr = L"USB0::0x2A8D::0x0397::CN62154267::INSTR";

		//usbaddr = L"USB0::*::*::*::INSTR";
		//usbaddr = ConvertUtf8ToWide(item);
		//usbaddr = L"magic";

		// Create the proper implementation of cProtocol with a factory method
		std::unique_ptr<IProtocol> device = factory.make(PROTOCOLENUM::VISAUSB, usbaddr.c_str());

		std::wstring instr_name = L"";
		if (device->init().err_code < 0) 
		{ 
			continue; 
		}
		device->write(L"*IDN?\n");
		device->read(instr_name);
		if (instr_name.compare(L"") == 0)
		{
			device->write(L"*IDN?\r");
			device->read(instr_name);
		}
		device->close();

		cDev dev;
		dev.set_addr(usbaddr);

		if (instr_name.compare(L"") != 0)
		{
			dev.set_type(L"USBTMC");
			dev.set_name(instr_name);
		}
		else
		{
			dev.set_type(L"USB");
			dev.set_name(L"Unknown");
		}
		std::cout << "[*] cDeviceMonitor found: " << dev.get_type() << " " << dev.get_name() << " at USB" << item << "\n";
		dev_list.push_back(dev);
	}
}

std::vector<cDev> cDeviceMonitor::get_device_vec()
{
	return dev_list;
}

void cDeviceMonitor::lookup_stop()
{
	bool run = true;//this->IsRunning();
	if (run)
	{
		//Stop();
	}
	//this->kill();
}

void cDeviceMonitor::lookup_start()
{
	//Start();
}
