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
