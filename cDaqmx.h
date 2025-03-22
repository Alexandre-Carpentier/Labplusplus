/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "data_types.h"
#include <wx/treectrl.h>

class cImagePanel;
class cMeasurementmanager;
class cCycle;
class cUsb6001;
class cDaqsim;
class cMeasurement;
class cTable;
class cPlot;
class cImagePanel;
class cSignalTable;

static wxImage temp_img;

static wxPanel* config_rightpanel_ = nullptr;
static wxTreeCtrl* config_tree_ctrl = nullptr;

class cDaqmx : public wxFrame
{
private:
	
	cImagePanel* InstrImg = nullptr;
	DEVICE_CONFIG_STRUCT label;		// Control label configuration struct in memory
	CURRENT_DEVICE_CONFIG_STRUCT config; // Current selected configuration
	void set_chan_mode(int access);
	void set_chan_mode(int access, int chan_number);
public:
	// Signal color map
	float COLORS[32][3] =
	{
		{0.5f, 0.15f, 0.15f},
		{0.1f, 0.15f, 0.15f},
		{1.0f, 0.15f, 0.15f},
		{0.1f, 0.5f, 0.15f},
		{0.1f, 0.5f, 0.9f},
		{0.9f, 0.5f, 0.15f},
		{0.1f, 0.15f, 0.5f},
		{0.1f, 0.9f, 0.15f},
		{0.9f, 0.15f, 0.15f},
		{0.1f, 0.15f, 0.15f},
		{0.1f, 0.4f, 0.15f},
		{0.1f, 0.15f, 0.15f},
		{0.1f, 0.7f, 0.15f},
		{0.1f, 0.15f, 0.15f},
		{0.8f, 0.15f, 0.15f},
		{0.1f, 0.8f, 0.8f},
		{0.1f, 0.7f, 0.8f},
		{0.1f, 0.6f, 0.8f},
		{0.1f, 0.5f, 0.8f},
		{0.1f, 0.4f, 0.8f},
		{0.1f, 0.3f, 0.8f},
		{0.1f, 0.2f, 0.8f},
		{0.1f, 0.1f, 0.8f},
		{0.1f, 0.0f, 0.8f}
	};

	wxWindow* inst_ = nullptr;
	cTable* m_table_ = nullptr;

	cMeasurementmanager* meas_manager = nullptr; // Measurement manager singleton
	cMeasurement* m_daq_ = nullptr; // daq measurement gui

	wxGridSizer* chan_grid = nullptr;
	wxStaticBoxSizer* device_group_sizer = nullptr;
	wxStaticBoxSizer* channel_group_sizer = nullptr;
	wxStaticBoxSizer* channel_linearize_group_sizer = nullptr;
	wxStaticBoxSizer* channel_signal_group_sizer = nullptr;

	bool enable_pan = 0;
	wxButton* daq_activate;

	wxComboBox* addr_ctrl = nullptr;

	wxStaticText* static_device_sn = nullptr;
	wxComboBox* device_sn = nullptr;

	//wxComboBox* max_ctrl = nullptr;
	//wxComboBox* min_ctrl = nullptr;
	//wxComboBox* mode_ctrl = nullptr;

	wxButton* previous_chan = nullptr;
	wxButton* next_chan = nullptr;

	// enable
	// Name to assign
	// physical channel
	// 
	// Max input range
	// Min input range
	// Input mode type
	wxStaticText* staticenablechan;
	wxButton* checkchan;
	wxStaticText* static_chan_name;
	wxTextCtrl* chan_name;
	wxStaticText* staticchan;
	wxComboBox* chan_addr_ctrl;

	wxStaticText* statictype;
	wxComboBox* meas_type_ctrl;

	// Voltage param
	wxStaticText* static_chan_max_input_range;
	std::string str_max_input_range;
	wxComboBox* chan_max_input_range;
	wxStaticText* static_chan_min_input_range;
	std::string str_min_input_range;
	wxComboBox* chan_min_input_range;
	wxStaticText* static_chan_input_mode_type;
	std::string str_input_mode_type;
	wxComboBox* chan_input_mode_type;

	// Tc param
	wxStaticText* static_chan_tc_type;
	std::string str_tc_type;
	wxComboBox* chan_tc_type;
	wxStaticText* static_chan_tc_min_range;
	std::string str_tc_min_range;
	wxTextCtrl* chan_tc_min_range;
	wxStaticText* static_chan_tc_max_range;
	std::string str_tc_max_range;
	wxTextCtrl* chan_tc_max_range;

	// Digital param
	wxStaticText *static_digitaltype = nullptr;
	wxComboBox *digitaltype = nullptr;
	wxStaticText* static_digitalmode = nullptr;
	wxComboBox* digitalmode = nullptr;

	wxFlexGridSizer* flexchansizer1;

