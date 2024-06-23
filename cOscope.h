#ifndef _OSCOPE_H_
#define _OSCOPE_H_
#include <wx/wx.h>
#include "data_types.h"
class cTable;
class cMeasurementmanager;
class cMeasurement;
class cDeviceMonitor;

/*
#include <winsock2.h>



class cCycle;
class cUsb6001;
class cDaqsim;

class cPlot;
class cImagePanel;
#include "cSupplysim.h"
#include "cSupplyusb.h"
*/

static wxImage oscope_instrument_img;

static wxPanel* oscope_instrument_rightpanel_ = nullptr;

class cOscope : public wxFrame
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
	cDeviceMonitor* devmon_ = nullptr;
	cTable* m_table_ = nullptr;
	wxStaticBoxSizer* device_group_sizer;
	cMeasurementmanager* meas_manager = nullptr; // Measurement manager singleton
	cMeasurement* m_oscope_ = nullptr; // daq measurement gui

	bool enable_pan = false;
	wxButton* oscope_controler_activate;

	wxComboBox* addr_ctrl = nullptr;

	// Style
	const float text_size = 1;
	const int STATIC_CTRL_STYLE = wxNO_BORDER | wxALIGN_CENTRE_HORIZONTAL;
	const wxSize static_ctrl_size = wxSize(120, 20);
	const int TEXT_CTRL_STYLE = wxSUNKEN_BORDER;
	const wxSize text_ctrl_size = wxSize(120, 24);
	wxColor* bgcolor = new wxColor(245, 245, 248);

	cOscope(wxWindow* inst, cDeviceMonitor* devmon);
	~cOscope();

	void RefreshPort();

	void DestroySubsystem();

	void set_table(cTable* m_table);
	void UpdateChannelTable(bool isDisplayed);

	void OnOscopeEnableBtn(wxCommandEvent& evt);
	void OnOscopeAddrSelBtn(wxCommandEvent& evt);
	void OnPaint(wxPaintEvent& event);
	void EnableOscopeChannel(bool isDisplayed);

	void save_current_device_config(int channel_index);
	void load_current_device_config(int channel_index);

	void load_combobox(wxComboBox* combo, wxString str);
	void load_combobox(wxComboBox* combo, double floating);

	wxPanel* get_right_panel();

	CURRENT_DEVICE_CONFIG_STRUCT GetOscopeConfigStruct();
};
#endif
