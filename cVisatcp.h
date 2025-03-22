/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#ifndef _TCP_H__
#define _TCP_H__
#pragma comment (lib, "visa32.lib") 
#include <visa.h> 

#include <iostream>
#include <locale>
#include <codecvt>

#include "encoding.h"
#include "cVisa.h" 

// A Tcp class is a Visa protocol
class cTcp : public cVisa {
public:
	cTcp();
	cTcp(std::wstring addr);
	virtual err_struct init() override;
	virtual err_struct close() override;
};

#endif
