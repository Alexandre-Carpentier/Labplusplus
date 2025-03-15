/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#include "cFooter.h"
#include <memory>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "cCycleControler.h"
#include "cMain.h"
#include "cGnuplot.h"
#include "cDeviceMonitor.h"
#include "cDeviceIDSaver.h"

cFooter::cFooter(wxWindow* inst, cPlot* m_plot, cTable* m_table, cConfig* m_config, std::shared_ptr <cDeviceMonitor> devmon)
{
	cycle_controler = std::make_shared<cCycleControler>(m_table, inst);

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


	scale_btn = new wxScaleButton((wxFrame*)inst_, wxID_ANY);

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

	freq = new wxTextCtrl(inst_, IDC_FREQTEXTCTRL, L"500", wxDefaultPosition, inst->FromDIP(wxSize(30, 25)));
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
	if (m_plot_->get_graph_state() == false)
	{
			// reset zoom slider to x1

		scale_btn->slider_reset();

			// show every signals

		m_plot_->show_all_signals(true);

			// Sanity check

		if ((m_table_->get_loop_number() < 1) || (m_table_->get_step_number() < 1))
		{
			wxCommandEvent evt = wxCommandEvent(wxEVT_COMMAND_TOOL_CLICKED, IDTOOL_EDITBTN);
			wxPostEvent(inst_, evt);
			MessageBox(GetFocus(), L"Fill the cycle table please", L"Error", S_OK | MB_ICONERROR);
			evt.Skip();
			return;
		}

			// Sanity check

		size_t measurement_number = meas_manager->get_measurement_pool_size();
		std::cout << "[*] " << measurement_number << " measurements are in pool.\n";
		if (measurement_number == 0)
		{
			wxCommandEvent evt = wxCommandEvent(wxEVT_COMMAND_TOOL_CLICKED, IDTOOL_SETTINGS);
			wxPostEvent(inst_, evt);
			MessageBox(GetFocus(), L"No active device\ntry to enable one instrument first", L"Error", S_OK | MB_ICONERROR);
			evt.Skip();
			return;
		}

			// configure each device
			

		// Load DAQ System
		//
		//
		cDaqmx* daqconfig = obj_manager->get_daqmx();
		if (daqconfig->m_daq_ != nullptr)
		{
				// Save last change in daq gui fields

			size_t channel_index = daqconfig->get_channel_index();
			daqconfig->save_current_device_config(channel_index);
			daqconfig->save_current_chan_config(channel_index);

				// Retrieve current daq config		

			CURRENT_DEVICE_CONFIG_STRUCT config = daqconfig->GetDaqConfigStruct();

			daqconfig->serialize(config.device_name.ToStdString());

			int i = 0;
			int c = 0;
			std::vector <bool> bChannels = daqconfig->GetChannelEnabledVector();
			for (auto chan : bChannels)
			{
				if (chan == true)
				{
					m_plot_->set_signal_name(config.channel_name.at(c), i);
					m_plot_->set_signal_filter(FILTER_NONE, i);
					m_plot_->set_signal_filter_threshold(0.5, i);
					i++;
				}
				c++;
			}

			// Load config in DAQ
			daqconfig->m_daq_->set_configuration_struct(config);
		}

		// Load Pressure System
		//
		//
		cPressure* pressureconfig = obj_manager->get_pressuredevice();
		if (pressureconfig->m_pressure_ != nullptr)
		{
			pressureconfig->save_current_device_config(0);
			CURRENT_DEVICE_CONFIG_STRUCT config = pressureconfig->GetPressureConfigStruct();

			pressureconfig->m_pressure_->set_configuration_struct(config);

		}

		// Load Voltage System 1
		//
		//
		cVoltage* voltageconfig = obj_manager->get_voltagedevice();
		if (voltageconfig->m_voltage_ != nullptr)
		{
			voltageconfig->save_current_device_config(0);
			CURRENT_DEVICE_CONFIG_STRUCT config = voltageconfig->GetVoltageConfigStruct();

			voltageconfig->m_voltage_->set_configuration_struct(config);

		}

		// Load Voltage System 2
		//
		//
		cVoltageRs* voltagersconfig = obj_manager->get_voltagersdevice();
		if (voltagersconfig->m_voltage_ != nullptr)
		{
			voltagersconfig->save_current_device_config(0);
			CURRENT_DEVICE_CONFIG_STRUCT config = voltagersconfig->GetVoltageConfigStruct();

			voltagersconfig->m_voltage_->set_configuration_struct(config);
		}

		// Load Oscope System
		//
		//
		cOscope* oscope_gui = obj_manager->get_oscopedevice();
		if (oscope_gui == nullptr)
		{
			std::cout << "[!] obj_manager->get_oscopedevice() return nullptr. Exiting.\n";
			evt.Skip();
			return;
		}
		oscope_gui->save_current_device_config(0);

		// Load Daq6510 System
		//
		//
		c6510ui* c6510ui_gui = obj_manager->get_6510device();
		if (c6510ui_gui == nullptr)
		{
			std::cout << "[!] obj_manager->get_6510device() return nullptr. Exiting.\n";
			evt.Skip();
			return;
		}
		c6510ui_gui->save_current_device_config(0);

		/////////////////////////////////////////////////////////
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
		// STOP LOOK UP FOR DEVICE
		////////////////////////////////////////////////////////////////////////////////
		//cDeviceMonitor* devmon = devmon->getInstance();
		//devmon->lookup_stop();
		//cMain* m_main = nullptr;
		//m_main = dynamic_cast<cMain*>(inst_);
		//if (m_main)
			//m_main->StopDiscoverDeviceTimer();





		////////////////////////////////////////////////////////////////////////////////
		// SAVE INSTRUMENT ID TO LOGFILE
		////////////////////////////////////////////////////////////////////////////////

		/*
		size_t sizesig = meas_manager->get_measurement_total_channel_number();
		std::string head;
		cDeviceIDSaver cDeviceIDSaverDlg(sizesig, this->inst_, IDCDEVICEIDSAVERDLG, "Reccord your instrument ID?");
		cDeviceIDSaverDlg.Centre();
		int ret = cDeviceIDSaverDlg.ShowModal();
		if (ret == wxID_OK)
		{
			head = cDeviceIDSaverDlg.get_header();// save instrument ID in header
		}
		else
		{
			head = " No instruments ID/SN saved by the user";
		}
		*/
		size_t pos = 0;
		std::string head;
		auto daq_config = daqconfig->GetDaqConfigStruct();	
		for (auto is_enable : daq_config.channel_enabled)
		{
			if (is_enable)
			{
				head.append(std::format("{}::{}\t", daq_config.device_serial_number[pos].ToStdString(), daq_config.channel_serial_number[pos].ToStdString())); pos++;
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		// START DAQ CTRL
		////////////////////////////////////////////////////////////////////////////////
		if (daqconfig->m_daq_ != nullptr)
		{
			if (daqconfig->m_daq_->launch_device() < 0)
			{
				evt.Skip();
				return;
			}
		}

		// Lock daq interface when running	
		daqconfig->previous_chan->Enable(false);
		daqconfig->next_chan->Enable(false);
		for (auto& ch_btn : daqconfig->chanbtn)
		{
			ch_btn->Enable(false);
		}
		daqconfig->device_group_sizer->GetStaticBox()->Enable(false);
		daqconfig->channel_group_sizer->GetStaticBox()->Enable(false);
		daqconfig->channel_linearize_group_sizer->GetStaticBox()->Enable(false);
		daqconfig->channel_signal_group_sizer->GetStaticBox()->Enable(false);

		////////////////////////////////////////////////////////////////////////////////
		// START PRESSURE CTRL
		////////////////////////////////////////////////////////////////////////////////
		if (pressureconfig->m_pressure_ != nullptr)
		{
			if (pressureconfig->m_pressure_->launch_device() < 0)
			{
				evt.Skip();
				return;
			}
			// Lock pressure controler interface
			pressureconfig->device_group_sizer->GetStaticBox()->Enable(false);
		}

		////////////////////////////////////////////////////////////////////////////////
		// START VOLTAGE CTRL 1
		////////////////////////////////////////////////////////////////////////////////
		if (voltageconfig->m_voltage_ != nullptr)
		{
			if (voltageconfig->m_voltage_->launch_device() < 0)
			{
				evt.Skip();
				return;
			}
			// Lock pressure controler interface
			voltageconfig->device_group_sizer->GetStaticBox()->Enable(false);
		}

		////////////////////////////////////////////////////////////////////////////////
		// START VOLTAGE CTRL 2
		////////////////////////////////////////////////////////////////////////////////
		if (voltagersconfig->m_voltage_ != nullptr)
		{
			if (voltagersconfig->m_voltage_->launch_device() < 0)
			{
				evt.Skip();
				return;
			}

			// Lock pressure controler interface
			voltagersconfig->device_group_sizer->GetStaticBox()->Enable(false);
		}
		////////////////////////////////////////////////////////////////////////////////
		// START OSCOPE CTRL 
		////////////////////////////////////////////////////////////////////////////////
		if (oscope_gui != nullptr)
		{
			if (oscope_gui->launch_device() < 0)
			{
				std::cout << "[!] oscope_gui->launch_device() return < 0. Exiting.\n";
				evt.Skip();
				return;
			}
			oscope_gui->lockBtn(false);
			std::cout << "[*] OSCOPE	[RUNNING]\n";
		}

		////////////////////////////////////////////////////////////////////////////////
		// START 6510 CTRL 
		////////////////////////////////////////////////////////////////////////////////
		if (c6510ui_gui != nullptr)
		{
			if (c6510ui_gui->launch_device() < 0)
			{
				std::cout << "[!] c6510ui_gui->launch_device() return < 0. Exiting.\n";
				evt.Skip();
				return;
			}
			c6510ui_gui->lockBtn(false);
			std::cout << "[*] 6510	[RUNNING]\n";
		}

		////////////////////////////////////////////////////////////////////////////////
		// CYCLE CONTROLER
		////////////////////////////////////////////////////////////////////////////////
		std::cout << "[*] Starting cycle controler.\n";
		cycle_controler->start();

		////////////////////////////////////////////////////////////////////////////////
		// MEASUREMENT MANAGER
		////////////////////////////////////////////////////////////////////////////////
		meas_manager->start_all_devices();

		////////////////////////////////////////////////////////////////////////////////
		// MEASUREMENT CONTROLER
		////////////////////////////////////////////////////////////////////////////////
		std::cout << "Launching Measurement controler\n";
		meas_controler = make_shared<cMeasurementControler>(cycle_controler);
		meas_manager->set_measurement_controler(meas_controler);
		meas_controler->subscribe(m_table_);
	
		size_t sizesig = meas_manager->get_measurement_total_channel_number();
		meas_controler->start();
		m_plot_->start_graph(Rec, sizesig, head);

		startbtn->SetBackgroundColour(wxColor(250, 80, 90));
		startbtn->SetLabelText(L"Stop");	
	}
	else
	{
		std::cout << "[*] Stopping cycle controler.\n";
		cycle_controler->stop();

		m_plot_->stop_graph();

		std::cout << "m_table_->destroy_cycle() in Footer.cpp\n";

		startbtn->SetBackgroundColour(wxColor(180, 250, 90));
		startbtn->SetLabelText(L"Start");

		cDaqmx* daqconfig = obj_manager->get_daqmx();
		cPressure* pressureconfig = obj_manager->get_pressuredevice();
		cVoltage* voltageconfig = obj_manager->get_voltagedevice();
		cVoltageRs* voltagersconfig = obj_manager->get_voltagersdevice();
		cOscope* oscope_gui = obj_manager->get_oscopedevice();
		c6510ui* c6510ui_gui = obj_manager->get_6510device();

		if ( (daqconfig->m_daq_ == nullptr) && (pressureconfig == nullptr) && (voltageconfig == nullptr) && (oscope_gui == nullptr))
		{
			wxCommandEvent evt = wxCommandEvent(wxEVT_COMMAND_TOOL_CLICKED, IDTOOL_SETTINGS);
			wxPostEvent(inst_, evt);
			MessageBox(GetFocus(), L"No device loaded", 0, S_OK | MB_ICONERROR);
			evt.Skip();
			return;
		}
		else
		{
			// Unlock daq interface when not running 
			daqconfig->previous_chan->Enable(true);
			daqconfig->next_chan->Enable(true);
			for (auto& ch_btn : daqconfig->chanbtn)
			{
				ch_btn->Enable(true);
			}
			daqconfig->device_group_sizer->GetStaticBox()->Enable(true);
			daqconfig->channel_group_sizer->GetStaticBox()->Enable(true);
			daqconfig->channel_linearize_group_sizer->GetStaticBox()->Enable(true);
			daqconfig->channel_signal_group_sizer->GetStaticBox()->Enable(true);

			// Unlock pressure controler interface
			pressureconfig->device_group_sizer->GetStaticBox()->Enable(true);
			voltageconfig->device_group_sizer->GetStaticBox()->Enable(true);
			voltagersconfig->device_group_sizer->GetStaticBox()->Enable(true);
			//oscopeconfig->device_group_sizer->GetStaticBox()->Enable(true);
			oscope_gui->lockBtn(true);
			c6510ui_gui->lockBtn(true);
		}

		meas_manager->stop_all_devices();
		////////////////////////////////////////////////////////////////////////////////
		// DRAW FULL GRAPH WITH GNUPLOT?
		////////////////////////////////////////////////////////////////////////////////
		cGnuplot cGnuplotDlg(this->inst_, IDCGNUPLOTDLG, "Create PDF graph and save campain?");
		cGnuplotDlg.Centre();

		int ret = cGnuplotDlg.ShowModal();
		if (ret == wxID_OK)
		{
			// retrieve current file with raw data
			std::string raw_name = m_plot_->get_graph_filename();
			// remove (xx).lab
			std::string raw_noext = raw_name.substr(0, raw_name.find_last_of("."));
			raw_noext = raw_noext.substr(0, raw_noext.find_last_of("("));
			// replace _ by -
			std::replace(raw_noext.begin(), raw_noext.end(), '_', '-');
			// get new name
			std::string name = cGnuplotDlg.get_filename();
			// add extension
			std::string new_name = raw_noext + name + std::string(".lab");
			// rename the file
			wxRenameFile(raw_name, new_name);

			// read header to build the script accordingly
			std::ifstream file(new_name);
			std::string instrID;
			std::string header;
			std::getline(file, instrID);// 1st line is equipment ID
			std::getline(file, header);

			// Count every collomn and add to vector for legend
			std::vector<std::string> signals_vec;

			std::string token;
			std::stringstream input_stringstream(header);

			//tokenize each col
			int col = 0;
			while (getline(input_stringstream, token, '\t'))
			{
				if (col >= 1)
				{
					signals_vec.push_back(token);
				}
				col++;
			}	

			std::string script_file = new_name;
			script_file = script_file.substr(0, script_file.find("."));
			std::string pdf_file = script_file+std::string(".pdf");
			script_file = script_file + std::string(".plt");
			//char const* gnuname = "gnu.script";
			const std::string sig_colours[]
			{
				"#DB7093",
				"#008000",
				"#FFE4B5",
				"#F4A460",
				"#A0522D",
				"#FFB6C1",
				"#DA70D6",
				"#6A5ACD",
				"#4169E1",
				"#87CEFA",
				"#4682B4",
				"#48D1CC",
				"#008080",
				"#2E8B57",
				"#8FBC8F",
				"#32CD32",
				"#FFFF00",
				"#DAA520",
				"#FA8072",
				"#FF6347",
				"#DC143C",
				"#800000"
			};

			FILE* f = fopen(script_file.c_str(), "w");
			if (f != NULL)
			{
				// set gnuplot script
	
				fprintf(f, "set terminal pdf\n");
				fprintf(f, "set encoding utf8\n");	
				fprintf(f, "set output \"%s\"\n", pdf_file.c_str());
				fprintf(f, "set encoding utf8\n");
				fprintf(f, "#set xrange [0.0:600.0]\n");
				fprintf(f, "#set yrange [0.0:20.0]\n");
	
				fprintf(f, "set key outside\n");
				
				for (size_t i =1; i <= signals_vec.size(); i++)
				{
					fprintf(f, "set style line %zi linecolor rgb '%s' linewidth 0.1\n", i, sig_colours[i-1].c_str());
					//fprintf(f, "set style line 2 linecolor rgb '#dd181f' linewidth 0.1\n");
				}

				fprintf(f, "set title \"%s\"\n", new_name.c_str());
				fprintf(f, "set xlabel \"T(s)\"\n");
				fprintf(f, "set ylabel \"Amplitude\"\n");
				
				size_t j = 1;
				fprintf(f, "plot \"%s\" every ::1 using 1:%zu w l linestyle %zu title \"%s\"", new_name.c_str(),j+1, j, signals_vec.at(0).c_str());
				for (size_t i=j; i< signals_vec.size(); i++)
				{
					j++;
					fprintf(f, ", \"%s\" using 1:%zu w l linestyle %zu title \"%s\"", new_name.c_str(),j+1, j, signals_vec.at(i).c_str());
				}
				fprintf(f, "\n");

				fclose(f);

				// Run GNUplot on the control script (for Windows)
				char command[300];
				const char* gnupath = "\"C:\\Program Files\\gnuplot\\bin\"";
				sprintf(command, "gnuplot.exe \"%s\"", script_file.c_str());
				printf("[*] Executing %s...\n", command);
				system(command);

				// Wait when building PDF file
				if (!wxFileExists(pdf_file))
				{
					std::cout << "[!] Building pdf file takes time !\n";
					Sleep(500);
				}

				if (!wxFileExists(pdf_file))
				{
					std::cout << "[!] Building pdf file takes time ! (waiting 5s more)\n";
					Sleep(5000);
				}
				if (!wxFileExists(pdf_file))
				{
					fprintf(stderr, "Could not open %s.\n", pdf_file.c_str());
					MessageBox(0, L"Could not open pdf file.", L"Fail", S_OK);
				}
				else
				{
					sprintf(command, "%s", pdf_file.c_str());
					printf("[*] Executing %s...\n", command);
					system(command);
				}
			}
			else
			{
				fprintf(stderr, "Could not create %s.\n", script_file.c_str());
				MessageBox(0, L"Could not open script file.", L"Fail", S_OK);
			}	
		}
		
		////////////////////////////////////////////////////////////////////////////////
		// START LOOK UP FOR DEVICE
		////////////////////////////////////////////////////////////////////////////////
		//cDeviceMonitor* devmon = devmon->getInstance();

		//devmon->lookup_start();
	}

	evt.Skip();
}

void cFooter::freqButtonClicked(wxCommandEvent& evt)
{
	wxString str = freq->GetValue();


	std::shared_ptr<cMeasurementControler> m_controler = meas_manager->get_measurement_controler();
	if (m_controler != nullptr)
	{
		m_controler->set_aquisition_rate(wxAtoi(str));
	}

	evt.Skip();
}





