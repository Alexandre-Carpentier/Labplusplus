/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include <wx/wx.h>
#include <wx/timer.h>
#include <iomanip> 
#include <sstream>
#include <string>

#include "cPlot.h"
#include "cFooter.h"
/*
class cPlot;
class cFooter;
*/

class cStatrender : public wxTimer
{
public:

	cPlot* m_plot_ = nullptr;
	cFooter* m_footer_ = nullptr;

	cStatrender(cPlot* m_plot, const short disp_freq);

	void add_freq_footer(cFooter* m_footer);

	void Notify();



};

