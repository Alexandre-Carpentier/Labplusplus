#pragma once
#include <iostream>

struct err_struct {
	std::wstring err_msg;
	int err_code;
};

class IProtocol
{
public:
	// Abstract interface
	virtual err_struct init() const noexcept = 0;
	virtual err_struct write(std::wstring scpi) const noexcept = 0;
	virtual err_struct read(std::wstring& scpi) const noexcept = 0;
	virtual err_struct close() const noexcept = 0;

	// Be sure to make derived class destructor callable
	virtual ~IProtocol() {};

	err_struct last_error;
	std::wstring  device_name_;
};
