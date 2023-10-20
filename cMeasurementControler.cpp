#include "cMeasurementControler.h"

#include <wx/wx.h>
#include <wx/app.h> 
#include <thread>
#include <Windows.h>

#include "cPlot.h"
#include "cFooter.h"
#include "cObjectmanager.h"
#include "cMeasurementmanager.h"
#include "cUSB6001.h"
#include "cMeasurement.h"
#include "cTick.h"

void cMeasurementControler::poll()
{
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

	if (bRunning)
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

		if (bRunning)
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
					// Set the setpoint
					meas->set(12.0);

					// Get the read value
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
	cMeasurementControler::control_thread = std::thread(&cMeasurementControler::poll, this);

}

void cMeasurementControler::stop()
{
	bRunning = false;
	std::cout << "cMeasurementcontroler->stoping...\n";
	control_thread.join();
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


