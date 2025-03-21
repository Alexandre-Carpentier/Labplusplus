#include "c6510sim.h"
#include "cMeasurement.h"
#include <string>

c6510sim::c6510sim()
{
    std::cout << "c6510sim ctor...\n";
    result.buffer_size = 1;
};

std::string c6510sim::device_name() {
    return config_struct_.device_name.ToStdString();
}

MEAS_TYPE c6510sim::device_type() { return VOLTAGE_CONTROLER_INSTR; };

size_t c6510sim::chan_count()
{
    size_t nb_sig = 1;
    return nb_sig;
}

size_t c6510sim::chan_read_count()
{
    size_t nb_sig = 1;
    return nb_sig;
}

size_t c6510sim::chan_write_count()
{
    size_t nb_sig = 0;
    return nb_sig;
}

void c6510sim::set_configuration_struct(CURRENT_DEVICE_CONFIG_STRUCT config_struct)
{
    config_struct_ = config_struct;
}

int c6510sim::launch_device()
{
    return 0;
}

DATAS c6510sim::read()
{
    result.buffer[0] = (current_fake_value + ((double)(rand() % 30) + 5) / 100.0);
    result.buffer_size = 1;

    return result;
}

void c6510sim::set(double* value, size_t length)
{
    assert(length > 0);
    current_fake_value = value[0];
}

void c6510sim::stop_device() { std::cout << "c6510sim->stoping...\n"; }

c6510sim::~c6510sim() { std::cout << "c6510sim dtor...\n"; };


