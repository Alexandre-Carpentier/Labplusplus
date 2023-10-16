#include "cFooter.h"

#include "cPlot.h"
#include "cConfig.h"
#include "cTable.h"
#include "cCycle.h"
#include "cCycleControler.h"
#include "cObjectmanager.h"
#include "cMeasurementmanager.h"
#include "cMeasurementControler.h"
#include "cDaqmx.h"
#include "cUsb6001.h"
#include "cScaleBtn.h"

cFooter::cFooter(wxWindow* inst, cPlot* m_plot, cTable* m_table, cConfig* m_config)
{
	std::cout << "cFooter ctor...\n";
	inst_ = inst;
	m_plot_ = m_plot;
	m_table_ = m_table;
	m_config_ = m_config;

	std::cout << "cObjectmanager->getInstance()\n";
	obj_manager = obj_manager->getInstance();
	std::cout << "cMeasurementmanager->getInstance()\n";
	meas_manager = meas_manager->getInstance();

	hfootersizer = new wxBoxSizer(wxHORIZONTAL);

	startbtn = new wxButton(inst_, IDC_STARTBTN, L"Start", wxDefaultPosition, inst->FromDIP(wxSize(200, 25)));
	startbtn->SetBackgroundColour(wxColor(180, 250, 90));
	inst->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cFooter::startButtonClicked, this, IDC_STARTBTN);

	staticratetxt = new wxStaticText(inst_, IDC_RATETXT, L"Read rate:", wxDefaultPosition, inst->FromDIP(wxSize(70, 25)), wxTE_CENTER);
	staticratetxt->SetBackgroundColour(wxColor(240, 245, 255));

	ratetxt = new wxTextCtrl(inst_, IDC_RATETXTCTRL, L"0 ms", wxDefaultPosition, inst->FromDIP(wxSize(80, 25)));


	wxStaticText* staticfiltertxt = new wxStaticText(inst_, IDC_FILTERTXT, L"Filter:", wxDefaultPosition, inst->FromDIP(wxSize(70, 25)), wxTE_CENTER);
	staticfiltertxt->SetBackgroundColour(wxColor(240, 245, 250));
	staticfiltertxt->Hide();

	wxArrayString   m_arrItems1;
	m_arrItems1.Add(wxT("None"));
	m_arrItems1.Add(wxT("Hanning"));
	m_arrItems1.Add(wxT("Besel"));
	m_arrItems1.Add(wxT("EMA"));
	combo1 = new wxComboBox(inst_, IDC_FILTERCOMBO, L"None", wxDefaultPosition, inst->FromDIP(wxSize(100, 25)), m_arrItems1, wxCB_READONLY, wxDefaultValidator, _T("ID_COMBOBOX1"));
	combo1->SetBackgroundColour(wxColor(240, 245, 255));
	inst->Bind(wxEVT_COMMAND_COMBOBOX_CLOSEUP, &cFooter::filterButtonClicked, this, IDC_FILTERCOMBO);
	combo1->Hide();

	wxScaleButton* scale_btn = new wxScaleButton((wxFrame*)inst_, wxID_ANY);

	wxStaticText* staticrectxt = new wxStaticText(inst_, IDC_RECTXT, L"Rec:", wxDefaultPosition, inst->FromDIP(wxSize(70, 25)), wxTE_CENTER);
	staticrectxt->SetBackgroundColour(wxColor(240, 245, 250));

	wxArrayString   m_arrItems2;
	m_arrItems2.Add(wxT("None"));
	m_arrItems2.Add(wxT("Ascii"));
	m_arrItems2.Add(wxT("TDMS"));
	m_arrItems2.Add(wxT("Xlsx"));
	combo2 = new wxComboBox(inst_, IDC_RECCOMBO, L"Ascii", wxDefaultPosition, inst->FromDIP(wxSize(100, 25)), m_arrItems2, wxCB_READONLY, wxDefaultValidator, _T("ID_COMBOBOX1"));
	combo2->SetBackgroundColour(wxColor(240, 245, 255));

	wxStaticText* staticfreqtxt = new wxStaticText(inst_, IDC_FREQTXT, L"freq (ms):", wxDefaultPosition, inst->FromDIP(wxSize(60, 25)), wxTE_CENTER);
	staticfreqtxt->SetBackgroundColour(wxColor(240, 245, 250));

	freq = new wxTextCtrl(inst_, IDC_FREQTEXTCTRL, L"100", wxDefaultPosition, inst->FromDIP(wxSize(30, 25)));
	freq->SetBackgroundColour(wxColor(255, 255, 255));
	inst->Bind(wxEVT_COMMAND_TEXT_UPDATED, &cFooter::freqButtonClicked, this, IDC_FREQTEXTCTRL);

	hfootersizer->Add(staticfreqtxt);
	hfootersizer->Add(freq, 1, wxEXPAND);
	hfootersizer->Add(staticrectxt);
	hfootersizer->Add(combo2, 1, wxEXPAND);
	//hfootersizer->Add(staticfiltertxt);
	//hfootersizer->Add(combo1, 1, wxEXPAND);
	hfootersizer->Add(scale_btn, 0, wxEXPAND);
	hfootersizer->Add(startbtn);
	hfootersizer->Add(staticratetxt);
	hfootersizer->Add(ratetxt);
}

