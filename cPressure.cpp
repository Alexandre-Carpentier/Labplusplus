#include "cPressure.h"

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
#include "cPacesim.h"
#include "cPlot.h"
#include "cImagePanel.h"

cPressure::cPressure(wxWindow* inst)
{
	std::cout << "cPressure ctor...\n";
	inst_ = inst;

	////////////////////////////////////////////////////////////
	// Load default labels in memory
	////////////////////////////////////////////////////////////
	label.device_enabled = false;
	label.device_name.push_back("COM1");
	label.device_name.push_back("COM2");
	label.device_name.push_back("Simulated");

	////////////////////////////////////////////////////////////
	// Load default configuration in memory
	////////////////////////////////////////////////////////////
	config.device_enabled = false;
	config.device_name = wxT("Simulated");

	////////////////////////////////////////////////////////////

	config_rightpanel_ = new wxPanel(inst, IDC_PRESSURERIGHT_PAN, wxDefaultPosition, inst->FromDIP(wxSize(600, 600)));
	config_rightpanel_->SetBackgroundColour(wxColor(165, 165, 165)); // Make inside group box dark grey

	////////////////////////////////////////////////////////////

	wxStaticText* dummy = new wxStaticText(config_rightpanel_, wxID_ANY, "", wxDefaultPosition, inst->FromDIP(wxSize(400, 0)));

	////////////////////////////////////////////////////////////

	wxFlexGridSizer* flexsizer = new wxFlexGridSizer(2, 2, 10, 20);

	////////////////////////////////////////////////////////////

	wxStaticBox* device_group = new wxStaticBox(config_rightpanel_, wxID_ANY, "Pressure controler", wxDefaultPosition, inst->FromDIP(wxDefaultSize));
	device_group_sizer = new wxStaticBoxSizer(device_group, wxVERTICAL);

	////////////////////////////////////////////////////////////
	wxStaticText* enablepressure = new wxStaticText(device_group, IDCSTATICENABLEDAQ, L"Pace 6000:", wxDefaultPosition, inst->FromDIP(static_ctrl_size), wxNO_BORDER | wxALIGN_CENTRE_HORIZONTAL);
	enablepressure->SetFont(enablepressure->GetFont().Scale(text_size));
	enablepressure->SetBackgroundColour(*bgcolor);
	enablepressure->Hide(); // Just use enabledaq to fill the flex sizer -> hide it after creating

	////////////////////////////////////////////////////////////

	pressure_controler_activate = new wxButton(device_group, IDCPRESSUREACTIVATE, L"OFF", wxDefaultPosition, inst->FromDIP(wxSize(50, 25)), wxSUNKEN_BORDER);
	inst_->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cPressure::OnPressureEnableBtn, this, IDCPRESSUREACTIVATE);
	pressure_controler_activate->SetFont(pressure_controler_activate->GetFont().Scale(text_size));
	pressure_controler_activate->SetBackgroundColour(wxColor(250, 120, 120));

	////////////////////////////////////////////////////////////

	wxStaticText* staticaddr = new wxStaticText(device_group, IDCSTATICADDR, L"Device list:", wxDefaultPosition, inst->FromDIP(static_ctrl_size), wxNO_BORDER | wxALIGN_CENTRE_HORIZONTAL);
	staticaddr->SetFont(staticaddr->GetFont().Scale(text_size));
	//staticaddr->SetBackgroundColour(*bgcolor);

	////////////////////////////////////////////////////////////

	addr_ctrl = new wxComboBox(device_group, IDCPRESSUREADDR, label.device_name[0], wxDefaultPosition, inst->FromDIP(wxDefaultSize), label.device_name, wxCB_READONLY | wxSUNKEN_BORDER | wxBG_STYLE_TRANSPARENT, wxDefaultValidator, _T(""));
	inst_->Bind(wxEVT_COMMAND_COMBOBOX_SELECTED, &cPressure::OnPressureAddrSelBtn, this, IDCPRESSUREADDR);
	addr_ctrl->SetFont(addr_ctrl->GetFont().Scale(text_size));
	addr_ctrl->Disable();
	
	flexsizer->Add(enablepressure);
	flexsizer->Add(pressure_controler_activate);
	flexsizer->Add(staticaddr);
	flexsizer->Add(addr_ctrl);
	device_group_sizer->Add(flexsizer);

	config_rightpanel_->SetSizerAndFit(device_group_sizer);

	wxCommandEvent evt = wxCommandEvent(wxEVT_COMMAND_BUTTON_CLICKED, IDCPRESSUREACTIVATE);
	wxPostEvent(inst_, evt);
}

