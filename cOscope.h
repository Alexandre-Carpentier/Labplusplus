/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#ifndef _OSCOPE_H_
#define _OSCOPE_H_
#include <wx/wx.h>
#include "data_types.h"
#include <memory>

class cTable;
class cDeviceMonitor;

static wxImage oscope_instrument_img;
static wxPanel* oscope_instrument_rightpanel_ = nullptr;

class cOscope : public wxFrame
{
public:
	cOscope(wxWindow* inst, std::shared_ptr <cDeviceMonitor> devmon);
	~cOscope();

	size_t launch_device(); // Start device with current config
	wxPanel* get_right_panel();
	CURRENT_DEVICE_CONFIG_STRUCT GetOscopeConfigStruct();
	void set_table(cTable* m_table);
	void save_current_device_config(int channel_index);
	void lockBtn(bool lock);
private:
	void OnPaint(wxPaintEvent& event);
	struct Oscopeimpl;
	std::unique_ptr<Oscopeimpl> pimpl; // Firewall
};
#endif
