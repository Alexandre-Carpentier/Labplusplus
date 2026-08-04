#include "cPoller.h"
//#include "cMeasurementControler.h"
#include <print>
#include <chrono>
#include <thread>
#include <memory>

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

	assert(m_cycle_controler->get_total_loop() >= 0); // 0 to n
	assert(m_cycle_controler->get_total_step() >= 0);// 0 to n
	assert(m_cycle_controler->get_total_step() < 300);
	assert(m_cycle_controler->get_current_loop() >= 0);// 0 to n
	if (m_plot->get_graph_state() == false)
	{
		std::print("[!] cMeasurementcontroler->get_graph_state() state = stop\n");
		return;
	}

		// Set instrument initial value

	write_instruments();
}

void cSimplePoll::loop() {
	write_instruments();
	read_instruments();
}

void cSimplePoll::stop() {
	std::print("[*] cSimPoll stop\n");
	if (m_meas_pool.size() > 0)
	{
		zero_instrument(m_meas_pool);
	}
}

// Helper functions
// Used by write instruments methods
inline bool check_instr_setpoint(double value[MAX_CHAN], size_t read, size_t len)
{
	std::cout << std::format("[GET] double[]:{};{};{};{}, length:{}\n", value[0], value[1], value[2], value[3], read);
	if (len != read)
	{
		MessageBox(GetFocus(), L"Can't read instrument command in the table view\n", L"Fail", S_OK);
		assert(len == read);
		return false;
	}

	if (read > MAX_CHAN)
	{
		MessageBox(GetFocus(), L"[!] Read > MAX_CHAN\n", L"Fail", S_OK);
		assert(read < MAX_CHAN);
		return false;
	}

	return true;
}

void cSimplePoll::write_instruments()
{
	std::print("[*] cSimPoll write setpoint to instruments\n");

	static double old_value[MAX_CHAN] = { 0 };
	
	for (auto meas : m_meas_pool)
	{
		size_t writeNumbers = meas->chan_write_count();
		assert(writeNumbers < MAX_CHAN);

		if (writeNumbers > 0)
		{
			size_t read;

			double value[MAX_CHAN] = { 0 };
			STEPSTRUCT step{};
			bool success = m_cycle_controler->get_current_step_param(step);
			if (!success)
			{
				std::cout << "[!] cSimplePoll: Failed to get current step.\n";
				return;
			}

			success = get_instr_setpoint(meas, step, value, &read);
			if (!success)
			{
				std::cout << "[!] cSimplePoll: Failed to get instrument setpoint.\n";
				return;
			}

			check_instr_setpoint(value, read, writeNumbers);

			//std::cout << "Value: " << value[0] << "\n";
			//std::cout << "Old Value: " << old_value[0] << "\n";

			int mod = 1;

			std::cout << value[0] << " " << value[1] << "\n";
			std::cout << old_value[0] << " " << old_value[1] << "\n";

			for (size_t i = 0; i < read; i++)
			{
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
				mod = 0; // reset
			}
		}
	}
	return;
}

void cSimplePoll::read_instruments()
{
	assert(m_meas_pool.size() > 0);
	assert(m_meas_pool.size() < MAX_CHAN);

	std::print("[*] cSimPoll read from instruments\n");
	DATAS datas;

	std::vector<std::vector<double>> datas_pack;
	for (auto meas : m_meas_pool)
	{
			// Read data from instrument

		datas = meas->read();
		assert(datas.buffer);
		assert(datas.buffer_size > 0);
		assert(datas.buffer_size < MAX_CHAN);

			// Add raw point to a vector

		std::vector<double> val;
		for (size_t i = 0; i < datas.buffer_size; i++)
		{	
			assert(datas.buffer[i]);
			val.push_back(datas.buffer[i]);

				// Add the vector of size 1 to the datas_pack of n signals

			datas_pack.push_back(val);
		}
	}
	assert(datas_pack.size() > 0);
	assert(datas_pack.size() < MAX_CHAN);

	m_plot->graph_addpoint(datas.buffer_size, datas.buffer);
	return;
}
