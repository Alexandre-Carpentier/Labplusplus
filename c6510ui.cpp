#include "c6510ui.h"
#include <wx/dcbuffer.h>
#include <wx/combobox.h>
#include <wx/treectrl.h>
#include <string>
#include <wx/fileconf.h>
#include <format>

#include "enum.h"
#include "cMeasurementmanager.h"
#include "cDeviceMonitor.h"
#include "cTable.h"

#include "c6510sim.h"
#include "c6510usb.h"

struct c6510ui::my_impl{

	my_impl(wxWindow* parent, std::shared_ptr <cDeviceMonitor> devmon, std::string instrument_name)
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
		wxBitmap bmp = wxBitmap("DAQ6510PNG", wxBITMAP_TYPE_PNG_RESOURCE);			// Load bmp from ressources
		daq6510_instrument_img = bmp.ConvertToImage();								// Convert bmp to image to scale purpose

		daq6510_instrument_rightpanel_ = new wxPanel(parent, IDC_DAQ6510RIGHT_PAN, wxDefaultPosition, parent->FromDIP(wxSize(600, 600)));
		daq6510_instrument_rightpanel_->SetBackgroundColour(wxColor(165, 165, 165));		// Make inside group box dark grey
		daq6510_instrument_rightpanel_->Connect(wxEVT_PAINT, wxPaintEventHandler(c6510ui::OnPaint));

		////////////////////////////////////////////////////////////

		wxStaticText* dummy = new wxStaticText(daq6510_instrument_rightpanel_, wxID_ANY, "", wxDefaultPosition, parent->FromDIP(wxSize(0, 400)));

		////////////////////////////////////////////////////////////

		wxFlexGridSizer* flexsizer = new wxFlexGridSizer(2, 2, 10, 20);

		////////////////////////////////////////////////////////////

		wxStaticBox* device_group = new wxStaticBox(daq6510_instrument_rightpanel_, wxID_ANY, "Daq6510 controler", wxDefaultPosition, parent->FromDIP(wxDefaultSize));
		device_group_sizer = new wxStaticBoxSizer(device_group, wxVERTICAL);

		////////////////////////////////////////////////////////////
		wxStaticText* enabledaq6510 = new wxStaticText(device_group, IDCSTATICENABLEDAQ6510, L"DC DAQ6510:", wxDefaultPosition, parent->FromDIP(static_ctrl_size), wxNO_BORDER | wxALIGN_CENTRE_HORIZONTAL);
		enabledaq6510->SetFont(enabledaq6510->GetFont().Scale(text_size));
		enabledaq6510->SetBackgroundColour(*bgcolor);
		enabledaq6510->Hide(); // Just use enabledaq to fill the flex sizer -> hide it after creating

		////////////////////////////////////////////////////////////

