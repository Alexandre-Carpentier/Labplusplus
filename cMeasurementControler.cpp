#include "cMeasurementControler.h"

#include <wx/wx.h>
#include <wx/app.h> 
#include <thread>
#include <Windows.h>

#include "cCycle.h"
#include "cCycleControler.h"
#include "cPlot.h"
#include "cFooter.h"
#include "cObjectmanager.h"
#include "cMeasurementmanager.h"
#include "cUSB6001.h"
#include "cMeasurement.h"
#include "cTick.h"

double get_instr_setpoint(cMeasurement *meas, std::vector<STEPSTRUCT> step_table, int step_number)
{
	double val=0.0;
	int current_step = 0;
	std::string dev_name = meas->device_name();
	std::cout << "[*] Found dev_name: " << dev_name << "\n";
	for (auto& step : step_table)
	{
		if (current_step == step_number)
		{
			for (auto& controler : step.controler_vec)
			{
				std::cout << "[*] Searching controler: " << controler.first << "\n";
				if (controler.first.find(dev_name) == 0)
				{
					val = controler.second;
					std::cout << "[*] Command found: " << controler.second << "\n";
					break;
				}
			}
		}
		current_step++;
	}
	return val;
}

void cMeasurementControler::poll()
{
	std::cout << "cMeasurementcontroler->get_stop_token...\n";
	auto st = measurement_controler_thread.get_stop_token();

	meas_pool = meas_manager->get_measurement_pool();
	std::cout << "cMeasurementcontroler->polling...\n";
	/*
	// Rearange the meas_pool to display signal at the right place
	// First class in order the MEAS_TYPE in the sigtable (1)
	// Then put the measurement in the same order in a temporary list (2)
	// Swap the list (3)
	// so measurement will be displayed in the same order

	// (1)
	cSignalTable* sigt = sigt->getInstance();
	std::list<CHAN_LEGEND_STRUCT> list = sigt->get_signal_table();

	int meas_count = 0;

	std::vector<int> order;
	for (auto& chan : list)
	{
		if (chan.type != MEAS_TYPE::VOID_INSTR)
		{
			order.push_back(chan.type);
			meas_count++;
		}
	}

	// (2)
	std::vector<cMeasurement*> temp_pool;
	int i = 0;
	for (auto& meas : meas_pool)
	{
		if (meas_count > 0)
		{
			if (meas->device_type() == order.at(i))
			{
				temp_pool.push_back(meas);
				i++;
				meas_count--;
			}
		}
	}

	// (3)
	meas_pool.clear();
	meas_pool = temp_pool;
	*/

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
			if (m_cycle_ != nullptr)
			{		
				meas->set(0.0);
			}		

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
				Sleep(4000);
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

				
				std::vector<double> read_pool;
				for (auto meas : meas_pool)
				{

					// Write data to instrument (controler)
					if (m_cycle_ != nullptr)
					{				
						double value = 0.0;
						MEAS_TYPE type = meas->device_type();
		
						switch (type)
						{
						case MEAS_TYPE::PRESSURE_CONTROLER_INSTR:
						{					
							static double old_pressure=0.0;
							m_cyclecontroler_->critical_section.lock(); ////////////////////////////////CRITICAL_SECTION///////////
							value = get_instr_setpoint(meas, m_cycle_->pcycle->step_table, m_cycle_->get_current_step());
							m_cyclecontroler_->critical_section.unlock(); ////////////////////////////////CRITICAL_SECTION///////////
							if(old_pressure == value)
							{			
								goto read;
							}
							old_pressure = value; // save old value
							break;
						}
						/*
						case MEAS_TYPE::DAQ_INSTR:
						{
							static double old_daq = 0.0;
							value = m_cycle_->get_voltage();
							if (old_daq == value)
							{		
								goto read;
							}
							old_daq = value; // save old value
							break;
						}
						*/
						//... add other controler here
						default:
							value = 0.0;
							goto read;
						}
						meas->set(value);
					}
					
read:
					// Read data from instrument
					val = meas->read();

					// Add vector to store points
					for (int i = 0; i < val.buffer_size; i++)
					{
						read_pool.push_back(val.buffer[i]);
					}
					val.buffer_size = 0;
				}

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
	bRunning = true;
	//cMeasurementControler::control_thread = std::thread(&cMeasurementControler::poll, this);

	measurement_controler_thread = std::jthread(&cMeasurementControler::poll, this);
	

}

void cMeasurementControler::stop()
{
	bRunning = false;
	std::cout << "cMeasurementcontroler->stoping...\n";
	measurement_controler_thread.request_stop();
	//control_thread.join();
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


