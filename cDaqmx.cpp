#include "cDaqmx.h"

#include <wx/wx.h>
#include <wx/combobox.h>
#include <wx/dcbuffer.h>
#include <format>

#include "enum.h"

#include "cMeasurementControler.h"
#include "cMeasurement.h"
#include "cObjectmanager.h"
#include "cMeasurementmanager.h"
#include "cCycle.h"
#include "cUSB6001.h"
#include "cDaqsim.h"
#include "cPlot.h"
#include "cDaqmxScaleDlg.h"
#include "cImagePanel.h"

cDaqmx::cDaqmx(wxWindow* inst)
{
	std::cout << "cDaqmx ctor...\n";
	inst_ = inst;

	////////////////////////////////////////////////////////////
	// Load default labels in memory
	////////////////////////////////////////////////////////////
	label.device_enabled = false;
	label.device_name.push_back("DEV1");
	label.device_name.push_back("DEV2");
	label.device_name.push_back("Simulated");

	label.channel_index = 0;

	label.chan_number = max_chan_number;

	for (int i = 0; i < max_chan_number; i++)
	{
		label.channel_enabled.push_back(false);

		label.channel_name.push_back("Analog");
		label.channel_name.at(i).append(std::to_string(i));

		for (int j = 0; j < max_chan_number; j++)
		{
			label.channel_physical_name[i].push_back("/ai" + std::to_string(j));
		}

		label.channel_type[i].push_back("Voltage");
		label.channel_type[i].push_back("Thermocouple");
		label.channel_type[i].push_back("Thermistance");

		label.channel_max[i].push_back("0V");
		label.channel_max[i].push_back("5V");
		label.channel_max[i].push_back("10V");

		label.channel_min[i].push_back("0V");
		label.channel_min[i].push_back("-5V");
		label.channel_min[i].push_back("-10V");

		label.channel_mode_type[i].push_back("Grounded");
		label.channel_mode_type[i].push_back("Differential");

		label.channel_tc_type[i].push_back("T");
		label.channel_tc_type[i].push_back("K");

		label.channel_tc_min[i] = "-100";							// Tc min temp
		label.channel_tc_max[i] = "200";

		label.channel_linearize[i] = LoadScalePresetArray(scale_file_name);

		label.channel_linearize_slope[i] = "1";
		label.channel_linearize_shift[i] = "0";
		label.channel_linearize_unit[i] = "Volt";

		label.channel_filter[i].push_back("Disabled");
		label.channel_filter[i].push_back("EMA");
		label.channel_filter[i].push_back("Hanning");

		label.channel_filter_intensity[i] = "0.5";

		label.channel_trigger[i].push_back("Disabled");
		label.channel_trigger[i].push_back("Rise");
		label.channel_trigger[i].push_back("Fall");

		label.channel_trigger_threshold[i] = "0";
	}
	////////////////////////////////////////////////////////////
	// Load default configuration in memory
	////////////////////////////////////////////////////////////
	config.device_enabled = false;
	config.device_name = wxT("Simulated");

	config.channel_index = 0;

	config.chan_number = max_chan_number;

	for (int i = 0; i < max_chan_number; i++)
	{
		config.channel_enabled.push_back(false);

		config.channel_name.push_back("Analog");
		config.channel_name.at(i).append(std::to_string(i));

		config.channel_physical_name[i] = "/ai" + std::to_string(i);

		config.channel_type[i] = "Voltage";

		config.channel_max[i] = "10";

		config.channel_min[i] = "0";

		config.channel_mode_type[i] = "Grounded";

		config.channel_linearize[i] = "No";

		config.channel_linearize_slope[i] = "1";
		config.channel_linearize_shift[i] = "0";
		config.channel_linearize_unit[i] = "Volt";

		config.channel_filter[i] = "Disabled";

		config.channel_filter_intensity[i] = "0.5";

		config.channel_trigger[i] = "Disabled";

		config.channel_trigger_threshold[i] = "0";
	}
	////////////////////////////////////////////////////////////

	config_rightpanel_ = new wxPanel(inst, IDC_DAQRIGHT_PAN, wxDefaultPosition, inst->FromDIP(wxSize(600, 600)));
	config_rightpanel_->SetBackgroundColour(wxColor(165, 165, 165)); // Make inside group box dark grey
	config_rightpanel_->Connect(wxEVT_PAINT, wxPaintEventHandler(cDaqmx::OnPaint));

	////////////////////////////////////////////////////////////

	//wxInitAllImageHandlers();
	//image.LoadFile(wxT("multi_function_daq_img600x300.png"), wxBITMAP_TYPE_PNG);
	//image.LoadFile(wxT("IDB_PNG2"), wxBITMAP_TYPE_ANY);

	::wxInitAllImageHandlers();
	wxBitmap bmp("DAQPNG", wxBITMAP_TYPE_PNG_RESOURCE);
	image = bmp.ConvertToImage();

	//wxBitmap imagebmp("IDB_PNG2", wxBITMAP_TYPE_PNG_RESOURCE);
	//image = imagebmp.ConvertToImage();

	wxStaticText* dummy = new wxStaticText(config_rightpanel_, wxID_ANY, "", wxDefaultPosition, inst->FromDIP(wxSize(400, 0)));

	////////////////////////////////////////////////////////////

	wxFlexGridSizer* flexsizer = new wxFlexGridSizer(2, 2, 10, 20);

	////////////////////////////////////////////////////////////

	wxStaticBox* device_group = new wxStaticBox(config_rightpanel_, wxID_ANY, "NiDAQmx Hardware", wxDefaultPosition, inst->FromDIP(wxDefaultSize));
	device_group_sizer = new wxStaticBoxSizer(device_group, wxVERTICAL);

	////////////////////////////////////////////////////////////
	wxStaticText* enabledaq = new wxStaticText(device_group, IDCSTATICENABLEDAQ, L"NI DAQmx:", wxDefaultPosition, inst->FromDIP(static_ctrl_size), wxNO_BORDER | wxALIGN_CENTRE_HORIZONTAL);
	enabledaq->SetFont(enabledaq->GetFont().Scale(text_size));
	enabledaq->SetBackgroundColour(*bgcolor);
	enabledaq->Hide(); // Just use enabledaq to fill the flex sizer -> hide it after creating

	////////////////////////////////////////////////////////////

	daq_activate = new wxButton(device_group, IDCDAQACTIVATE, L"OFF", wxDefaultPosition, inst->FromDIP(wxSize(50, 25)), wxSUNKEN_BORDER);
	inst_->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cDaqmx::OnDaqEnableBtn, this, IDCDAQACTIVATE);
	daq_activate->SetFont(daq_activate->GetFont().Scale(text_size));
	daq_activate->SetBackgroundColour(wxColor(250, 120, 120));

	////////////////////////////////////////////////////////////

	wxStaticText* staticaddr = new wxStaticText(device_group, IDCSTATICADDR, L"Device list:", wxDefaultPosition, inst->FromDIP(static_ctrl_size), wxNO_BORDER | wxALIGN_CENTRE_HORIZONTAL);
	staticaddr->SetFont(staticaddr->GetFont().Scale(text_size));
	//staticaddr->SetBackgroundColour(*bgcolor);

	////////////////////////////////////////////////////////////

	addr_ctrl = new wxComboBox(device_group, IDCADDR, label.device_name[0], wxDefaultPosition, inst->FromDIP(wxDefaultSize), label.device_name, wxCB_READONLY | wxSUNKEN_BORDER | wxBG_STYLE_TRANSPARENT, wxDefaultValidator, _T(""));
	inst_->Bind(wxEVT_COMMAND_COMBOBOX_SELECTED, &cDaqmx::OnDaqAddrSelBtn, this, IDCADDR);
	addr_ctrl->SetFont(addr_ctrl->GetFont().Scale(text_size));
	addr_ctrl->Disable();

	flexsizer->Add(daq_activate);
	flexsizer->Add(enabledaq);
	flexsizer->Add(staticaddr);
	flexsizer->Add(addr_ctrl);
	device_group_sizer->Add(flexsizer);

	wxBoxSizer* btn_hsizer = new wxBoxSizer(wxHORIZONTAL);
	wxButton* previous_chan = new wxButton(config_rightpanel_, IDCPREVIOUS, "Previous", wxDefaultPosition, inst->FromDIP(wxSize(100, 30)), wxRAISED_BORDER);
	previous_chan->SetFont(previous_chan->GetFont().Scale(text_size + 0.5f));
	previous_chan->SetBackgroundColour(wxColor(250, 248, 240));
	inst_->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cDaqmx::OnPreviousCliqued, this, IDCPREVIOUS);

	wxButton* next_chan = new wxButton(config_rightpanel_, IDCNEXT, "Next", wxDefaultPosition, inst->FromDIP(wxSize(100, 30)), wxSUNKEN_BORDER);
	next_chan->SetFont(next_chan->GetFont().Scale(text_size + 0.5f));
	next_chan->SetBackgroundColour(wxColor(250, 248, 240));
	inst_->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cDaqmx::OnNextCliqued, this, IDCNEXT);

	btn_hsizer->Add(previous_chan, 0, wxALL | wxEXPAND, inst->FromDIP(5));
	btn_hsizer->Add(next_chan, 0, wxALL | wxEXPAND, inst->FromDIP(5));

	// Draw btn with channel numbers 0 1 2 3 4 5 ...

	wxGridSizer* chan_grid = new  wxGridSizer(max_chan_number, 0, 0);
	for (int i = 0; i < max_chan_number; i++)
	{
		std::string ind; ind.append(std::to_string(i));
		chanbtn[i] = new wxButton(config_rightpanel_, IDCCHANBTN0 + i, ind, wxDefaultPosition, inst->FromDIP(wxSize(18, 18)), wxSUNKEN_BORDER);
		inst_->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cDaqmx::OnChannelBtnNumberCliqued, this, IDCCHANBTN0 + i);
		chanbtn[i]->SetFont(chanbtn[i]->GetFont().Scale(0.7f));
		chan_grid->Add(chanbtn[i]);
	}
	chanbtn[0]->SetBackgroundColour(wxColor(120, 140, 120));

	//////////////////////////////////////////////////////////////////
	// DAQ chan settings
	//////////////////////////////////////////////////////////////////
		// enable
		// Name to assign
		// physical channel

		// linearize
		// slope linear
		// shift linear
		// unit

		// filter
		// filter type
		// filter intensity

		// Max input range
		// Min input range
		// Input mode type

		// trigger
		// threshold
		// criteria


	flexchansizer1 = new wxFlexGridSizer(14, 2, 10, 20);
	flexchansizer2 = new wxFlexGridSizer(6, 2, 10, 20);
	flexchansizer3 = new wxFlexGridSizer(4, 2, 10, 20);

	wxStaticBox* channel_group = new wxStaticBox(config_rightpanel_, wxID_ANY, "Channel general configuration");
	channel_group_sizer = new wxStaticBoxSizer(channel_group, wxVERTICAL);
	////////////////////////////////////////////////////////////

	staticenablechan = new wxStaticText(channel_group, IDCENABLECHAN, L"Active:", wxDefaultPosition, inst->FromDIP(static_ctrl_size), STATIC_CTRL_STYLE);
	staticenablechan->SetFont(staticenablechan->GetFont().Scale(text_size));
	staticenablechan->Hide(); // Just use as dummy for flexsizer spacing, hide it after creation
	//staticenablechan->SetBackgroundColour(*bgcolor);

	checkchan = new wxButton(channel_group, IDCCHECKCHAN, L"", wxDefaultPosition, inst->FromDIP(wxSize(50, 25)), wxSUNKEN_BORDER);
	inst_->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cDaqmx::OnDaqChanEnableBtn, this, IDCCHECKCHAN);
	checkchan->SetFont(checkchan->GetFont().Scale(text_size));
	checkchan->SetBackgroundColour(*bgcolor);
	checkchan->Disable();

	////////////////////////////////////////////////////////////

	static_chan_name = new wxStaticText(channel_group, IDCSTATICCHANNAME, L"Name to assign:", wxDefaultPosition, inst->FromDIP(static_ctrl_size), STATIC_CTRL_STYLE);
	static_chan_name->SetFont(static_chan_name->GetFont().Scale(text_size));
	//static_chan_name->SetBackgroundColour(*bgcolor);

	chan_name = new wxTextCtrl(channel_group, IDCCHANNAME, label.channel_name[0], wxDefaultPosition, inst->FromDIP(text_ctrl_size), TEXT_CTRL_STYLE);
	inst_->Bind(wxEVT_TEXT, &cDaqmx::OnDaqChanNameModified, this, IDCCHANNAME);
	chan_name->SetFont(chan_name->GetFont().Scale(text_size));
	chan_name->SetBackgroundColour(wxColor(250, 250, 250));
	chan_name->Disable();
	////////////////////////////////////////////////////////////

	staticchan = new wxStaticText(channel_group, IDCSTATICCHAN, L"Physical channel:", wxDefaultPosition, inst->FromDIP(static_ctrl_size), STATIC_CTRL_STYLE);
	staticchan->SetFont(staticchan->GetFont().Scale(text_size));
	//staticchan->SetBackgroundColour(*bgcolor);

	chan_addr_ctrl = new wxComboBox(channel_group, IDCCHANADDR, label.channel_physical_name[0].front(), wxDefaultPosition, inst->FromDIP(wxSize(150, 20)), label.channel_physical_name[0], wxCB_READONLY | wxSUNKEN_BORDER, wxDefaultValidator, _T(""));
	inst_->Bind(wxEVT_TEXT, &cDaqmx::OnDaqChanPhysicalAddressNameModified, this, IDCCHANADDR);
	chan_addr_ctrl->SetFont(chan_addr_ctrl->GetFont().Scale(text_size));
	chan_addr_ctrl->Disable();
	////////////////////////////////////////////////////////////

	statictype = new wxStaticText(channel_group, IDCSTATICTTYPE, L"Type:", wxDefaultPosition, inst->FromDIP(static_ctrl_size), STATIC_CTRL_STYLE);
	statictype->SetFont(statictype->GetFont().Scale(text_size));
	//statictype->SetBackgroundColour(*bgcolor);

	meas_type_ctrl = new wxComboBox(channel_group, IDCCHANTYPE, label.channel_type[0].front(), wxDefaultPosition, inst->FromDIP(wxSize(150, 20)), label.channel_type[0], wxCB_READONLY | wxSUNKEN_BORDER, wxDefaultValidator, _T(""));
	inst_->Bind(wxEVT_TEXT, &cDaqmx::OnDaqChanTypeModified, this, IDCCHANTYPE);
	meas_type_ctrl->SetFont(meas_type_ctrl->GetFont().Scale(text_size));
	meas_type_ctrl->Disable();
	////////////////////////////////////////////////////////////

	static_chan_max_input_range = new wxStaticText(channel_group, IDCSTATICMAX, L"Max input voltage:", wxDefaultPosition, inst->FromDIP(static_ctrl_size), STATIC_CTRL_STYLE);
	static_chan_max_input_range->SetFont(static_chan_max_input_range->GetFont().Scale(text_size));
	//static_chan_max_input_range->SetBackgroundColour(*bgcolor);

	chan_max_input_range = new wxComboBox(channel_group, IDCMAX, label.channel_max[0].back(), wxDefaultPosition, inst->FromDIP(wxDefaultSize), label.channel_max[0], wxCB_READONLY | wxSUNKEN_BORDER, wxDefaultValidator, _T(""));
	chan_max_input_range->SetFont(chan_max_input_range->GetFont().Scale(text_size));
	chan_max_input_range->Disable();
	////////////////////////////////////////////////////////////

	static_chan_min_input_range = new wxStaticText(channel_group, IDCSTATICMIN, L"Min input voltage:", wxDefaultPosition, inst->FromDIP(static_ctrl_size), STATIC_CTRL_STYLE);
	static_chan_min_input_range->SetFont(static_chan_min_input_range->GetFont().Scale(text_size));
	//static_chan_min_input_range->SetBackgroundColour(*bgcolor);

	chan_min_input_range = new wxComboBox(channel_group, IDCMIN, label.channel_min[0].front(), wxDefaultPosition, inst->FromDIP(wxDefaultSize), label.channel_min[0], wxCB_READONLY | wxSUNKEN_BORDER, wxDefaultValidator, _T(""));
	chan_min_input_range->SetFont(chan_min_input_range->GetFont().Scale(text_size));
	chan_min_input_range->Disable();

	////////////////////////////////////////////////////////////

	static_chan_input_mode_type = new wxStaticText(channel_group, IDCSTATICMODE, L"Input mode:", wxDefaultPosition, inst->FromDIP(static_ctrl_size), STATIC_CTRL_STYLE);
	static_chan_input_mode_type->SetFont(static_chan_input_mode_type->GetFont().Scale(text_size));
	//static_chan_input_mode_type->SetBackgroundColour(*bgcolor);

	chan_input_mode_type = new wxComboBox(channel_group, IDCMODE, label.channel_mode_type[0].front(), wxDefaultPosition, inst->FromDIP(wxDefaultSize), label.channel_mode_type[0], wxCB_READONLY | wxSUNKEN_BORDER, wxDefaultValidator, _T(""));
	chan_input_mode_type->SetFont(chan_input_mode_type->GetFont().Scale(text_size));
	chan_input_mode_type->Disable();

	////////////////////////////////////////////////////////////

	static_chan_tc_type = new wxStaticText(channel_group, IDCSTATICTCTYPE, L"Tc type:", wxDefaultPosition, inst->FromDIP(static_ctrl_size), STATIC_CTRL_STYLE);
	static_chan_tc_type->SetFont(static_chan_tc_type->GetFont().Scale(text_size));
	//static_chan_tc_type->SetBackgroundColour(*bgcolor);

	chan_tc_type = new wxComboBox(channel_group, IDCTCTYPE, label.channel_tc_type[0].front(), wxDefaultPosition, inst->FromDIP(wxDefaultSize), label.channel_tc_type[0], wxCB_READONLY | wxSUNKEN_BORDER, wxDefaultValidator, _T(""));
	chan_tc_type->SetFont(chan_tc_type->GetFont().Scale(text_size));

	////////////////////////////////////////////////////////////

	static_chan_tc_min_range = new wxStaticText(channel_group, IDCSTATICTCMIN, L"Min temperature °C:", wxDefaultPosition, inst->FromDIP(static_ctrl_size), STATIC_CTRL_STYLE);
	static_chan_tc_min_range->SetFont(static_chan_tc_min_range->GetFont().Scale(text_size));
	//static_chan_tc_min_range->SetBackgroundColour(*bgcolor);

	chan_tc_min_range = new wxTextCtrl(channel_group, IDCTCMIN, label.channel_tc_min[0], wxDefaultPosition, inst->FromDIP(text_ctrl_size), TEXT_CTRL_STYLE);
	chan_tc_min_range->SetFont(chan_tc_min_range->GetFont().Scale(text_size));
	chan_tc_min_range->SetBackgroundColour(wxColor(250, 250, 250));

	////////////////////////////////////////////////////////////

	static_chan_tc_max_range = new wxStaticText(channel_group, IDCSTATICTCMAX, L"Max temperature °C:", wxDefaultPosition, inst->FromDIP(static_ctrl_size), STATIC_CTRL_STYLE);
	static_chan_tc_max_range->SetFont(static_chan_tc_max_range->GetFont().Scale(text_size));
	//static_chan_tc_max_range->SetBackgroundColour(*bgcolor);

	chan_tc_max_range = new wxTextCtrl(channel_group, IDCTCMAX, label.channel_tc_max[0], wxDefaultPosition, inst->FromDIP(text_ctrl_size), TEXT_CTRL_STYLE);
	chan_tc_max_range->SetFont(chan_tc_max_range->GetFont().Scale(text_size));
	chan_tc_max_range->SetBackgroundColour(wxColor(250, 250, 250));

	////////////////////////////////////////////////////////////

	flexchansizer1->Add(checkchan);
	flexchansizer1->Add(staticenablechan);
	flexchansizer1->Add(static_chan_name);
	flexchansizer1->Add(chan_name);
	flexchansizer1->Add(staticchan);
	flexchansizer1->Add(chan_addr_ctrl);
	flexchansizer1->Add(statictype);
	flexchansizer1->Add(meas_type_ctrl);


	flexchansizer1->Add(static_chan_max_input_range);
	flexchansizer1->Add(chan_max_input_range);
	flexchansizer1->Add(static_chan_min_input_range);
	flexchansizer1->Add(chan_min_input_range);
	flexchansizer1->Add(static_chan_input_mode_type);
	flexchansizer1->Add(chan_input_mode_type);

	flexchansizer1->Add(static_chan_tc_type);
	flexchansizer1->Add(chan_tc_type);
	flexchansizer1->Add(static_chan_tc_min_range);
	flexchansizer1->Add(chan_tc_min_range);
	flexchansizer1->Add(static_chan_tc_max_range);
	flexchansizer1->Add(chan_tc_max_range);

	channel_group_sizer->Add(flexchansizer1);


	static_chan_tc_type->Show(false);
	chan_tc_type->Show(false);
	static_chan_tc_min_range->Show(false);
	chan_tc_min_range->Show(false);
	static_chan_tc_max_range->Show(false);
	chan_tc_max_range->Show(false);

	////////////////////////////////////////////////////////////

	wxStaticBox* channel_linearize_group = new wxStaticBox(config_rightpanel_, wxID_ANY, "Signal template");
	channel_linearize_group_sizer = new wxStaticBoxSizer(channel_linearize_group, wxVERTICAL);

	////////////////////////////////////////////////////////////

	static_chan_scale = new wxStaticText(channel_linearize_group, IDCSTATICCHANSCALE, L"Linearize:", wxDefaultPosition, inst->FromDIP(static_ctrl_size), STATIC_CTRL_STYLE);
	static_chan_scale->SetFont(static_chan_scale->GetFont().Scale(text_size));
	//static_chan_scale->SetBackgroundColour(*bgcolor);

	chan_scale = new wxComboBox(channel_linearize_group, IDCCHANSCALE, label.channel_linearize[0].front(), wxDefaultPosition, inst->FromDIP(wxDefaultSize), label.channel_linearize[0], wxCB_READONLY | wxSUNKEN_BORDER, wxDefaultValidator, _T(""));
	chan_scale->SetFont(chan_scale->GetFont().Scale(text_size));
	chan_scale->SetBackgroundColour(wxColor(250, 250, 250));

	inst_->Bind(wxEVT_COMMAND_COMBOBOX_SELECTED, &cDaqmx::OnDaqScaleSelBtn, this, IDCCHANSCALE);
	chan_scale->Disable();

	////////////////////////////////////////////////////////////

	static_chan_slope = new wxStaticText(channel_linearize_group, IDCSTATICCHANSLOPE, L"Slope:", wxDefaultPosition, inst->FromDIP(static_ctrl_size), STATIC_CTRL_STYLE);
	static_chan_slope->SetFont(static_chan_slope->GetFont().Scale(text_size));
	//static_chan_slope->SetBackgroundColour(*bgcolor);

	chan_slope = new wxTextCtrl(channel_linearize_group, IDCCHANSLOPE, label.channel_linearize_slope[0], wxDefaultPosition, inst->FromDIP(text_ctrl_size), TEXT_CTRL_STYLE);
	chan_slope->SetFont(chan_slope->GetFont().Scale(text_size));
	chan_slope->SetBackgroundColour(wxColor(250, 250, 250));
	chan_slope->Enable(false);
	chan_slope->Disable();

	////////////////////////////////////////////////////////////

	static_chan_shift = new wxStaticText(channel_linearize_group, IDCSTATICCHANSHIFT, L"Shift:", wxDefaultPosition, inst->FromDIP(static_ctrl_size), STATIC_CTRL_STYLE);
	static_chan_shift->SetFont(static_chan_shift->GetFont().Scale(text_size));
	//static_chan_shift->SetBackgroundColour(*bgcolor);

	chan_shift = new wxTextCtrl(channel_linearize_group, IDCCHANSHIFT, label.channel_linearize_shift[0], wxDefaultPosition, inst->FromDIP(text_ctrl_size), TEXT_CTRL_STYLE);
	chan_shift->SetFont(chan_shift->GetFont().Scale(text_size));
	chan_shift->SetBackgroundColour(wxColor(250, 250, 250));
	chan_shift->Enable(false);
	chan_shift->Disable();

	////////////////////////////////////////////////////////////

	static_chan_unit = new wxStaticText(channel_linearize_group, IDCSTATICCHANUNIT, L"Unit:", wxDefaultPosition, inst->FromDIP(static_ctrl_size), STATIC_CTRL_STYLE);
	static_chan_unit->SetFont(static_chan_unit->GetFont().Scale(text_size));
	//static_chan_unit->SetBackgroundColour(*bgcolor);

	chan_unit = new wxTextCtrl(channel_linearize_group, IDCCHANUNIT, label.channel_linearize_unit[0], wxDefaultPosition, inst->FromDIP(text_ctrl_size), TEXT_CTRL_STYLE);
	chan_unit->SetFont(chan_unit->GetFont().Scale(text_size));
	chan_unit->SetBackgroundColour(wxColor(250, 250, 250));
	chan_unit->Enable(false);
	chan_unit->Disable();

	////////////////////////////////////////////////////////////

	flexchansizer2->Add(static_chan_scale);
	flexchansizer2->Add(chan_scale);
	flexchansizer2->Add(static_chan_slope);
	flexchansizer2->Add(chan_slope);
	flexchansizer2->Add(static_chan_shift);
	flexchansizer2->Add(chan_shift);
	flexchansizer2->Add(static_chan_unit);
	flexchansizer2->Add(chan_unit);

	channel_linearize_group_sizer->Add(flexchansizer2);

	////////////////////////////////////////////////////////////

	wxStaticBox* channel_signal_group = new wxStaticBox(config_rightpanel_, wxID_ANY, "Signal operation");
	channel_signal_group_sizer = new wxStaticBoxSizer(channel_signal_group, wxVERTICAL);

	////////////////////////////////////////////////////////////

	static_chan_trigger = new wxStaticText(channel_signal_group, IDCSTATICCHANSCALE, L"Trigger:", wxDefaultPosition, inst->FromDIP(static_ctrl_size), STATIC_CTRL_STYLE);
	static_chan_trigger->SetFont(static_chan_trigger->GetFont().Scale(text_size));
	//static_chan_trigger->SetBackgroundColour(*bgcolor);

	chan_trigger = new wxComboBox(channel_signal_group, IDCCHANSCALE, label.channel_trigger[0].front(), wxDefaultPosition, inst->FromDIP(wxDefaultSize), label.channel_trigger[0], wxCB_READONLY | wxSUNKEN_BORDER, wxDefaultValidator, _T(""));
	chan_trigger->SetFont(chan_scale->GetFont().Scale(text_size));
	chan_trigger->SetBackgroundColour(wxColor(250, 250, 250));
	chan_trigger->Disable();

	////////////////////////////////////////////////////////////

	static_chan_trigger_threshold = new wxStaticText(channel_signal_group, IDCSTATICCHANSLOPE, L"Threshold:", wxDefaultPosition, inst->FromDIP(static_ctrl_size), STATIC_CTRL_STYLE);
	static_chan_trigger_threshold->SetFont(static_chan_trigger_threshold->GetFont().Scale(text_size));
	//static_chan_trigger_threshold->SetBackgroundColour(*bgcolor);

	chan_trigger_threshold = new wxTextCtrl(channel_signal_group, IDCCHANSLOPE, label.channel_trigger_threshold[0], wxDefaultPosition, inst->FromDIP(text_ctrl_size), TEXT_CTRL_STYLE);
	chan_trigger_threshold->SetFont(chan_trigger_threshold->GetFont().Scale(text_size));
	chan_trigger_threshold->SetBackgroundColour(wxColor(250, 250, 250));
	chan_trigger_threshold->Enable(false);
	chan_trigger_threshold->Disable();

	////////////////////////////////////////////////////////////

	static_chan_filter = new wxStaticText(channel_signal_group, IDCSTATICCHANFILTER, L"Filter:", wxDefaultPosition, inst->FromDIP(static_ctrl_size), STATIC_CTRL_STYLE);
	static_chan_filter->SetFont(static_chan_filter->GetFont().Scale(text_size));
	//static_chan_filter->SetBackgroundColour(*bgcolor);

	chan_filter = new wxComboBox(channel_signal_group, IDCCHANFILTER, label.channel_filter[0].front(), wxDefaultPosition, inst->FromDIP(wxDefaultSize), label.channel_filter[0], wxCB_READONLY | wxSUNKEN_BORDER, wxDefaultValidator, _T(""));
	chan_filter->SetFont(chan_filter->GetFont().Scale(text_size));
	chan_filter->SetBackgroundColour(wxColor(250, 250, 250));
	chan_filter->Disable();

	////////////////////////////////////////////////////////////

	static_chan_filter_intensity = new wxStaticText(channel_signal_group, IDCSTATICCHANFILTERLEVEL, L"Level:", wxDefaultPosition, inst->FromDIP(static_ctrl_size), STATIC_CTRL_STYLE);
	static_chan_filter_intensity->SetFont(static_chan_filter_intensity->GetFont().Scale(text_size));
	//static_chan_filter_intensity->SetBackgroundColour(*bgcolor);

	chan_filter_intensity = new wxTextCtrl(channel_signal_group, IDCCHANSLOPE, label.channel_filter_intensity[0], wxDefaultPosition, inst->FromDIP(text_ctrl_size), TEXT_CTRL_STYLE);
	chan_filter_intensity->SetFont(chan_filter_intensity->GetFont().Scale(text_size));
	chan_filter_intensity->SetBackgroundColour(wxColor(250, 250, 250));
	chan_filter_intensity->Enable(false);

	flexchansizer3->Add(static_chan_trigger);
	flexchansizer3->Add(chan_trigger);
	flexchansizer3->Add(static_chan_trigger_threshold);
	flexchansizer3->Add(chan_trigger_threshold);
	flexchansizer3->Add(static_chan_filter);
	flexchansizer3->Add(chan_filter);
	flexchansizer3->Add(static_chan_filter_intensity);
	flexchansizer3->Add(chan_filter_intensity);

	channel_signal_group_sizer->Add(flexchansizer3);

	DaqChanAllOn(false);

	wxBoxSizer* grid_vsizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* upper_sizer = new wxBoxSizer(wxHORIZONTAL); // device + dummy
	wxBoxSizer* grid_hsizer = new wxBoxSizer(wxHORIZONTAL); // channels

	upper_sizer->Add(device_group_sizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, inst->FromDIP(50)); // Device
	upper_sizer->Add(dummy, 1, wxRIGHT, 400); // dummy

	chan_vsizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* chan_hsizer = new wxBoxSizer(wxHORIZONTAL);

	grid_vsizer->Add(upper_sizer/*dummy*/, 1, wxALIGN_CENTER, inst->FromDIP(10)); // Space for daq img
	grid_vsizer->Add(grid_hsizer, 1, wxALL | wxALIGN_CENTER, inst->FromDIP(10));

	//grid_hsizer->Add(device_group_sizer/*flexsizer*/, 0, wxALIGN_CENTER, 10);
	grid_hsizer->Add(chan_vsizer, 1, wxALIGN_CENTER);

	chan_vsizer->Add(btn_hsizer, 0, wxEXPAND);
	chan_vsizer->Add(chan_grid, 0);
	chan_vsizer->Add(chan_hsizer, 0);

	chan_hsizer->Add(channel_group_sizer/*flexchansizer1*/, 0, wxALL | wxEXPAND, inst->FromDIP(10));
	chan_hsizer->Add(channel_linearize_group_sizer/*flexchansizer2*/, 0, wxALL | wxEXPAND, inst->FromDIP(10));
	chan_hsizer->Add(channel_signal_group_sizer/*flexchansizer3*/, 0, wxALL | wxEXPAND, inst->FromDIP(10));

	config_rightpanel_->SetSizerAndFit(grid_vsizer);

	wxCommandEvent evt = wxCommandEvent(wxEVT_COMMAND_BUTTON_CLICKED, IDCDAQACTIVATE);
	wxPostEvent(inst_, evt);
}



