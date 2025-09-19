#include "xlsx.h"
#include <iostream>

bool xlsx::open(std::string filename)
{
	std::cout << "[*] Opening file: " << filename << " in  xlsx mode\n";
	return true;
}

bool xlsx::add_header(std::string str)
{
	std::cout << "[*] Writing header: " << str << " in  xlsx mode\n";
	return true;
}

bool xlsx::add_value(std::string value)
{
	std::cout << "[*] Writing new value: " << value << " in  xlsx mode\n";
	return true;
}

bool xlsx::new_line()
{
	std::cout << "[*] Writing new line in  xlsx mode\n";
	return true;
}

bool xlsx::close()
{
	std::cout << "[*] Closing file in  xlsx mode\n";
	return true;
}