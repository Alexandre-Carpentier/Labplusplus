#ifndef _BUILDER_H_
#define _BUILDER_H_

//#define __WXMSW__ // To define with wxWidget
//#define WXUSINGDLL// To define to indicate DLL are use on this build


#include <wx/wx.h>
#include <wx/scrolwin.h>
#include <wx/spinctrl.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <iostream>
#include <memory>
#include "cProtocol.h"
#include "types.h"

#include <psapi.h> //GetModuleName()
EXTERN_C IMAGE_DOS_HEADER __ImageBase; // To get module base of the dll

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
    std::unique_ptr<cProtocol> protocol;

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
    std::string plugin_control_name = "Pressure";
    std::string plugin_control_unit = "Bar";

    // Define the plugin device name used to display
    std::string device_name;

    // When controls are drawn on plugin book
    void OnPaint();
};

//////////////////////////////////////////////////////////////
// INTERFACE TO BUILD
// -----------------------------------------------------------
// PROTOCOL
// 
// 
// 
// 
// 
class Builder {
public:
    virtual ~Builder() {}
    virtual void ProduceProtocol(SCPIMODE mode) const = 0; // One object to build
    virtual void ProducePanel(wxWindow* inst) const = 0; // Multiple wxControl* to build
    virtual void ProduceImage(wxWindow* inst) const = 0;   // Load afiliated picture on top of panel
    virtual void AddPanelCtrl(CONTROLTYPE type, int length, int height, std::string label)const = 0; // Build a button on the panel
};

class DeviceBuilder1 : public Builder {
private:

    std::unique_ptr <cDevice> product;

    /**
     * A fresh builder instance should contain a blank product object, which is
     * used in further assembly.
     */
public:

    DeviceBuilder1() {
        this->Reset();
    }

    ~DeviceBuilder1() {
        //delete product;
    }

    void Reset() {
        this->product = std::make_unique<cDevice>();
    }

    // build selected protocol

    void ProduceProtocol(SCPIMODE mode)const override;
    void ProducePanel(wxWindow* inst)const override;
    void ProduceImage(wxWindow* inst)const override;
    void AddPanelCtrl(CONTROLTYPE type, int length, int height, std::string label)const override;

    /*
    void ProducePartB()const override {
        this->product->parts_.push_back("PartB1");
    }

    void ProducePartC()const override {
        this->product->parts_.push_back("PartC1");
    }
    */
    /**
     * Concrete Builders are supposed to provide their own methods for
     * retrieving results. That's because various types of builders may create
     * entirely different products that don't follow the same interface.
     * Therefore, such methods cannot be declared in the base Builder interface
     * (at least in a statically typed programming language). Note that PHP is a
     * dynamically typed language and this method CAN be in the base interface.
     * However, we won't declare it there for the sake of clarity.
     *
     * Usually, after returning the end result to the client, a builder instance
     * is expected to be ready to start producing another product. That's why
     * it's a usual practice to call the reset method at the end of the
     * `getProduct` method body. However, this behavior is not mandatory, and
     * you can make your builders wait for an explicit reset call from the
     * client code before disposing of the previous result.
     */

     /**
      * Please be careful here with the memory ownership. Once you call
      * GetProduct the user of this function is responsable to release this
      * memory. Here could be a better option to use smart pointers to avoid
      * memory leaks
      */

    std::unique_ptr<cDevice> GetProduct()
    {
        // Scale controls on the panel before returning the full object
        //product->right_panel->SetSizerAndFit(product->grid_sizer);

        std::unique_ptr <cDevice> result = std::move(product);
        this->Reset();
        return result;
    }
};
#endif