void cDaqmx::OnPaint(wxPaintEvent& event)
{
	wxBufferedPaintDC dc(this, wxBUFFER_CLIENT_AREA);


	wxRect size = config_rightpanel_->GetRect();

	size.x = 0;
	dc.GradientFillLinear(size, wxColor(105, 105, 105), wxColor(255, 255, 255), wxUP);

	wxSize sz = image.GetSize();
	int pos = (size.width / 2) - (sz.x / 2);
	dc.DrawBitmap(image, pos + 120, 0, false);

	event.Skip();
}

void cDaqmx::show_right_panel(bool show)
{
	config_rightpanel_->Hide();
}

void cDaqmx::show_channel(const int index, bool show)
{
	//flexchansizer[index]->Show(show);
	// TODO load data into ctrls
}

void cDaqmx::show_channel(bool show)
{
	//for (int i = 0; i < chan_number; i++)
	//{
		//flexchansizer[i]->Show(show);
		// TODO load data into ctrls
	//}
}

void cDaqmx::show_next_channel()
{
	if (label.channel_index >= 0 and label.channel_index < (label.chan_number - 1))
	{
		// Save current device and channels
		save_current_device_config(label.channel_index);
		save_current_chan_config(label.channel_index);

		// Inc index
		label.channel_index++;

		// Load next channel
		load_current_device_config(label.channel_index);
		load_current_chan_config(label.channel_index);

		// Mark button with color
		if (label.channel_enabled[label.channel_index - 1] == true)
			chanbtn[label.channel_index - 1]->SetBackgroundColour(wxColor(160, 250, 160)); // green
		else
			chanbtn[label.channel_index - 1]->SetBackgroundColour(wxColor(250, 250, 250)); // grey

		chanbtn[label.channel_index]->SetBackgroundColour(wxColor(120, 140, 120)); // dark

		// If channel disabled gray it out
		EnableChannelItems(label.channel_enabled.at(label.channel_index));
	}
}

