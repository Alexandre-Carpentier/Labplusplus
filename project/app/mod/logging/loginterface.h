#pragma once
#include <iostream>
#include <string>

///////////////////////////////////////////////////////////////////////////
/// LOGGER INTERFACE CLASS
///////////////////////////////////////////////////////////////////////////

class dolog
{
public:
	virtual ~dolog() = default;
	virtual bool open(std::string filename) = 0;
	virtual bool add_header(std::string str) = 0;
	virtual bool add_value(std::string value) = 0;
	virtual bool new_line() = 0;
	virtual bool close() = 0;
};