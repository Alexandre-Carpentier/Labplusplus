/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#include "cDevice.h"

//////////////////////////////////////////////////////////////
// COMPONENT TO BE BUILD
// -----------------------------------------------------------
// PROTOCOL
// iMG
// PANEL COMPONENT WITH DIFFERENT CONTROLS
// DEFINE DIFFERENT CALLBACK TO LINK CONTROLS AND FUNCTIONS
// INIT SEQ
// CLOSING SEQ


void cDevice::DisplayConfiguration()
{
    std::cout << "[*] Communication are configurated with: " << this->protocol->get_type() << "\n";
    return;
}

#ifdef _C_STYLE_
void cDevice::set_device_name(char* name)
{
    strncpy(device_name, name, MAX_PATH);
}

void cDevice::set_access_type(int type)
{
    plugin_access_type = (PLUGIN_ACCESS)type;
}

void cDevice::set_measurement_name(char* meas_name)
{
    strncpy(plugin_measurement_name ,meas_name, MAX_PATH);
}

void cDevice::set_measurement_unit(char* meas_unit)
{
    strncpy(plugin_measurement_unit, meas_unit, MAX_PATH);
}

void cDevice::scpi_open(char* addr)
{
    std::cout << "[*] SCPI open called.\n";
    protocol->open(addr);
    return;
}

void cDevice::scpi_write(char* command)
{
    std::cout << "[*] SCPI write called.\n";
    protocol->send(command);
    return;
}

char* cDevice::scpi_read()
{
    char response[MAX_PATH] = "";
    protocol->recv(response);
    std::cout << "[*] SCPI read recv: " << response << "\n";
    return response;
}

void cDevice::scpi_close()
{
    std::cout << "[*] SCPI close called.\n";
    return;
}

char* cDevice::get_device_name()
{
    return device_name;
}

int cDevice::get_access_type()
{
    return static_cast<int>(plugin_access_type);
}

char* cDevice::get_measurement_name()
{
    return plugin_measurement_name;
}

char* cDevice::get_measurement_unit()
{
    return plugin_measurement_unit;
}
#else
void cDevice::set_device_name(std::string name)
{
    device_name = name;
}

void cDevice::set_access_type(int type)
{
    plugin_access_type = (PLUGIN_ACCESS)type;
}

void cDevice::set_measurement_name(std::string meas_name)
{
    plugin_measurement_name = meas_name;
}

void cDevice::set_measurement_unit(std::string meas_unit)
{
    plugin_measurement_unit = meas_unit;
}

void cDevice::scpi_open(std::string addr)
{
    std::cout << "[*] SCPI open called.\n";
    protocol->open(addr);
    return;
}

void cDevice::scpi_write(std::string command)
{
    std::cout << "[*] SCPI write called.\n";
    protocol->send(command);
    return;
}

std::string cDevice::scpi_read()
{
    std::string response;
    protocol->recv(response);
    std::cout << "[*] SCPI read recv: " << response << "\n";
    return response;
}

void cDevice::scpi_close()
{
    std::cout << "[*] SCPI close called.\n";
    return;
}

std::string cDevice::get_device_name()
{
    return device_name;
}

int cDevice::get_access_type()
{
    return static_cast<int>(plugin_access_type);
}

std::string cDevice::get_measurement_name()
{
    return plugin_measurement_name;
}

std::string cDevice::get_measurement_unit()
{
    return plugin_measurement_unit;
}
#endif

void cDevice::OnPaint()
{
    return;
}