		daq6510_controler_activate = new wxButton(device_group, IDCDAQ6510ACTIVATE, L"OFF", wxDefaultPosition, parent->FromDIP(wxSize(50, 25)), wxSUNKEN_BORDER);
		parent->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &c6510ui::my_impl::OnDaq6510EnableBtn, this, IDCDAQ6510ACTIVATE);
		daq6510_controler_activate->SetFont(daq6510_controler_activate->GetFont().Scale(text_size));
		daq6510_controler_activate->SetBackgroundColour(wxColor(250, 120, 120));

		////////////////////////////////////////////////////////////

		wxStaticText* staticaddr = new wxStaticText(device_group, IDCSTATIC6510ADDR, L"Device list:", wxDefaultPosition, parent->FromDIP(static_ctrl_size), wxNO_BORDER | wxALIGN_CENTRE_HORIZONTAL);
		staticaddr->SetFont(staticaddr->GetFont().Scale(text_size));

		////////////////////////////////////////////////////////////

		addr_ctrl = new wxComboBox(device_group, IDCDAQ6510ADDR, label.device_name[0], wxDefaultPosition, parent->FromDIP(wxDefaultSize), label.device_name, wxCB_READONLY | wxSUNKEN_BORDER | wxBG_STYLE_TRANSPARENT, wxDefaultValidator, _T(""));
		parent->Bind(wxEVT_COMMAND_COMBOBOX_SELECTED, &c6510ui::my_impl::OnDaq6510AddrSelBtn, this, IDCDAQ6510ADDR);
		addr_ctrl->SetFont(addr_ctrl->GetFont().Scale(text_size));
		RefreshPort();
		addr_ctrl->Disable();

		flexsizer->Add(enabledaq6510);
		flexsizer->Add(daq6510_controler_activate);
		flexsizer->Add(staticaddr);
		flexsizer->Add(addr_ctrl);
		device_group_sizer->Add(flexsizer);

		wxBoxSizer* v_sizer = new wxBoxSizer(wxVERTICAL);
		v_sizer->Add(dummy); // Space for daq img
		v_sizer->Add(device_group_sizer, 0, wxALIGN_CENTER, parent->FromDIP(10));
		daq6510_instrument_rightpanel_->SetSizerAndFit(v_sizer);
	};

	~my_impl()
	{
		// not called...
		//save_table_values();
	}

	wxWindow* Parent_ = nullptr; // ref to parent object
	//wxWindow* inst_;
	cMeasurement* m_daq6510_ = nullptr;

	// Save table config
	// C:\Users\The Hive\AppData\Roaming
	std::string instrument_name_;
	std::string configfile = "";
	wxFileConfig* cfg = nullptr;

	DEVICE_CONFIG_STRUCT label;		// Control label configuration struct in memory
	CURRENT_DEVICE_CONFIG_STRUCT config; // Current selected configuration
	// Signal color map
	float COLORS[3] =
	{
		0.1f, 0.0f, 0.8f
	};

	std::shared_ptr <cDeviceMonitor> devmon_ = nullptr;
	cTable* m_table_ = nullptr;
	wxStaticBoxSizer* device_group_sizer;
	cMeasurementmanager* meas_manager = nullptr; // Measurement manager singleton

	bool enable_pan = false;
	wxButton* daq6510_controler_activate;

	wxComboBox* addr_ctrl = nullptr;

	// Style
	const float text_size = 1;
	const int STATIC_CTRL_STYLE = wxNO_BORDER | wxALIGN_CENTRE_HORIZONTAL;
	const wxSize static_ctrl_size = wxSize(120, 20);
	const int TEXT_CTRL_STYLE = wxSUNKEN_BORDER;
	const wxSize text_ctrl_size = wxSize(120, 24);
	wxColor* bgcolor = new wxColor(245, 245, 248);

	void OnDaq6510AddrSelBtn(wxCommandEvent& evt);
	void OnDaq6510EnableBtn(wxCommandEvent& evt);
	void DestroySubsystem();
	void EnableDaq6510Channel(bool isDisplayed);
	void RefreshPort();
	void UpdateChannelTable(bool isDisplayed);
	void load_table_values(void);
	void save_table_values(void);
	void load_current_device_config(int channel_index);
	void load_combobox(wxComboBox* combo, wxString str);
	void load_combobox(wxComboBox* combo, double floating);
};

c6510ui::c6510ui(wxWindow* inst, std::shared_ptr <cDeviceMonitor> devmon)
{
	std::cout << "[*] c6510ui ctor...\n";
	pimpl = std::make_unique<my_impl>(inst, devmon, "DAQ6510");
	std::cout << "[*] my_impl created...\n";
}

c6510ui::~c6510ui()
{
	std::cout << "[*] c6510ui dtor...\n";

	cMeasurementmanager* meas_manager = meas_manager->getInstance();
	meas_manager->destroy_subsystem(MEAS_TYPE::VOLTAGE_CONTROLER_INSTR);
	pimpl->save_table_values();
}

void c6510ui::my_impl::RefreshPort()
{
	std::wcout << L"[*] Refresh port called\n";

	//cDeviceMonitor* devmon = devmon->getInstance();
	std::vector<cDev> dev_list = devmon_->get_device_vec();
	addr_ctrl->Clear();

	for (auto& dev : dev_list)
	{
		std::wstring full_name = dev.get_addr();
		full_name.append(L"::");
		full_name.append(dev.get_name());
		full_name.append(L"::");
		full_name.append(dev.get_type());

		label.device_name.push_back(full_name);
		addr_ctrl->Append(full_name);
	}
	addr_ctrl->Append("Simulated");
	addr_ctrl->SetSelection(0);
}