	// linearize
	// slope linear
	// shift linear
	// unit
	// 
	// filter
	// filter intensity
	wxString scale_file_name = "Lab++ScalePreset.ini";
	wxStaticBox* channel_linearize_group = nullptr;
	wxStaticText* static_chan_scale;
	std::string str_scale;
	wxComboBox* chan_scale;
	wxStaticText* static_chan_slope;
	std::string str_slope;
	wxTextCtrl* chan_slope;
	wxStaticText* static_chan_shift;
	std::string str_shift;
	wxTextCtrl* chan_shift;
	wxStaticText* static_chan_unit;
	std::string str_unit;
	wxTextCtrl* chan_unit;
	wxStaticText* static_channel_sn = nullptr;
	wxComboBox* channel_sn = nullptr;

	wxStaticText* static_chan_filter;
	std::string str_filter;
	wxComboBox* chan_filter;
	wxStaticText* static_chan_filter_intensity;
	std::string str_filter_intensity;
	wxTextCtrl* chan_filter_intensity;

	wxFlexGridSizer* flexchansizer2;

	// trigger
	// threshold
	wxStaticBox* channel_signal_group = nullptr;
	wxStaticText* static_chan_trigger;
	std::string str_trigger;
	wxComboBox* chan_trigger;
	wxStaticText* static_chan_trigger_threshold;
	std::string str_trigger_threshold;
	wxTextCtrl* chan_trigger_threshold;

	wxFlexGridSizer* flexchansizer3;

	wxBoxSizer* chan_vsizer;

	wxBoxSizer* btn_box_sizer[max_chan_number];
	wxBoxSizer* daqinfo_v_sizer = nullptr;
	wxTextCtrl* daq_type = nullptr;
	wxButton* chanbtntype[max_chan_number];
	wxButton* chanbtn[max_chan_number];

	// Style
	const float text_size = 1;
	const int STATIC_CTRL_STYLE = wxNO_BORDER | wxALIGN_CENTRE_HORIZONTAL;
	const wxSize static_ctrl_size = wxSize(80, 20);
	const int TEXT_CTRL_STYLE = wxSUNKEN_BORDER;
	const wxSize text_ctrl_size = wxSize(110, 24);
	wxColor* bgcolor = new wxColor(245, 245, 248);

	void serialize(std::string device);
	void deserialize(std::string device);

	cDaqmx(wxWindow* inst);
	~cDaqmx();

	void OnPaint(wxPaintEvent& event);

	void show_right_panel(bool show);
	void show_channel(const int index, bool show);
	void show_channel(bool show);
	void show_next_channel();
	void OnNextCliqued(wxCommandEvent& evt);
	void show_previous_channel();
	void OnPreviousCliqued(wxCommandEvent& evt);

	void save_current_device_config(int channel_index);
	void load_current_device_config(int channel_index);
	void save_current_chan_config(int channel_index);
	void load_current_chan_config(int channel_index);

	void load_combobox(wxComboBox* combo, wxString str);
	void load_combobox(wxComboBox* combo, double floating);

	wxArrayString LoadScalePresetArray(wxString filename);
	//void EnableChanProperties();
	void OnDaqDigitalChanTypeModified(wxCommandEvent& evt);
	void OnDaqDigitalChanModeModified(wxCommandEvent& evt);

	void OnDaqChanTypeModified(wxCommandEvent& evt);
	void OnDaqChanNameModified(wxCommandEvent& evt);
	void OnDaqChanPhysicalAddressNameModified(wxCommandEvent& evt);
	void OnDaqEnableBtn(wxCommandEvent& evt);
	void OnDaqAddrSelBtn(wxCommandEvent& evt);
	void OnDaqScaleSelBtn(wxCommandEvent& evt);
	void DaqChanAllOn(bool enable);
	void display_channel_as(int access);
	void OnDaqChanEnableBtn(wxCommandEvent& evt);
	void OnChannelBtnNumberCliqued(wxCommandEvent& evt);
	bool isDeviceMeasurable(std::string dev_name);

	void DoChannelUpdate(bool isDisplayed);
	void SwitchChannelON(bool isDisplayed);
	void UpdateChannelSig(bool isDisplayed);
	void AddControlColomnTable(bool isDisplayed);
	void UpdateChannelTable(bool isDisplayed);

	void EnableChannelItems(bool isDisplayed);
	void SwitchChannelColor(bool isDisplayed);
	void show_voltage_param(bool show);
	void show_tc_param(bool show);
	void reload_current_channel_type();

	void set_table(cTable* m_table);

	wxPanel* get_right_panel();
	size_t get_channel_index();
	int GetChannelNumber();
	std::vector <bool> GetChannelEnabledVector();
	CURRENT_DEVICE_CONFIG_STRUCT GetDaqConfigStruct();
};

