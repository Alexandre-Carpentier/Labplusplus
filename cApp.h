#pragma once

#include <wx/wx.h>
#include "cMain.h"
#include <cassert>

class cApp :public wxApp
{
public:
	virtual bool OnInit();

private:
	cMain* m_Frame = nullptr;
};

