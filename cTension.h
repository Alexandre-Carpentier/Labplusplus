#ifndef _Tension_H_
#define _Tension_H_

#include <winsock2.h> 
#include <wx/wx.h>
#include <wx/combobox.h>
#include <wx/treectrl.h>
#include <string>
#include <wx/fileconf.h>
#include "data_types.h"


class cMeasurementmanager;
class cCycle;
class cUsb6001;
class cDaqsim;
class cMeasurement;
class cPlot;
class cImagePanel;




class cTension : public wxFrame
{
private:
	wxPanel* config_rightpanel_ = nullptr;
	wxStaticBoxSizer* device_group_sizer;
	DEVICE_CONFIG_STRUCT label;		// Control label configuration struct in memory
	CURRENT_DEVICE_CONFIG_STRUCT config; // Current selected configuration

public:
	// Signal color map
	float COLORS[3] =
	{
		0.1f, 0.0f, 0.8f
	};


	wxWindow* inst_ = nullptr;

	cMeasurementmanager* meas_manager = nullptr; // Measurement manager singleton
	cMeasurement* m_Tension_ = nullptr; // daq measurement gui


	bool enable_pan = false;
	wxButton* tension_controler_activate;

	wxComboBox* addr_ctrl = nullptr;

	// Style
	const float text_size = 1;
	const int STATIC_CTRL_STYLE = wxNO_BORDER | wxALIGN_CENTRE_HORIZONTAL;
	const wxSize static_ctrl_size = wxSize(120, 20);
	const int TEXT_CTRL_STYLE = wxSUNKEN_BORDER;
	const wxSize text_ctrl_size = wxSize(120, 24);
	wxColor* bgcolor = new wxColor(245, 245, 248);

	cTension(wxWindow* inst);

	void OnTensionEnableBtn(wxCommandEvent& evt);
	void OnTensionAddrSelBtn(wxCommandEvent& evt);

	void save_current_device_config(int channel_index);
	void load_current_device_config(int channel_index);

	void load_combobox(wxComboBox* combo, wxString str);
	void load_combobox(wxComboBox* combo, double floating);

	wxPanel* get_right_panel();

	CURRENT_DEVICE_CONFIG_STRUCT GetTensionConfigStruct();

};

#endif
