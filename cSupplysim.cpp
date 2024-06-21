#include "cSupplysim.h"
#include "cMeasurement.h"
#include <string>

cSupplysim::cSupplysim()
{
    std::cout << "cSupplysim ctor...\n";
    result.buffer_size = 1;
};

std::string cSupplysim::device_name() {
    return config_struct_.device_name.ToStdString();
}

MEAS_TYPE cSupplysim::device_type() { return VOLTAGE_CONTROLER_INSTR; };

size_t cSupplysim::chan_count()
{
    size_t nb_sig = 1;
    return nb_sig;
}

size_t cSupplysim::chan_read_count()
{
    size_t nb_sig = 1;
    return nb_sig;
}

size_t cSupplysim::chan_write_count()
{
    size_t nb_sig = 1;
    return nb_sig;
}

int cSupplysim::launch_device(CURRENT_DEVICE_CONFIG_STRUCT config_struct)
{
    config_struct_ = config_struct;
    return 0;
}

DATAS cSupplysim::read()
{
    result.buffer[0] = (current_fake_value + ((double)(rand() % 30) - 15.0) / 100.0);
    result.buffer_size = 1;

    return result;
}

void cSupplysim::set(double* value, size_t length)
{
    assert(length > 0);
    current_fake_value = value[0];
}

void cSupplysim::stop_device() { std::cout << "cSupplysim->stoping...\n"; }

cSupplysim::~cSupplysim() { std::cout << "cSupplysim dtor...\n"; };


