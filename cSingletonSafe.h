/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include <mutex>
class cSingletonSafe 
{
public:
	static cSingletonSafe* getInstance();
private:
	static cSingletonSafe *single_;
	static std::mutex mut;
};
