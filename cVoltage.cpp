#include "cVoltage.h"
#include <wx/dcbuffer.h>
#include "enum.h"
#include "cMeasurementmanager.h"
#include "cDeviceMonitor.h"
#include "cTable.h"
#include "cSupplysim.h"
#include "cSupplyusb.h"

cVoltage::cVoltage(wxWindow* inst, std::shared_ptr<cDeviceMonitor> devmon)
{
	std::cout << "cVoltage ctor...\n";
	inst_ = inst;
	devmon_ = devmon;

	////////////////////////////////////////////////////////////
	// Load default labels in memory
	////////////////////////////////////////////////////////////
	label.device_enabled = false;
	label.device_name.push_back("Simulated");
	label.channel_permision.push_back(CHANWRITE);

	////////////////////////////////////////////////////////////
	// Load default configuration in memory
	////////////////////////////////////////////////////////////
	config.device_enabled = false;
	config.device_name = wxT("Simulated");

	////////////////////////////////////////////////////////////

	//::wxInitAllImageHandlers();
	wxBitmap bmp = wxBitmap("DCSUPPLYPNG", wxBITMAP_TYPE_PNG_RESOURCE);			// Load bmp from ressources
	voltage_instrument_img = bmp.ConvertToImage();								// Convert bmp to image to scale purpose

	voltage_instrument_rightpanel_ = new wxPanel(inst, IDC_VOLTAGERIGHT_PAN, wxDefaultPosition, inst->FromDIP(wxSize(600, 600)));
	voltage_instrument_rightpanel_->SetBackgroundColour(wxColor(165, 165, 165));		// Make inside group box dark grey
	voltage_instrument_rightpanel_->Connect(wxEVT_PAINT, wxPaintEventHandler(cVoltage::OnPaint));

	////////////////////////////////////////////////////////////

	wxStaticText* dummy = new wxStaticText(voltage_instrument_rightpanel_, wxID_ANY, "", wxDefaultPosition, inst->FromDIP(wxSize(0, 400)));

	////////////////////////////////////////////////////////////

	wxFlexGridSizer* flexsizer = new wxFlexGridSizer(2, 2, 10, 20);

	////////////////////////////////////////////////////////////

	wxStaticBox* device_group = new wxStaticBox(voltage_instrument_rightpanel_, wxID_ANY, "Voltage controler", wxDefaultPosition, inst->FromDIP(wxDefaultSize));
	device_group_sizer = new wxStaticBoxSizer(device_group, wxVERTICAL);

	////////////////////////////////////////////////////////////
	wxStaticText* enablevoltage = new wxStaticText(device_group, IDCSTATICENABLEDAQ, L"DC 2280S:", wxDefaultPosition, inst->FromDIP(static_ctrl_size), wxNO_BORDER | wxALIGN_CENTRE_HORIZONTAL);
	enablevoltage->SetFont(enablevoltage->GetFont().Scale(text_size));
	enablevoltage->SetBackgroundColour(*bgcolor);
	enablevoltage->Hide(); // Just use enabledaq to fill the flex sizer -> hide it after creating

	////////////////////////////////////////////////////////////

	voltage_controler_activate = new wxButton(device_group, IDCVOLTAGEACTIVATE, L"OFF", wxDefaultPosition, inst->FromDIP(wxSize(50, 25)), wxSUNKEN_BORDER);
	inst_->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cVoltage::OnVoltageEnableBtn, this, IDCVOLTAGEACTIVATE);
	voltage_controler_activate->SetFont(voltage_controler_activate->GetFont().Scale(text_size));
	voltage_controler_activate->SetBackgroundColour(wxColor(250, 120, 120));

	////////////////////////////////////////////////////////////

	wxStaticText* staticaddr = new wxStaticText(device_group, IDCSTATICADDR, L"Device list:", wxDefaultPosition, inst->FromDIP(static_ctrl_size), wxNO_BORDER | wxALIGN_CENTRE_HORIZONTAL);
	staticaddr->SetFont(staticaddr->GetFont().Scale(text_size));
	//staticaddr->SetBackgroundColour(*bgcolor);

	////////////////////////////////////////////////////////////

	addr_ctrl = new wxComboBox(device_group, IDCVOLTAGEADDR, label.device_name[0], wxDefaultPosition, inst->FromDIP(wxDefaultSize), label.device_name, wxCB_READONLY | wxSUNKEN_BORDER | wxBG_STYLE_TRANSPARENT, wxDefaultValidator, _T(""));
	inst_->Bind(wxEVT_COMMAND_COMBOBOX_SELECTED, &cVoltage::OnVoltageAddrSelBtn, this, IDCVOLTAGEADDR);
	addr_ctrl->SetFont(addr_ctrl->GetFont().Scale(text_size));
	RefreshPort();
	addr_ctrl->Disable();


	flexsizer->Add(enablevoltage);
	flexsizer->Add(voltage_controler_activate);
	flexsizer->Add(staticaddr);
	flexsizer->Add(addr_ctrl);
	device_group_sizer->Add(flexsizer);

	wxBoxSizer* v_sizer = new wxBoxSizer(wxVERTICAL);
	v_sizer->Add(dummy); // Space for daq img
	v_sizer->Add(device_group_sizer, 0, wxALIGN_CENTER, inst->FromDIP(10));
	voltage_instrument_rightpanel_->SetSizerAndFit(v_sizer);

	//wxCommandEvent evt0 = wxCommandEvent(wxEVT_COMMAND_BUTTON_CLICKED, IDcVoltageACTIVATE);
	//wxPostEvent(inst_, evt0);
}

