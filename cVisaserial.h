/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#ifndef _SERIAL_H__
#define _SERIAL_H__
#pragma comment (lib, "visa32.lib") 
#include <visa.h>

#include <format>
#include <string>
#include <iostream>
#include <locale>
#include <codecvt>
#include "cVisa.h" 

// A Tcp class is a Visa protocol
class cSerial : public cVisa {
public:
	cSerial();
	cSerial(std::wstring addr);
	virtual err_struct init() override;
	virtual err_struct close() override;
};

#endif


