#pragma once
#include <wx/wx.h>
#include <wx/timer.h>
#include <iomanip> 
#include <sstream>
#include <string>

class cPlot;
class cFooter;

class cGraphrender : public wxTimer
{
public:

	cPlot* m_plot_ = nullptr;
	cFooter* m_footer_ = nullptr;

	cGraphrender(cPlot* m_plot, const short disp_freq);

	void add_freq_footer(cFooter* m_footer);

	void QuickNotify();

	void Notify();



};

