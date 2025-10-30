#include "sim.h"
#include <iostream>

bool sim::open(std::string filename)
{
	std::cout << "[*] Opening file: " << filename << " in  SIM mode\n";
	return true;
}

bool sim::add_header(std::string str)
{
	std::cout << "[*] Writing header: " << str << " in  SIM mode\n";
	return true;
}

bool sim::add_value(std::string value)
{
	std::cout << "[*] Writing new value: " << value << " in  SIM mode\n";
	return true;
}

bool sim::new_line()
{
	std::cout << "[*] Writing new line in  SIM mode\n";
	return true;
}

bool sim::close()
{
	std::cout << "[*] Closing file in  SIM mode\n";
	return true;
}
