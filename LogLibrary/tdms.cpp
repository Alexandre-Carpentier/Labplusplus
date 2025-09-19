#include "tdms.h"
#include <iostream>

bool tdms::open(std::string filename)
{
	std::cout << "[*] Opening file: " << filename << " in  tdms mode\n";
	return true;
}

bool tdms::add_header(std::string str)
{
	std::cout << "[*] Writing header: " << str << " in  tdms mode\n";
	return true;
}

bool tdms::add_value(std::string value)
{
	std::cout << "[*] Writing new value: " << value << " in  tdms mode\n";
	return true;
}

bool tdms::new_line()
{
	std::cout << "[*] Writing new line in  tdms mode\n";
	return true;
}

bool tdms::close()
{
	std::cout << "[*] Closing file in  tdms mode\n";
	return true;
}