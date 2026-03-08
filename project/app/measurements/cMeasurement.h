/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include <string>
#include <print>
#include <array>
#include <vector>
#include "data_types.h"

const short MAX_CHAN = 64;
const short MAX_FRAME = 512;

// Use DATAS to read single frame from instruments
struct DATAS
{
	size_t buffer_size =0;
	double buffer[MAX_CHAN] = { 0 };
	std::string unit = "Unit";
};

// Use CHUNKS to read multiple frames from instruments
// Example:
// 
//CHUNKS chunks(4, 1000, "V");
//std::for_each(chunks.begin(), chunks.end(), [](auto& frame) {
// traiter chaque trame (std::array<double, MAX_FRAME>)
//});
class CHUNKS {
private:
	std::vector<std::array<double, MAX_FRAME>> m_signals_buffers;
	double m_rate_hz = 1000;
	std::string m_unit = "Unit";
public:
	using frame_vec = std::vector<std::array<double, MAX_FRAME>>;
	using frame = std::array<double, MAX_FRAME>;

	using iterator = frame_vec::iterator;
	using const_iterator = frame_vec::const_iterator;

	// Constructor
	CHUNKS(size_t channels = 0, size_t rate_hz = 1, std::string unit = "Unit")
		:  m_signals_buffers(channels, std::array<double, MAX_FRAME>{}),
		  m_rate_hz(rate_hz),
		  m_unit(unit)
	{
		std::print("[*] CHUNKS ctor with {} channels at {} Hz\n", channels, rate_hz);
	}

	// Push back a new frame
	void push_back(const std::array<double, MAX_FRAME>& frame)
	{
		m_signals_buffers.push_back(frame);
	}

	void clear()
	{
		m_signals_buffers.clear();
	}

	double get_rate_hz() const noexcept { return m_rate_hz; }
	void set_rate_hz(double rate) noexcept { m_rate_hz = rate; }
	void get_unit(std::string& unit) const noexcept { unit = m_unit; }
	frame_vec get_buffers() const noexcept { return m_signals_buffers; }
	frame get_buffer(size_t idx) const noexcept { return m_signals_buffers[idx]; }
	
	// Iterators (non-const / const)
	iterator begin() noexcept { return m_signals_buffers.begin(); }
	iterator end() noexcept { return m_signals_buffers.end(); }
	const_iterator begin() const noexcept { return m_signals_buffers.begin(); }
	const_iterator end() const noexcept { return m_signals_buffers.end(); }
	const_iterator cbegin() const noexcept { return m_signals_buffers.cbegin(); }
	const_iterator cend() const noexcept { return m_signals_buffers.cend(); }

	// Size and element access
	size_t size() const noexcept { return m_signals_buffers.size(); }
	std::array<double, MAX_FRAME>& operator[](size_t idx) { return m_signals_buffers[idx]; }
	const std::array<double, MAX_FRAME>& operator[](size_t idx) const { return m_signals_buffers[idx]; }
};

class cMeasurement {
private:
	DATAS result_struct;
	CURRENT_DEVICE_CONFIG_STRUCT config_struct_;
public:

	virtual void set_configuration_struct(CURRENT_DEVICE_CONFIG_STRUCT *config_struct) = 0;
	virtual int launch_device() = 0;

	virtual std::string device_name() = 0;
	virtual MEAS_TYPE device_type() = 0;
	virtual size_t chan_count() = 0;
	virtual size_t chan_read_count() = 0;
	virtual size_t chan_write_count() = 0;

	virtual DATAS read()=0;
	virtual CHUNKS read_multiple();
	virtual void set(double *value, size_t length) = 0;

	
	virtual void set_device_name(std::string name);
	virtual void set_device_addr(std::string addr);

	void start_device();
	virtual void stop_device() = 0;
};








