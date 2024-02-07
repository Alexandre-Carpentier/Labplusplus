#include "cPacecom.h"
#include "cMeasurement.h"
#include <string>

static double readpoint = 0.0;
static double setpoint = 0.0;

cPacecom::cPacecom()
{
    std::cout << "cPacesim ctor...\n";
    result.buffer_size = 1;
};

std::string cPacecom::device_name() { return std::string("PACECOM"); }
MEAS_TYPE cPacecom::device_type() { return PRESSURECONTROLER_INSTR; };

size_t cPacecom::chan_count()
{
    size_t nb_sig = 1;
    return nb_sig;
}

int cPacecom::launch_device(CURRENT_DEVICE_CONFIG_STRUCT config_struct)
{
    config_struct_ = config_struct;

    // Create the proper implementation of cProtocol with a factory method
    //std::unique_ptr<cProtocol> device = factory.make(PROTOCOLENUM::VISATCP, L"TCPIP0::169.254.254.001::inst0::INSTR");
    //std::unique_ptr<cProtocol> device = factory.make(PROTOCOLENUM::VISASERIAL, L"\\\\.\\COM20");

    device = factory.make(PROTOCOLENUM::VISASERIAL, L"ASRL*::INSTR");

    err = device->init();

    // Here configure PACE6000 specifics


    // Reset the device

    device->write(L"*RST\n");

    // init PACE6000 specific command

    device->write(L"SOUR:PRES 0\n"); // Set to 1 bar

    device->write(L":UNIT BAR\n");
    device->write(L":SOUR:SLEW:MODE max\n");
    device->write(L":OUTP 1\n");

    readpoint = 0.0;
    setpoint = 0.0;

    acquireloop = std::jthread(&cPacecom::acquire, this);


    return 0;
}

void cPacecom::acquire()
{
    double setpoint_saved = 0.0;
    auto st = acquireloop.get_stop_token();
    while (!st.stop_requested())
    {
        if (setpoint != setpoint_saved)
        {
            std::wstring cmd;
            std::wstring msg;

            cmd = std::format(L"SOUR:PRES {}\n", setpoint);
            device->write(cmd); // Set to value

            device->read(msg);
            //readpoint = std::stof(msg);
            readpoint = setpoint;

            setpoint_saved = setpoint;
            Sleep(5000);
        }
    }
}

DATAS cPacecom::read()
{
    result.buffer[0] = readpoint;
    result.buffer_size = 1;

    return result;
}

void cPacecom::set(double value)
{
    setpoint = value;
}

void cPacecom::stop_device() { 
    std::cout << "cPacesim->stoping...\n"; acquireloop.request_stop();
}

cPacecom::~cPacecom() { std::cout << "cPacesim dtor...\n"; };


