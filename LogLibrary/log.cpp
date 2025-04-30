#include "log.h"
#include "loginterface.h"
#include "none.h"
#include "sim.h"

#include <iostream>

class log_factory
{
public:
	std::list<std::unique_ptr<dolog>> create(std::list<LOGTYPE> types)
	{
		for (auto type : types)
		{
			switch (type)
			{
			case LOGTYPE::NONE:
				std::cout << "[*] NONE logger selected\n";
				loggers.emplace_back(std::move(std::make_unique<none>()));
				break;
			case LOGTYPE::SIM:
				std::cout << "[*] SIM logger selected\n";
				loggers.emplace_back(std::move(std::make_unique<sim>()));
				break;
			default:
				std::cout << "[!] Unknown logger type selected\n";
			}
		}
		return std::move(loggers);
	}
private:
	std::list<std::unique_ptr<dolog>> loggers;
};

///////////////////////////////////////////////////////////////////////////
/// IMPLEMENTATION CLASS
///////////////////////////////////////////////////////////////////////////

class cLog::logimpl
{
public:
	bool set(std::list<LOGTYPE> types, std::string filename)
	{
		loggers = factory.create(types);
		if (loggers.size() == 0)
		{
			std::cout << "[!] No loggers created.\n";
			return false;
		}

		std::cout << "[*] loggers created with success...\n";
		return true;
	}

	bool create(std::string filename)
	{

		return true;
	}

	bool add_header(std::string str)
	{
		return true;
	}

	bool add_value(std::string value)
	{
		return true;
	}

	bool new_line()
	{
		return true;
	}


private:
	log_factory factory;
	std::list<std::unique_ptr<dolog>> loggers;
};

///////////////////////////////////////////////////////////////////////////
/// FORWARD CLASS
///////////////////////////////////////////////////////////////////////////

// Setup the implementation 
cLog::cLog()
{
	std::cout << "log ctor...\n";
	pimpl = std::make_unique<logimpl>();
}

// forward the list of loggers to create from the implementation
bool cLog::set(std::list<LOGTYPE> types, std::string filename)
{
	if (!pimpl->set(types, filename))
	{
		std::cout << "[!] Failed to set the implementation.\n";
		return false;
	}
	std::cout << "[*] Log implementation success...\n";
	return true;
}

bool cLog::create(std::string filename)
{
	pimpl->create(filename);
	return true;
}

bool cLog::add_header(std::string str)
{
	pimpl->add_header(str);
	return true;
}

bool cLog::add_value(std::string value)
{
	pimpl->add_value(value);
	return true;
}

bool cLog::new_line()
{
	pimpl->new_line();
	return true;
}