void cDaqmx::OnNextCliqued(wxCommandEvent& evt)
{
	show_next_channel();
	// Display the current selected channel measure type
	reload_current_channel_type();
	//inst_->Layout();
	evt.Skip();
}

void cDaqmx::show_previous_channel()
{
	if (label.channel_index <= (label.chan_number - 1) and label.channel_index > 0)
	{
		// Save current device and channels
		save_current_device_config(label.channel_index);
		save_current_chan_config(label.channel_index);

		// Dec index
		label.channel_index--;

		// Load next channel
		load_current_device_config(label.channel_index);
		load_current_chan_config(label.channel_index);

		// Mark button with color
		if (label.channel_enabled[label.channel_index + 1] == true)
			chanbtn[label.channel_index + 1]->SetBackgroundColour(wxColor(160, 250, 160)); // green
		else
			chanbtn[label.channel_index + 1]->SetBackgroundColour(wxColor(250, 250, 250)); // grey	

		chanbtn[label.channel_index]->SetBackgroundColour(wxColor(120, 140, 120)); // dark

		// If channel disabled gray it out
		EnableChannelItems(label.channel_enabled.at(label.channel_index));
	}
}

void cDaqmx::OnPreviousCliqued(wxCommandEvent& evt)
{
	show_previous_channel();

	// Display the current selected channel measure type
	reload_current_channel_type();

	evt.Skip();
}

