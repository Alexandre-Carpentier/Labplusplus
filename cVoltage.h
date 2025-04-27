/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#ifndef _VOLTAGE_H_
#define _VOLTAGE_H_
#include <wx/wx.h>
#include "data_types.h"
class cTable;
class cMeasurementmanager;
class cMeasurement;
class cDeviceMonitor;

static wxImage voltage_instrument_img;
static wxPanel* voltage_instrument_rightpanel_ = nullptr;

class cVoltage : public wxFrame
{
private:
	DEVICE_CONFIG_STRUCT label;		// Control label configuration struct in memory
	CURRENT_DEVICE_CONFIG_STRUCT config; // Current selected configuration
public:
	// Signal color map
	float COLORS[3] =
	{
		0.1f, 0.0f, 0.8f
	};

	wxWindow* inst_ = nullptr;
	std::shared_ptr <cDeviceMonitor> devmon_ = nullptr;
	cTable* m_table_ = nullptr;
	wxStaticBoxSizer* device_group_sizer;
	cMeasurementmanager* meas_manager = nullptr; // Measurement manager singleton
	cMeasurement* m_voltage_ = nullptr; // daq measurement gui

	bool enable_pan = false;
	wxButton* voltage_controler_activate;

	wxComboBox* addr_ctrl = nullptr;

	// Style
	const float text_size = 1;
	const int STATIC_CTRL_STYLE = wxNO_BORDER | wxALIGN_CENTRE_HORIZONTAL;
	const wxSize static_ctrl_size = wxSize(120, 20);
	const int TEXT_CTRL_STYLE = wxSUNKEN_BORDER;
	const wxSize text_ctrl_size = wxSize(120, 24);
	wxColor* bgcolor = new wxColor(245, 245, 248);

	cVoltage(wxWindow* inst, std::shared_ptr <cDeviceMonitor> devmon);
	~cVoltage();

	void RefreshPort();

	void DestroySubsystem();

	void set_table(cTable* m_table);
	void UpdateChannelTable(bool isDisplayed);

	void OnVoltageEnableBtn(wxCommandEvent& evt);
	void OnVoltageAddrSelBtn(wxCommandEvent& evt);
	void OnPaint(wxPaintEvent& event);
	void EnableVoltageChannel(bool isDisplayed);

	void save_current_device_config(int channel_index);
	void load_current_device_config(int channel_index);

	void load_combobox(wxComboBox* combo, wxString str);
	void load_combobox(wxComboBox* combo, double floating);

	wxPanel* get_right_panel();

	CURRENT_DEVICE_CONFIG_STRUCT *GetVoltageConfigStruct();

};



#endif
