#pragma once

#include <wx/wx.h>

#include <vector>
#include <string>

#define max_chan_number  48

typedef struct {

	// General config
	bool device_enabled;
	wxArrayString  device_name;
	// Channels config

	int chan_number;// = max_chan_number;												// flex sizer 1
	int channel_index;
	std::vector <bool> channel_enabled;											// enable channel
	std::vector<std::string> channel_name;										// Name to assign
	wxArrayString channel_physical_name[max_chan_number];						// physical channel
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
}DEVICE_CONFIG_STRUCT;

typedef struct {

	// General config
	bool device_enabled;
	wxString  device_name;
	// Channels config

	int chan_number;// = max_chan_number;										// flex sizer 1
	int channel_index;
	std::vector <bool> channel_enabled;											// enable channel
	std::vector<std::string> channel_name;										// Name to assign
	wxString channel_physical_name[max_chan_number];							// physical channel
	wxString channel_type[max_chan_number];										// channel type: Voltage, termocouple, thermistor
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
}CURRENT_DEVICE_CONFIG_STRUCT;


enum MEAS_TYPE
{
	VOID_INSTR = 0,
	SUPER_INSTR,
	DAQ_INSTR,
	PRESSURECONTROLER_INSTR,
	VACUUMSENSOR_INSTR,
};

typedef struct
{
	MEAS_TYPE type;
	std::string  channel_legend_name;
	std::string  channel_legend_addr;
	std::string  channel_legend_unit;
	double channel_legend_min_value;
	double channel_legend_average_value;
	double channel_legend_max_value;
	wxColor channel_legend_color;
}CHAN_LEGEND_STRUCT, * PCHAN_LEGEND_STRUCT;