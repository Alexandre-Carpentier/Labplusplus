#include "cOscopesim.h"
#include "cMeasurement.h"
#include <string>

cOscopesim::cOscopesim()
{
    std::cout << "cOscopesim ctor...\n";
    result.buffer_size = 1;
};

std::string cOscopesim::device_name() {
    return config_struct_.device_name.ToStdString();
}

MEAS_TYPE cOscopesim::device_type() { return VOLTAGE_CONTROLER_INSTR; };

size_t cOscopesim::chan_count()
{
    size_t nb_sig = 1;
    return nb_sig;
}

size_t cOscopesim::chan_read_count()
{
    size_t nb_sig = 1;
    return nb_sig;
}

size_t cOscopesim::chan_write_count()
{
    size_t nb_sig = 0;
    return nb_sig;
}

void cOscopesim::set_configuration_struct(CURRENT_DEVICE_CONFIG_STRUCT config_struct)
{
    config_struct_ = config_struct;
}

int cOscopesim::launch_device()
{
    return 0;
}

DATAS cOscopesim::read()
{
    result.buffer[0] = (current_fake_value + ((double)(rand() % 30) +5) / 100.0);
    result.buffer_size = 1;

    return result;
}

void cOscopesim::set(double* value, size_t length)
{
    assert(length > 0);
    current_fake_value = value[0];
}

void cOscopesim::stop_device() { std::cout << "cOscopesim->stoping...\n"; }

cOscopesim::~cOscopesim() { std::cout << "cOscopesim dtor...\n"; };


