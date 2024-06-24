#ifndef _COM_H__
#define _COM_H__
#include <Windows.h>
#include <locale>
#include <codecvt>
#include <iostream>
#include <vector>
#include <cassert>
#include "cProtocol.h"

// A Com class is a protocol
class cCom : public IProtocol {
public:
	cCom();
	cCom(std::wstring addr);
	virtual err_struct init();
	err_struct write(std::wstring scpi);
	err_struct read(std::wstring& scpi);
	virtual err_struct close();

	err_struct last_error;

private:
	HANDLE hCom;
	DCB dcbSerialParams;

	std::string instrument_name;
};

#endif