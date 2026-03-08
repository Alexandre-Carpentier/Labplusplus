#include "cPoller.h"
//#include "cMeasurementControler.h"
#include <print>
#include <memory>
#include <array>

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

	// chrono start
	
	tick.start_tick();

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

			// call only if setpoint modified

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
	std::print("[*] cMultiPoll read from instruments\n");

	CHUNKS chunks;
	frame_vec datas_pack;

	assert(m_meas_pool.size() > 0);
	assert(m_meas_pool.size() < MAX_CHAN);
	assert(MAX_FRAME <= 4096);

	for (auto meas : m_meas_pool)
	{

			// Read data from instruments

		chunks = meas->read_multiple();
		assert(chunks.size() > 0);
		assert(chunks.size() <= MAX_CHAN);

			// Stack all frames

		for (auto &chunk : chunks)
		{
			datas_pack.push_back(chunk);
		}
	}
	assert(chunks.size() <= MAX_FRAME);

		// Build timestamps frame

	auto create_time_stamp = [](const double& initial_time, const double& dt) {
		frame timestamps ;
		double t0 = initial_time;
		for (double& t : timestamps)
		{
			t0 = t0 + (1/dt);
			t = t0;
		}
		return timestamps;
		};

	double initial_time = tick.get_tick();
	double dt = chunks.get_rate_hz();
	frame timestamps = create_time_stamp(initial_time, dt);

		// Send all points to plot module

	m_plot->graph_addpoints( timestamps, datas_pack);

	return;
}
