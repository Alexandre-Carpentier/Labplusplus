/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#ifndef _LOADER_H_
#define _LOADER_H_
#include <memory>
#include <iostream>
#define WIN32_LEAN_AND_MEAN
#define WXUSINGDLL
#pragma comment (lib, "comctl32.lib")
#pragma comment(lib, "comctl32")
#pragma comment(lib, "Rpcrt4")

//#include <winsock2.h>
#include <Windows.h>
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include "types.h"
#include "cDeviceBuilder.h"

// Entrypoint to load custom GUI
extern "C"
{
	__declspec(dllexport)cDevice* PLUGIN_Attach(wxWindow* inst);
	__declspec(dllexport)bool PLUGIN_Dettach();
	__declspec(dllexport)bool PLUGIN_Start();
	__declspec(dllexport)bool PLUGIN_Stop();
}

class cLoader
{
};
#endif