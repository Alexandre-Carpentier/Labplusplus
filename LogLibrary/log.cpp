#include "log.h"
#include "loginterface.h"
#include "none.h"
#include "sim.h"
#include "csv.h"
#include "tdms.h"
#include "xlsx.h"

#include <iostream>
#include <print>
#include <chrono>

///////////////////////////////////////////////////////////////////////////
/// BUILD THE SELECTED LOGGER
///////////////////////////////////////////////////////////////////////////
class log_factory
{
public:
	std::unique_ptr<dolog> create(LOGTYPE type)
	{
			switch (type)
			{
			case LOGTYPE::NONE:
				std::println("[*] log_factory: NONE logger selected");
				return std::make_unique<none>();
			case LOGTYPE::SIM:
				std::println("[*] log_factory: SIM logger selected");
				return std::make_unique<sim>();
			case LOGTYPE::CSV:
				std::println("[*] log_factory: CSV logger selected");
				return std::make_unique<csv>();
			case LOGTYPE::TDMS:
				std::println("[*] log_factory: TDMS logger selected");
				return std::make_unique<tdms>();
			case LOGTYPE::XLSX:
				std::println("[*] log_factory: XLSX logger selected");
				return std::make_unique<xlsx>();
			default:
				std::println("\x1b[31m[!] log_factory: Unknown logger type selected\x1b[0m");
				break;
			}		
			return nullptr;
	}
};

///////////////////////////////////////////////////////////////////////////
/// IMPLEMENTATION CLASS
///////////////////////////////////////////////////////////////////////////

class cLog::logimpl
{
public:
	bool set(LOGTYPE type, std::string filename)
	{
		log_factory factory;
		logger = factory.create(type);
		if (logger == nullptr)
		{
			std::println("\x1b[31m[!] Logimpl: no logger created.\x1b[0m");
			return false;
		}

		std::println("[*] Logimpl: logger set with success...");

		if(filename.size() == 0)
		{
			std::println("\x1b[31m[!] Logimpl: filename is empty.\x1b[0m");
			return false;
		}

		if (filename.size() > 260)
		{
			std::println("\x1b[31m[!] Logimpl: filename length is too long.\x1b[0m");
			return false;
		}

		m_filename = filename;
		return true;
	}

	bool isReady()
	{
		return isOk;
	}

	void isReady(bool isReady)
	{
		isOk = isReady;
	}

	bool create()
	{
		if (logger->open(m_filename) == false)
		{
			std::println("\x1b[31m[!] Failed to open logger: {}\x1b[0m", m_filename);
				return false;
		}
		return true;
	}

	bool add_header(std::string str)
	{
		if (logger->add_header(str) == false)
		{
			std::println("\x1b[31m[!] Failed to add logger header: {}\x1b[0m", str);
			return false;
		}
		return true;
	}

	bool add_value(std::string value)
	{
		if (logger->add_value(value) == false)
		{
			std::println("\x1b[31m[!] Failed to add logger value: {}\x1b[0m", value);
			return false;
		}
		return true;
	}

	bool new_line()
	{
		if (logger->new_line() == false)
		{
			std::println("\x1b[31m[!] Failed to feed new line\x1b[0m");
			return false;
		}
		return true;
	}

	std::string get_filename()
	{
		return m_filename;
	}

	bool close()
	{
		if (logger->close() == false)
		{
			std::println("\x1b[31m[!] Failed close the logger\x1b[0m");
			return false;
		}
		return true;
	}

private:
	std::unique_ptr<dolog> logger;
	std::string m_filename;
	bool isOk;
};

///////////////////////////////////////////////////////////////////////////
/// FORWARD CLASS
///////////////////////////////////////////////////////////////////////////

// Setup the implementation 
cLog::cLog() :pimpl(std::make_unique<logimpl>())
{
	pimpl->isReady(false);
	std::println("[*] Log ctor");
}

cLog::cLog(LOGTYPE type, std::string filename) :pimpl(std::make_unique<logimpl>())
{
	std::println("[*] Log ctor");
	if (!pimpl->set(type, filename))
	{
		std::println("\x1b[31m[!] Failed to set the implementation.\x1b[0m\n");
		pimpl->isReady(false);
		return;
	}
	std::println("[*] Log implementation success...");
	pimpl->isReady(true);
}

cLog::~cLog()
{
	std::println("[*] Log dtor");
}

bool cLog::set(LOGTYPE type, std::string filename)
{
	if (!pimpl->set(type, filename))
	{
		std::println("\x1b[31m[!] Failed to set the implementation.\x1b[0m");
		pimpl->isReady(false);
		return false;
	}
	std::cout << "[*] Log implementation success...\n";
	pimpl->isReady(true);
	return true;
}

bool cLog::isReady()
{
	return pimpl->isReady();
}

// Using the implementation
bool cLog::create()
{
	if(pimpl->isReady() == false)
	{
		std::println("\x1b[31m[!] Logger is not ready to create\x1b[0m");
		return false;
	}

	pimpl->create();
	return true;
}

bool cLog::add_header(std::string str)
{
	if (pimpl->isReady() == false)
	{
		std::println("\x1b[31m[!] Logger is not ready to add header\x1b[0m");
		return false;
	}

	pimpl->add_header(str);
	return true;
}

bool cLog::add_value(std::string value)
{
	if (pimpl->isReady() == false)
	{
		std::println("\x1b[31m[!] Logger is not ready to add value\x1b[0m");
		return false;
	}

	pimpl->add_value(value);
	return true;
}

bool cLog::new_line()
{
	if (pimpl->isReady() == false)
	{
		std::println("\x1b[31m[!] Logger is not ready to put new line\x1b[0m");
		return false;
	}

	pimpl->new_line();
	return true;
}

std::string cLog::get_filename()
{
	return pimpl->get_filename();
}

bool cLog::close()
{
	if (pimpl->isReady() == false)
	{
		std::println("\x1b[31m[!] Logger is not ready to close\x1b[0m");
		return false;
	}
	pimpl->close();
	return true;
}