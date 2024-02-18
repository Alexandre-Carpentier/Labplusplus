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

void cDevice::set_device_name(std::string name)
{
    device_name = name;
}

void cDevice::scpi_open(std::string addr)
{
    std::cout << "[*] SCPI open called.\n";
    this->protocol->open(addr);
    return;
}

void cDevice::scpi_write(std::string command)
{
    std::cout << "[*] SCPI write called.\n";
    this->protocol->send(command);
    return;
}

std::string cDevice::scpi_read()
{
    std::string response;
    this->protocol->recv(response);
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

int cDevice::get_device_access_type()
{
    return static_cast<int>(plugin_access_type);
}

std::string cDevice::get_plugin_name()
{
    return plugin_control_name;
}

std::string cDevice::get_plugin_unit()
{
    return plugin_control_unit;
}

void cDevice::OnPaint()
{
    return;
}