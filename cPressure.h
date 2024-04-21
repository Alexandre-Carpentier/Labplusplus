#ifndef _PRESSURE_H_
#define _PRESSURE_H_

#include <winsock2.h> 
#include <wx/wx.h>
#include <wx/combobox.h>
#include <wx/treectrl.h>
#include <string>
#include <wx/fileconf.h>
#include "data_types.h"

#include "cTable.h"

static wxImage pace_img;

class cMeasurementmanager;
class cCycle;
class cUsb6001;
class cDaqsim;
class cMeasurement;
class cPlot;
class cImagePanel;

static wxPanel* pace_rightpanel_ = nullptr;

class cPressure : public wxFrame
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
	cTable* m_table_ = nullptr;
	wxStaticBoxSizer* device_group_sizer;
	cMeasurementmanager* meas_manager = nullptr; // Measurement manager singleton
	cMeasurement* m_pressure_ = nullptr; // daq measurement gui

	bool enable_pan = false;
	wxButton* pressure_controler_activate;

	wxComboBox* addr_ctrl = nullptr;

	// Style
	const float text_size = 1;
	const int STATIC_CTRL_STYLE = wxNO_BORDER | wxALIGN_CENTRE_HORIZONTAL;
	const wxSize static_ctrl_size = wxSize(120, 20);
	const int TEXT_CTRL_STYLE = wxSUNKEN_BORDER;
	const wxSize text_ctrl_size = wxSize(120, 24);
	wxColor* bgcolor = new wxColor(245, 245, 248);

	cPressure(wxWindow* inst);
	~cPressure();

	void RefreshPort();

	void DestroySubsystem();

	void set_table(cTable* m_table);
	void UpdateChannelTable(bool isDisplayed);

	void OnPressureEnableBtn(wxCommandEvent& evt);
	void OnPressureAddrSelBtn(wxCommandEvent& evt);
	void OnPaint(wxPaintEvent& event);
	void EnablePressureChannel(bool isDisplayed);

	void save_current_device_config(int channel_index);
	void load_current_device_config(int channel_index);

	void load_combobox(wxComboBox* combo, wxString str);
	void load_combobox(wxComboBox* combo, double floating);

	wxPanel* get_right_panel();

	CURRENT_DEVICE_CONFIG_STRUCT GetPressureConfigStruct();

};



#endif
