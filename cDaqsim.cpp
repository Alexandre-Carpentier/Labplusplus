/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#include "cDaqsim.h"
#include "cMeasurement.h"
#include <string>

cDaqsim::cDaqsim()
{
    std::cout << "cDaqsim ctor...\n";
    result.buffer_size = 1;
};

std::string cDaqsim::device_name() { 
    return config_struct_.device_name.ToStdString(); 
}

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

size_t cDaqsim::chan_read_count()
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
size_t cDaqsim::chan_write_count()
{
    return 0;
}

void cDaqsim::set_configuration_struct(CURRENT_DEVICE_CONFIG_STRUCT config_struct)
{
    config_struct_ = config_struct;
    return;
}

int cDaqsim::launch_device()
{
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

void cDaqsim::set(double* value, size_t length)
{
    assert(value != nullptr);
    assert(length>0);

    current_fake_value = value;
}

void cDaqsim::stop_device() { std::cout << "cDaqsim->stoping...\n"; }

cDaqsim::~cDaqsim() { std::cout << "cDaqsim dtor...\n"; };


