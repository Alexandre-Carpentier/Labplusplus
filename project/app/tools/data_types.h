/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include <wx/wx.h>

#include <vector>
#include <string>

#define max_chan_number  48

#define CHANANALOG 0
#define CHANDIGITAL 1

#define CHANREAD 0
#define CHANWRITE  1

typedef struct {

	// General config
	bool device_enabled;
	wxArrayString  device_name;
	wxArrayString  device_serial_number[max_chan_number];
	// Channels config

	int chan_number;// = max_chan_number;										// flex sizer 1
	int channel_index;
	std::vector <bool> channel_enabled;											// enable channel
	std::vector<std::string> channel_name;										// Name to assign
	std::vector <int> channel_mode;												// ANALOG/DIGITAL channel
	std::vector <int> channel_permision;										// READ/WRITE channel
	wxArrayString channel_physical_name[max_chan_number];						// physical channel
	wxArrayString channel_physical_unit[max_chan_number];						// physical unit
	wxArrayString  channel_serial_number[max_chan_number];						// Connected device serial number S/N ex "LE140"

	//////////////////////////////////////////////////////////
	// ANALOG
	//////////////////////////////////////////////////////////
	wxArrayString channel_type[max_chan_number];								// channel type: Voltage, termocouple, thermistor
	wxArrayString channel_max[max_chan_number];									// Max input range
	wxArrayString channel_min[max_chan_number];									// Min input range
	wxArrayString channel_mode_type[max_chan_number];							// Input mode type
	wxArrayString channel_tc_type[max_chan_number];								// Tc type
	std::string channel_tc_min[max_chan_number];								// Tc min temp
	std::string channel_tc_max[max_chan_number];								// Tc max temp
	// flex sizer 2
	wxArrayString channel_linearize[max_chan_number];							// linearize
	std::string channel_linearize_slope[max_chan_number];						// slope linear
	std::string channel_linearize_shift[max_chan_number];						// shift linear
	std::string channel_linearize_unit[max_chan_number];						// unit
	wxArrayString channel_filter[max_chan_number];								// filter
	std::string channel_filter_intensity[max_chan_number];						// filter intensity
	// flex sizer 3
	wxArrayString channel_trigger[max_chan_number];								// trigger
	std::string channel_trigger_threshold[max_chan_number];						// threshold

	//////////////////////////////////////////////////////////
	// DIGITAL
	//////////////////////////////////////////////////////////
	wxArrayString digital_channel_type[max_chan_number];						// channel type: input/output
	wxArrayString digital_channel_mode_type[max_chan_number];					// Input mode type: internal pullup/ no pullup

}DEVICE_CONFIG_STRUCT;

typedef struct {

	// General config
	bool device_enabled;
	wxString  device_name;														// Device addr ex "DEV1"
	wxString  device_addr;														// Device addr ex "USB-6001::Dev1"
	wxString  device_serial_number[max_chan_number];							// Device serial number S/N ex "LE107"
	// Channels config
	
	int chan_number;// = max_chan_number;										// flex sizer 1
	int channel_index;
	std::vector <bool> channel_enabled;											// enable channel
	std::vector<std::string> channel_name;										// Name to assign
	std::vector <int> channel_mode;												// ANALOG/DIGITAL channel
	std::vector <int> channel_permision;										// READ/WRITE channel
	wxString channel_physical_name[max_chan_number];							// physical channel
	wxString channel_physical_unit[max_chan_number];							// physical unit
	wxString  channel_serial_number[max_chan_number];							// Connected device serial number S/N ex "LE140"

	//////////////////////////////////////////////////////////
	// ANALOG
	//////////////////////////////////////////////////////////
	wxString channel_type[max_chan_number];										// channel type: Voltage, termocouple, thermistor, LOGIC_IN
	wxString channel_max[max_chan_number];										// Max input range
	wxString channel_min[max_chan_number];										// Min input range
	wxString channel_mode_type[max_chan_number];								// Input mode type
	wxString channel_tc_type[max_chan_number];									// Tc type
	wxString channel_tc_min[max_chan_number];									// Tc min temp
	wxString channel_tc_max[max_chan_number];									// Tc max temp
	// flex sizer 2
	wxString channel_linearize[max_chan_number];								// linearize
	wxString channel_linearize_slope[max_chan_number];							// slope linear
	wxString channel_linearize_shift[max_chan_number];							// shift linear
	std::string channel_linearize_unit[max_chan_number];						// unit
	std::string channel_filter[max_chan_number];								// filter
	wxString channel_filter_intensity[max_chan_number];							// filter intensity
	// flex sizer 3
	std::string channel_trigger[max_chan_number];								// trigger
	wxString channel_trigger_threshold[max_chan_number];						// threshold

	//////////////////////////////////////////////////////////
	// DIGITAL
	//////////////////////////////////////////////////////////
	wxString digital_channel_type[max_chan_number];								// channel type: input/output
	wxString digital_channel_mode_type[max_chan_number];						// Input mode type: internal pullup/ no pullup

}CURRENT_DEVICE_CONFIG_STRUCT;


enum MEAS_TYPE
{
	VOID_INSTR = 0,
	SUPER_INSTR,
	DAQ_INSTR,
	PRESSURE_CONTROLER_INSTR,
	VOLTAGE_CONTROLER_INSTR,
	FLOW_CONTROLER_INSTR,
	FLOW_METER_INSTR,
	VALVE_CONTROLER_INSTR
};

typedef struct
{
	size_t id;
	std::string  channel_legend_name;
	std::string  channel_legend_addr;
	std::string  channel_legend_unit;
	double channel_legend_min_value;
	double channel_legend_average_value;
	double channel_legend_max_value;
	wxColor channel_legend_color;
}CHAN_LEGEND_STRUCT, * PCHAN_LEGEND_STRUCT;