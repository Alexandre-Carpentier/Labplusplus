/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////

#include "cMeasurementControler.h"


#include "cMeasurement.h"
#include "cMeasurementmanager.h"
#include "cObjectmanager.h"
#include "cFooter.h"

#include <wx/wx.h>
#include <wx/app.h> 

#include <Windows.h>

#include "cPoller.h"
#include <algorithm>
#include <print>

void zero_instrument(std::vector<cMeasurement*> meas_pool)
{
	assert(meas_pool.size() > 0);

	for (auto meas : meas_pool)
	{
		// Write data = 0 to every instruments 
		size_t length = meas->chan_write_count();
		if (length > 0)
		{
			std::vector<double> values(length, 0.0);
			meas->set(values.data(), length);
		}
	}
}

bool get_instr_setpoint(cMeasurement* meas, STEPSTRUCT step, double* values, size_t buffer_length, size_t *read)
{
	std::string dev_name = meas->device_name();
	assert(dev_name.size() > 0);
	int i = 0;

	for (auto& controler : step.controler_vec)
	{
		if (controler.first.find(dev_name) == 0)
		{
			*values = controler.second;
			values++;
			i++;
		}
	}	
	*read = i;
	return true;
}

void cMeasurementControler::poll()
{
	cTick m_tick;
	double m_time = 0.0;

		// Get stop signal from jthread

	std::print("[*] cMeasurementcontroler->get_stop_token()... \n");
	auto st = measurement_controler_thread.get_stop_token();

		// Get external references

	cObjectmanager* object_manager = object_manager->getInstance();
	cMeasurementmanager* meas_manager = meas_manager->getInstance();
	m_footer_ = object_manager->get_footer();
	std::vector<cMeasurement*> meas_pool = meas_manager->get_measurement_pool();
	assert(object_manager != nullptr); 
	assert(meas_manager != nullptr);
	assert(m_footer_ != nullptr);
	assert(meas_pool.size() >0);

		// Get the available step to perform

	std::cout << m_cyclecontroler->get_current_step() << "\n";

		// Choose implementation 

	Poller obj = cSimplePoll(object_manager, meas_manager, m_cyclecontroler, meas_pool);

		// Handle frequency rate

	wxString frequency = m_footer_->freq->GetValue();
	frequency.ToCDouble(&freq_s_);
	m_tick.start_tick();

		// Start job

	std::visit([](auto&& arg) {arg.runonce(); }, obj);
	while (!st.stop_requested())
	{
			// Do work

		m_time = m_tick.get_tick();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		if (m_time > (freq_s_ / 1000))
		{
			m_tick.start_tick();
			std::visit([](auto&& arg) {arg.loop(); }, obj);	
			m_footer_->ratetxt->SetValue(wxString::Format(wxT("%.1lf"), m_time * 1000));// Update acquire rate
		}
	}
	std::visit([](auto&& arg) {arg.stop(); }, obj);

		// Exit thread

	std::print("[*] cMeasurementcontroler->exiting thread... \n");
	return;

	///////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////




	/*
	meas_pool = meas_manager->get_measurement_pool();
	std::cout << "cMeasurementcontroler->polling...\n";

	DATAS val;
	CHUNK chunk;

	double* Y[80];
	*Y = new double[2];
	*(Y + 1) = new double[2];
	Y[0][0] = -1.0;
	Y[0][1] = -1.0;
	Y[1][0] = -2.0;
	Y[1][1] = -2.0;
	
	//memset(Y, 0, sizeof(Y));
	cTick tick;
	double time = 0.0;

	//////////////////////////////////////////////////////////////////
	//
	// RUNONCE
	//
	cObjectmanager* object_manager = object_manager->getInstance();
	wxStatusBar* statusbar = object_manager->get_status_bar();
	statusbar->SetLabelText("Begin reading instrument values...");

	if (!st.stop_requested())
	{
		wxString frequency = m_footer_->freq->GetValue();
		frequency.ToCDouble(&freq_s_);

		if (m_plot_->get_graph_state() == false)
		{
			Sleep(4000);
			if (m_plot_->get_graph_state() == false)
			{
				std::cout << "cMeasurementcontroler->program break \n";
				return;
			}
		}

		int buffer_len = 0;
		for (auto meas : meas_pool)
		{
			// Write data to instrument (controler)
		
			//size_t length = meas->chan_count();
			if (meas->chan_write_count() > 0)
			{
				size_t length = meas->chan_write_count();
				double* values = new double[length];
				memset(values, 0.0, length*sizeof(double));
				meas->set(values, length);
			}

			// Read data from instrument
			val = meas->read();
			for (size_t c = buffer_len; c < val.buffer_size; c++)
			{
				Y[c][0] = val.buffer[c];

				// prepare send to observers
				//currentValues.add_values(meas->device_name(), Y[c]);

				buffer_len++;
			}
		}

		//this->notify(static_cast<void*>(&currentValues));
		//currentValues.clear();

		// Add the first point to update min avg max value in indicator
		m_plot_->graph_addpoints(buffer_len, Y, 1);

		*Y[0] = -1.0;
		*Y[1] = -1.0;

		// Update min avg max value

		for (int z = 0; z < buffer_len; z++)
		{
			m_plot_->reset_chan_statistic_by_signal_position(z);
		}

		// Update acquire rate
		m_footer_->ratetxt->SetValue(wxString::Format(wxT("%.1lf"), time * 1000));
	}

	m_plot_->update_chan_statistic_labels();

	//////////////////////////////////////////////////////////////////
	//
	// LOOP
	//
	statusbar->SetLabelText("Reading/Writing instruments...");
	tick.start_tick();


	static volatile double old_value[MAX_CHAN] = { 0 };
	while (1)
	{
		if (!st.stop_requested())
		{
			wxString frequency = m_footer_->freq->GetValue();
			frequency.ToCDouble(&freq_s_);

			if (m_plot_->get_graph_state() == false)
			{
				Sleep(1000);
				if (m_plot_->get_graph_state() == false)
				{					
					// ---------------
					std::cout << "[!] m_plot_->get_graph_state() == false \n";
					std::cout << "[!] cMeasurementcontroler->program break \n";
					break;
				}
			}
			time = tick.get_tick();
			if (time > (freq_s_ / 1000))
			{
				tick.start_tick();
				int buffer_index = 0;

				// EXEC instrument command
				std::vector<double> read_pool;
				for (auto meas : meas_pool)
				{
						MEAS_TYPE type = meas->device_type();
		
						switch (type)
						{
							// ALL (READ|WRITE)
							//
							//
						case MEAS_TYPE::VOLTAGE_CONTROLER_INSTR:
						case MEAS_TYPE::PRESSURE_CONTROLER_INSTR:
						case MEAS_TYPE::DAQ_INSTR:
						{
							if (meas->chan_write_count() > 0)
							{
								double value[MAX_CHAN];
								ZeroMemory(value, MAX_CHAN);

								// protect
								m_cyclecontroler_->cycle_mutex.lock();

								if (m_cyclecontroler_->get_total_loop() == 0)
								{
									break;
								}
								STEPSTRUCT step = m_cyclecontroler_->get_current_step_param();
								size_t length = meas->chan_write_count(); 
								size_t read = 0;

								assert(length < MAX_CHAN);
								bool success = get_instr_setpoint(meas, step, value, length, &read);
								//std::cout << std::format("[GET] double[]:{};{};{};{}, length:{}\n", value[0], value[1], value[2], value[3], read);
								
								if (length != read)
								{
									MessageBox(GetFocus(), L"Can't read instrument command in the table view\nExiting...", L"Fail", S_OK);
								}

								assert(length == read);
								assert(read < MAX_CHAN);

								// unprotect
								m_cyclecontroler_->cycle_mutex.unlock();

								//std::cout << "Value: " << value[0] << "\n";
								//std::cout << "Old Value: " << old_value[0] << "\n";

								int mod = 1;

								std::cout << value[0] << " " << value[1] << "\n";
								std::cout << old_value[0] << " " << old_value[1] << "\n";

								for (size_t i = 0; i < read; i++)
								{
									// Add keyword volatile to prevent compiler optimizing on old_value
									if (old_value[i] != value[i])
									{
										mod++;
										old_value[i] = value[i]; // save old value
									}
									std::cout << mod << "\n";
								}

								// call if modified
								if (mod > 0)
								{
									meas->set(value, read);
									mod = 0;
								}
							}
							// Read data from instrument
							val = meas->read();

							// Add vector to store points
							for (size_t i = 0; i < val.buffer_size; i++)
							{
								// prepare send to observers
								//currentValues.add_values(meas->device_name(), val.buffer[i]);

								read_pool.push_back(val.buffer[i]);
							}
							val.buffer_size = 0;
							break;
						}
						}
						
				}

				//this->notify(static_cast<void*>(&currentValues));
				//currentValues.clear();
				double** read_pool_ptr = new double* [2];
				read_pool_ptr[0] = &read_pool[0];
				read_pool_ptr[1] = &read_pool[1];

				assert(read_pool.size() > 0);
				m_plot_->graph_addpoints(read_pool.size(), read_pool_ptr, 1);

				// Update acquire rate
				m_footer_->ratetxt->SetValue(wxString::Format(wxT("%.1lf"), time * 1000));
			}

		}
	}
	// TODO:
	// doesn't called 
	if (meas_pool.size() > 0)
	{
		zero_instrument(meas_pool);
	}

	
	std::cout << "cMeasurementcontroler->exiting thread... \n";
	return;
	*/
}

void cMeasurementControler::start()
{
	std::cout << "[*] cMeasurementcontroler->starting called\n";
	measurement_controler_thread = std::jthread([this](std::stop_token st){
		this->poll();
		});
	std::cout << "[*] cMeasurementcontroler->started\n";
}

void cMeasurementControler::stop()
{
	std::cout << "cMeasurementcontroler->stoping...\n";
	measurement_controler_thread.request_stop();
	std::cout << "cMeasurementcontroler has joined...\n";
}

double cMeasurementControler::get_aquisition_rate()
{
	std::cout << "cMeasurementcontroler->get_rate...\n";
	return freq_s_;
}

void cMeasurementControler::set_aquisition_rate(double freq_s)
{
	std::cout << "cMeasurementcontroler->set_rate...\n";
	freq_s_ = freq_s;
	return;
}


