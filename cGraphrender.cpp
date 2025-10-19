/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#include "cGraphrender.h"

cGraphrender::cGraphrender(cPlot* m_plot, const short disp_freq) : wxTimer()
{
	std::cout << "cGraphrender ctor...\n";
	m_plot_ = m_plot;

	wxTimer::Start(disp_freq);
}

void cGraphrender::add_freq_footer(cFooter* m_footer)
{
	m_footer_ = m_footer;
}

void cGraphrender::QuickNotify()
{
	//printf("cGraphrender quicknotify !\n");
	m_plot_->SizeGraph();
	m_plot_->RenderGraph();
	
	return;
}

void cGraphrender::Notify()
{

	// Issue on the renderer stack overflow when state go to false
	// suspicious stack overflow from widget above cTable displaying current value
	// suspicious race condition
	//if (m_plot_->get_graph_state() == true)
	//{
		m_plot_->RenderGraph();
		m_plot_->Refresh();
	//}
	
	//m_plot_->update_chan_statistic_labels();

	//printf("cGraphrender notify !\n");
}