// Reccord device info selected in GUI field 
// and save it to an CURRENT_DEVICE_STRUCT witch hold the datas
// happen each time previous or next is pressed
void cDaqmx::save_current_device_config(int channel_index)
{
	std::cout << "[*] Saving device GUI field in memory at channel " << channel_index << ".\n";

	// Device enable
	config.device_enabled = false;
	if (daq_activate->GetLabelText().compare("ON") == 0)
	{
		config.device_enabled = true;
	}

	// Device name
	//int iSelection = addr_ctrl->GetCurrentSelection();
	//config.device_name = label.device_name[iSelection];
	config.device_name = addr_ctrl->GetValue();

	return;
}

// Reccord device info selected in GUI field 
// and save it to an CURRENT_DEVICE_STRUCT witch hold the datas
// happen each time previous or next is pressed
void cDaqmx::load_current_device_config(int channel_index)
{
	std::cout << "[*] Loading device configuration from memory in GUI at channel " << channel_index << ".\n";

	// Device enable
	//config.device_enabled = false;
	//if (daq_activate->GetLabelText().compare("ON"))
	//{
	//	config.device_enabled = true;
	//}

	// Device name
	int iTotalItem = addr_ctrl->GetCount();
	for (int i = 0; i < iTotalItem; i++)
	{
		wxString value = addr_ctrl->GetString(i);
		if (value.compare(config.device_name) == 0)
		{
			addr_ctrl->SetSelection(i);
			std::cout << "[*] Found device in configuration memory struct.\n";
		}
	}

	return;
}

