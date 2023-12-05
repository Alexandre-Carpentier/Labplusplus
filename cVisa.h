#ifndef _VISA_H__
#define _VISA_H__
#pragma comment (lib, "visa32.lib") 
#include <visa.h> 
#include <locale>
#include <codecvt>
#include <iostream>
#include <vector>
#include "cProtocol.h"

// A Visa class is a protocol
class cVisa : public cProtocol {
public:
	cVisa();
	err_struct init() override;
	err_struct write(std::wstring scpi);
	err_struct read(std::wstring& scpi);
	err_struct close() override;

	err_struct last_error;

	ViSession ressource_manager;
	ViSession device_;
	ViStatus status = 0;
};

#endif