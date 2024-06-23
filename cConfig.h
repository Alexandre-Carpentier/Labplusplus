#pragma once
#include <wx/wx.h>
#include <wx/simplebook.h>

class cTable;
class cPlot;
class cDaqmx;
class cDevice;
class cPressure;
class cVoltage;
class cOscope;
class cDeviceMonitor;

class cConfig : public wxFrame
{
private:
	wxWindow* inst_;
	wxPanel* config_leftpanel_ = nullptr;
	wxPanel* config_rightpanel_ = nullptr;
	wxBoxSizer* config_hsizer_ = nullptr;

	wxSimplebook* book = nullptr;

	wxBoxSizer* config_panel_sizer = nullptr;
	wxPanel* config_panel = nullptr;

	cDeviceMonitor* devmon_ = nullptr;
	cTable* m_table_ = nullptr;
	cPlot* m_plot_ = nullptr;
	cDaqmx* m_daqmx = nullptr;
	cDevice* m_daq_dev = nullptr;
	cPressure* m_pressure = nullptr;
	cVoltage* m_voltage = nullptr;
	cOscope* m_oscope = nullptr;

	typedef cDevice* (*PLUGIN_ATTACH)(wxWindow*);
	PLUGIN_ATTACH Attach = nullptr;
	typedef bool (*PLUGIN_DETTACH)();
	PLUGIN_DETTACH Dettach = nullptr;
	typedef bool (*PLUGIN_START)();
	PLUGIN_START PStart = nullptr;
	typedef bool (*PLUGIN_STOP)();
	PLUGIN_START PStop = nullptr;

	struct PLUGIN_DATA
	{
		std::wstring name = L"";
		wxPanel* panel = nullptr;
		cDevice* device;
		HINSTANCE hInst = nullptr;
		// Fct ptr
		PLUGIN_ATTACH Attach = nullptr;
		PLUGIN_DETTACH Dettach = nullptr;
		PLUGIN_START PStart = nullptr;
		PLUGIN_STOP PStop = nullptr;
		// Signal configuration
		size_t input_count;
		size_t outputcount;
		size_t signal_count;
		short uniqueID;
	};
	std::vector<PLUGIN_DATA> plugin_vec;

public:
	cConfig(wxWindow* inst, cTable* m_table, cDeviceMonitor* devmon);
	~cConfig();
	void OnClickdrop(wxMouseEvent& evt);
	void StopTest(bool stop);
	void unload_plugins();
	void load_plugins(wxWindow* parent, std::wstring folder_path);
	wxPanel* Getleftpan();
	wxPanel* Getrightpan();
	wxBoxSizer* Get_hsizer();

	// Plugin and device access here
	cPressure* get_pressuredevice();
	cVoltage* get_voltagedevice();
	cOscope* get_oscopedevice();
	cDaqmx* get_nidaq();

	std::vector<cDevice*> get_device_vec();
	std::vector<std::string> get_plugin_name_vec();
	std::vector<std::string> get_plugin_unit_vec();
	std::vector<size_t> get_plugin_output_number();

	void set_graph(cPlot* m_plot);
	void set_table(cTable* m_table);
	void OnPaint(wxPaintEvent& event);
};