// Reccord all channels data selected in GUI field 
// and save it to a CURRENT_DEVICE_STRUCT witch hold the datas
// happen each time previous or next button is pressed
void cDaqmx::save_current_chan_config(int channel_index)
{
	std::cout << "[*] Saving channels GUI field in memory at channel " << channel_index << "\n";

	// Channel enable
	config.channel_enabled[channel_index] = false;
	if (checkchan->GetLabelText().compare("ON") == 0)
	{
		config.channel_enabled[channel_index] = true;
	}

	// Channel name
	config.channel_name[channel_index] = chan_name->GetValue();

	// Channel physical name
	int iSelection = chan_addr_ctrl->GetCurrentSelection();
	if (iSelection < 0)
	{
		iSelection = 0;
		MessageBox(GetFocus(), L"Channel physical name is empty", L"[!] Warning", S_OK);
	}
	config.channel_physical_name[channel_index] = chan_addr_ctrl->GetValue();

	// Channel physical type
	iSelection = meas_type_ctrl->GetCurrentSelection();
	if (iSelection < 0)
	{
		iSelection = 0;
		MessageBox(GetFocus(), L"Channel physical name is empty", L"[!] Warning", S_OK);
	}
	config.channel_type[channel_index] = meas_type_ctrl->GetValue();

	// Channel max voltage range
	iSelection = chan_max_input_range->GetCurrentSelection();
	config.channel_max[channel_index] = label.channel_max[channel_index][iSelection];

	// Channel min voltage range
	iSelection = chan_min_input_range->GetCurrentSelection();
	config.channel_min[channel_index] = label.channel_min[channel_index][iSelection];

	// Channel reference mode
	iSelection = chan_input_mode_type->GetCurrentSelection();
	config.channel_mode_type[channel_index] = label.channel_mode_type[channel_index][iSelection];

	// Channel Tc type (T,K,...)
	config.channel_tc_type[channel_index] = chan_tc_type->GetValue();

	// Channel Tc min temperature
	config.channel_tc_min[channel_index] = chan_tc_min_range->GetValue();

	// Channel Tc max temperature
	config.channel_tc_max[channel_index] = chan_tc_max_range->GetValue();

	// Channel linearize preset
	config.channel_linearize[channel_index] = chan_scale->GetValue();

	// Channel slope value
	config.channel_linearize_slope[channel_index] = chan_slope->GetValue();

	// Channel shift value
	config.channel_linearize_shift[channel_index] = chan_shift->GetValue();

	// Channel unit value	
	config.channel_linearize_unit[channel_index] = chan_unit->GetValue().ToStdString();

	// Channel filter 
	iSelection = chan_filter->GetCurrentSelection();
	config.channel_filter[channel_index] = label.channel_filter[channel_index][iSelection];

	// Channel filter intensity
	config.channel_filter_intensity[channel_index] = chan_filter_intensity->GetValue();

	// Channel trigger
	iSelection = chan_trigger->GetCurrentSelection();
	config.channel_trigger[channel_index] = label.channel_trigger[channel_index][iSelection];

	// Channel threshold
	config.channel_trigger_threshold[channel_index] = chan_trigger_threshold->GetValue();

	std::cout << "[*] Field " << channel_index << " saved\n";

	return;
}

// Load all channels data from memory in GUI field 
// load it from the CURRENT_DEVICE_STRUCT witch hold the datas
// happen each time previous or next is pressed
void cDaqmx::load_current_chan_config(int channel_index)
{
	std::cout << "[*] Loading channels from memory to GUI at channel " << channel_index << "\n";

	// Channel enable
	if (config.channel_enabled[channel_index] == false)
	{
		checkchan->SetBackgroundColour(wxColor(250, 120, 120)); // RED
		checkchan->SetLabel("OFF");
	}
	else
	{
		checkchan->SetBackgroundColour(wxColor(160, 250, 160)); // GREEN	
		checkchan->SetLabel("ON");
	}

	// Channel name
	chan_name->SetLabelText(config.channel_name[channel_index]);

	// Channel physical name
	load_combobox(chan_addr_ctrl, config.channel_physical_name[channel_index]);

	/*
	wxString wxChanAddr = config.channel_physical_name[channel_index];
	int iTotalItem = chan_addr_ctrl->GetCount();
	for (int i = 0; i < iTotalItem; i++)
	{
		if (chan_addr_ctrl->GetString(i).compare(wxChanAddr) == 0)
		{
			chan_addr_ctrl->SetSelection(i);
		}
	}*/

	// Channel physical type
	load_combobox(meas_type_ctrl, config.channel_type[channel_index]);

	// Channel max voltage range
	load_combobox(chan_max_input_range, config.channel_max[channel_index]);

	// Channel min voltage range
	load_combobox(chan_min_input_range, config.channel_min[channel_index]);

	// Channel reference mode
	load_combobox(chan_input_mode_type, config.channel_mode_type[channel_index]);

	// Channel Tc type (T,K,...)
	load_combobox(chan_tc_type, config.channel_tc_type[channel_index]);

	// Channel Tc min temperature
	chan_tc_min_range->SetValue(config.channel_tc_min[channel_index]);

	// Channel Tc max temperature
	chan_tc_max_range->SetValue(config.channel_tc_max[channel_index]);

	// Channel linearize preset
	load_combobox(chan_scale, config.channel_linearize[channel_index]);

	// Channel slope value
	chan_slope->SetLabelText(config.channel_linearize_slope[channel_index]);

	// Channel shift value
	chan_shift->SetLabelText(config.channel_linearize_shift[channel_index]);

	// Channel unit value	
	chan_unit->SetLabelText(config.channel_linearize_unit[channel_index]);

	// Channel filter 
	load_combobox(chan_filter, config.channel_filter[channel_index]);

	// Channel filter intensity
	chan_filter_intensity->SetLabelText(config.channel_filter_intensity[channel_index]);

	// Channel trigger
	chan_trigger->SetLabelText(config.channel_trigger[channel_index]);

	// Channel threshold
	chan_trigger_threshold->SetLabelText(config.channel_trigger_threshold[channel_index]);

	std::cout << "[*] Field " << channel_index << " saved\n";
	return;
}

void cDaqmx::load_combobox(wxComboBox* combo, wxString str)
{
	wxString wxStr = str;
	int iTotalItem = combo->GetCount();
	for (int i = 0; i < iTotalItem; i++)
	{
		if (combo->GetString(i).compare(wxStr) == 0)
		{
			combo->SetSelection(i);
		}
	}
}

