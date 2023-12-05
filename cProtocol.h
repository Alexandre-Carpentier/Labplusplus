#pragma once
#include <iostream>
struct err_struct {
	std::wstring err_msg;
	int err_code;
};

class cProtocol
{
public:
	// Abstract interface
	virtual err_struct init() = 0;
	virtual err_struct write(std::wstring scpi) = 0;
	virtual err_struct read(std::wstring& scpi) = 0;
	virtual err_struct close() = 0;

	// Be sure to make derived class destructor callable
	virtual ~cProtocol() {};

	err_struct last_error;
	std::wstring  device_name_;
};
