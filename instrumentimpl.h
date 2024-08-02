/*#ifndef _INSTRIMPL_H_
#define _INSTRIMPL_H_
#include "IInstrument.h"
#include <wx/fileconf.h>
class cMeasurement;
class cMeasurementmanager;
#include "cDeviceMonitor.h"

struct IInstrument::Instrumentimpl {

	wxWindow* Parent_ = nullptr; // ref to parent object
	//wxWindow* inst_;
	// 
	// ptr to right panel
	wxPanel* rightpanel_ = nullptr;

	cMeasurement* m_oscope_ = nullptr;

	// Save table config
	// C:\Users\The Hive\AppData\Roaming
	std::wstring instrument_name_;
	std::string configfile = "";
	wxFileConfig* cfg = nullptr;

	DEVICE_CONFIG_STRUCT label;		// Control label configuration struct in memory
	CURRENT_DEVICE_CONFIG_STRUCT config; // Current selected configuration
	// Signal color map
	float COLORS[3] =
	{
		0.1f, 0.0f, 0.8f
	};

	cDeviceMonitor* devmon_ = nullptr;
	cTable* m_table_ = nullptr;
	wxStaticBoxSizer* device_group_sizer;
	cMeasurementmanager* meas_manager = nullptr; // Measurement manager singleton

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

	Instrumentimpl(wxWindow* parent, std::shared_ptr<cDeviceMonitor> devmon, std::wstring instrument_name);
	void OnPaint(wxPaintEvent& event);
	void OnOscopeAddrSelBtn(wxCommandEvent& evt);
	void OnOscopeEnableBtn(wxCommandEvent& evt);
	void DestroySubsystem();
	void EnableOscopeChannel(bool isDisplayed);
	void RefreshPort();
	void UpdateChannelTable(bool isDisplayed);
	void load_table_values(void);
	void save_table_values(void);
	void load_current_device_config(int channel_index);
	void load_combobox(wxComboBox* combo, wxString str);
	void load_combobox(wxComboBox* combo, double floating);
};
#endif
*/