void cDaqmx::load_combobox(wxComboBox* combo, double floating)
{
	wxString wxFloating = wxString::Format(wxT("%lf"), floating);
	int iTotalItem = combo->GetCount();
	for (int i = 0; i < iTotalItem; i++)
	{
		if (combo->GetString(i).compare(wxFloating) == 0)
		{
			combo->SetSelection(i);
		}
	}
}
/*
void cDaqmx::EnableChanProperties()
{

	for (int i = 0; i < chan_number; i++)
	{
		int detect = 0;
		int count = 0;
		wxSizerItemList wnds = flexchansizer[i]->GetChildren(); // iterate over daq wxWindow elements
		for (auto& wnd : wnds)
		{
			wxWindow* p_item = wnd->GetWindow();

			if (p_item->GetLabel() == "OFF")
			{
				detect = 1;
				continue;
			}
			else
			{
				if (count == 4)
				{
					detect = 0;
				}

				if (detect == 1)
				{
					p_item->Enable(false);
					count++;

					wxSizerItemList wnds2 = flexchansizer2[i]->GetChildren(); // iterate over daq wxWindow elements
					for (auto& wnd2 : wnds2)
					{
						wxWindow* p_item2 = wnd2->GetWindow();
						p_item2->Enable(false);
					}
				}
				else
				{
					p_item->Enable(true);

					wxSizerItemList wnds2 = flexchansizer2[i]->GetChildren(); // iterate over daq wxWindow elements
					for (auto& wnd2 : wnds2)
					{
						wxWindow* p_item2 = wnd2->GetWindow();
						p_item2->Enable(true);
					}
				}

			}
		}
	}

}
*/
void cDaqmx::OnDaqEnableBtn(wxCommandEvent& evt)
{
	// Enable/disable controls
	enable_pan = !enable_pan;
	if (daq_activate)
	{
		if (!enable_pan)
		{
			constexpr size_t bufferSize = 1000;
			char buffer[bufferSize] = {};
			if (DAQmxGetSysDevNames(buffer, bufferSize) != 0)
			{
				MessageBox(GetFocus(), L"[!] Warning", L"DAQmxGetSysDevNames() failed to resolve devices.\n", S_OK | MB_ICONERROR);
				evt.Skip();
				return;
			}

			if (strcmp(buffer, "") == 0)
			{
				MessageBox(GetFocus(), L"Warning\n\n Connect a DAQ", L"There is no DAQ connected to your computer.", S_OK | MB_ICONINFORMATION);
			}
			// Global to device

			std::string s(buffer);
			std::string delimiter = ",";

			size_t pos_start = 0, pos_end, delim_len = delimiter.length();
			std::string token;
			std::vector<std::string> names;

			while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
				token = s.substr(pos_start, pos_end - pos_start);
				pos_start = pos_end + delim_len;
				names.push_back(token);
			}

			names.push_back(s.substr(pos_start));

			addr_ctrl->Clear();

			int first_measurable_device = -1;

			for (auto name : names)
			{
				if (isDeviceMeasurable(name) == false)
				{
					continue;
				}

				first_measurable_device++;

				char product_type[256] = "";
				if (DAQmxGetDevProductType(name.c_str(), product_type, sizeof(product_type)) != 0)
				{
					MessageBox(GetFocus(), L"[!] Warning", L"DAQmxGetDevProductType() failed to resolve devices product types.\n", S_OK | MB_ICONERROR);
					evt.Skip();
					return;
				}
				std::cout << "Dev name: " << name << "\n";
				std::cout << "product_type: " << product_type << "\n";
				addr_ctrl->Append(name);
			}
			// Add separator
			addr_ctrl->Append("Simulated");

			// Channel specific


		// List all channels available on each device and concat them 
			std::vector<std::string> channels;
			for (auto name : names)
			{
				if (isDeviceMeasurable(name) == false)
				{
					std::cout << "[!] " << name << " is not measurable, skip next\n";
					continue;
				}
				if (DAQmxGetDevAIPhysicalChans(name.c_str(), buffer, bufferSize) != 0)
				{
					MessageBox(GetFocus(), L"[!] Warning", L"DAQmxGetDevAIPhysicalChans() failed to resolve channels.\n", S_OK | MB_ICONERROR);
					evt.Skip();
					return;
				}

				std::string s(buffer);
				std::string delimiter = ", ";
				size_t pos_start = 0, pos_end, delim_len = delimiter.length();
				std::string token;


				while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
					token = s.substr(pos_start, pos_end - pos_start);
					pos_start = pos_end + delim_len;
					channels.push_back(token);
				}

				channels.push_back(s.substr(pos_start));
			}

			if (channels.size() == 0)
			{
				// create fake simulated channels
				std::string fake_chan;
				for (int i = 0; i < 32; i++)
				{
					fake_chan = std::format("DevSim/ai{}", i);
					channels.push_back(fake_chan);
				}

			}

			//Hide unused channel in the button grid
			for (int i = channels.size(); i < label.chan_number; i++)
			{
				chanbtn[i]->Show(false);
			}

			// fill analog channel number in list
			chan_addr_ctrl->Clear();
			for (auto chan : channels)
			{
				chan_addr_ctrl->Append(chan);
			}
			chan_addr_ctrl->SetSelection(0);

			// fill analog channel physical name in the config structure
			for (int i = 0; i < channels.size(); i++)
			{
				config.channel_physical_name[i] = channels.at(i);
			}
			label.chan_number = channels.size();


			// Update channels legend numbers
			std::cout << "cObjectmanager->getInstance()\n";
			cObjectmanager* object_manager = object_manager->getInstance();
			cPlot* m_plot = object_manager->get_plot();
			m_plot->resize_chan_number_to_gui(channels.size());




			// Select the first device measurable
			// Add it to the measurement manager
			// by sending an event
			// This event is responsible to apply in the callback the new policy (daqmx/simulate/...) according to the strategy pattern 
			if (first_measurable_device < 0)
			{
				std::cout << "[!] Warning", L"No module available to take measurement are connected.\n";
				addr_ctrl->SetSelection(first_measurable_device + 2); // When failed "Simulated" are at pos 1

			}
			else
			{
				addr_ctrl->SetSelection(first_measurable_device);
			}

			wxCommandEvent evt = wxCommandEvent(wxEVT_COMMAND_COMBOBOX_SELECTED, IDCADDR);
			wxPostEvent(inst_, evt);

			this->daq_activate->SetBackgroundColour(wxColor(160, 250, 160)); // GREEN	
			this->daq_activate->SetLabel("ON");

			addr_ctrl->Enable(true);
			checkchan->Enable(true);

		}
		else
		{
			this->daq_activate->SetBackgroundColour(wxColor(250, 120, 120)); // RED
			this->daq_activate->SetLabel("OFF");

			addr_ctrl->Enable(false);
			checkchan->Enable(false);
		}
	}
	evt.Skip();
}

wxArrayString cDaqmx::LoadScalePresetArray(wxString filename)
{
	// Load registered scale into an array
	wxArrayString m_scale;
	m_scale.Add("No");
	m_scale.Add("Custom scale");
	// Read saved scales and add values in the list
	// load user scale from disk
	// C:\Users\The Hive\AppData\Roaming
	//std::cout << "[*] Loading scale preset from file : " << filename << "\n";
	//std::cout << "[*] Default location is : C:\\Users\\The Hive\\AppData\\Roaming \n";

	wxFileConfig* rec_scale = nullptr;
	rec_scale = new wxFileConfig(wxEmptyString, wxEmptyString, filename, wxEmptyString, wxCONFIG_USE_LOCAL_FILE, wxConvAuto());
	wxString group;
	wxString entry;
	wxString value;
	long group_cookie;
	long entry_cookie;
	bool bok = false;
	bok = rec_scale->GetFirstGroup(group, group_cookie);
	while (bok)
	{
		//std::cout << "Group: " << group << "\n";

		// Entering the group 
		// And then
		// Enumerate each entry in the current group
		wxString strOldPath = rec_scale->GetPath();
		rec_scale->SetPath(group);
		bok = rec_scale->GetFirstEntry(entry, entry_cookie);
		while (bok)
		{
			//std::cout << entry << " " << rec_scale->Read(entry, value) << "\n";
			// Add to list if name found
			if (entry.compare("name") == 0)
			{
				m_scale.Add(rec_scale->Read(entry, value));
			}
			bok = rec_scale->GetNextEntry(entry, entry_cookie);
		}
		rec_scale->SetPath(strOldPath);

		bok = rec_scale->GetNextGroup(group, group_cookie);
	}
	delete rec_scale;
	m_scale.Add("Create new");
	// End loading scale preset
	return m_scale;
}


void cDaqmx::OnDaqAddrSelBtn(wxCommandEvent& evt)
{
	// Destroy and release previous ressource

	std::cout << "[*] cMeasurementmanager->getInstance()\n";
	meas_manager = meas_manager->getInstance();

	// Destroy item in the list
	bool isDestroyed = meas_manager->destroy_subsystem(DAQ_INSTR);
	// If item destroyed delete from memory
	if (isDestroyed)
	{
		std::cout << "[*] [delete] m_daq in cDaqmx.cpp\n";

		delete m_daq_;
		m_daq_ = nullptr;
	}

	// Read the new sub system name selected
	wxString current_device = this->addr_ctrl->GetValue();
	std::string current = current_device.ToStdString();
	constexpr size_t bufferSize = 1000;
	char buffer[bufferSize] = {};

	if (current.compare("") == 0)
	{
		MessageBox(GetFocus(), L"No device selected", L"Select a device", S_OK | MB_ICONERROR);
		evt.Skip();
		return;
	}

	if (current.compare("Simulated") == 0)
	{
		if (m_daq_ == nullptr)
		{
			std::cout << "[*] [new] Create cDaqsim\n";
			m_daq_ = new(cDaqsim);
			meas_manager->set_measurement(m_daq_);
		}
		evt.Skip();
		return;
	}

	if (isDeviceMeasurable(current) != true)
	{
		std::cout << "[!] Failed to load: " << current << "\n";
		MessageBox(GetFocus(), L"[!] Fail to load this device", L"Device not measurable", S_OK | MB_ICONERROR);
		evt.Skip();
		return;
	}

	if (m_daq_ != nullptr)
	{
		std::cout << "[!] Failed to load: " << current << "\n";
		MessageBox(GetFocus(), L"[!] Fail", L"Object already exist", S_OK);
		evt.Skip();
		return;
	}

	// Then create appropriate DAQ object
	std::cout << "[*] [new] Create cUsb6001\n";
	m_daq_ = new(cUsb6001);

	// Object failed to create in memory
	if (m_daq_ == nullptr)
	{
		std::cout << "[!] Impossible to load a DAQ system object\n";
		MessageBox(GetFocus(), L"[!] Fail", L"Imposible to create object in memory.\n", S_OK | MB_ICONERROR);
		evt.Skip();
		return;
	}

	meas_manager->set_measurement(m_daq_);
	m_daq_->set_device_addr(current);
	evt.Skip();
	return;
}

