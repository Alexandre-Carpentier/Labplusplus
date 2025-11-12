#include "cPoller.h"
#include <print>
#include <chrono>
#include <thread>

cSimPoll::cSimPoll(cObjectmanager* obj_manager, cMeasurementmanager* meas_manager, std::shared_ptr<cCycleControler>cycle_controler)
	: m_obj_manager(obj_manager), m_meas_manager(meas_manager), m_cycle_controler(cycle_controler)
{
	std::print("[*] cSimPoll ctor...\n");
	assert(m_obj_manager != nullptr);
	assert(m_meas_manager != nullptr);
	assert(m_cycle_controler != nullptr);
}

void cSimPoll::runonce() 
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

void cSimPoll::loop() {
	read_instruments();
}

void cSimPoll::stop() {
	std::print("[*] cSimPoll stop\n");
	if (m_meas_pool.size() > 0)
	{
		zero_instrument(m_meas_pool);
	}
}

void cSimPoll::write_instruments()
{
	std::print("[*] cSimPoll write setpoint to instruments\n");
	return;
}

void cSimPoll::read_instruments()
{
	std::print("[*] cSimPoll read from instruments\n");
	double val1 = 12.0 + rand() % 2;
	std::vector<std::vector<double>> data;
	data.push_back(std::vector<double>{ val1 });
	m_plot->graph_addpoints(1, data, 1);
	return;
}
