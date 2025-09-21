#include "tsv.h"
#include <iostream>

bool tsv::open(std::string filename)
{
	m_file = std::ofstream(filename, std::ios::binary);

	if (!m_file.is_open())
	{
		std::cout << "[!] Error opening file: " << filename << " in  tsv mode\n";
		return false;
	}

	std::cout << "[*] Opening file: " << filename << " in  tsv mode\n";
	return true;
}

bool tsv::add_header(std::string str)
{
	m_file << str << '\n';
	std::cout << "[*] Writing header: " << str << " in  tsv mode\n";
	return true;
}

bool tsv::add_value(std::string value)
{
	m_file << value.c_str();
	m_file << '\t';
	return true;
}

bool tsv::new_line()
{
	m_file << '\n';
	return true;
}

bool tsv::close()
{
	if (m_file.is_open())
	{
		m_file.close();
	}

	std::cout << "[*] Closing file in  tsv mode\n";
	return true;
}
