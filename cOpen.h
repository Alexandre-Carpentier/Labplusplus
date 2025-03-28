/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/grid.h>

#include <iostream>

class cOpen
{
public:
	wxWindow* inst_ = nullptr;
	wxPanel* open_leftpanel_ = nullptr;;
	wxPanel* open_rightpanel_ = nullptr;;
	wxBoxSizer* open_hsizer_ = nullptr;;

	cOpen(wxWindow* inst);
	~cOpen();

	wxPanel* Getleftpan();
	wxPanel* Getrightpan();
	wxBoxSizer* Get_hsizer();
};

