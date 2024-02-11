#include "cLoader.h"
/*
#ifndef _DEBUG
class cMain :public wxFrame
{
public:
    cMain() : wxFrame(nullptr, wxID_ANY, "Lab++", wxPoint(200, 100), wxSize(1280, 720))
    {

    }
};
#endif

//////////////////////////////////////////////////////////////
// COMPONENT LOADER DLL
// -----------------------------------------------------------
// DLL ENTRYPOINT
//
//
//
//
//
#ifndef _DEBUG
    class cApp :public wxApp
    {
    public:
        virtual bool OnInit() {
            wxInitAllImageHandlers();
            m_Frame = new cMain();
            m_Frame->Show();

            // Run building function
            attach(m_Frame);

            return true;
        }
    private:
        cMain* m_Frame = nullptr;
    };

    wxIMPLEMENT_APP_CONSOLE(cApp);

#else
*/
#include <wx/wx.h>
#include <wx/msw/private.h>

class cDevice;
class DeviceBuilder1;

class nameDLLApp : public wxApp
{
    bool OnInit() { return true; }
};
IMPLEMENT_APP_NO_MAIN(nameDLLApp)

BOOL APIENTRY DllMain(HANDLE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        //std::cout << "[*] Dll process attach...\n";
        // Issue when loading the DLL, must set the instance with Windows
         wxSetInstance((HINSTANCE)hModule);
       // int argc = 0;
        //char** argv = NULL;
       // wxEntryStart(argc, argv);
    }
    break;

    case DLL_THREAD_ATTACH:
        break;

    case DLL_THREAD_DETACH:
        break;

    case DLL_PROCESS_DETACH:
        wxEntryCleanup();
        break;
    }
    return TRUE;
}
//#endif

cDevice* Attach(wxWindow* inst)
{
    // Define the object builder first
    // All differents creationnal choice are inside builder
    // it provide an interface to get the final object
    std::unique_ptr<DeviceBuilder1> builder = std::make_unique<DeviceBuilder1>();

    // Create our final product object before building it with different components
    std::unique_ptr<cDevice> dev;

    std::cout << "[*] Plugin builder started...\n";
    // Build a protocol object containing specific concrete code for communicate with the device
    // 
    // Set protocol to COM
    builder->ProduceProtocol(SCPIMODE::COM);

    builder->ProducePanel(inst);
    builder->ProduceImage(inst);

    builder->AddPanelCtrl(CONTROLTYPE::TXT, 150, 20, "Device name: ");
    builder->AddPanelCtrl(CONTROLTYPE::TXTFIELD, 150, 20, "Controler0");

    builder->AddPanelCtrl(CONTROLTYPE::TXT, 150, 40, "Communication interface (COM/TCP/USB): ");
    builder->AddPanelCtrl(CONTROLTYPE::TXTFIELD, 150, 20, "COM");

    builder->AddPanelCtrl(CONTROLTYPE::TXT, 150, 20, "Adrress: ");
    builder->AddPanelCtrl(CONTROLTYPE::TXTFIELD, 150, 20, "DEV0/");

    builder->AddPanelCtrl(CONTROLTYPE::BUT, 150, 20, "Query test equipment?");
    builder->AddPanelCtrl(CONTROLTYPE::SPINBUT, 150, 20, "Run self test checkup?");

    builder->AddPanelCtrl(CONTROLTYPE::TXT, 150, 20, "Init command sequence: ");
    builder->AddPanelCtrl(CONTROLTYPE::TXTFIELD, 400, 200, "*IDN?\\n");

    builder->AddPanelCtrl(CONTROLTYPE::TXT, 150, 20, "Read operation sequence: ");
    builder->AddPanelCtrl(CONTROLTYPE::TXTFIELD, 400, 200, "*Read 1000 24?\\n");

    builder->AddPanelCtrl(CONTROLTYPE::TXT, 150, 20, "Write command sequence: ");
    builder->AddPanelCtrl(CONTROLTYPE::TXTFIELD, 400, 200, "SETpoint 12.0\\n");

    builder->AddPanelCtrl(CONTROLTYPE::TXT, 150, 20, "Cleanup sequence: ");
    builder->AddPanelCtrl(CONTROLTYPE::TXTFIELD, 400, 200, "*RST\\n");

    // Get the final product to return to the main application
    dev = builder->GetProduct();

    // Size properly items inside the sizer
    dev->header_v_sizer->Add(dev->header_h_sizer); // Header
    dev->header_v_sizer->Add(dev->grid_sizer); // Grid
    dev->panel->SetSizerAndFit(dev->header_v_sizer); // Main vertical sizer



    // Display attributres of the object for debug purposes
    dev->DisplayConfiguration();
    // Use SCPI
    dev->scpi_open("COM0");
    dev->scpi_write("*IDN?");
    dev->scpi_close();
    return dev.release(); // Release smart pointer... to raw pointer...
}

