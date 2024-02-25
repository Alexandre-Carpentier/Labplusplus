#ifndef _PROTOCOLFACTORY_H__
#define _PROTOCOLFACTORY_H__
#include <memory>
#include "cProtocol.h"
#include "cVisa.h"
#include "cTcp.h"
#include "cSerial.h"
#include "cCom.h"
#include <string>

enum PROTOCOLENUM { COM, USB, TCP, VISASERIAL, VISATCP };

class cProtocolFactory
{
public:
	cProtocolFactory() {};
	std::unique_ptr<cProtocol> make(PROTOCOLENUM type, std::wstring addr);
};

#endif
