#include "cPacesim.h"
#include "cMeasurement.h"
#include <string>

cPacesim::cPacesim()
{
    std::cout << "cPacesim ctor...\n";
    result.buffer_size = 1;
};

std::string cPacesim::device_name() { return std::string("PACESIM"); }
MEAS_TYPE cPacesim::device_type() { return PRESSURECONTROLER_INSTR; };

size_t cPacesim::chan_count()
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

int cPacesim::launch_device(CURRENT_DEVICE_CONFIG_STRUCT config_struct)
{
    config_struct_ = config_struct;
    return 0;
}

DATAS cPacesim::read()
{
    int i = 0;
    for (auto active : config_struct_.channel_enabled)
    {
        if (active == true)
        {
            result.buffer[i] = (current_fake_value + rand() % 10) + (10 * i);
            i++;
            result.buffer_size = i;
        }
    }
    return result;
}

void cPacesim::set(double value)
{
    current_fake_value = value;
}

void cPacesim::stop_device() { std::cout << "cPacesim->stoping...\n"; }

cPacesim::~cPacesim() { std::cout << "cPacesim dtor...\n"; };