// Reccord device info selected in GUI field 
// and save it to an CURRENT_DEVICE_STRUCT witch hold the datas
// happen each time previous or next is pressed
void c6510ui::save_current_device_config(int channel_index)
{
	std::cout << "[*] Saving device GUI field in memory at channel " << channel_index << ".\n";

	// Device enable
	pimpl->config.device_enabled = false;
	if (pimpl->daq6510_controler_activate->GetLabelText().compare("ON") == 0)
	{
		pimpl->config.device_enabled = true;
	}

	// Device name
	pimpl->config.device_name = "DAQ6510";
	pimpl->config.device_addr = pimpl->addr_ctrl->GetValue();

	return;
}

void c6510ui::lockBtn(bool lock)
{
	pimpl->device_group_sizer->GetStaticBox()->Enable(lock);
};

// Reccord device info selected in GUI field 
// and save it to an CURRENT_DEVICE_STRUCT witch hold the datas
// happen each time previous or next is pressed
void c6510ui::my_impl::load_current_device_config(int channel_index)
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

void c6510ui::my_impl::load_combobox(wxComboBox* combo, wxString str)
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

void c6510ui::my_impl::load_combobox(wxComboBox* combo, double floating)
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

void c6510ui::my_impl::DestroySubsystem()
{
	// Destroy and release previous ressource

	std::cout << "[*] cMeasurementmanager->getInstance()\n";
	cMeasurementmanager* meas_manager = meas_manager->getInstance();
	bool isDestroyed = meas_manager->destroy_subsystem(VOLTAGE_CONTROLER_INSTR);
	// If item destroyed delete from memory
	if (isDestroyed)
	{
		std::cout << "[*] [delete] m_daq6510_ in c6510ui.cpp\n";

		delete m_daq6510_;
		m_daq6510_ = nullptr;
	}
	return;
}

void c6510ui::set_table(cTable* m_table)
{
	assert(m_table != nullptr);
	pimpl->m_table_ = m_table; // Save cTable to add or remove colomn afterward

	////////////////////////////////////////////////////////////
	// Load string in cTable at startup
	////////////////////////////////////////////////////////////
	pimpl->load_table_values();
}

void c6510ui::my_impl::UpdateChannelTable(bool isDisplayed)
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

void c6510ui::my_impl::load_table_values(void)
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

void c6510ui::my_impl::save_table_values(void)
{
	std::cout << "[*] cInicfg:save_table start...\n";
	int row_count = m_table_->get_last_active_line();

	cfg = new wxFileConfig(wxEmptyString, wxEmptyString, configfile, wxEmptyString, wxCONFIG_USE_LOCAL_FILE, wxConvAuto());
	cfg->DeleteAll();
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

void c6510ui::my_impl::OnDaq6510EnableBtn(wxCommandEvent& evt)
{
	// Enable/disable controls
	enable_pan = !enable_pan;
	if (daq6510_controler_activate)
	{
		UpdateChannelTable(enable_pan);
		if (enable_pan)
		{
			RefreshPort();

			wxCommandEvent evt = wxCommandEvent(wxEVT_COMMAND_COMBOBOX_SELECTED, IDCDAQ6510ADDR);
			wxPostEvent(Parent_, evt);

			// Add a channel for daq6510 at the end

			cSignalTable* sigt = sigt->getInstance();

			// Remove old range
			sigt->slot_remove_range(MEAS_TYPE::VOLTAGE_CONTROLER_INSTR);

			// Add a new range
			if (!sigt->slot_register(MEAS_TYPE::VOLTAGE_CONTROLER_INSTR))
			{
				MessageBox(nullptr, L"Critical error in cSignalTable, cannot register new signal range.", L"[!] Critical failure.", S_OK);
			}

			daq6510_controler_activate->SetBackgroundColour(wxColor(160, 250, 160)); // GREEN	
			daq6510_controler_activate->SetLabel("ON");

			addr_ctrl->Enable(true);
			EnableDaq6510Channel(true);
		}
		else
		{
			daq6510_controler_activate->SetBackgroundColour(wxColor(250, 120, 120)); // RED
			daq6510_controler_activate->SetLabel("OFF");

			addr_ctrl->Enable(false);

			EnableDaq6510Channel(false);
		}
	}
	evt.Skip();
}

void c6510ui::my_impl::OnDaq6510AddrSelBtn(wxCommandEvent& evt)
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
		if (m_daq6510_ == nullptr)
		{
			std::cout << "[*] [new] Create c6510sim\n";
			m_daq6510_ = new c6510sim;
			meas_manager->set_measurement(m_daq6510_);
			m_daq6510_->set_device_addr("Simulated");
			m_daq6510_->set_device_name("DAQ6510");
		}
		evt.Skip();
		return;
	}

	if (m_daq6510_ != nullptr)
	{
		std::cout << "[!] Failed to load: " << current << "\n";
		MessageBox(GetFocus(), L"[!] Fail", L"Object already exist", S_OK);
		evt.Skip();
		return;
	}

	// Then create appropriate daq6510 object
	std::cout << "[*] [new] Create cSupplyusb\n";
	m_daq6510_ = new c6510usb;

	// Object failed to be created in memory
	if (m_daq6510_ == nullptr)
	{
		std::cout << "[!] Impossible to load a daq6510 controler system object\n";
		MessageBox(GetFocus(), L"[!] Fail", L"Imposible to create object in memory.\n", S_OK | MB_ICONERROR);
		evt.Skip();
		return;
	}
	assert(meas_manager != nullptr);
	meas_manager->set_measurement(m_daq6510_);

	m_daq6510_->set_device_addr(current);
	m_daq6510_->set_device_name("DAQ6510");

	evt.Skip();
	return;
}