cVoltage::~cVoltage()
{
	// Free memory on heap
	cMeasurementmanager* meas_manager = meas_manager->getInstance();
	meas_manager->destroy_subsystem(MEAS_TYPE::VOLTAGE_CONTROLER_INSTR);
}

void cVoltage::RefreshPort()
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
void cVoltage::save_current_device_config(int channel_index)
{
	std::cout << "[*] Saving device GUI field in memory at channel " << channel_index << ".\n";

	// Device enable
	config.device_enabled = false;
	if (voltage_controler_activate->GetLabelText().compare("ON") == 0)
	{
		config.device_enabled = true;
	}

	// Device name
	//int iSelection = addr_ctrl->GetCurrentSelection();
	//config.device_name = label.device_name[iSelection];
	config.device_name = "2280S";
	config.device_addr = addr_ctrl->GetValue();

	return;
}

// Reccord device info selected in GUI field 
// and save it to an CURRENT_DEVICE_STRUCT witch hold the datas
// happen each time previous or next is pressed
void cVoltage::load_current_device_config(int channel_index)
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

void cVoltage::load_combobox(wxComboBox* combo, wxString str)
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

void cVoltage::load_combobox(wxComboBox* combo, double floating)
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

void cVoltage::DestroySubsystem()
{
	// Destroy and release previous ressource

	std::cout << "[*] cMeasurementmanager->getInstance()\n";
	cMeasurementmanager* meas_manager = meas_manager->getInstance();
	bool isDestroyed = meas_manager->destroy_subsystem(VOLTAGE_CONTROLER_INSTR);
	// If item destroyed delete from memory
	if (isDestroyed)
	{
		std::cout << "[*] [delete] m_voltage_ in cVoltage.cpp\n";

		delete m_voltage_;
		m_voltage_ = nullptr;
	}
	return;
}

void cVoltage::set_table(cTable* m_table)
{
	assert(m_table != nullptr);
	m_table_ = m_table; // Save cTable to add or remove colomn afterward
}

