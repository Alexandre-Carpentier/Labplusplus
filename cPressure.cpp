#include "cPressure.h"

#include <wx/wx.h>
#include <wx/combobox.h>
#include <wx/dcbuffer.h>
#include <format>
#include <locale>

#include "enum.h"

#include "cMeasurementControler.h"
#include "cMeasurement.h"
#include "cObjectmanager.h"
#include "cMeasurementmanager.h"
#include "cCycle.h"
#include "cPlot.h"
#include "cImagePanel.h"

#include "cDeviceMonitor.h"
#include "cPacesim.h"
#include "cPacecom.h"



cPressure::cPressure(wxWindow* inst)
{
	std::cout << "cPressure ctor...\n";
	inst_ = inst;

	////////////////////////////////////////////////////////////
	// Load default labels in memory
	////////////////////////////////////////////////////////////
	label.device_enabled = false;	
	label.device_name.push_back("Simulated");

	////////////////////////////////////////////////////////////
	// Load default configuration in memory
	////////////////////////////////////////////////////////////
	config.device_enabled = false;
	config.device_name = wxT("Simulated");

	////////////////////////////////////////////////////////////



	::wxInitAllImageHandlers();
	wxBitmap bmp = wxBitmap("PACEPNG", wxBITMAP_TYPE_PNG_RESOURCE);			// Load bmp from ressources
	pace_img = bmp.ConvertToImage();										// Convert bmp to image to scale purpose

	pace_rightpanel_ = new wxPanel(inst, IDC_PRESSURERIGHT_PAN, wxDefaultPosition, inst->FromDIP(wxSize(600, 600)));
	pace_rightpanel_->SetBackgroundColour(wxColor(165, 165, 165));		// Make inside group box dark grey
	pace_rightpanel_->Connect(wxEVT_PAINT, wxPaintEventHandler(cPressure::OnPaint));

	////////////////////////////////////////////////////////////

	wxStaticText* dummy = new wxStaticText(pace_rightpanel_, wxID_ANY, "", wxDefaultPosition, inst->FromDIP(wxSize(0, 400)));

	////////////////////////////////////////////////////////////

	wxFlexGridSizer* flexsizer = new wxFlexGridSizer(2, 2, 10, 20);

	////////////////////////////////////////////////////////////

	wxStaticBox* device_group = new wxStaticBox(pace_rightpanel_, wxID_ANY, "Pressure controler", wxDefaultPosition, inst->FromDIP(wxDefaultSize));
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
	RefreshPort();
	addr_ctrl->Disable();
	
	
	flexsizer->Add(enablepressure);
	flexsizer->Add(pressure_controler_activate);
	flexsizer->Add(staticaddr);
	flexsizer->Add(addr_ctrl);
	device_group_sizer->Add(flexsizer);

	wxBoxSizer* v_sizer = new wxBoxSizer(wxVERTICAL);
	v_sizer->Add(dummy); // Space for daq img
	v_sizer->Add(device_group_sizer, 0, wxALIGN_CENTER, inst->FromDIP(10));
	pace_rightpanel_->SetSizerAndFit(v_sizer);

	//wxCommandEvent evt0 = wxCommandEvent(wxEVT_COMMAND_BUTTON_CLICKED, IDCPRESSUREACTIVATE);
	//wxPostEvent(inst_, evt0);
}

cPressure::~cPressure()
{
	if (m_pressure_ != nullptr)
	{
		delete m_pressure_;
	}
}