void cDaqmx::OnDaqScaleSelBtn(wxCommandEvent& evt)
{
	wxString val = chan_scale->GetValue();

	if (val.compare("No") == 0)
	{
		std::cout << "[*] Daq set to no scaling enabled\n";
		chan_slope->SetLabelText("1");
		chan_slope->Enable(false);

		chan_shift->SetLabelText("0");
		chan_shift->Enable(false);

		chan_unit->SetLabelText("Volt");
		chan_unit->Enable(false);
	}
	else if (val.compare("Custom scale") == 0)
	{
		std::cout << "[*] Daq set to custom user scale unit\n";
		chan_slope->Enable(true);
		chan_shift->Enable(true);
		chan_unit->Enable(true);
	}
	else if (val.compare("Create new") == 0)
	{
		std::cout << "[*] Daq create a new custom scale\n";

		ScaleDialog ScaleDlg(this->inst_, IDCSCALEDLG, "Create your custom scale here");
		ScaleDlg.Centre();
		int ret = ScaleDlg.ShowModal();
		if (ret == wxID_OK)
		{

			// save user scale on disk
			// C:\Users\The Hive\AppData\Roaming
			std::string custom_scale_filename = "Lab++ScalePreset.ini";

			wxFileConfig* custom_scale = nullptr;
			custom_scale = new wxFileConfig(wxEmptyString, wxEmptyString, custom_scale_filename, wxEmptyString, wxCONFIG_USE_LOCAL_FILE, wxConvAuto());
			custom_scale->SetPath(ScaleDlg.get_scale_name());
			custom_scale->Write("name", wxString(ScaleDlg.get_scale_name()));
			custom_scale->Write("slope", wxString(ScaleDlg.get_scale_slope()));
			custom_scale->Write("shift", wxString(ScaleDlg.get_scale_shift()));
			custom_scale->Write("unit", wxString(ScaleDlg.get_scale_unit()));
			delete custom_scale;

			// load user scale
			chan_scale->Insert(ScaleDlg.get_scale_name(), 1);
			chan_scale->SetValue(ScaleDlg.get_scale_name());
			chan_slope->SetLabelText(ScaleDlg.get_scale_slope());
			chan_shift->SetLabelText(ScaleDlg.get_scale_shift());
			chan_unit->SetLabelText(ScaleDlg.get_scale_unit());

			// TODO: refresh panel to size the new combobox item

		}
		else
		{
			// Set selection on last selected
			wxString filled = config.channel_linearize[config.channel_index];
			wxArrayString combo_item = LoadScalePresetArray(scale_file_name);
			for (int i = 0; i < combo_item.size(); i++)
			{
				if (filled.compare(combo_item[i]) == 0)
				{
					chan_scale->SetSelection(i);
					break;
				}
				chan_scale->SetSelection(0);
			}
		}
	}
	else
	{
		// Read scale preset and load value if needed

		wxFileConfig* rec_scale = nullptr;
		rec_scale = new wxFileConfig(wxEmptyString, wxEmptyString, scale_file_name, wxEmptyString, wxCONFIG_USE_LOCAL_FILE, wxConvAuto());
		wxString group;
		wxString entry;
		long group_cookie;
		long entry_cookie;
		bool bok = false;
		bool bFound = false;
		bok = rec_scale->GetFirstGroup(group, group_cookie);
		while (bok)
		{
			// Entering the group 
			// And then
			// Enumerate each entry in the current group
			wxString strOldPath = rec_scale->GetPath();
			rec_scale->SetPath(group);
			bok = rec_scale->GetFirstEntry(entry, entry_cookie);
			if (bok)
			{
				// If name found
				if (entry.compare("name") == 0)
				{
					// If name found in the list == name in the file
					// read and load the current value in the panel
					if (val.compare(rec_scale->Read(entry)) == 0)
					{
						// Slope
						rec_scale->GetNextEntry(entry, entry_cookie);
						chan_shift->SetLabelText(rec_scale->Read(entry));
						static_chan_shift->Enable(true);
						chan_shift->Enable(false);

						// Shift
						rec_scale->GetNextEntry(entry, entry_cookie);
						chan_slope->SetLabelText(rec_scale->Read(entry));
						static_chan_slope->Enable(true);
						chan_slope->Enable(false);

						// Unit 
						rec_scale->GetNextEntry(entry, entry_cookie);
						chan_unit->SetLabelText(rec_scale->Read(entry));
						static_chan_unit->Enable(true);
						chan_unit->Enable(false);

						bFound = true;
						break;
					}
				}
			}
			rec_scale->SetPath(strOldPath);

			bok = rec_scale->GetNextGroup(group, group_cookie);
		}
		delete rec_scale;

		// If nothing found set to first item
		if (!bFound)
		{
			chan_scale->SetSelection(0);
		}
	}
	evt.Skip();
}


void cDaqmx::DaqChanAllOn(bool enable)
{

	for (auto chan : label.channel_enabled)
		chan = enable;

	if (enable)
	{
		this->checkchan->SetBackgroundColour(wxColor(160, 250, 160)); // GREEN	
		this->checkchan->SetLabel("ON");
	}
	else
	{
		this->checkchan->SetBackgroundColour(wxColor(250, 120, 120)); // RED
		this->checkchan->SetLabel("OFF");
	}

	/*
	wxSizerItemList wnds = flexchansizer->GetChildren();
	for (auto& item : wnds)
	{
		if (item->IsWindow())
		{
			wxWindow* wnd = item->GetWindow();
			if (wnd != checkchan[i])
				wnd->Enable(enable);
		}
	}
*/
}

void cDaqmx::OnDaqChanNameModified(wxCommandEvent& evt)
{
	std::cout << "cObjectmanager->getInstance()\n";
	cObjectmanager* object_manager = object_manager->getInstance();
	cPlot* m_plot = object_manager->get_plot();

	m_plot->update_chan_name_to_gui(chan_name->GetValue().ToStdString(), label.channel_index);
}

void cDaqmx::OnDaqChanPhysicalAddressNameModified(wxCommandEvent& evt)
{
	std::cout << "cObjectmanager->getInstance()\n";
	cObjectmanager* object_manager = object_manager->getInstance();
	cPlot* m_plot = object_manager->get_plot();

	m_plot->update_chan_physical_name_to_gui(chan_addr_ctrl->GetValue().ToStdString(), label.channel_index);
}

void cDaqmx::OnDaqChanTypeModified(wxCommandEvent& evt)
{
	// Save current device and channels
	save_current_device_config(label.channel_index);
	save_current_chan_config(label.channel_index);

	// Check if it is comapatible with current DAQ
	int32 meas_types[64]; memset(meas_types, 0, sizeof(meas_types)); // officially 29 different type is supported put 64 to be sure there is enought space for further NI API updates
	if (0 != DAQmxGetDevAISupportedMeasTypes(config.device_name.c_str(), meas_types, sizeof(meas_types)))
	{
		MessageBox(0, L"Fail to check supported types.", L"DAQmxGetDevAISupportedMeasTypes Failure", 0);
		return;
	}
	// DAQmx_Val_Voltage 	    10322 	Voltage measurement.
	// DAQmx_Val_Temp_TC 	    10303 	Temperature measurement using a thermocouple.
	// DAQmx_Val_Temp_Thrmstr   10302 	Temperature measurement using a thermistor.     
	bool bFound = false;
	int ires = meas_type_ctrl->GetSelection();

	int32 wanted_type = 0;
	switch (ires)
	{
	case 0:
	{
		show_tc_param(false);
		show_voltage_param(true);
		wanted_type = DAQmx_Val_Voltage;
		break;
	}
	case 1:
	{
		show_tc_param(true);
		show_voltage_param(false);
		wanted_type = DAQmx_Val_Temp_TC;
		break;
	}
	case 2:
	{
		show_tc_param(false);
		show_voltage_param(false);
		wanted_type = DAQmx_Val_Temp_Thrmstr;
		break;
	}
	default:
		wanted_type = DAQmx_Val_Voltage;
	}

	for (auto& meastype : meas_types)
	{
		if (meastype == wanted_type)
		{
			bFound = true;
			break;
		}
	}

	if (!bFound)
	{
		MessageBox(0, L"Be carefull your DAQ system desn't support this type.", L"DAQmxGetDevAISupportedMeasTypes Warnings", 0);
	}


	std::cout << "cObjectmanager->getInstance()\n";
	cObjectmanager* object_manager = object_manager->getInstance();
	cPlot* m_plot = object_manager->get_plot();
	m_plot->update_chan_physical_unit_to_gui("°C", label.channel_index); // If temperature measurement update unit in channel listed in graph

	inst_->Layout();
}