void cVoltage::UpdateChannelTable(bool isDisplayed)
{
	assert(m_table_ != nullptr);
	assert(label.channel_permision.size() > 0);
	if (m_table_)
	{
		// ex: NI-DAQ_0(Volt)
		if (label.channel_permision.at(label.channel_index) == CHANWRITE) // If channel is a controler
		{
			std::string col_name = std::format("2280S_{}(Volt)", label.channel_index);
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

void cVoltage::OnVoltageEnableBtn(wxCommandEvent& evt)
{
	// Enable/disable controls
	enable_pan = !enable_pan;
	if (voltage_controler_activate)
	{
		/*
		//If cPlot legend active remove it
		std::cout << "cObjectmanager->getInstance()\n";
		cObjectmanager* object_manager = object_manager->getInstance();
		cPlot* m_plot = object_manager->get_plot();
		size_t element_nb = m_plot->get_chan_number_to_gui();
		*/

		UpdateChannelTable(enable_pan);

		if (enable_pan)
		{
			RefreshPort();

			wxCommandEvent evt = wxCommandEvent(wxEVT_COMMAND_COMBOBOX_SELECTED, IDCVOLTAGEADDR);
			wxPostEvent(inst_, evt);

			// Add a channel for voltage at the end
			//m_plot->resize_chan_number_to_gui(element_nb+1);

			cSignalTable* sigt = sigt->getInstance();

			// Remove old range
			sigt->slot_remove_range(MEAS_TYPE::VOLTAGE_CONTROLER_INSTR);

			// Add a new range
			if (!sigt->slot_register_range(2, MEAS_TYPE::VOLTAGE_CONTROLER_INSTR)) // 2 slot, I+V
			{
				MessageBox(nullptr, L"Critical error in cSignalTable, cannot register new signal range.", L"[!] Critical failure.", S_OK);
			}

			this->voltage_controler_activate->SetBackgroundColour(wxColor(160, 250, 160)); // GREEN	
			this->voltage_controler_activate->SetLabel("ON");

			addr_ctrl->Enable(true);
			EnableVoltageChannel(true);
		}
		else
		{

			//m_plot->resize_chan_number_to_gui(element_nb - 1);

			this->voltage_controler_activate->SetBackgroundColour(wxColor(250, 120, 120)); // RED
			this->voltage_controler_activate->SetLabel("OFF");

			addr_ctrl->Enable(false);

			EnableVoltageChannel(false);
		}
	}
	evt.Skip();
}

void cVoltage::OnVoltageAddrSelBtn(wxCommandEvent& evt)
{
	// Destroy and release previous ressource

	DestroySubsystem();

	// get singleton

	cMeasurementmanager* meas_manager = meas_manager->getInstance();

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
		if (m_voltage_ == nullptr)
		{
			std::cout << "[*] [new] Create cSupplysim\n";
			m_voltage_ = new cSupplysim;
			meas_manager->set_measurement(m_voltage_);
			m_voltage_->set_device_addr("Simulated");
			m_voltage_->set_device_name("2280S");
		}
		evt.Skip();
		return;
	}

	if (m_voltage_ != nullptr)
	{
		std::cout << "[!] Failed to load: " << current << "\n";
		MessageBox(GetFocus(), L"[!] Fail", L"Object already exist", S_OK);
		evt.Skip();
		return;
	}

	// Then create appropriate voltage object
	std::cout << "[*] [new] Create cSupplyusb\n";
	m_voltage_ = new cSupplyusb;

	// Object failed to be created in memory
	if (m_voltage_ == nullptr)
	{
		std::cout << "[!] Impossible to load a voltage controler system object\n";
		MessageBox(GetFocus(), L"[!] Fail", L"Imposible to create object in memory.\n", S_OK | MB_ICONERROR);
		evt.Skip();
		return;
	}
	assert(meas_manager != nullptr);
	meas_manager->set_measurement(m_voltage_);

	m_voltage_->set_device_addr(current);
	m_voltage_->set_device_name("2280S");

	evt.Skip();
	return;
}

void cVoltage::OnPaint(wxPaintEvent& event)
{
	// Load bitmap from ressource
	// consuming a lot of cpu... to fix. But wxImage not accesible from private, public member, so... unbinding here.

	wxBufferedPaintDC dc(this, wxBUFFER_CLIENT_AREA);

	// retrieving different size and proportions
	wxRect size = voltage_instrument_rightpanel_->GetRect();
	size.x = 0;
	dc.GradientFillLinear(size, wxColor(105, 105, 105), wxColor(255, 255, 255), wxUP);
	wxSize sz = voltage_instrument_img.GetSize();

	// Scale the image
	/*
	double r = size.GetHeight();
	r = r / 600.0;

	int new_width = sz.GetWidth() * r;
	pace_img = pace_img.Scale(new_width, sz.GetHeight()*r);
	*/

	int pos = (size.width / 2) - (sz.x / 2);				// Center
	dc.DrawBitmap(voltage_instrument_img, pos, 0, false);					// Draw the final bmp on context

	event.Skip();
}

void cVoltage::EnableVoltageChannel(bool isDisplayed)
{
	if (!isDisplayed)
	{
		/*
		//If cPlot legend active remove it
		std::cout << "cObjectmanager->getInstance()\n";
		cObjectmanager* object_manager = object_manager->getInstance();
		cPlot* m_plot = object_manager->get_plot();
		m_plot->remove_chan_to_gui(m_plot->gui_get_last_active_channel_number()); // BUG // remove last
		*/

		// Kill instance

		DestroySubsystem();

		std::cout << "cSignalTable->getInstance()\n";
		cSignalTable* sigt = sigt->getInstance();
		if (!sigt->sig_remove(MEAS_TYPE::VOLTAGE_CONTROLER_INSTR, 1))
		{
			MessageBox(nullptr, L"Critical error at slot_register in cSignalTable, cannot register voltage signal.", L"[!] Critical failure.", S_OK);
		}
		if (!sigt->sig_remove(MEAS_TYPE::VOLTAGE_CONTROLER_INSTR, 0))
		{
			MessageBox(nullptr, L"Critical error at slot_register in cSignalTable, cannot register voltage signal.", L"[!] Critical failure.", S_OK);
		}

		// Update signals in GUI
		std::cout << "cObjectmanager->getInstance()\n";
		cObjectmanager* object_manager = object_manager->getInstance();
		cPlot* m_plot = object_manager->get_plot();
		m_plot->update_gui();
	}
	else
	{
		/*
		//Update cPlot gui with the chan name and color
		std::cout << "cObjectmanager->getInstance()\n";
		cObjectmanager* object_manager = object_manager->getInstance();
		cPlot* m_plot = object_manager->get_plot();
		m_plot->add_chan_to_gui("Pace 6000 simulated", "Simulated", "Bar", wxColor(45, 30, 30), m_plot->gui_get_last_active_channel_number()); // add last
		*/

		std::cout << "cSignalTable->getInstance()\n";
		cSignalTable* sigt = sigt->getInstance();
		std::string instr_name = addr_ctrl->GetValue().ToStdString();
		if (!sigt->sig_add(0, MEAS_TYPE::VOLTAGE_CONTROLER_INSTR, "2280S_A", instr_name, "Amp", wxColor(25, 200, 130)))
		{
			MessageBox(nullptr, L"Critical error at slot_register in cSignalTable, cannot register voltage signal.", L"[!] Critical failure.", S_OK);
		}
		if (!sigt->sig_add(1, MEAS_TYPE::VOLTAGE_CONTROLER_INSTR, "2280S_V", instr_name, "V", wxColor(200, 27, 27)))
		{
			MessageBox(nullptr, L"Critical error at slot_register in cSignalTable, cannot register voltage signal.", L"[!] Critical failure.", S_OK);
		}

		// Update signals in GUI
		std::cout << "cObjectmanager->getInstance()\n";
		cObjectmanager* object_manager = object_manager->getInstance();
		cPlot* m_plot = object_manager->get_plot();
		m_plot->update_gui();
	}
}


wxPanel* cVoltage::get_right_panel()
{
	return voltage_instrument_rightpanel_;
}

CURRENT_DEVICE_CONFIG_STRUCT cVoltage::GetVoltageConfigStruct()
{
	return config;
}






