/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#ifndef _PROTOCOLFACTORY_H__
#define _PROTOCOLFACTORY_H__
#include <memory>
#include "cProtocol.h"
#include "cUsb.h"
#include "cVisa.h"
#include "cVisausb.h"
#include "cVisatcp.h"
#include "cVisaserial.h"
#include "cCom.h"
#include <string>

enum PROTOCOLENUM { COM, USB, TCP, VISASERIAL, VISAUSB, VISATCP };

class cProtocolFactory
{
public:
	cProtocolFactory() {};
	std::unique_ptr<IProtocol> make(PROTOCOLENUM type, std::wstring addr);
};

#endif
