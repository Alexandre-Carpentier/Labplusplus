#pragma once

#include "cMain.h"


class cApp :public wxApp
{
public:
	virtual bool OnInit();

private:
	cMain* m_Frame = nullptr;
};

