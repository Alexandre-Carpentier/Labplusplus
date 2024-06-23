#include "cStatrender.h"

cStatrender::cStatrender(cPlot* m_plot, const short disp_freq) : wxTimer()
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




