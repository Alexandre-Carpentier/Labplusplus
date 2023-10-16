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

