#include "cObjectmanager.h"
#include "cMeasurementControler.h"
#include "cCycleControler.h"
#include "cFooter.h"
#include <iostream>
#include <Windows.h>

// Meyers singleton - thread-safe since C++11
cObjectmanager* cObjectmanager::getInstance()
{
    static cObjectmanager instance;
    return &instance;
}

void cObjectmanager::kill()
{
    std::cout << "[*] cObjectmanager::kill()\n";

    // [!] Status bar is owned by the main frame (wxFrame::CreateStatusBar).
    // [!] Do not delete it here; simply clear our pointer reference.
    if (statusBar_)
    {
        std::cout << "  - clearing statusBar_ reference\n";
        statusBar_ = nullptr;
    }

    // [*] Stop threads BEFORE deleting objects that own them
    if (m_footer_)
    {
        if (m_footer_->cycle_controler)
        {
            m_footer_->cycle_controler->stop();
        }
        if (m_footer_->meas_controler)
        {
            m_footer_->meas_controler->stop();
        }
        // [*] Give threads time to gracefully exit
        Sleep(500);
    }

    // [*] Delete owned objects AFTER stopping threads
    if (m_footer_)
    {
        std::cout << "  - deleting m_footer_\n";
        delete m_footer_;
        m_footer_ = nullptr;
    }

    if (m_plot_)
    {
        std::cout << "  - deleting m_plot_\n";
        // [!] Uncomment when ready: delete m_plot_;
        // m_plot_ = nullptr;
    }

    if (m_table_)
    {
        std::cout << "  - deleting m_table_\n";
        // [!] Uncomment when ready: delete m_table_;
        // m_table_ = nullptr;
    }

    if (m_config_)
    {
        std::cout << "  - deleting m_config_\n";
        // [!] Uncomment when ready: delete m_config_;
        // m_config_ = nullptr;
    }

    // [*] Free heap memory 
    cMeasurementmanager* meas_manager = cMeasurementmanager::getInstance();
    bool isDestroyed = meas_manager->destroy_subsystem(MEAS_TYPE::DAQ_INSTR);

    // [*] If item destroyed delete from memory
    if (isDestroyed)
    {
        std::cout << "[*] [delete] m_daq in cDaqmx.cpp\n";
        if (m_daq_)
        {
            delete m_daq_;
            m_daq_ = nullptr;
        }
    }

    // Optionally reset other internal state
    // e.g. any vectors/lists of pointers maintained by the manager.
}