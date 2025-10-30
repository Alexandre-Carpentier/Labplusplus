/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>

std::string ConvertWideToANSI(const std::wstring& wstr);
std::wstring ConvertAnsiToWide(const std::string& str);
std::string ConvertWideToUtf8(const std::wstring& wstr);
std::wstring ConvertUtf8ToWide(const std::string& str);
