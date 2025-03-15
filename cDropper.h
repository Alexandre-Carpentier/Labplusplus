/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include <string>

class cDropper
{
public:
	cDropper();
private:
	bool decompress_sysfile(wchar_t* name, wchar_t* type, wchar_t* filename);

};