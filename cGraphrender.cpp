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
	//m_plot_->RenderGraph();
	return;
}

void cGraphrender::Notify()
{

	//m_plot_->SizeGraph();
	m_plot_->RenderGraph();
	//m_plot_->update_chan_statistic_labels();

	//printf("cGraphrender notify !\n");
}




