#pragma once
#include "loginterface.h"
#include <string>
#include <fstream>


class tsv : public dolog
{
public:
	virtual ~tsv() {};
	virtual bool open(std::string filename) override;
	virtual bool add_header(std::string str) override;
	virtual bool add_value(std::string value)override;
	virtual bool new_line() override;
	virtual bool close() override;
private:
	std::string m_filename;
	std::ofstream m_file;
};