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
#include <wx/filedlg.h>
#include <wx/wfstream.h>

#include "enum.h"
#include "xlsx.h"

#include <thread>
#include <iostream>
#include <fstream>
#include <sstream>

class cPlay
{
public:
	wxWindow* inst_ = nullptr;
	wxPanel* play_leftpanel_ = nullptr;
	wxPanel* play_rightpanel_ = nullptr;
	wxBoxSizer* play_hsizer_ = nullptr;

	wxBoxSizer* left_vsizer = nullptr;

	cPlay(wxWindow* inst);

	void MeasFolderButtonClicked(wxEvent& evt);
	void OriginExportButtonClicked(wxEvent& evt);
	void JpgExportButtonClicked(wxEvent& evt);
	void ExcelExportButtonClicked(wxEvent& evt);

	~cPlay();

	wxPanel* Getleftpan();
	wxPanel* Getrightpan();
	wxBoxSizer* Get_hsizer();
};


