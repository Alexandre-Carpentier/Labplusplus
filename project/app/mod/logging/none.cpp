#pragma once
#include "none.h"
#include <iostream>

bool none::open(std::string filename)
{
	return true;
}

bool none::add_header(std::string str)
{
	return true;
}

bool none::add_value(std::string value)
{
	return true;
}

bool none::new_line()
{
	return true;
}

bool none::close()
{
	return true;
}
