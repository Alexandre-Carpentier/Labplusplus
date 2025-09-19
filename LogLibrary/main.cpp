#include "main.h"
#include "log.h"
#include <vector> 
#include <print>

#ifdef _WIN32
#ifdef _WIN64
#include "Windows.h"
#endif
#endif

bool run_test()
{
	bool status = true;
	// Single logger test
	std::println("\x1b[32m[*] Single logger test\x1b[0m");
	cLog rec;
	status = rec.set(LOGTYPE::SIM, "test.txt");
	status = rec.add_header("New file created to make measurements");
	status = rec.add_value("T(s)");
	status = rec.add_value("Voltage(V)");
	status = rec.add_value("Current (A)");
	status = rec.new_line();
	status = rec.add_value("0.1");
	status = rec.add_value("3.3");
	status = rec.add_value("0.5");
	status = rec.new_line();
	status = rec.close();

	// Multiple logger test
	std::println("\x1b[32m[*] Multiple logger test\x1b[0m");
	std::vector<cLog*> loggers;	

	cLog none(LOGTYPE::NONE, "test.none");
	cLog sim(LOGTYPE::SIM, "test.sim");
	cLog csv(LOGTYPE::CSV, "test.csv");
	cLog tdms(LOGTYPE::TDMS, "test.tdms");
	cLog xlsx(LOGTYPE::XLSX, "test.xlsx");
	
	loggers.push_back(&none);
	loggers.push_back(&sim);
	loggers.push_back(&csv);
	loggers.push_back(&tdms);
	loggers.push_back(&xlsx);

	for(auto& logger : loggers)
	{
		if (!logger->isReady())
		{
			std::println("\x1b[31m[!] Logger is not ready\x1b[0m");
			status = false;
			continue;
		}
		status = logger->create();
		status = logger->add_header("New file created to make measurements");
		status = logger->add_value("T(s)");
		status = logger->add_value("Voltage(V)");
		status = logger->add_value("Current (A)");
		status = logger->new_line();
		status = logger->add_value("0.1");
		status = logger->add_value("3.3");
		status = logger->add_value("0.5");
		status = logger->new_line();
		status = logger->close();
	}
	return status;
}

int main()
{
	std::println("[*] Running tests");
	if (!run_test())
	{
		std::println("\x1b[31m[*] Test done with failure\x1b[0m"); 
		return -1;
	}

	std::println("\x1b[32m[*] Test done with success\x1b[0m");
	return 0;
}