#include "cObjectmanager.h"
#include <iostream>

// Meyers singleton - thread-safe since C++11
cObjectmanager* cObjectmanager::getInstance()
{
    static cObjectmanager instance;
    return &instance;
}

void cObjectmanager::kill()
{
    std::cout << "[*] cObjectmanager::kill()\n";

    // Status bar is owned by the main frame (wxFrame::CreateStatusBar).
    // // Do not delete it here; simply clear our pointer reference.
    if (statusBar_)
    {
        std::cout << "  - clearing statusBar_ reference\n";
        statusBar_ = nullptr;
    }

    // Delete owned objects if any. Be conservative: only delete objects
    // that this manager is clearly the owner of. If some objects are also
    // deleted elsewhere adjust accordingly to prevent double-delete.



    if (m_footer_)
    {
        m_footer_->cycle_controler->stop();
        m_footer_->meas_controler->stop();
		Sleep(500); // Wait for threads to stop before deleting objects
        std::cout << "  - deleting m_footer_\n";
        delete m_footer_;
        m_footer_ = nullptr;
    }

    if (m_plot_)
    {
        std::cout << "  - deleting m_plot_\n";
        //delete m_plot_;
        //m_plot_ = nullptr;
    }

    if (m_table_)
    {
        std::cout << "  - deleting m_table_\n";
        //delete m_table_;
        //m_table_ = nullptr;
    }

    if (m_config_)
    {
        std::cout << "  - deleting m_config_\n";
        //delete m_config_;
        //m_config_ = nullptr;
    }

        // Free heap memory 
    cMeasurementmanager* meas_manager = meas_manager->getInstance();
    bool isDestroyed = meas_manager->destroy_subsystem(MEAS_TYPE::DAQ_INSTR);

    // If item destroyed delete from memory
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