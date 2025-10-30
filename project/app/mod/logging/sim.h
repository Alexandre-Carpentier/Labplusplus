#pragma once
#include "loginterface.h"

class sim: public dolog
{
public:
	virtual ~sim() {};
	virtual bool open(std::string filename) override;
	virtual bool add_header(std::string str) override;
	virtual bool add_value(std::string value)override;
	virtual bool new_line() override;
	virtual bool close() override;
};