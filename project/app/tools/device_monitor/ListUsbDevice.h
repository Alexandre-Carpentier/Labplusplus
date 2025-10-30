/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include <windows.h>
#include <tchar.h>

#pragma comment(lib, "Setupapi.lib")
#include <setupapi.h>
#include <initguid.h>

#include <stdio.h>

#include "encoding.h"
#include <vector>
#include <string>


std::vector<std::string> get_USBTMCport_list();