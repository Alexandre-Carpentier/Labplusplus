#include "cObjectmanager.h"

#include <wx/wx.h>
//#include "cSingleton.h"

#include "cPlot.h"
#include "cFooter.h"
#include "cGraphrender.h"
#include "cTable.h"
#include "cConfig.h"
#include "cMeasurementControler.h"

void cObjectmanager::set_status_bar(wxStatusBar* statusBar)
{
	statusBar_ = statusBar;
}

void cObjectmanager::set_config(cConfig* m_config)
{
	m_config_ = m_config;
}

void cObjectmanager::set_daqmx(cDaqmx* m_daq)
{
	m_daq_ = m_daq;
}

void cObjectmanager::set_pressuredevice(cPressure* m_pressure)
{
	m_pressure_ = m_pressure;
}

void cObjectmanager::set_table(cTable* m_table)
{
	m_table_ = m_table;
}

void cObjectmanager::set_plot(cPlot* m_plot)
{
	m_plot_ = m_plot;
}

void cObjectmanager::set_render(cRender* m_render)
{
	m_render_ = m_render;
}

void cObjectmanager::set_footer(cFooter* m_footer)
{
	m_footer_ = m_footer;
}

wxStatusBar* cObjectmanager::get_status_bar()
{
	return statusBar_;
}

cConfig* cObjectmanager::get_config()
{
	return m_config_;
}

cDaqmx* cObjectmanager::get_daqmx()
{
	return m_daq_;
}

cPressure* cObjectmanager::get_pressuredevice()
{
	return m_pressure_;
}

cTable* cObjectmanager::get_table()
{
	return m_table_;
}

cPlot* cObjectmanager::get_plot()
{
	return m_plot_;
}

cRender* cObjectmanager::get_render()
{
	return m_render_;
}

cFooter* cObjectmanager::get_footer()
{
	return m_footer_;
}

