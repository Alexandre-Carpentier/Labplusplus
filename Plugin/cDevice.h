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

    //typedef std::shared_ptr<cDevice>(__cdecl* ATTACH)(wxWindow*);

//typedef cDevice* (*ATTACH)(wxWindow*);


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
    // Plugin information
//#define _C_STYLE_
#ifdef _C_STYLE_
// C style
    char device_name[MAX_PATH] = "2280S";
    PLUGIN_ACCESS plugin_access_type = ALL;
    int plugin_input_number = 1;
    int plugin_output_number = 1;
    char plugin_measurement_name[MAX_PATH] = "Voltage";
    char plugin_measurement_unit[MAX_PATH] = "Volt";
#else
    //modern c++
    std::string device_name = "2280S";
    PLUGIN_ACCESS plugin_access_type = ALL;
    int plugin_input_number = 1;
    int plugin_output_number = 1;
    std::string plugin_measurement_name = "Voltage";
    std::string plugin_measurement_unit = "Volt";
#endif
    short uniqueID = rand() % 100 + std::hash <std::string>{}(device_name);

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


    void set_lua_state_ptr(void* inst)
    {
        lua_state_ptr = inst;
    }
#ifdef _C_STYLE_
    // C style 
    void scpi_open(char* addr);
    void scpi_write(char* command);
    char* scpi_read();
    void scpi_close();


    char* get_device_name();
    int get_access_type();
    char* get_measurement_name();
    char* get_measurement_unit();

    void set_device_name(char* name);
    void set_access_type(int type);
    void set_measurement_name(char* meas_name);
    void set_measurement_unit(char* meas_unit); 
     //modern c++
#else
    void scpi_open(std::string addr);
    void scpi_write(std::string command);
    std::string scpi_read();
    void scpi_close();


    std::string get_device_name();
    int get_access_type();
    int get_input_number() { return plugin_input_number; };
    int get_output_number() { return plugin_output_number; };

    std::string get_measurement_name();
    std::string get_measurement_unit();

    void set_device_name(std::string name);
    void set_access_type(int type);
    void set_measurement_name(std::string meas_name);
    void set_measurement_unit(std::string meas_unit);
#endif
    // 
    // Get current measured value
    // Set current instrument set point
    // 
    // Start plugin acquizition function
    // Stop plugin acquizition function

    void* get_lua_state_ptr()
    {
        return lua_state_ptr;
    }



    // Store the lua instance here
    void* lua_state_ptr = nullptr; 

    // When controls are drawn on plugin book
    void OnPaint();
};