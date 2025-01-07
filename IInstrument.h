#ifndef _INSTRUMENT_H_
#define _INSTRUMENT_H_
#include <wx/wx.h>
#include <wx/simplebook.h>
#include <wx/panel.h>

#include <memory>

#include "data_types.h"
#include "enum.h"
#include "cMeasurementmanager.h"
//#include "..\Lab++\Plugin\cDevice.h"
#include "..\Lab\Plugin\cDevice.h"

#include "cDeviceMonitor.h";

#include "cTable.h"

class cInstrumentparam
{
public:
		cInstrumentparam(wxWindow* page, std::shared_ptr<cDeviceMonitor> devmon, std::string instrument_name) {

			uniqueID = (uint32_t)(&uniqueID) + rand() % 5;
			page_ = page;
			// right panel is the page parent
			panel_ = (wxPanel*)page->GetParent();
			devmon_ = std::move(devmon);

			instrument_name_ = instrument_name;
			instrument_address_ = devmon_->get_first_available();
			if (instrument_address_.size() == 0) { instrument_address_ = L"Empty"; };

			/////////////////////////////////////////////////////////////////
			// Default param
			/////////////////////////////////////////////////////////////////
			input_count_ = 1; // one measurement to perform
			outputcount_ = 1; // one command to send
			signal_count_ = 1; // one signal to plot in the window
	}

	size_t setDeviceStrategy(std::unique_ptr<cDevice> devimpl) { device_ = std::move(devimpl); }

	void setInputCount(size_t nb) { input_count_ = nb; }
	void setOutputCount(size_t nb) { outputcount_ = nb; }
	void setSignalCount(size_t nb) { signal_count_ = nb; }

	size_t getInputCount() { return input_count_; }
	size_t getOutputCount() { return outputcount_; }
	size_t getSignalCount() { return signal_count_; }

	wxBookCtrl* getPage() { if (page_) { return (wxBookCtrl*)page_; } return nullptr; }
	wxPanel* getPanel() { if (panel_) { return (wxPanel*)panel_; } }
	std::shared_ptr<cDeviceMonitor> getDeviceMonitor() { return std::move(devmon_); }
	std::string getDeviceName() { return instrument_name_; }

	std::unique_ptr<cDevice> getDeviceStrategy() { return std::move(device_); }
	size_t getUniqueID() { return uniqueID; }

private:
	// Mandatory to communicate with wxWidget (raw ptr is ok)
	// page is the book page to display plugin properties in.
	// panel is the panel where the book page is loaded (parent)
	wxWindow* page_ = nullptr;
	wxPanel* panel_ = nullptr;
	// Device lookup class shared between instruments
	// Need to retrieve available device lists
	std::shared_ptr<cDeviceMonitor> devmon_ = nullptr;
	// The name of the current instrument ex DSOX1202G
	std::string instrument_name_ = "";
	std::wstring instrument_address_ = L"";
	// Strategy to communicate: tcp, usb, com, visa, ...
	// Must be changed at runtime
	std::unique_ptr<cDevice> device_;
	// Identify how many connection is needed with others components
	size_t input_count_;
	size_t outputcount_;
	size_t signal_count_;
	// Unique identifier to reference if using same equipement names
	uint32_t uniqueID = 0 ;
};

class IInstrument : public wxFrame
{
public:
	// Constructor
	IInstrument(cInstrumentparam& param);
	// Destructor
	~IInstrument();
	// move ctor
	//IInstrument(IInstrument&& t);
	// move assign
	//IInstrument& operator=(IInstrument&& t);
	// Start device with current config
	size_t launch_device(); 
	// Retrieve the right panel
	wxPanel* get_right_panel();
	// Retrieve the configuration
	CURRENT_DEVICE_CONFIG_STRUCT GetOscopeConfigStruct();
	// Give the cTable reference to modify/read/...
	void set_table(cTable* m_table);
	// Save the config currently applied 
	void save_current_device_config(int channel_index);
	// Enable/disable buttons
	void lockBtn(bool lock);
	struct Instrumentimpl;
private:
	cInstrumentparam &param_;
	void OnPaint(wxPaintEvent& event);
	std::unique_ptr<Instrumentimpl> pimpl; // Firewall
};
#endif
