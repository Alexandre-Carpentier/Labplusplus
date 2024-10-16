#include "cMeasurementControler.h"

#include "cCycleControler.h"
#include "cMeasurement.h"
#include "cMeasurementmanager.h"
#include "cObjectmanager.h"
#include "cFooter.h"

#include <wx/wx.h>
#include <wx/app.h> 

#include <Windows.h>

bool get_instr_setpoint(cMeasurement *meas, STEPSTRUCT step, double* values, size_t buffer_length, size_t *read)
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

void zero_instrument(std::vector<cMeasurement*> meas_pool)
{
	assert(meas_pool.size() > 0);

	for (auto meas : meas_pool)
	{
		// Write data to instrument (controler)
		size_t length = meas->chan_write_count();
		if (length > 0)
		{
			double* values = new double(length);
			memset(values, 0.0, length);
			meas->set(values, length);
			delete(values);
		}
	}
}

void cMeasurementControler::poll()
{
	assert(m_cyclecontroler_->get_total_loop() > 0);
	assert(m_cyclecontroler_->get_total_step() > 0);
	assert(m_cyclecontroler_->get_total_step() < 300);
	assert(m_cyclecontroler_->get_current_loop() > 0);
	//assert(m_cyclecontroler_->get_current_step() == 0); // might be fail if call is long
	std::cout << m_cyclecontroler_->get_current_step() << "\n";

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
		
			//size_t length = meas->chan_count();
			if (meas->chan_write_count() > 0)
			{
				size_t length = meas->chan_write_count();
				double* values = new double();
				memset(values, 0.0, length);
				meas->set(values, length);
				delete(values);

				// Read data from instrument
				val = meas->read();
				for (size_t c = buffer_index; c < val.buffer_size; c++)
				{
					Y[c] = val.buffer[c];

					buffer_index++;
				}
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

	static double old_pressure[MAX_CHAN];

	while (1)
	{
		//std::cout << "bRunning: "<< bRunning <<"\n";
		old_pressure[0] = 0.0;
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
			if (time > (this->freq_s_ / 1000))
			{
				tick.start_tick();
				int buffer_index = 0;

				// EXEC instrument command
				std::vector<double> read_pool;
				for (auto meas : meas_pool)
				{
					std::unique_ptr<double> value ;

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
							static double old_value[MAX_CHAN];

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
									MessageBox(GetFocus(), L"Can't read instrument command\nExiting...", L"Fail", S_OK);
								}

								assert(length == read);
								assert(read < MAX_CHAN);

								// unprotect
								m_cyclecontroler_->cycle_mutex.unlock();
								int mod = 0;
								for (size_t i = 0; i < read; i++)
								{
									if (old_value[i] != value[i])
									{
										mod++;
										old_value[i] = value[i]; // save old value
									}
								}

								// call if modified
								if (mod > 0)
									meas->set(old_value, read);


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

						}
						
				}

				assert(read_pool.size() > 0);
				m_plot_->graph_addpoint(read_pool.size(), &read_pool.at(0));
				//memset(Y, 0, sizeof(Y));


				// Update acquire rate
				m_footer_->ratetxt->SetValue(wxString::Format(wxT("%.1lf"), time * 1000));
			}

		}
	}
	// TODO:
	// it doesn't called 
	if (meas_pool.size() > 0)
	{
		zero_instrument(meas_pool);
	}


	std::cout << "cMeasurementcontroler->exiting thread... \n";
	return;
}

void cMeasurementControler::start()
{
	std::cout << "[*] cMeasurementcontroler->starting called\n";
	obj_manager = obj_manager->getInstance();
	m_plot_ = obj_manager->get_plot();
	m_footer_ = obj_manager->get_footer();
	meas_manager = meas_manager->getInstance();

	//meas_pool = meas_manager->get_measurement_pool();

	//control_thread = std::thread([this, meas_pool]() { this->poll(meas_pool); });
	//std::thread control_thread(&cMeasurementControler::poll, this);
	//control_thread.detach();

	//control_thread.detach();
	//cMeasurementControler::control_thread = std::thread(&cMeasurementControler::poll, this);

	measurement_controler_thread = std::jthread(&cMeasurementControler::poll, this);	
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


