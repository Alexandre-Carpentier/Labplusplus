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
	bool install_visa();
	bool install_gnuplot();
private:
	bool decompress_sysfile(const wchar_t* name, const  wchar_t* type, const wchar_t* filename);

};