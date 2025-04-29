/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#include "cNiDaqsim.h"
#include "cMeasurement.h"
#include <string>

cNiDaqsim::cNiDaqsim()
{
    std::cout << "cNiDaqsim ctor...\n";
    result.buffer_size = 1;
};

std::string cNiDaqsim::device_name() { 
    return config_struct_.device_name.ToStdString(); 
}

MEAS_TYPE cNiDaqsim::device_type() { return DAQ_INSTR; };
size_t cNiDaqsim::chan_count()
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

size_t cNiDaqsim::chan_read_count()
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
size_t cNiDaqsim::chan_write_count()
{
    return 0;
}

void cNiDaqsim::set_configuration_struct(CURRENT_DEVICE_CONFIG_STRUCT *config_struct)
{
    config_struct_ = *config_struct;
    return;
}

int cNiDaqsim::launch_device()
{
    return 0;
}

DATAS cNiDaqsim::read()
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

void cNiDaqsim::set(double* value, size_t length)
{
    assert(value != nullptr);
    assert(length>0);

    current_fake_value = value;
}

void cNiDaqsim::stop_device() { std::cout << "cNiDaqsim->stoping...\n"; }

cNiDaqsim::~cNiDaqsim() { std::cout << "cNiDaqsim dtor...\n"; };


