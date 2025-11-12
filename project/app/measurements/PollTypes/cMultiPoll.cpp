#include "cPoller.h"
//#include "cMeasurementControler.h"
#include <print>
#include <chrono>
#include <thread>
#include <memory>

cMultiPoll::cMultiPoll(cObjectmanager* obj_manager, cMeasurementmanager* meas_manager, std::shared_ptr<cCycleControler>cycle_controler, std::vector<cMeasurement*> meas_pool)
	: m_obj_manager(obj_manager), m_meas_manager(meas_manager), m_cycle_controler(cycle_controler), m_meas_pool(meas_pool)
{
	std::print("[*] cSimPoll ctor...\n");
	assert(m_obj_manager != nullptr);
	assert(m_meas_manager != nullptr);
	assert(m_cycle_controler != nullptr);
}

void cMultiPoll::runonce()
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

	// Set instrument initial value

	write_instruments();
}

void cMultiPoll::loop() {
	write_instruments();
	read_instruments();
}

void cMultiPoll::stop() {
	std::print("[*] cSimPoll stop\n");
	if (m_meas_pool.size() > 0)
	{
		zero_instrument(m_meas_pool);
	}
}

// Helper functions
// Used by write instruments methods
inline void check_instr_setpoint(double value[MAX_CHAN], size_t read, size_t len)
{
	std::print("[GET] double[]:{};{};{};{}, length:{}\n", value[0], value[1], value[2], value[3], read);
	assert(len == read);
	assert(read < MAX_CHAN);
}

void cMultiPoll::write_instruments()
{
	std::print("[*] cSimPoll write setpoint to instruments\n");

	static double old_value[MAX_CHAN] = { 0 };

	for (auto meas : m_meas_pool)
	{
		size_t length = meas->chan_write_count();
		assert(length < MAX_CHAN);

		if (length > 0)
		{
			size_t read;

			// protect

			m_cycle_controler->cycle_mutex.lock();

			double value[MAX_CHAN];
			STEPSTRUCT step = m_cycle_controler->get_current_step_param();
			bool success = get_instr_setpoint(meas, step, value, &read);
			// unprotect

			m_cycle_controler->cycle_mutex.unlock();

			if (!success)
			{
				continue;
			}

			assert(read == length);
			check_instr_setpoint(value, read, length);

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

void cMultiPoll::read_instruments()
{
	CHUNKS chunks;
	assert(m_meas_pool.size() > 0);
	assert(m_meas_pool.size() < MAX_CHAN);
	assert(MAX_FRAME <= 4096);

	std::print("[*] cSimPoll read from instruments\n");
	std::vector<std::vector<double>> datas_pack;

	size_t signals_count = 0;
	for (auto meas : m_meas_pool)
	{
			// Read data from instruments

		chunks = meas->read_multiple();
		assert(chunks.buffer_numbers > 0);
		assert(chunks.buffer_numbers <= MAX_CHAN);

			// Add buffers count

		signals_count += chunks.buffer_numbers;

			// Stack all frames

		for (auto chunk : chunks.buffer)
		{
			datas_pack.push_back(chunk);
		}
	}
	assert(chunks.buffer[0].size() <= MAX_FRAME);
	assert(chunks.buffer.size() == signals_count);
	assert(signals_count > 0);
	assert(signals_count < MAX_CHAN);

		// Send all points to plot module

	m_plot->graph_addpoints(signals_count, datas_pack, chunks.buffer[0].size());
	return;
}
