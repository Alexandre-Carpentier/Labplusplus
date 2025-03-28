/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#include "cMeasurementmanager.h"
#include "cMeasurementControler.h"

void cMeasurementmanager::set_measurement_controler(std::shared_ptr<cMeasurementControler> m_controler)
{
	m_meas_controler_ = m_controler;
}

void cMeasurementmanager::set_measurement(cMeasurement* measurement)
{
	m_meas_pool.push_back(measurement);
}

std::shared_ptr<cMeasurementControler> cMeasurementmanager::get_measurement_controler()
{
	return m_meas_controler_;
}

std::vector<cMeasurement*> cMeasurementmanager::get_measurement_pool()
{
	return m_meas_pool;
}

size_t cMeasurementmanager::get_measurement_pool_size()
{
	return m_meas_pool.size();
}

cMeasurement* cMeasurementmanager::get_measurement(size_t position)
{
	return m_meas_pool.at(position);
}

MEAS_TYPE cMeasurementmanager::get_measurement_type(cMeasurement* meas)
{
	return meas->device_type();
}

size_t cMeasurementmanager::get_measurement_channel_number(cMeasurement* meas)
{
	return meas->chan_count();
}

size_t cMeasurementmanager::get_measurement_total_channel_number()
{
	size_t count = 0;
	for (auto obj : m_meas_pool)
	{
		count += obj->chan_count();
	}
	std::cout << "[*] Measurement total signals:" << count << "\n";
	return count;
}

void cMeasurementmanager::destroy_measurement_pool()
{
	for (auto meas : m_meas_pool)
	{
		std::cout << "[*] Stoping: " << meas->device_name() << "\n";
		meas->stop_device();
		std::cout << "[*] [delete] meas in cMeasurementmanager.cpp\n";
		delete meas;
		meas = nullptr;
	}
	m_meas_pool.clear();
}

void cMeasurementmanager::start_all_devices()
{
	for (auto meas : m_meas_pool)
	{
		std::cout << "[*] Starting: " << meas->device_name() << "\n";
		meas->start_device();
	}
}

void cMeasurementmanager::stop_all_devices()
{
	for (auto meas : m_meas_pool)
	{
		std::cout << "[*] Stoping: " << meas->device_name() << "\n";
		meas->stop_device();
	}
}

bool cMeasurementmanager::destroy_subsystem(MEAS_TYPE type)
{
	if (get_measurement_pool_size() < 1)
	{
		std::cout << "[*] No subsystem to destroy\n";
		return false;
	}

	int i = 0;
	for (auto meas : m_meas_pool)
	{
		if (meas->device_type() == type)
		{
			m_meas_pool.erase(m_meas_pool.begin() + i);
			std::cout << "[*] ";
			std::cout << meas->device_name();
			std::cout << " subsystem destroyed\n";
			return true;
		}
		i++;
	}

	return false;
}