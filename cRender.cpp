#include "cRender.h"

#include "cPlot.h"
#include "cFooter.h"

cRender::cRender(cPlot* m_plot, const short disp_freq) : wxTimer()
{
	std::cout << "cRender ctor...\n";
	m_plot_ = m_plot;

	wxTimer::Start(disp_freq);
}

void cRender::add_freq_footer(cFooter* m_footer)
{
	m_footer_ = m_footer;
}

void cRender::Notify()
{
	m_plot_->SizeGraph();
	m_plot_->RenderGraph();
	m_plot_->update_chan_statistic_labels();
}




