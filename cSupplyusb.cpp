/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#include "cSupplyusb.h"
#include "cMeasurement.h"
#include <string>

static double readpoint[3]{ 0.0 };
static double setpoint = 0.0;

cSupplyusb::cSupplyusb()
{
    std::cout << "cSupplyusb ctor...\n";
    result.buffer_size = 1;
};

std::string cSupplyusb::device_name() {
    return config_struct_.device_name.ToStdString();
}

MEAS_TYPE cSupplyusb::device_type() { return VOLTAGE_CONTROLER_INSTR; };

size_t cSupplyusb::chan_count()
{
    size_t nb_sig = 2; // 2 channel I+V
    return nb_sig;
}

size_t cSupplyusb::chan_read_count()
{
    size_t nb_sig = 2;
    return nb_sig;
}

size_t cSupplyusb::chan_write_count()
{
    size_t nb_sig = 1;
    return nb_sig;
}

int cSupplyusb::launch_device()
{
    ZeroMemory(readpoint, sizeof(readpoint));
    setpoint = 0.0;

    // Create the proper implementation of cProtocol with the factory method
    //std::unique_ptr<cProtocol> device = factory.make(PROTOCOLENUM::VISATCP, L"TCPIP0::169.254.254.001::inst0::INSTR");
    //std::unique_ptr<cProtocol> device = factory.make(PROTOCOLENUM::VISASERIAL, L"\\\\.\\COM20");

    //device = factory.make(PROTOCOLENUM::VISASERIAL, L"ASRL*::INSTR");
    device = factory.make(PROTOCOLENUM::VISAUSB, config_struct_.device_addr.ToStdWstring());

    // TODO: test device not null

    err = device->init();

    if (err.err_code != 0)
    {
        MessageBox(GetFocus(), L"Failed to launch cSupplyusb at init()", L"FAIL", S_OK);
        return -1;
    }
    // Reset the device and configure default settings
    device->write(L"*RST\n");
    device->write(L":TRAC:CLE\n"); // Clear buffer
    device->write(L":SENS:FUNC \"CONC\"\n"); // Set I+V mode

    //device->write(L"CURR 0.1\n"); // limit current to 100mA
    //device->write(L":VOLTage:LIMit 10\n"); // limit voltage to 10V
    device->write(L":VOLT 0\n"); // set output to 0V
    device->write(L":OUTP ON\n"); // output enable

    // acquizition loop
    acquireloop = std::jthread(&cSupplyusb::acquire, this);

    return 0;
}

void cSupplyusb::acquire()
{
    double setpoint_saved = 0.0;
    auto st = acquireloop.get_stop_token();
    while (!st.stop_requested())
    {
        assert(setpoint >= 0.0);
        assert(setpoint <= 70.0);
        assert(setpoint_saved >= 0.0);
        assert(setpoint_saved <= 70.0);

        if (setpoint != setpoint_saved)
        {
            std::cout << "[*] New set point: " << setpoint << "\n";
            std::wstring cmd;
            
            cmd = std::format(L":VOLT {}\n", setpoint);
            device->write(cmd); // Set to value
            //device->write(L"SOUR:PRES 0\r\n");
            setpoint_saved = setpoint;
        }

        std::wstring msg;
        //device->write(L":MEAS:VOLT? 32,5\n"); // Set to value
        device->write(L":MEAS:CONC?\n"); // Set to value
        device->read(msg);

        std::string utf8 = ConvertWideToUtf8(msg);

        std::vector<std::string> vec;
        // I
        vec.push_back(utf8.substr(0, utf8.find(",")));
        utf8 = utf8.substr(utf8.find(",")+1);
        // V
        vec.push_back(utf8.substr(0, utf8.find(",")));
        utf8 = utf8.substr(utf8.find(",")+1);
        // t
        //vec.push_back(utf8.substr(0, utf8.find(","))); // We don't want to plot time(s)
        //utf8 = utf8.substr(0, utf8.find(","));

        int i = 0;
        for (auto val : vec)
        {
            char* p = nullptr;
            readpoint[i] = strtod(val.c_str(), &p);

            assert(readpoint[i] > -70.0);
            assert(readpoint[i] < 70.0);
            i++;
        }

        // if reading issue -> assuming readpoint is the setpoint
        //std::cout << "[*] setpoint is " << setpoint << "and setpoint_saved is " << setpoint_saved << "\n";
        //readpoint = setpoint;
        Sleep(500);
    }
}

DATAS cSupplyusb::read()
{
    result.buffer[0] = readpoint[0];
    result.buffer[1] = readpoint[1];
    result.buffer_size = 2;

    return result;
}

void cSupplyusb::set(double* value, size_t length)
{
    assert(length > 0);
    assert(value[0] >= 0.0);
    assert(value[0] <= 70.0);
    setpoint = value[0];
}

void cSupplyusb::set_configuration_struct(CURRENT_DEVICE_CONFIG_STRUCT *config_struct)
{
    config_struct_ = *config_struct;
}

void cSupplyusb::set_device_name(std::string name)
{
    config_struct_.device_name = name;
}

void cSupplyusb::stop_device() {
    std::cout << "cSupplyusb->stoping...\n"; acquireloop.request_stop();

    device->write(L":VOLT 0\n"); 
    Sleep(500);
    device->write(L":OUTP 0\r\n"); 
    Sleep(500);
    //device->write(L"LOC\r\n"); 
    device->close();
}

cSupplyusb::~cSupplyusb() { std::cout << "cSupplyusb dtor...\n"; };


