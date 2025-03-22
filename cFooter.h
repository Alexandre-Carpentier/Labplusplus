/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#pragma once
class cTable;
class cConfig;
class cMeasurementControler;
class cCycleControler;
class cMeasurementmanager;
class cDeviceMonitor;

#include <wx/wx.h>


#include "cScaleBtn.h"

/*
#include <vector>
#include <Windows.h>

#include "cPlot.h"
#include "cConfig.h"
#include "cTable.h"
#include "cCycle.h"
#include "cGnuplot.h"

#include "cObjectmanager.h"
#include "cMeasurementmanager.h"
#include "cMeasurementControler.h"
#include "cDaqmx.h"
#include "cPressure.h"
#include "cUsb6001.h"

#include "data_types.h"
#include "cMain.h"
#include "cDeviceMonitor.h"

#include "enum.h"
*/
/*
class cPlot;


class cCycle;

class cObjectmanager;

class wxScaleButton;

*/

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

	wxComboBox* combo2 = nullptr;
	wxTextCtrl* freq = nullptr;

	cObjectmanager* obj_manager = nullptr;
	cMeasurementmanager* meas_manager = nullptr;


	cFooter(wxWindow* inst, cPlot* m_plot, cTable* m_table, cConfig* m_config, std::shared_ptr <cDeviceMonitor> devmon);

	wxBoxSizer* GetSizer();

	void startButtonClicked(wxCommandEvent& evt);

	void freqButtonClicked(wxCommandEvent& evt);

};


