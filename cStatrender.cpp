/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#include "cStatrender.h"
#include "cSignalTable.h"

cStatrender::cStatrender(cPlot* m_plot, const short disp_freq, cSignalTable* signal_table) : wxTimer()
{
	std::cout << "cStatrender ctor...\n";
	m_plot_ = m_plot;
	wxTimer::Start(disp_freq);
}

void cStatrender::add_freq_footer(cFooter* m_footer)
{
	m_footer_ = m_footer;
}

void cStatrender::Notify()
{
	m_plot_->update_chan_statistic_labels();
}