void cPressure::RefreshPort()
{
	std::wcout << L"[*] Refresh port called\n";

	cDeviceMonitor* devmon = devmon->getInstance();
	std::vector<cDev> dev_list = devmon->get_device_vec();
	addr_ctrl->Clear();

	for (auto& dev : dev_list)
	{
		//std::wstring wname = dev.get_name();
		//if (wname.compare(L"Unknown") != 0)
		//{
			addr_ctrl->Append(dev.get_addr());		
			//}
	}
	addr_ctrl->Append("Simulated");
	addr_ctrl->SetSelection(0);
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
	config.device_name = "Pace 6000";
	config.device_addr = addr_ctrl->GetValue();

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
		/*
		//If cPlot legend active remove it
		std::cout << "cObjectmanager->getInstance()\n";
		cObjectmanager* object_manager = object_manager->getInstance();
		cPlot* m_plot = object_manager->get_plot();
		size_t element_nb = m_plot->get_chan_number_to_gui();
		*/


		if (enable_pan)
		{
			RefreshPort();

			wxCommandEvent evt = wxCommandEvent(wxEVT_COMMAND_COMBOBOX_SELECTED, IDCPRESSUREADDR);
			wxPostEvent(inst_, evt);

			// Add a channel for pressure at the end
			//m_plot->resize_chan_number_to_gui(element_nb+1);

			cSignalTable* sigt = sigt->getInstance();

			// Remove old range
			sigt->slot_remove_range(MEAS_TYPE::PRESSURECONTROLER_INSTR);

			// Add a new range
			if (!sigt->slot_register(MEAS_TYPE::PRESSURECONTROLER_INSTR))
			{
				MessageBox(nullptr, L"Critical error in cSignalTable, cannot register new signal range.", L"[!] Critical failure.", S_OK);
			}

			this->pressure_controler_activate->SetBackgroundColour(wxColor(160, 250, 160)); // GREEN	
			this->pressure_controler_activate->SetLabel("ON");

			addr_ctrl->Enable(true);			

			EnablePressureChannel(true);
		}
		else
		{

			//m_plot->resize_chan_number_to_gui(element_nb - 1);

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
			std::cout << "[*] [new] Create cPacesim\n";
			m_pressure_ = new cPacesim;
			meas_manager->set_measurement(m_pressure_);
			m_pressure_->set_device_addr("Simulated");
			m_pressure_->set_device_name("Pace 6000");
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

	// Then create appropriate pressure object
	std::cout << "[*] [new] Create cPacecom\n";
	m_pressure_ = new cPacecom;

	// Object failed to be created in memory
	if (m_pressure_ == nullptr)
	{
		std::cout << "[!] Impossible to load a pressure controler system object\n";
		MessageBox(GetFocus(), L"[!] Fail", L"Imposible to create object in memory.\n", S_OK | MB_ICONERROR);
		evt.Skip();
		return;
	}

	meas_manager->set_measurement(m_pressure_);
	m_pressure_->set_device_addr(current);
	m_pressure_->set_device_name("Pace 6000");
	
	evt.Skip();
	return;
}

void cPressure::OnPaint(wxPaintEvent& event)
{
	// Load bitmap from ressource
	// consuming a lot of cpu... to fix. But wxImage not accesible from private, public member, so... unbinding here.

	wxBufferedPaintDC dc(this, wxBUFFER_CLIENT_AREA);

	// retrieving different size and proportions
	wxRect size = pace_rightpanel_->GetRect();
	size.x = 0;
	dc.GradientFillLinear(size, wxColor(105, 105, 105), wxColor(255, 255, 255), wxUP);
	wxSize sz = pace_img.GetSize();

	// Scale the image
	/*
	double r = size.GetHeight();
	r = r / 600.0;

	int new_width = sz.GetWidth() * r;
	pace_img = pace_img.Scale(new_width, sz.GetHeight()*r);
	*/

	int pos = (size.width / 2) - (sz.x / 2);				// Center
	dc.DrawBitmap(pace_img, pos, 0, false);					// Draw the final bmp on context

	event.Skip();
}

void cPressure::EnablePressureChannel(bool isDisplayed)
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

		std::cout << "cSignalTable->getInstance()\n";
		cSignalTable* sigt = sigt->getInstance();
		if (!sigt->sig_remove(MEAS_TYPE::PRESSURECONTROLER_INSTR, 0))
		{
			MessageBox(nullptr, L"Critical error at slot_register in cSignalTable, cannot register pressure signal.", L"[!] Critical failure.", S_OK);
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
		if (!sigt->sig_add(0, MEAS_TYPE::PRESSURECONTROLER_INSTR, "Pace 6000", instr_name, "Bar", wxColor(45, 30, 30)))
		{
			MessageBox(nullptr, L"Critical error at slot_register in cSignalTable, cannot register pressure signal.", L"[!] Critical failure.", S_OK);
		}

		// Update signals in GUI
		std::cout << "cObjectmanager->getInstance()\n";
		cObjectmanager* object_manager = object_manager->getInstance();
		cPlot* m_plot = object_manager->get_plot();
		m_plot->update_gui();
	}
}


wxPanel* cPressure::get_right_panel()
{
	return pace_rightpanel_;
}

CURRENT_DEVICE_CONFIG_STRUCT cPressure::GetPressureConfigStruct()
{
	return config;
}






