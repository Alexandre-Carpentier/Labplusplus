#ifndef _DAQ6510_H_
#define _DAQ6510_H_
#include <wx/wx.h>
#include "data_types.h"
#include <memory>

class cTable;
class cDeviceMonitor;

static wxImage daq6510_instrument_img;
static wxPanel* daq6510_instrument_rightpanel_ = nullptr;

class c6510ui : public wxFrame
{
public:
	c6510ui(wxWindow* inst, std::shared_ptr <cDeviceMonitor> devmon);
	~c6510ui();

	size_t launch_device(); // Start device with current config
	wxPanel* get_right_panel();
	CURRENT_DEVICE_CONFIG_STRUCT GetDaq6510ConfigStruct();
	void set_table(cTable* m_table);
	void save_current_device_config(int channel_index);
	void lockBtn(bool lock);
private:
	void OnPaint(wxPaintEvent& event);
	struct my_impl;
	std::unique_ptr<my_impl> pimpl; // Firewall
};
#endif