wxBoxSizer* cFooter::GetSizer()
{
	return this->hfootersizer;
}

void cFooter::startButtonClicked(wxCommandEvent& evt)
{

	size_t measurement_number = meas_manager->get_measurement_pool_size();
	std::cout << "[*] " << measurement_number << " measurements are in pool.\n";
	if (measurement_number == 0)
	{
		MessageBox(GetFocus(), L"No active device\ntry to enable one instrument first", L"Error", S_OK | MB_ICONERROR);
		evt.Skip();
		return;
	}
	if (m_plot_->get_graph_state() == false)
	{

		cDaqmx* daqconfig = obj_manager->get_daqmx();
		if (daqconfig->m_daq_ == nullptr)
		{
			MessageBox(GetFocus(), L"No device loaded", 0, S_OK | MB_ICONERROR);
			evt.Skip();
			return;
		}

		cCycle* m_cycle = nullptr;
		m_cycle = m_table_->load_cycle();
		if (m_cycle == nullptr)
		{
			MessageBox(GetFocus(), L"Fill the cycle table please", L"Error", S_OK | MB_ICONERROR);
		}
		else
		{
			cycle_controler = new cCycleControler(m_cycle, m_table_, inst_);

			int iFilter = this->combo1->GetCurrentSelection();
			FILTER_M Filtering = FILTER_NONE;
			switch (iFilter)
			{
			case 0:
			{
				Filtering = FILTER_NONE;
				break;
			}
			case 1:
			{
				Filtering = FILTER_HANNING;
				break;
			}
			case 2:
			{
				Filtering = FILTER_BESEL;
				break;
			}
			case 3:
			{
				Filtering = FILTER_EMA;
				break;
			}
			}

			int iRec = this->combo2->GetCurrentSelection();
			LOGGER_M Rec = LOGGER_NONE;
			if (iRec == 0)
			{
				Rec = LOGGER_NONE;
			}
			else if (iRec == 1)
			{
				Rec = LOGGER_ASCII;
			}
			else if (iRec == 2)
			{
				Rec = LOGGER_TDMS;
			}
			else if (iRec == 3)
			{
				Rec = LOGGER_XLSX;
			}

			////////////////////////////////////////////////////////////////////////////////
			// MEASUREMENT CONTROLER
			////////////////////////////////////////////////////////////////////////////////
			std::cout << "Launching Measurement controler\n";
			cMeasurementControler* meas_controler = new cMeasurementControler; //////////////////////LEAK/////////////////LEAK///////////////
			meas_manager->set_measurement_controler(meas_controler);


			// DAQ Configure
			// Fill the device config struct with user parameters
			//

			/*
			DEVICE_CONFIG_STRUCT device_config_struct;

			// Retrieve DAQ addr
			wxString t = daqconfig->addr_ctrl->GetValue();
			device_config_struct.device_addr = t.ToStdString();


			// Retrieve channel physical addr and name
			std::vector<std::string> channels_addr;
			std::vector<std::string> channels_names;
			//for (int i = 0; i < daqconfig->GetChannelNumber(); i++)
			//{
			int i = 0;
			std::vector <bool> bChannels = daqconfig->GetChannelEnabledVector();
			for (auto chan : bChannels)
			{
				if (chan == true)
				{

					device_config_struct.channels_addr.push_back(std::string(daqconfig->chan_addr_ctrl->GetValue().mb_str()));
					device_config_struct.channels_names.push_back(std::string(daqconfig->chan_name->GetValue().mb_str()));
					m_plot_->set_signal_name(std::string(daqconfig->chan_name->GetValue().mb_str()), i);

					if (device_config_struct.channels_addr.size() == 0)
					{
						MessageBox(GetFocus(), L"No DAQ channels selected", L"Error", S_OK | MB_ICONERROR);
						std::cout << "cycle_controler deleted in Footer.cpp\n";
						delete cycle_controler;
						evt.Skip();
						return;
					}

					// Retrieve DAQ grounding mode
					// TODO: use for all channel not one
					int DAQ_pinmode = DAQmx_Val_RSE;
					if (daqconfig->mode_ctrl->GetValue().compare("Referenced") == 0)
					{
						DAQ_pinmode = DAQmx_Val_RSE;
					}
					if (daqconfig->mode_ctrl->GetValue().compare("Differential") == 0)
					{
						DAQ_pinmode = DAQmx_Val_Diff;
					}
					if (daqconfig->mode_ctrl->GetValue().compare("NRSE") == 0)
					{
						DAQ_pinmode = DAQmx_Val_NRSE;
					}
					if (daqconfig->mode_ctrl->GetValue().compare("Pseudo Diff") == 0)
					{
						DAQ_pinmode = DAQmx_Val_PseudoDiff;
					}
					device_config_struct.reference_mode.push_back(DAQ_pinmode);

					// Retrieve min:max range daq capabilities
					// TODO: use for all channel not one
					wxString max_str = daqconfig->max_ctrl->GetValue();
					double max_double = 0.0;
					max_str.ToCDouble(&max_double);
					device_config_struct.reference_mode_high.push_back(max_double);

					wxString min_str = daqconfig->min_ctrl->GetValue();
					double min_double = 0.0;
					min_str.ToCDouble(&min_double);
					device_config_struct.reference_mode_low.push_back(min_double);

					// Retrieve scale options
					wxString scale_strA = daqconfig->chan_slope[i]->GetValue();
					double A = 0.0;
					scale_strA.ToCDouble(&A);
					device_config_struct.scale_mode_A.push_back(A);

					wxString scale_strB = daqconfig->chan_shift[i]->GetValue();
					double B = 0.0;
					scale_strB.ToCDouble(&B);
					device_config_struct.scale_mode_B.push_back(B);

					// Retrieve filtering options
					// TODO: use for all channel not one
					device_config_struct.filtering_mode_enable.push_back(true);
					device_config_struct.filtering_mode.push_back(Filtering);

					// Save current number of channel loaded
					device_config_struct.chan_number = i;
					std::cout << "[*] " << i << " Channel configured in daq device.\n";
				}
			}
			*/

			// Save last change in daq gui fields

			size_t channel_index = daqconfig->get_channel_index();
			daqconfig->save_current_device_config(channel_index);
			daqconfig->save_current_chan_config(channel_index);

			// Retrieve current daq config		

			CURRENT_DEVICE_CONFIG_STRUCT config = daqconfig->GetDaqConfigStruct();


			int i = 0;
			int c = 0;
			std::vector <bool> bChannels = daqconfig->GetChannelEnabledVector();
			for (auto chan : bChannels)
			{
				if (chan == true)
				{
					m_plot_->set_signal_name(config.channel_name.at(c), i);
					i++;
				}
				c++;
			}

			// Launch DAQ System

			daqconfig->m_daq_->launch_device(config);
			meas_controler->start();
			size_t sizesig = meas_manager->get_measurement_total_channel_number();
			m_plot_->start_graph(Filtering, Rec, sizesig);

			startbtn->SetBackgroundColour(wxColor(250, 80, 90));
			startbtn->SetLabelText(L"Stop");

			// Lock daq interface when running				
			daqconfig->device_group_sizer->GetStaticBox()->Enable(false);
			daqconfig->channel_group_sizer->GetStaticBox()->Enable(false);
			daqconfig->channel_linearize_group_sizer->GetStaticBox()->Enable(false);
			daqconfig->channel_signal_group_sizer->GetStaticBox()->Enable(false);
		}
	}
	else
	{
		m_plot_->stop_graph();
		cycle_controler->stop();
		std::cout << "cycle_controler deleted in Footer.cpp\n";
		delete cycle_controler;
		cycle_controler = nullptr;

		std::cout << "m_table_->destroy_cycle() in Footer.cpp\n";
		m_table_->destroy_cycle();

		startbtn->SetBackgroundColour(wxColor(180, 250, 90));
		startbtn->SetLabelText(L"Start");

		cDaqmx* daqconfig = obj_manager->get_daqmx();
		if (daqconfig->m_daq_ == nullptr)
		{
			MessageBox(GetFocus(), L"No device loaded", 0, S_OK | MB_ICONERROR);
			evt.Skip();
			return;
		}

		// Unlock daq interface when running
		daqconfig->device_group_sizer->GetStaticBox()->Enable(true);
		daqconfig->channel_group_sizer->GetStaticBox()->Enable(true);
		daqconfig->channel_linearize_group_sizer->GetStaticBox()->Enable(true);
		daqconfig->channel_signal_group_sizer->GetStaticBox()->Enable(true);

		meas_manager->stop_all_devices();
	}

	evt.Skip();
}

void cFooter::filterButtonClicked(wxCommandEvent& evt)
{
	int iFilter = this->combo1->GetCurrentSelection();
	FILTER_M Filtering = FILTER_NONE;
	switch (iFilter)
	{
	case 0:
	{
		Filtering = FILTER_NONE;
		break;
	}
	case 1:
	{
		Filtering = FILTER_HANNING;
		break;
	}
	case 2:
	{
		Filtering = FILTER_BESEL;
		break;
	}
	case 3:
	{
		Filtering = FILTER_EMA;
		break;
	}
	}

	m_plot_->set_graph_filter(Filtering);
	evt.Skip();
}

void cFooter::freqButtonClicked(wxCommandEvent& evt)
{
	wxString str = freq->GetValue();


	cMeasurementControler* m_controler = meas_manager->get_measurement_controler();
	if (m_controler != nullptr)
	{
		m_controler->set_aquisition_rate(wxAtoi(str));
	}

	evt.Skip();
}





