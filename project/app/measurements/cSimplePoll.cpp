#include "cPoller.h"
//#include "cMeasurementControler.h"
#include <print>
#include <chrono>
#include <thread>



cSimplePoll::cSimplePoll(cObjectmanager* obj_manager, cMeasurementmanager* meas_manager, std::shared_ptr<cCycleControler>cycle_controler, std::vector<cMeasurement*> meas_pool)
	: m_obj_manager(obj_manager), m_meas_manager(meas_manager), m_cycle_controler(cycle_controler), m_meas_pool(meas_pool)
{
	std::print("[*] cSimPoll ctor...\n");
	assert(m_obj_manager != nullptr);
	assert(m_meas_manager != nullptr);
	assert(m_cycle_controler != nullptr);
}

void cSimplePoll::runonce()
{
	std::print("[*] cSimPoll runonce\n");

	// Get external references

	m_statusbar = m_obj_manager->get_status_bar();
	m_plot = m_obj_manager->get_plot();
	m_footer = m_obj_manager->get_footer();
	assert(m_statusbar != nullptr);
	assert(m_plot != nullptr);
	assert(m_footer != nullptr);

	// Check everything is ok

	assert(m_cycle_controler->get_total_loop() > 0);
	assert(m_cycle_controler->get_total_step() > 0);
	assert(m_cycle_controler->get_total_step() < 300);
	assert(m_cycle_controler->get_current_loop() > 0);
	if (m_plot->get_graph_state() == false)
	{
		std::print("[!] cMeasurementcontroler->get_graph_state() state = stop\n");
		return;
	}
}

void cSimplePoll::loop() {
	read_instruments();
}

void cSimplePoll::stop() {
	std::print("[*] cSimPoll stop\n");
	if (m_meas_pool.size() > 0)
	{
		zero_instrument(m_meas_pool);
	}
}

void cSimplePoll::write_instruments()
{
	std::print("[*] cSimPoll write setpoint to instruments\n");

	static double old_value[MAX_CHAN] = { 0 };
	double value[MAX_CHAN] = { 0 };

	for (auto meas : m_meas_pool)
	{
		if (meas->chan_write_count() > 0)
		{
			// protect
			m_cycle_controler->cycle_mutex.lock();

			STEPSTRUCT step = m_cycle_controler->get_current_step_param();
			size_t length = meas->chan_write_count();
			size_t read = 0;

			assert(length < MAX_CHAN);
			bool success = get_instr_setpoint(meas, step, value, length, &read);
			std::cout << std::format("[GET] double[]:{};{};{};{}, length:{}\n", value[0], value[1], value[2], value[3], read);
			if (length != read)
			{
				MessageBox(GetFocus(), L"Can't read instrument command in the table view\nExiting...", L"Fail", S_OK);
			}

			assert(length == read);
			assert(read < MAX_CHAN);

			// unprotect
			m_cycle_controler->cycle_mutex.unlock();

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
			ZeroMemory(value, MAX_CHAN);
		}
	}
	return;
}

void cSimplePoll::read_instruments()
{
	assert(m_meas_pool.size() > 0);
	std::print("[*] cSimPoll read from instruments\n");
	DATAS datas;

	std::vector<double> read_pool;
	for (auto meas : m_meas_pool)
	{
		// Read data from instrument
		datas = meas->read();

		// Add vector to store points
		for (size_t i = 0; i < datas.buffer_size; i++)
		{
			// prepare send to observers
			//currentValues.add_values(meas->device_name(), val.buffer[i]);

			read_pool.push_back(datas.buffer[i]);
		}
		datas.buffer_size = 0;
		break;
	}

	double** read_pool_ptr = new double* [2];
	read_pool_ptr[0] = &read_pool[0];
	read_pool_ptr[1] = &read_pool[1];
	assert(read_pool.size() > 0);
	m_plot->graph_addpoints(read_pool.size(), read_pool_ptr, 1);
	return;
}
