#include "cProtocolFactory.h"

std::unique_ptr<IProtocol> cProtocolFactory::make(PROTOCOLENUM type, std::wstring addr)
{
	std::unique_ptr<IProtocol> protocol_obj = nullptr;

	if (type == PROTOCOLENUM::USB)
	{
		if (addr.length() == 0)
		{
			std::wcout << L"No device adress supplied, trying to find first\n";
			protocol_obj = std::make_unique<cUsb>();
		}
		protocol_obj = std::make_unique<cUsb>(addr);
	}
	if (type == PROTOCOLENUM::COM)
	{
		if (addr.length() == 0)
		{
			std::wcout << L"No device adress supplied, trying to find first\n";
			protocol_obj = std::make_unique<cCom>();
		}
		protocol_obj = std::make_unique<cCom>(addr);
	}
	if (type == PROTOCOLENUM::VISASERIAL)
	{
		if (addr.length() == 0)
		{
			std::wcout << L"No device adress supplied, trying to find first\n";
			protocol_obj = std::make_unique<cSerial>();
		}
		protocol_obj = std::make_unique<cSerial>(addr);
	}
	if (type == PROTOCOLENUM::VISAUSB)
	{
		if (addr.length() == 0)
		{
			std::wcout << L"No device adress supplied, trying to find first\n";
			protocol_obj = std::make_unique<cVisausb>();
		}
		protocol_obj = std::make_unique<cVisausb>(addr);
	}
	if (type == PROTOCOLENUM::VISATCP)
	{
		if (addr.length() == 0)
		{
			std::wcout << L"No device adress supplied, trying to find first\n";
			protocol_obj = std::make_unique<cTcp>();
		}
		protocol_obj = std::make_unique<cTcp>(addr);
	}
	return std::move(protocol_obj);
}
