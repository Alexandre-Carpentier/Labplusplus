/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
/*
#include "instrumentimpl.h"
#include "IInstrument.h"
#include "cMeasurementmanager.h"
#include "wx/wx.h"
#include "wx/window.h"

IInstrument::Instrumentimpl(wxWindow* parent, std::shared_ptr<cDeviceMonitor> devmon, std::wstring instrument_name)
{
	Parent_ = parent;
	devmon_ = devmon;
	instrument_name_ = instrument_name;
	configfile = instrument_name + ".ini";

	////////////////////////////////////////////////////////////
	// Load default labels in memory
	////////////////////////////////////////////////////////////
	label.device_enabled = false;
	label.device_name.push_back("Simulated");
	label.channel_permision.push_back(CHANWRITE);
	label.channel_index = 0;

	////////////////////////////////////////////////////////////
	// Load default configuration in memory
	////////////////////////////////////////////////////////////
	config.device_enabled = false;
	config.device_name = wxT("Simulated");

	////////////////////////////////////////////////////////////
	wxBitmap bmp = wxBitmap("DCOSCOPE", wxBITMAP_TYPE_PNG_RESOURCE);			// Load bmp from ressources
	oscope_instrument_img = bmp.ConvertToImage();								// Convert bmp to image to scale purpose

	oscope_instrument_rightpanel_ = new wxPanel(parent, IDC_OSCOPERIGHT_PAN, wxDefaultPosition, parent->FromDIP(wxSize(600, 600)));
	oscope_instrument_rightpanel_->SetBackgroundColour(wxColor(165, 165, 165));		// Make inside group box dark grey
	oscope_instrument_rightpanel_->Connect(wxEVT_PAINT, wxPaintEventHandler(cOscope::OnPaint));

	////////////////////////////////////////////////////////////

	wxStaticText* dummy = new wxStaticText(oscope_instrument_rightpanel_, wxID_ANY, "", wxDefaultPosition, parent->FromDIP(wxSize(0, 400)));

	////////////////////////////////////////////////////////////

	wxFlexGridSizer* flexsizer = new wxFlexGridSizer(2, 2, 10, 20);

	////////////////////////////////////////////////////////////

	wxStaticBox* device_group = new wxStaticBox(oscope_instrument_rightpanel_, wxID_ANY, "Oscope controler", wxDefaultPosition, parent->FromDIP(wxDefaultSize));
	device_group_sizer = new wxStaticBoxSizer(device_group, wxVERTICAL);

	////////////////////////////////////////////////////////////
	wxStaticText* enableoscope = new wxStaticText(device_group, IDCSTATICENABLEDAQ, L"DC DSOX1202G:", wxDefaultPosition, parent->FromDIP(static_ctrl_size), wxNO_BORDER | wxALIGN_CENTRE_HORIZONTAL);
	enableoscope->SetFont(enableoscope->GetFont().Scale(text_size));
	enableoscope->SetBackgroundColour(*bgcolor);
	enableoscope->Hide(); // Just use enabledaq to fill the flex sizer -> hide it after creating

	////////////////////////////////////////////////////////////

	oscope_controler_activate = new wxButton(device_group, IDCOSCOPEACTIVATE, L"OFF", wxDefaultPosition, parent->FromDIP(wxSize(50, 25)), wxSUNKEN_BORDER);
	parent->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cOscope::Oscopeimpl::OnOscopeEnableBtn, this, IDCOSCOPEACTIVATE);
	oscope_controler_activate->SetFont(oscope_controler_activate->GetFont().Scale(text_size));
	oscope_controler_activate->SetBackgroundColour(wxColor(250, 120, 120));

	////////////////////////////////////////////////////////////

	wxStaticText* staticaddr = new wxStaticText(device_group, IDCSTATICADDR, L"Device list:", wxDefaultPosition, parent->FromDIP(static_ctrl_size), wxNO_BORDER | wxALIGN_CENTRE_HORIZONTAL);
	staticaddr->SetFont(staticaddr->GetFont().Scale(text_size));

	////////////////////////////////////////////////////////////

	addr_ctrl = new wxComboBox(device_group, IDCOSCOPEADDR, label.device_name[0], wxDefaultPosition, parent->FromDIP(wxDefaultSize), label.device_name, wxCB_READONLY | wxSUNKEN_BORDER | wxBG_STYLE_TRANSPARENT, wxDefaultValidator, _T(""));
	parent->Bind(wxEVT_COMMAND_COMBOBOX_SELECTED, &cOscope::Oscopeimpl::OnOscopeAddrSelBtn, this, IDCOSCOPEADDR);
	addr_ctrl->SetFont(addr_ctrl->GetFont().Scale(text_size));
	RefreshPort();
	addr_ctrl->Disable();

	flexsizer->Add(enableoscope);
	flexsizer->Add(oscope_controler_activate);
	flexsizer->Add(staticaddr);
	flexsizer->Add(addr_ctrl);
	device_group_sizer->Add(flexsizer);

	wxBoxSizer* v_sizer = new wxBoxSizer(wxVERTICAL);
	v_sizer->Add(dummy); // Space for daq img
	v_sizer->Add(device_group_sizer, 0, wxALIGN_CENTER, parent->FromDIP(10));
	oscope_instrument_rightpanel_->SetSizerAndFit(v_sizer);
};

~IInstrument::Oscopeimpl()
{
	// not called...
	//save_table_values();
}

void IInstrument::Oscopeimpl::RefreshPort()
{
	std::wcout << L"[*] Refresh port called\n";

	//cDeviceMonitor* devmon = devmon->getInstance();
	std::vector<cDev> dev_list = devmon_->get_device_vec();
	addr_ctrl->Clear();

	for (auto& dev : dev_list)
	{
		addr_ctrl->Append(dev.get_addr());
	}
	addr_ctrl->Append("Simulated");
	addr_ctrl->SetSelection(0);
}




// Reccord device info selected in GUI field 
// and save it to an CURRENT_DEVICE_STRUCT witch hold the datas
// happen each time previous or next is pressed
void IInstrument::Oscopeimpl::load_current_device_config(int channel_index)
{
	std::cout << "[*] Loading device configuration from memory in GUI at channel " << channel_index << ".\n";

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

void IInstrument::Oscopeimpl::load_combobox(wxComboBox* combo, wxString str)
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

void IInstrument::Oscopeimpl::load_combobox(wxComboBox* combo, double floating)
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

void IInstrument::Oscopeimpl::DestroySubsystem()
{
	// Destroy and release previous ressource

	std::cout << "[*] cMeasurementmanager->getInstance()\n";
	cMeasurementmanager* meas_manager = meas_manager->getInstance();
	bool isDestroyed = meas_manager->destroy_subsystem(VOLTAGE_CONTROLER_INSTR);
	// If item destroyed delete from memory
	if (isDestroyed)
	{
		std::cout << "[*] [delete] m_oscope_ in cOscope.cpp\n";

		delete m_oscope_;
		m_oscope_ = nullptr;
	}
	return;
}



void IInstrument::Oscopeimpl::UpdateChannelTable(bool isDisplayed)
{
	assert(m_table_ != nullptr);
	assert(label.channel_permision.size() > 0);
	assert(label.channel_index >= 0);
	assert(label.channel_index < 64);

	if (m_table_)
	{
		// ex: DSOX_0(Volt)
		if (label.channel_permision.at(label.channel_index) == CHANWRITE) // If channel is a controler
		{
			std::string col_name = std::format("DSOX_{}(Volt)", label.channel_index);
			std::cout << "[*] Enabling controler in cTable colomn at: \"" << col_name << "\"\n";

			if (isDisplayed == true)
			{
				m_table_->enable_device_col(col_name);
			}
			else if (isDisplayed == false)
			{
				m_table_->disable_device_col(col_name);
			}
		}
	}
}

void IInstrument::Oscopeimpl::load_table_values(void)
{
	std::cout << "[*] cInicfg::read_table start...\n";

	// Save in : // C:\Users\The Hive\AppData\Roaming

	wxString str;

	// Scan until the MAX_TABLE_LINE !
	const int MAX_TABLE_LINE = 200;

	cfg = new wxFileConfig(wxEmptyString, wxEmptyString, configfile, wxEmptyString, wxCONFIG_USE_LOCAL_FILE, wxConvAuto());
	cfg->SetPath(wxT("TABLE"));

	for (int line = 0; line < MAX_TABLE_LINE; line++)
	{
		int col_count = m_table_->grid->GetNumberCols();
		for (int col = 0; col < col_count; col++)
		{
			std::string col_name = m_table_->grid->GetColLabelValue(col).ToStdString();
			if (col_name.find(instrument_name_) != std::string::npos)
			{
				std::string col_name_position = std::format("{}_{}", col_name, line);
				if (cfg->Read(col_name_position, &str)) { m_table_->grid->SetCellValue(line, col, str); }
			}
		}
	}
	delete cfg;
}

void IInstrument::Oscopeimpl::save_table_values(void)
{
	std::cout << "[*] cInicfg:save_table start...\n";
	int row_count = m_table_->get_last_active_line();

	cfg = new wxFileConfig(wxEmptyString, wxEmptyString, configfile, wxEmptyString, wxCONFIG_USE_LOCAL_FILE, wxConvAuto());
	cfg->SetPath(wxT("TABLE"));

	wxString str;
	for (int line = 0; line < row_count; line++)
	{
		int col_count = m_table_->grid->GetNumberCols();
		for (int col = 0; col < col_count; col++)
		{
			std::string col_name = m_table_->grid->GetColLabelValue(col).ToStdString();
			size_t exist = col_name.find(instrument_name_);
			if (exist != std::string::npos)
			{
				std::string value = m_table_->grid->GetCellValue(line, col).ToStdString();
				if (value.size() > 0)
				{
					std::string col_name_position = std::format("{}_{}", col_name, line);
					cfg->Write(col_name_position, std::stof(value));
				}
			}
		}
	}
	delete cfg;
}

void IInstrument::Oscopeimpl::OnOscopeEnableBtn(wxCommandEvent& evt)
{
	// Enable/disable controls
	enable_pan = !enable_pan;
	if (oscope_controler_activate)
	{
		UpdateChannelTable(enable_pan);
		if (enable_pan)
		{
			RefreshPort();

			wxCommandEvent evt = wxCommandEvent(wxEVT_COMMAND_COMBOBOX_SELECTED, IDCOSCOPEADDR);
			wxPostEvent(Parent_, evt);

			// Add a channel for oscope at the end

			cSignalTable* sigt = sigt->getInstance();

			// Remove old range
			sigt->slot_remove_range(MEAS_TYPE::VOLTAGE_CONTROLER_INSTR);

			// Add a new range
			if (!sigt->slot_register(MEAS_TYPE::VOLTAGE_CONTROLER_INSTR))
			{
				MessageBox(nullptr, L"Critical error in cSignalTable, cannot register new signal range.", L"[!] Critical failure.", S_OK);
			}

			oscope_controler_activate->SetBackgroundColour(wxColor(160, 250, 160)); // GREEN	
			oscope_controler_activate->SetLabel("ON");

			addr_ctrl->Enable(true);
			EnableOscopeChannel(true);
		}
		else
		{
			oscope_controler_activate->SetBackgroundColour(wxColor(250, 120, 120)); // RED
			oscope_controler_activate->SetLabel("OFF");

			addr_ctrl->Enable(false);

			EnableOscopeChannel(false);
		}
	}
	evt.Skip();
}

void IInstrument::Oscopeimpl::OnOscopeAddrSelBtn(wxCommandEvent& evt)
{
	// Destroy and release previous ressource

	DestroySubsystem();

	// get singleton

	cMeasurementmanager* meas_manager = meas_manager->getInstance();

	// Read the new sub system name selected
	wxString current_device = addr_ctrl->GetValue();
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
		if (m_oscope_ == nullptr)
		{
			std::cout << "[*] [new] Create cSupplysim\n";
			//m_oscope_ = new cOscopesim;
			meas_manager->set_measurement(m_oscope_);
			m_oscope_->set_device_addr("Simulated");
			m_oscope_->set_device_name("DSOX1202G");
		}
		evt.Skip();
		return;
	}

	if (m_oscope_ != nullptr)
	{
		std::cout << "[!] Failed to load: " << current << "\n";
		MessageBox(GetFocus(), L"[!] Fail", L"Object already exist", S_OK);
		evt.Skip();
		return;
	}

	// Then create appropriate oscope object
	std::cout << "[*] [new] Create cSupplyusb\n";
	m_oscope_ = new cOscopeusb;

	// Object failed to be created in memory
	if (m_oscope_ == nullptr)
	{
		std::cout << "[!] Impossible to load a oscope controler system object\n";
		MessageBox(GetFocus(), L"[!] Fail", L"Imposible to create object in memory.\n", S_OK | MB_ICONERROR);
		evt.Skip();
		return;
	}
	assert(meas_manager != nullptr);
	meas_manager->set_measurement(m_oscope_);

	m_oscope_->set_device_addr(current);
	m_oscope_->set_device_name("DSOX1202G");

	evt.Skip();
	return;
}

void IInstrument::OnPaint(wxPaintEvent& event)
{
	// Load bitmap from ressource
	// consuming a lot of cpu... to fix. But wxImage not accesible from private, public member, so... unbinding here.

	wxBufferedPaintDC dc(this, wxBUFFER_CLIENT_AREA);

	// retrieving different size and proportions
	wxRect size = oscope_instrument_rightpanel_->GetRect();
	size.x = 0;
	dc.GradientFillLinear(size, wxColor(105, 105, 105), wxColor(255, 255, 255), wxUP);
	wxSize sz = oscope_instrument_img.GetSize();

	// Scale the image

	//double r = size.GetHeight();
	//r = r / 600.0;

	//int new_width = sz.GetWidth() * r;
	//pace_img = pace_img.Scale(new_width, sz.GetHeight()*r);
	

	int pos = (size.width / 2) - (sz.x / 2);				// Center
	dc.DrawBitmap(oscope_instrument_img, pos, 0, false);					// Draw the final bmp on context

	event.Skip();
}

void IInstrument::Oscopeimpl::EnableOscopeChannel(bool isDisplayed)
{
	if (!isDisplayed)
	{

		// Kill instance
		DestroySubsystem();

		std::cout << "cSignalTable->getInstance()\n";
		cSignalTable* sigt = sigt->getInstance();
		if (!sigt->sig_remove(MEAS_TYPE::VOLTAGE_CONTROLER_INSTR, 0))
		{
			MessageBox(nullptr, L"Critical error at slot_register in cSignalTable, cannot register oscope signal.", L"[!] Critical failure.", S_OK);
		}

		// Update signals in GUI
		std::cout << "cObjectmanager->getInstance()\n";
		cObjectmanager* object_manager = object_manager->getInstance();
		cPlot* m_plot = object_manager->get_plot();
		m_plot->update_gui();
	}
	else
	{
		std::cout << "cSignalTable->getInstance()\n";
		cSignalTable* sigt = sigt->getInstance();
		std::string instr_name = addr_ctrl->GetValue().ToStdString();
		if (!sigt->sig_add(0, MEAS_TYPE::VOLTAGE_CONTROLER_INSTR, "DSOX1202G", instr_name, "Volt", wxColor(45, 30, 30)))
		{
			MessageBox(nullptr, L"Critical error at slot_register in cSignalTable, cannot register oscope signal.", L"[!] Critical failure.", S_OK);
		}

		// Update signals in GUI
		std::cout << "cObjectmanager->getInstance()\n";
		cObjectmanager* object_manager = object_manager->getInstance();
		cPlot* m_plot = object_manager->get_plot();
		m_plot->update_gui();
	}
}
*/