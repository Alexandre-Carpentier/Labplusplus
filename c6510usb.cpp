#include "c6510usb.h"

#include <string>
#include <format>

#include "encoding.h"
#include "cTick.h"

static double readpoint = 0.0;
static double setpoint = 0.0;

c6510usb::c6510usb()
{
    std::cout << "c6510usb ctor...\n";
    result.buffer_size = 1;
};

std::string c6510usb::device_name() {
    return config_struct_.device_name.ToStdString();
}

MEAS_TYPE c6510usb::device_type() { return VOLTAGE_CONTROLER_INSTR; };

size_t c6510usb::chan_count()
{
    size_t nb_sig = 1;
    return nb_sig;
}

size_t c6510usb::chan_read_count()
{
    size_t nb_sig = 1;
    return nb_sig;
}

size_t c6510usb::chan_write_count()
{
    size_t nb_sig = 0;
    return nb_sig;
}

int c6510usb::launch_device()
{
    readpoint = 0.0;
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
        MessageBox(GetFocus(), L"Failed to launch c6510usb at init()", L"FAIL", S_OK);
        return -1;
    }
    // Reset the device and configure default settings
    err = device->write(L"SENS:FUNC \"RES\", (@121)\n");
    if (err.err_code < 0)
    {
        std::wcout << err.err_msg;
        return -1;
    }

    // First close the channel 1 switch to read then

    err = device->write(L"ROUT:CLOS (@121)\n");
    if (err.err_code < 0)
    {
        std::wcout << err.err_msg;
        return -1;
    }

    // acquizition loop
    acquireloop = std::jthread(&c6510usb::acquire, this);

    return 0;
}

void c6510usb::acquire()
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

            //cmd = std::format(L"WGEN:VOLT {}\n", setpoint);
            //device->write(cmd); // Set to value
            setpoint_saved = setpoint;
        }

        std::wstring msg;
        device->write(L":MEAS:RES?\n"); // Set to value
        device->read(msg);


        std::string utf8 = ConvertWideToUtf8(msg);
        char* p = nullptr;
        readpoint = strtod(utf8.c_str(), &p);
        std::cout << readpoint << "\n";
        assert(readpoint > DBL_MIN);
        assert(readpoint < DBL_MAX);
    }
}

DATAS c6510usb::read()
{
    result.buffer[0] = readpoint;
    result.buffer_size = 1;

    return result;
}

void c6510usb::set(double* value, size_t length)
{
    assert(length > 0);
    assert(value[0] > -20.0);
    assert(value[0] < 20.0);
    setpoint = value[0];
}

void c6510usb::set_configuration_struct(CURRENT_DEVICE_CONFIG_STRUCT config_struct)
{
    config_struct_ = config_struct;
}

void c6510usb::set_device_name(std::string name)
{
    config_struct_.device_name = name;
}

void c6510usb::stop_device() {
    std::cout << "c6510usb->stoping...\n"; acquireloop.request_stop();
    Sleep(500);
    device->write(L"LOC\n"); // Set to value
    device->close();
}

c6510usb::~c6510usb() { std::cout << "c6510usb dtor...\n"; };


