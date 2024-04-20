#pragma once

//#define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING
//#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

#include <wx/dcbuffer.h>
#include <wx/wx.h>
#include <wx/combobox.h>
#include <wx/treectrl.h>
#include <wx/simplebook.h>

#include <locale>
#include <codecvt>
#include <map>

#include "cPlot.h"
#include "cCycle.h"
#include "cDaqmx.h"
#include "cPressure.h"
#include "cTension.h"
#include "cObjectmanager.h"
#include "cMeasurementControler.h"

//#pragma comment (lib, "Plugin.lib")
#include "..\Lab\Plugin\cDevice.h"
//#include "..\Lab++\Plugin\cDevice.h"

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

	cPlot* m_plot_ = nullptr;
	cDaqmx* m_daqmx = nullptr;
	cPressure* m_pressure = nullptr;
	cTension* m_tension = nullptr;

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
	cConfig(wxWindow* inst);
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
	cDaqmx* get_nidaq();

	std::vector<cDevice*> get_device_vec();
	std::vector<std::string> get_plugin_name_vec();
	std::vector<std::string> get_plugin_unit_vec();

	void set_graph(cPlot* m_plot);
	void OnPaint(wxPaintEvent& event);
};



