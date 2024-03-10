#include "cMeasurementControler.h"

#include <wx/wx.h>
#include <wx/app.h> 
#include <thread>
#include <Windows.h>

double get_instr_setpoint(cMeasurement *meas, STEPSTRUCT step)
{
	double val=0.0;
	std::string dev_name = meas->device_name();
	for (auto& controler : step.controler_vec)
	{
		if (controler.first.find(dev_name) == 0)
		{
			val = controler.second;
			break;
		}
	}	
	return val;
}

void zero_instrument(std::vector<cMeasurement*> meas_pool)
{
	assert(meas_pool.size() > 0);

	for (auto meas : meas_pool)
	{
		// Write data to instrument (controler)
		meas->set(0.0);
	}
}

void cMeasurementControler::poll()
{
	assert(m_cyclecontroler_->get_total_loop() > 0);
	assert(m_cyclecontroler_->get_total_step() > 0);
	assert(m_cyclecontroler_->get_total_step() < 300);
	assert(m_cyclecontroler_->get_current_loop() > 0);
	assert(m_cyclecontroler_->get_current_step() == 0);

	std::cout << "cMeasurementcontroler->get_stop_token...\n";
	auto st = measurement_controler_thread.get_stop_token();

	meas_pool = meas_manager->get_measurement_pool();
	std::cout << "cMeasurementcontroler->polling...\n";

	DATAS val;
	double Y[80]; memset(Y, 0, sizeof(Y));
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

		int buffer_index = 0;
		for (auto meas : meas_pool)
		{
			// Write data to instrument (controler)
		
			meas->set(0.0);
		

			// Read data from instrument
			val = meas->read();
			for (int c = buffer_index; c < val.buffer_size; c++)
			{
				Y[c] = val.buffer[c];

				buffer_index++;
			}
		}

		// Add the first point to update min avg max value in indicator
		m_plot_->graph_addpoint(buffer_index, Y);
		memset(Y, 0, sizeof(Y));

		// Update min avg max value
		HGRAPH hGraph_ = m_plot_->get_graph_handle();
		if (hGraph_ != nullptr)
		{
			for (int z = 0; z < buffer_index; z++)
			{
				SignalResetStatisticValue(hGraph_, z);
			}
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

	while (1)
	{
		//std::cout << "bRunning: "<< bRunning <<"\n";

		if (!st.stop_requested())
		{
			wxString frequency = m_footer_->freq->GetValue();
			frequency.ToCDouble(&freq_s_);

			if (m_plot_->get_graph_state() == false)
			{
				Sleep(1000);
				if (m_plot_->get_graph_state() == false)
				{					
					std::cout << "cMeasurementcontroler->program break \n";
					break;
				}
			}
			time = tick.get_tick();
			if (time > (this->freq_s_ / 1000))
			{
				tick.start_tick();
				int buffer_index = 0;

				// EXEC instrument command
				std::vector<double> read_pool;
				for (auto meas : meas_pool)
				{
						double value = 0.0;
						MEAS_TYPE type = meas->device_type();
		
						switch (type)
						{
							// ALL (READ|WRITE)
							//
							//
						case MEAS_TYPE::VOLTAGE_CONTROLER_INSTR:
						case MEAS_TYPE::PRESSURE_CONTROLER_INSTR:
						{
							static double old_pressure = 0.0;
							// protect
							m_cyclecontroler_->cycle_mutex.lock();

							STEPSTRUCT step = m_cyclecontroler_->get_current_step_param();
							value = get_instr_setpoint(meas, step);

							// unprotect
							m_cyclecontroler_->cycle_mutex.unlock();

							if (old_pressure != value)
							{
								old_pressure = value; // save old value
							}

							// Read data from instrument
							val = meas->read();

							// Add vector to store points
							for (int i = 0; i < val.buffer_size; i++)
							{
								read_pool.push_back(val.buffer[i]);
							}
							val.buffer_size = 0;

							break;
						}
						// WRITE
						//
						//
						case MEAS_TYPE::FLOW_CONTROLER_INSTR:
						case MEAS_TYPE::VALVE_CONTROLER_INSTR:
						{
							static double old_pressure = 0.0;
							// protect
							m_cyclecontroler_->cycle_mutex.lock();

							STEPSTRUCT step = m_cyclecontroler_->get_current_step_param();
							value = get_instr_setpoint(meas, step);

							// unprotect
							m_cyclecontroler_->cycle_mutex.unlock();

							if (old_pressure != value)
							{
								old_pressure = value; // save old value
							}
							break;
						}
						// READ
						//
						//
						case MEAS_TYPE::DAQ_INSTR:
						{
							// Read data from instrument
							val = meas->read();

							// Add vector to store points
							for (int i = 0; i < val.buffer_size; i++)
							{
								read_pool.push_back(val.buffer[i]);
							}
							val.buffer_size = 0;
							break;
						}
						}
						/*
						case MEAS_TYPE::PRESSURE_CONTROLER_INSTR:
						{					
							static double old_pressure=0.0;
							// protect
							m_cyclecontroler_->cycle_mutex.lock();

							STEPSTRUCT step = m_cyclecontroler_->get_current_step_param();
							value = get_instr_setpoint(meas, step);

							// unprotect
							m_cyclecontroler_->cycle_mutex.unlock();

							if(old_pressure == value)
							{			
								goto read;
							}
							old_pressure = value; // save old value
							break;
						}
						default:
							value = 0.0;
							goto read;
						}
						meas->set(value);				
			read:
					// Read data from instrument
					val = meas->read();

					// Add vector to store points
					for (int i = 0; i < val.buffer_size; i++)
					{
						read_pool.push_back(val.buffer[i]);
					}
					val.buffer_size = 0;
					*/
				}

				assert(read_pool.size() > 0);
				m_plot_->graph_addpoint(read_pool.size(), &read_pool.at(0));
				//memset(Y, 0, sizeof(Y));

					/*
					for (int c = buffer_index; c < val.buffer_size; c++)
					{
						Y[c] = val.buffer[c];
						buffer_index++;
						std::cout << "buffer_index" << buffer_index << "\n";
					}
					val.buffer_size = 0;
				}

				m_plot_->graph_addpoint(buffer_index, Y);
				memset(Y, 0, sizeof(Y));
				*/

				// Update acquire rate
				m_footer_->ratetxt->SetValue(wxString::Format(wxT("%.1lf"), time * 1000));
			}
		}
		else
		{
			if (meas_pool.size() > 0)
			{
				zero_instrument(meas_pool);
			}
			break;
		}	
	}
	std::cout << "cMeasurementcontroler->exiting thread... \n";
	return;
}

void cMeasurementControler::start()
{
	obj_manager = obj_manager->getInstance();
	m_plot_ = obj_manager->get_plot();
	m_footer_ = obj_manager->get_footer();
	meas_manager = meas_manager->getInstance();

	std::cout << "cMeasurementcontroler->starting...\n";

	//meas_pool = meas_manager->get_measurement_pool();

	//control_thread = std::thread([this, meas_pool]() { this->poll(meas_pool); });
	//std::thread control_thread(&cMeasurementControler::poll, this);
	//control_thread.detach();

	//control_thread.detach();
	//cMeasurementControler::control_thread = std::thread(&cMeasurementControler::poll, this);

	measurement_controler_thread = std::jthread(&cMeasurementControler::poll, this);
	

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


