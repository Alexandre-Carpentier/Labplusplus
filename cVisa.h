/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#ifndef _VISA_H__
#define _VISA_H__
#pragma comment (lib, "visa32.lib") 
#include <visa.h> 
#include <locale>
#include <codecvt>
#include <iostream>
#include <vector>
#include <cassert>

#include "cProtocol.h"
#include "encoding.h"

// A Visa class is a protocol
class cVisa : public IProtocol {
public:
	cVisa();
	err_struct init() override;
	err_struct write(std::wstring scpi);
	err_struct read(std::wstring& scpi);
	err_struct close()override;

	err_struct last_error;

	ViSession ressource_manager = 0;
	ViSession device_ = 0;
	ViStatus status = 0;
};

#endif