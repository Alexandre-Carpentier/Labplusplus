#pragma once
#include <windows.h>
#include <tchar.h>

#include <setupapi.h>
#include <initguid.h>

#include <stdio.h>

#include "encoding.h"
#include <vector>
#include <string>

std::vector<std::string> get_USBTMCport_list();