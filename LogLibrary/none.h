#pragma once
#include "loginterface.h"

class none : public dolog
{
public:
	virtual ~none() {};
	virtual bool open(std::string filename) override;
	virtual bool add_header(std::string str) override;
	virtual bool add_value(std::string value)override;
	virtual bool new_line() override;
	virtual bool close() override;
};