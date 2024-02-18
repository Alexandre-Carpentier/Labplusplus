#pragma once
#include <iostream>
#include "memory"
#include <wx/wx.h>
#include <wx/scrolwin.h>
#include <wx/spinctrl.h>
#include <wx/button.h>
#include <wx/sizer.h>

#include "cProtocol.h"
//#include "types.h"

enum PLUGIN_ACCESS {
    READ,
    WRITE,
    ALL
};

//////////////////////////////////////////////////////////////
// DEVICE TO BE BUILD
// -----------------------------------------------------------
// ACCESS TYPE (READ, WRITE, ALL)
// PROTOCOL
// iMG
// PANEL COMPONENT WITH DIFFERENT CONTROLS
// DEFINE DIFFERENT CALLBACK TO LINK CONTROLS AND FUNCTIONS
// INIT SEQ
// READ CURRENT VALUE
// SET CURRENT VALUE (controler only)
// CLOSING SEQ
class cDevice
{
public:
    // cProtocol abstract COM,USB,TCP,DAQMX,VISA protocol
    plug_cProtocol* protocol;

    // Raw ptr of the right panel to load inside main application
    wxScrolled <wxPanel>* panel = nullptr;
    // header with picture and text
    wxStaticBitmap* instr_img = nullptr;
    wxStaticText* header = nullptr;

    // Raw ptr of the main vertical sizer to handle every other sizers
    wxBoxSizer* header_v_sizer = nullptr;
    // Raw ptr of a sizer to handle header image and plugin name
    wxBoxSizer* header_h_sizer = nullptr;
    // Raw ptr of a grid to display item inside
    wxFlexGridSizer* grid_sizer = nullptr;

    // Debug configuration diplay
    void DisplayConfiguration();

    // attribute a name to the device
    void set_device_name(std::string name);

    // SCPI interface
    void scpi_open(std::string addr);
    void scpi_write(std::string command);
    std::string scpi_read();
    void scpi_close();

    // attribute a name to the device
    std::string get_device_name();
public:
    // Get device type (reader / writter/ all)
public:
    int get_device_access_type();
    // Get control name (access ALL only)
public:
    std::string get_plugin_name();
    // Get control unit (access ALL only)
public:
    std::string plugin_unit();
    // 
    // Get current measured value
    // Set current instrument set point
    // 
    // Start plugin acquizition function
    // Stop plugin acquizition function


private:
    // Define if the plugin can be set, can be read, or both
    // If writable, plugin must provide a unit to control in cTable.
    // This property must be accessible when launched

    PLUGIN_ACCESS plugin_access_type = READ;
    std::string plugin_control_name = "Voltage";
    std::string plugin_control_unit = "Volt";

    // Define the plugin device name used to display
    std::string device_name = "2280S";

    // When controls are drawn on plugin book
    void OnPaint();
};