// Reccord device info selected in GUI field 
// and save it to an CURRENT_DEVICE_STRUCT witch hold the datas
// happen each time previous or next is pressed
void cPressure::save_current_device_config(int channel_index)
{
	std::cout << "[*] Saving device GUI field in memory at channel " << channel_index << ".\n";

	// Device enable
	config.device_enabled = false;
	if (pressure_controler_activate->GetLabelText().compare("ON") == 0)
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
void cPressure::load_current_device_config(int channel_index)
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

void cPressure::load_combobox(wxComboBox* combo, wxString str)
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

void cPressure::load_combobox(wxComboBox* combo, double floating)
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

void cPressure::OnPressureEnableBtn(wxCommandEvent& evt)
{
	// Enable/disable controls
	enable_pan = !enable_pan;
	if (pressure_controler_activate)
	{
		if (!enable_pan)
		{
			/*
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
			*/

			this->pressure_controler_activate->SetBackgroundColour(wxColor(160, 250, 160)); // GREEN	
			this->pressure_controler_activate->SetLabel("ON");

			addr_ctrl->Enable(true);			

			EnablePressureChannel(true);
		}
		else
		{
			this->pressure_controler_activate->SetBackgroundColour(wxColor(250, 120, 120)); // RED
			this->pressure_controler_activate->SetLabel("OFF");

			addr_ctrl->Enable(false);

			EnablePressureChannel(false);
		}
	}
	evt.Skip();
	
}

void cPressure::OnPressureAddrSelBtn(wxCommandEvent& evt)
{
	// Destroy and release previous ressource

	std::cout << "[*] cMeasurementmanager->getInstance()\n";
	meas_manager = meas_manager->getInstance();

	
	// Destroy item in the list
	bool isDestroyed = meas_manager->destroy_subsystem(PRESSURECONTROLER_INSTR);
	// If item destroyed delete from memory
	if (isDestroyed)
	{
		std::cout << "[*] [delete] m_daq in cPressure.cpp\n";

		delete m_pressure_;
		m_pressure_ = nullptr;
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
		if (m_pressure_ == nullptr)
		{
			std::cout << "[*] [new] Create cDaqsim\n";
			m_pressure_ = new cPacesim;
			meas_manager->set_measurement(m_pressure_);
		}
		evt.Skip();
		return;
	}

	if (m_pressure_ != nullptr)
	{
		std::cout << "[!] Failed to load: " << current << "\n";
		MessageBox(GetFocus(), L"[!] Fail", L"Object already exist", S_OK);
		evt.Skip();
		return;
	}

	// Then create appropriate DAQ object
	std::cout << "[*] [new] Create cPace6000\n";
	m_pressure_ = new cPacesim;

	// Object failed to create in memory
	if (m_pressure_ == nullptr)
	{
		std::cout << "[!] Impossible to load a pressure controler system object\n";
		MessageBox(GetFocus(), L"[!] Fail", L"Imposible to create object in memory.\n", S_OK | MB_ICONERROR);
		evt.Skip();
		return;
	}

	meas_manager->set_measurement(m_pressure_);
	m_pressure_->set_device_addr(current);
	
	evt.Skip();
	return;
}

void cPressure::EnablePressureChannel(bool isDisplayed)
{
	if (!isDisplayed)
	{
		//If cPlot legend active remove it
		std::cout << "cObjectmanager->getInstance()\n";
		cObjectmanager* object_manager = object_manager->getInstance();
		cPlot* m_plot = object_manager->get_plot();
		m_plot->remove_chan_to_gui(33); // BUG
	}
	else
	{
		//Update cPlot gui with the chan name and color
		std::cout << "cObjectmanager->getInstance()\n";
		cObjectmanager* object_manager = object_manager->getInstance();
		cPlot* m_plot = object_manager->get_plot();

		m_plot->init_chan_to_gui("Pace6000", "COM1", "Bar", wxColor(230, 200, 200));
		//m_plot->add_chan_to_gui("Pressure controler Pace 6000", addr_ctrl->GetLabelText().ToStdString(), "Bar", wxColor(45,30,30), max_chan_number+1);	
	}
}


wxPanel* cPressure::get_right_panel()
{
	return config_rightpanel_;
}

CURRENT_DEVICE_CONFIG_STRUCT cPressure::GetPressureConfigStruct()
{
	return config;
}






