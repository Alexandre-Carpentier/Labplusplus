#pragma once
#include <memory>
#include <wx/wx.h>
#include "enum.h"

#include <vector>
#include <Windows.h>
#include <string>

#include "cCycleControler.h"

#include "cPlot.h"
#include "cConfig.h"
#include "cTable.h"
#include "cCycle.h"

#include "cObjectmanager.h"
#include "cMeasurementmanager.h"
#include "cMeasurementControler.h"
#include "cDaqmx.h"
#include "cPressure.h"
#include "cUsb6001.h"
#include "cScaleBtn.h"

#include "data_types.h"
#include "cMain.h"
#include "cDeviceMonitor.h"
/*
class cPlot;
class cConfig;
class cTable;
class cCycle;
class cCycleControler;
class cObjectmanager;
class cMeasurementmanager;
class wxScaleButton;
*/

class cCycleControler;

class cFooter
{
private:
	
public:
	std::shared_ptr<cCycleControler> cycle_controler;
	wxWindow* inst_ = nullptr;
	cPlot* m_plot_ = nullptr;
	cTable* m_table_ = nullptr;
	cConfig* m_config_ = nullptr;

	std::shared_ptr<cMeasurementControler> meas_controler;

	wxPanel* footerpanel = nullptr;
	wxBoxSizer* hfootersizer = nullptr;
	wxButton* startbtn = nullptr;
	wxScaleButton* scale_btn = nullptr;
	wxStaticText* staticratetxt = nullptr;
	wxTextCtrl* ratetxt = nullptr;

	wxComboBox* combo1 = nullptr;
	wxComboBox* combo2 = nullptr;
	wxTextCtrl* freq = nullptr;

	cObjectmanager* obj_manager = nullptr;
	cMeasurementmanager* meas_manager = nullptr;


	cFooter(wxWindow* inst, cPlot* m_plot, cTable* m_table, cConfig* m_config);

	wxBoxSizer* GetSizer();

	void startButtonClicked(wxCommandEvent& evt);

	void filterButtonClicked(wxCommandEvent& evt);

	void freqButtonClicked(wxCommandEvent& evt);

};


