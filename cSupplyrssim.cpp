/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#include "cSupplyrssim.h"
#include "cMeasurement.h"
#include <string>

cSupplyrssim::cSupplyrssim()
{
    std::cout << "cSupplyrssim ctor...\n";
    result.buffer_size = 1;
};

std::string cSupplyrssim::device_name() {
    return config_struct_.device_name.ToStdString();
}

MEAS_TYPE cSupplyrssim::device_type() { return VOLTAGE_CONTROLER_INSTR; };

size_t cSupplyrssim::chan_count()
{
    size_t nb_sig = 2;
    return nb_sig;
}

size_t cSupplyrssim::chan_read_count()
{
    size_t nb_sig = 2;
    return nb_sig;
}

size_t cSupplyrssim::chan_write_count()
{
    size_t nb_sig = 1;
    return nb_sig;
}

void cSupplyrssim::set_configuration_struct(CURRENT_DEVICE_CONFIG_STRUCT *config_struct)
{
    config_struct_ = *config_struct;
    return;
}

int cSupplyrssim::launch_device()
{
    return 0;
}

DATAS cSupplyrssim::read()
{
    result.buffer[0] = 0.9 + (((double)(rand() % 10) + 15.0) / 100.0);
    result.buffer[1] = (current_fake_value + ((double)(rand() % 30) - 15.0) / 100.0);
    result.buffer_size = 2;

    return result;
}

void cSupplyrssim::set(double* value, size_t length)
{
    assert(length > 0);
    current_fake_value = value[0];
}

void cSupplyrssim::stop_device() { std::cout << "cSupplyrssim->stoping...\n"; }

cSupplyrssim::~cSupplyrssim() { std::cout << "cSupplyrssim dtor...\n"; };


