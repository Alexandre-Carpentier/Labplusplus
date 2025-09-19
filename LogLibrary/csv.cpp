#include "csv.h"
#include <iostream>

bool csv::open(std::string filename)
{
	m_file = std::ofstream(filename, std::ios::binary);

	if (!m_file.is_open())
	{
		std::cout << "[!] Error opening file: " << filename << " in  csv mode\n";
		return false;
	}

	std::cout << "[*] Opening file: " << filename << " in  csv mode\n";
	return true;
}

bool csv::add_header(std::string str)
{
	m_file << str << '\n';
	std::cout << "[*] Writing header: " << str << " in  csv mode\n";
	return true;
}

bool csv::add_value(std::string value)
{
	m_file << value.c_str();
	m_file << ';';
	return true;
}

bool csv::new_line()
{
	m_file << '\n';
	return true;
}

bool csv::close()
{
	if (m_file.is_open())
	{
		m_file.close();
	}

	std::cout << "[*] Closing file in  csv mode\n";
	return true;
}