void c6510ui::OnPaint(wxPaintEvent& event)
{
	// Load bitmap from ressource
	// consuming a lot of cpu... to fix. But wxImage not accesible from private, public member, so... unbinding here.

	wxBufferedPaintDC dc(this, wxBUFFER_CLIENT_AREA);

	// retrieving different size and proportions
	wxRect size = daq6510_instrument_rightpanel_->GetRect();
	size.x = 0;
	dc.GradientFillLinear(size, wxColor(105, 105, 105), wxColor(255, 255, 255), wxUP);
	wxSize sz = daq6510_instrument_img.GetSize();

	// Scale the image
	/*
	double r = size.GetHeight();
	r = r / 600.0;

	int new_width = sz.GetWidth() * r;
	pace_img = pace_img.Scale(new_width, sz.GetHeight()*r);
	*/

	int pos = (size.width / 2) - (sz.x / 2);				// Center
	dc.DrawBitmap(daq6510_instrument_img, pos, 0, false);					// Draw the final bmp on context

	event.Skip();
}

void c6510ui::my_impl::EnableDaq6510Channel(bool isDisplayed)
{
	if (!isDisplayed)
	{

		// Kill instance
		DestroySubsystem();

		std::cout << "cSignalTable->getInstance()\n";
		cSignalTable* sigt = sigt->getInstance();
		if (!sigt->sig_remove(MEAS_TYPE::VOLTAGE_CONTROLER_INSTR, 0))
		{
			MessageBox(nullptr, L"Critical error at slot_register in cSignalTable, cannot register daq6510 signal.", L"[!] Critical failure.", S_OK);
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
		if (!sigt->sig_add(0, MEAS_TYPE::VOLTAGE_CONTROLER_INSTR, "DAQ6510", instr_name, "Volt", wxColor(45, 30, 30)))
		{
			MessageBox(nullptr, L"Critical error at slot_register in cSignalTable, cannot register daq6510 signal.", L"[!] Critical failure.", S_OK);
		}

		// Update signals in GUI
		std::cout << "cObjectmanager->getInstance()\n";
		cObjectmanager* object_manager = object_manager->getInstance();
		cPlot* m_plot = object_manager->get_plot();
		m_plot->update_gui();
	}
}

wxPanel* c6510ui::get_right_panel()
{
	return daq6510_instrument_rightpanel_;
}

CURRENT_DEVICE_CONFIG_STRUCT c6510ui::GetDaq6510ConfigStruct()
{
	return pimpl->config;
}

size_t c6510ui::launch_device()
{
	if (pimpl->m_daq6510_ == nullptr)
	{
		return -1;
	}

	// Clean address
	std::size_t pos = 0;
	for (size_t i = 0; i < 4; i++)
	{
		pos += pimpl->config.device_addr.find("::", pos); // Find end of correct addr
	}
	pimpl->config.device_addr = pimpl->config.device_addr.substr(0, pos - 1); // resize


	pimpl->m_daq6510_->set_configuration_struct(pimpl->config);
	return pimpl->m_daq6510_->launch_device();
}





