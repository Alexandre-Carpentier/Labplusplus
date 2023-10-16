#ifndef _LOADER_H_
#define _LOADER_H_
#include <memory>
#include <iostream>
#define WIN32_LEAN_AND_MEAN
#define WXUSINGDLL
//#include <winsock2.h>
#include <Windows.h>
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include "types.h"
#include "cDeviceBuilder.h"
class cDevice;

// Entrypoint to load custom GUI
extern "C" __declspec(dllexport)cDevice * Attach(wxWindow * inst);

class cLoader
{
};


#endif