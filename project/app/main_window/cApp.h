/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#pragma once
/*
#pragma comment (lib, "Advapi32.lib")
#pragma comment (lib, "Gdi32.lib")
#pragma comment (lib, "Winspool.lib")
#pragma comment (lib, "Ole32.lib")

#pragma comment (lib, "Comctl32.lib")
#pragma comment (lib, "Comdlg32.lib")
#pragma comment (lib, "Rpcrt4.lib")
#pragma comment (lib, "Kernel32.lib")
#pragma comment(lib, "SetupAPI.lib")
#pragma comment(lib, "Winusb.lib")
*/
#include <wx\wx.h>
#include "cMain.h"

class cApp :public wxApp
{
public:
	virtual bool OnInit();
private:
	cMain* m_Frame = nullptr;
};

