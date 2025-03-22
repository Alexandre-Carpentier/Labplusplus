/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#ifndef _TYPES_H_
#define _TYPES_H_
#include <string>
#include <memory>

enum SCPIMODE { COM, TCP, USB, DAQMX, VISA };
enum CONTROLTYPE { BUT, TXT, TXTFIELD, SPINBUT };

/*
struct DATAS
{
	size_t buffer_size;
	std::unique_ptr<double> buffer;
	std::string unit;
};
*/
#endif
