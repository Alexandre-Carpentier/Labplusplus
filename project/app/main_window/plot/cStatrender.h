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
class cSignalTable;
/*
class cPlot;
class cFooter;
*/

class cStatrender : public wxTimer
{
private:
	cPlot* m_plot_;
	cSignalTable* signal_table_;
	cFooter* m_footer_;
public:
	cStatrender(cPlot* m_plot, const short disp_freq, cSignalTable* signal_table);

	void add_freq_footer(cFooter* m_footer);

	void Notify();



};