void cDaqmx::OnDaqChanEnableBtn(wxCommandEvent& evt)
{
	// Enable/disable controls
	label.channel_enabled[label.channel_index] = !label.channel_enabled[label.channel_index];

	EnableChannelItems(label.channel_enabled.at(label.channel_index));
	/*
	if (!label.channel_enabled[label.channel_index])
	{
		checkchan->SetBackgroundColour(wxColor(250, 120, 120)); // RED
		checkchan->SetLabel("OFF");

		//If cPlot legend active remove it
		std::cout << "cObjectmanager->getInstance()\n";
		cObjectmanager* object_manager = object_manager->getInstance();
		cPlot* m_plot = object_manager->get_plot();
		m_plot->remove_chan_to_gui(label.channel_index);

		// Disallow editing
		chan_name->Enable(false);
		chan_addr_ctrl->Enable(false);
		meas_type_ctrl->Enable(false);
		chan_max_input_range->Enable(false);
		chan_min_input_range->Enable(false);
		chan_input_mode_type->Enable(false);
		chan_scale->Enable(false);
		chan_slope->Enable(false);
		chan_shift->Enable(false);
		chan_unit->Enable(false);
		chan_filter->Enable(false);
		chan_filter_intensity->Enable(false);
		chan_trigger->Enable(false);
		chan_trigger_threshold->Enable(false);

		// Mark button grid item as dark
		chanbtn[label.channel_index]->SetBackgroundColour(wxColor(120, 140, 120));

	}
	else
	{
		this->checkchan->SetBackgroundColour(wxColor(160, 250, 160)); // GREEN
		this->checkchan->SetLabel("ON");

		//Update cPlot gui with the chan name and color
		std::cout << "cObjectmanager->getInstance()\n";
		cObjectmanager* object_manager = object_manager->getInstance();
		cPlot* m_plot = object_manager->get_plot();
		wxString chan_name_str = chan_name->GetValue();
		wxString chan_physical_name = chan_addr_ctrl->GetValue();
		wxString chan_physical_unit = meas_type_ctrl->GetValue();
		m_plot->add_chan_to_gui(chan_name_str.ToStdString() , chan_physical_name.ToStdString(), chan_physical_unit.ToStdString(), wxColor(COLORS[label.channel_index][0] * 255, COLORS[label.channel_index][1] * 255, COLORS[label.channel_index][2] * 255), label.channel_index);

		// Allow editing
		chan_name->Enable(true);
		chan_addr_ctrl->Enable(true);
		meas_type_ctrl->Enable(true);
		chan_max_input_range->Enable(true);
		chan_min_input_range->Enable(true);
		chan_input_mode_type->Enable(true);
		chan_scale->Enable(true);
		if (chan_scale->GetValue().compare("Custom scale") == 0)
		{
			chan_slope->Enable(true);
			chan_shift->Enable(true);
			chan_unit->Enable(true);
		}
		chan_filter->Enable(true);
		if (chan_filter->GetValue().compare("Disabled") != 0)
		{
			chan_filter_intensity->Enable(true);
		}
		chan_trigger->Enable(true);
		if (chan_trigger->GetValue().compare("Disabled") != 0)
		{
			chan_trigger_threshold->Enable(true);
		}

		// Mark button grid item as green
		chanbtn[label.channel_index]->SetBackgroundColour(wxColor(160, 250, 160));

	}
	// Autosearch for channels
	*/
	evt.Skip();
}


void cDaqmx::OnChannelBtnNumberCliqued(wxCommandEvent& evt)
{
	// retrieve btn clicked
	int id = 0;
	id = evt.GetId();
	id = id - IDCCHANBTN0; // sub base ID to retrive the chan number

	// save and go to remote chan
	if (label.channel_index >= 0 and label.channel_index < (label.chan_number))
	{
		// Save current device and channels
		save_current_device_config(label.channel_index);
		save_current_chan_config(label.channel_index);

		// Mark button with old std color
		if (label.channel_enabled[label.channel_index] == true)
			chanbtn[label.channel_index]->SetBackgroundColour(wxColor(160, 250, 160)); // green
		else
			chanbtn[label.channel_index]->SetBackgroundColour(wxColor(250, 250, 250)); // grey	

		//chanbtn[label.channel_index]->SetBackgroundColour(wxColor(250, 250, 250));

		// Set new index
		label.channel_index = id;

		// Load next channel
		load_current_device_config(label.channel_index);
		load_current_chan_config(label.channel_index);

		// Mark button with new highlight color
		chanbtn[label.channel_index]->SetBackgroundColour(wxColor(120, 140, 120));
	}

	// Display the current selected channel measure type
	reload_current_channel_type();

	// If channel disabled gray it out
	EnableChannelItems(label.channel_enabled.at(label.channel_index));

	evt.Skip();
	return;
}

bool cDaqmx::isDeviceMeasurable(std::string dev_name)
{
	int32 dev_cat = 0;
	if (DAQmxGetDevProductCategory(dev_name.c_str(), &dev_cat) != 0)
	{
		//MessageBox(GetFocus(), L"[!] Warning", L"DAQmxGetDevProductCategory() failed to resolve product category.\n", S_OK);
		return false;
	}

	// DAQmx_Val_USBDAQ 									14646 	USB DAQ.
	// DAQmx_Val_CompactDAQChassis 							14658 	CompactDAQ chassis.
	// DAQmx_Val_CompactRIOChassis 							16144 	CompactRIO Chassis.
	// DAQmx_Val_CSeriesModule 								14659 	C Series I / O module.
	// DAQmx_Val_Unknown 									12588 	Unknown category.

	if (dev_cat == DAQmx_Val_Unknown)
		return false;

	if (dev_cat == DAQmx_Val_CompactDAQChassis)
		return false;

	if (dev_cat == DAQmx_Val_CompactRIOChassis)
		return false;

	if (dev_cat == DAQmx_Val_USBDAQ)
		std::cout << "[*] DAQmx_Val_USBDAQ found.\n";

	if (dev_cat == DAQmx_Val_CSeriesModule)
		std::cout << "[*] DAQmx_Val_CSeriesModule found.\n";

	return true;
}

void cDaqmx::EnableChannelItems(bool isDisplayed)
{
	if (!isDisplayed)
	{
		checkchan->SetBackgroundColour(wxColor(250, 120, 120)); // RED
		checkchan->SetLabel("OFF");

		//If cPlot legend active remove it
		std::cout << "cObjectmanager->getInstance()\n";
		cObjectmanager* object_manager = object_manager->getInstance();
		cPlot* m_plot = object_manager->get_plot();
		m_plot->remove_chan_to_gui(label.channel_index);

		// Disallow editing
		chan_name->Enable(false);
		chan_addr_ctrl->Enable(false);
		meas_type_ctrl->Enable(false);
		chan_max_input_range->Enable(false);
		chan_min_input_range->Enable(false);
		chan_input_mode_type->Enable(false);
		chan_scale->Enable(false);
		chan_slope->Enable(false);
		chan_shift->Enable(false);
		chan_unit->Enable(false);
		chan_filter->Enable(false);
		chan_filter_intensity->Enable(false);
		chan_trigger->Enable(false);
		chan_trigger_threshold->Enable(false);

		// Mark button grid item as dark
		chanbtn[label.channel_index]->SetBackgroundColour(wxColor(120, 140, 120));

	}
	else
	{
		this->checkchan->SetBackgroundColour(wxColor(160, 250, 160)); // GREEN	
		this->checkchan->SetLabel("ON");

		//Update cPlot gui with the chan name and color
		std::cout << "cObjectmanager->getInstance()\n";
		cObjectmanager* object_manager = object_manager->getInstance();
		cPlot* m_plot = object_manager->get_plot();
		wxString chan_name_str = chan_name->GetValue();
		wxString chan_physical_name = chan_addr_ctrl->GetValue();
		wxString chan_physical_unit = meas_type_ctrl->GetValue();
		m_plot->add_chan_to_gui(chan_name_str.ToStdString(), chan_physical_name.ToStdString(), chan_physical_unit.ToStdString(), wxColor(COLORS[label.channel_index][0] * 255, COLORS[label.channel_index][1] * 255, COLORS[label.channel_index][2] * 255), label.channel_index);

		// Allow editing
		chan_name->Enable(true);
		chan_addr_ctrl->Enable(true);
		meas_type_ctrl->Enable(true);
		chan_max_input_range->Enable(true);
		chan_min_input_range->Enable(true);
		chan_input_mode_type->Enable(true);
		chan_scale->Enable(true);
		if (chan_scale->GetValue().compare("Custom scale") == 0)
		{
			chan_slope->Enable(true);
			chan_shift->Enable(true);
			chan_unit->Enable(true);
		}
		chan_filter->Enable(true);
		if (chan_filter->GetValue().compare("Disabled") != 0)
		{
			chan_filter_intensity->Enable(true);
		}
		chan_trigger->Enable(true);
		if (chan_trigger->GetValue().compare("Disabled") != 0)
		{
			chan_trigger_threshold->Enable(true);
		}

		// Mark button grid item as green
		chanbtn[label.channel_index]->SetBackgroundColour(wxColor(160, 250, 160));

	}
}

void cDaqmx::show_voltage_param(bool show)
{
	static_chan_max_input_range->Show(show);
	chan_max_input_range->Show(show);
	static_chan_min_input_range->Show(show);
	chan_min_input_range->Show(show);
	static_chan_input_mode_type->Show(show);
	chan_input_mode_type->Show(show);
}
void cDaqmx::show_tc_param(bool show)
{
	static_chan_tc_type->Show(show);
	chan_tc_type->Show(show);
	static_chan_tc_min_range->Show(show);
	chan_tc_min_range->Show(show);
	static_chan_tc_max_range->Show(show);
	chan_tc_max_range->Show(show);
}

void cDaqmx::reload_current_channel_type()
{
	int ires = meas_type_ctrl->GetSelection();
	switch (ires)
	{
	case 0:
	{
		show_tc_param(false);
		show_voltage_param(true);

		break;
	}
	case 1:
	{
		show_tc_param(true);
		show_voltage_param(false);
		break;
	}
	case 2:
	{
		show_tc_param(false);
		show_voltage_param(false);
		break;
	}
	default:
		show_tc_param(false);
		show_voltage_param(true);
	}
	return;
}

wxPanel* cDaqmx::get_right_panel()
{
	return config_rightpanel_;
}

size_t cDaqmx::get_channel_index()
{
	return label.channel_index;
}

int cDaqmx::GetChannelNumber()
{
	return label.chan_number;
}

std::vector<bool> cDaqmx::GetChannelEnabledVector()
{
	return label.channel_enabled;
}

CURRENT_DEVICE_CONFIG_STRUCT cDaqmx::GetDaqConfigStruct()
{
	return config;
}






