#include "cPacesim.h"
#include "cMeasurement.h"
#include "cTick.h"

#include <string>

cPacesim::cPacesim()
{
    std::cout << "cPacesim ctor...\n";
    result.buffer_size = 1;
};

std::string cPacesim::device_name() { 
    return config_struct_.device_name.ToStdString();
}

MEAS_TYPE cPacesim::device_type() { return PRESSURE_CONTROLER_INSTR; };

size_t cPacesim::chan_count()
{
    size_t nb_sig = 1;
    return nb_sig;
}

size_t cPacesim::chan_read_count()
{
    size_t nb_sig = 1;
    return nb_sig;
}

size_t cPacesim::chan_write_count()
{
    size_t nb_sig = 1;
    return nb_sig;
}

int cPacesim::launch_device(CURRENT_DEVICE_CONFIG_STRUCT config_struct)
{
    config_struct_ = config_struct;
    return 0;
}

DATAS cPacesim::read()
{
    result.buffer[0] = (current_fake_value +  ( (double)(rand() % 30)-15.0)/100.0);
    result.buffer_size = 1;

    return result;
}

void cPacesim::set(double* value, size_t length)
{
    assert(length > 0);
    current_fake_value = value[0];
}

void cPacesim::stop_device() { std::cout << "cPacesim->stoping...\n"; }

cPacesim::~cPacesim() { std::cout << "cPacesim dtor...\n"; };


