#pragma once
class cRender;
#include <wx/wx.h>
#include "cSingleton.h"
#include "cPlot.h"
#include "cFooter.h"
#include "cGraphrender.h"
#include "cTable.h"
#include "cDaqmx.h"
#include "cVoltage.h"
#include "cVoltageRs.h"
#include "cPressure.h"
#include "cOscope.h"
#include "c6510ui.h"
#include "cConfig.h"
#include "cMeasurementControler.h"

class cObjectmanager :public cSingleton<cObjectmanager>
{
private:
	friend class cSingleton<cObjectmanager>;

	// Gui refs

	wxStatusBar* statusBar_ = nullptr;
	cConfig* m_config_ = nullptr;
	cDaqmx* m_daq_ = nullptr;
	cPressure* m_pressure_ = nullptr;
	cVoltage* m_voltage_ = nullptr;
	cVoltageRs* m_voltagers_ = nullptr;
	cOscope* m_oscope_ = nullptr;
	c6510ui* m_6510_ = nullptr;
	cTable* m_table_ = nullptr;
	cPlot* m_plot_ = nullptr;
	cRender* m_render_ = nullptr;
	cFooter* m_footer_ = nullptr;

public:
	void set_status_bar(wxStatusBar* statusBar);
	void set_config(cConfig* m_config);
	void set_daqmx(cDaqmx* m_daq);
	void set_pressuredevice(cPressure* m_pressure);
	void set_voltagedevice(cVoltage* m_voltage);
	void set_voltagersdevice(cVoltageRs* m_voltagers);
	void set_oscopedevice(cOscope* m_oscope);
	void set_daq6510device(c6510ui* m_6510);
	void set_table(cTable* m_table);
	void set_plot(cPlot* m_plot);
	void set_render(cRender* m_render);
	void set_footer(cFooter* m_footer);

	wxStatusBar* get_status_bar();
	cConfig* get_config();
	cDaqmx* get_daqmx();
	cPressure* get_pressuredevice();
	cVoltage* get_voltagedevice();
	cVoltageRs* get_voltagersdevice();
	cOscope* get_oscopedevice();
	c6510ui* get_6510device();
	cTable* get_table();
	cPlot* get_plot();
	cRender* get_render();
	cFooter* get_footer();
};

