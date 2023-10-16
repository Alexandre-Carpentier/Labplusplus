#include "cDaqsim.h"
#include "cMeasurement.h"
#include <string>

cDaqsim::cDaqsim()
{
    std::cout << "cDaqsim ctor...\n";
    result.buffer_size = 1;
};

std::string cDaqsim::device_name() { return std::string("DAQSIM"); }
MEAS_TYPE cDaqsim::device_type() { return DAQ_INSTR; };
size_t cDaqsim::chan_count()
{
    size_t nb_sig = 0;
    for (auto enable : config_struct_.channel_enabled)
    {
        if (enable == true)
        {
            nb_sig++;
        }
    }
    return nb_sig;
}

int cDaqsim::launch_device(CURRENT_DEVICE_CONFIG_STRUCT config_struct)
{
    config_struct_ = config_struct;
    return 0;
}

DATAS cDaqsim::read()
{
    int i = 0;
    for (auto active : config_struct_.channel_enabled)
    {
        if (active == true)
        {
            result.buffer[i] = (-42.0 + rand() % 10) + (10 * i);
            i++;
            result.buffer_size = i;
        }
    }
    return result;
}

void cDaqsim::stop_device() { std::cout << "cDaqsim->stoping...\n"; }

cDaqsim::~cDaqsim() { std::cout << "cDaqsim dtor...\n"; };


