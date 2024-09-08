#include "cSupplyrsusb.h"
#include "cMeasurement.h"
#include <string>

static double readpoint[3]{ 0.0 };
static double setpoint = 0.0;

cSupplyrsusb::cSupplyrsusb()
{
    std::cout << "cSupplyrsusb ctor...\n";
    result.buffer_size = 1;
};

std::string cSupplyrsusb::device_name() {
    return config_struct_.device_name.ToStdString();
}

MEAS_TYPE cSupplyrsusb::device_type() { return VOLTAGE_CONTROLER_INSTR; };

size_t cSupplyrsusb::chan_count()
{
    size_t nb_sig = 2; // 2 channel I+V
    return nb_sig;
}

size_t cSupplyrsusb::chan_read_count()
{
    size_t nb_sig = 2;
    return nb_sig;
}

size_t cSupplyrsusb::chan_write_count()
{
    size_t nb_sig = 1;
    return nb_sig;
}

int cSupplyrsusb::launch_device()
{
    ZeroMemory(readpoint, sizeof(readpoint));
    setpoint = 0.0;

    // Create the proper implementation of cProtocol with the factory method
    //std::unique_ptr<cProtocol> device = factory.make(PROTOCOLENUM::VISATCP, L"TCPIP0::169.254.254.001::inst0::INSTR");
    //std::unique_ptr<cProtocol> device = factory.make(PROTOCOLENUM::VISASERIAL, L"\\\\.\\COM20");

    //device = factory.make(PROTOCOLENUM::VISASERIAL, L"ASRL*::INSTR");
    device = factory.make(PROTOCOLENUM::VISASERIAL, config_struct_.device_addr.ToStdWstring());

    // TODO: test device not null

    err = device->init();

    if (err.err_code != 0)
    {
        MessageBox(GetFocus(), L"Failed to launch cSupplyrsusb at init()", L"FAIL", S_OK);
        return -1;
    }
    device->write(L":VSET:1:0\n"); // set output to 0V
    device->write(L":OUT1\n"); // output enable

    // acquizition loop
    acquireloop = std::jthread(&cSupplyrsusb::acquire, this);

    return 0;
}

void cSupplyrsusb::acquire()
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

            cmd = std::format(L"VSET1:{}\n", setpoint);
            device->write(cmd); // Set to value
            //device->write(L"SOUR:PRES 0\r\n");
            setpoint_saved = setpoint;
            Sleep(100);
        }

        std::wstring msg;
        //device->write(L":MEAS:VOLT? 32,5\n"); // Set to value
        device->write(L"VSET1?\n"); // Set to value
        device->read(msg);
        std::string utf8 = ConvertWideToUtf8(msg);

        char* p = nullptr;
        readpoint[0] = strtod(utf8.c_str(), &p);
        std::cout << readpoint[0] << "\n";
        assert(readpoint[0] > -70.0);
        assert(readpoint[0] < 70.0);

        // if reading issue -> assuming readpoint is the setpoint
        //std::cout << "[*] setpoint is " << setpoint << "and setpoint_saved is " << setpoint_saved << "\n";
        //readpoint = setpoint;
        Sleep(500);
    }
}

DATAS cSupplyrsusb::read()
{
    result.buffer[0] = readpoint[0];
    result.buffer[1] = -1;
    result.buffer_size = 2;

    return result;
}

void cSupplyrsusb::set(double* value, size_t length)
{
    assert(length > 0);
    assert(value[0] >= 0.0);
    assert(value[0] <= 70.0);
    setpoint = value[0];
}

void cSupplyrsusb::set_configuration_struct(CURRENT_DEVICE_CONFIG_STRUCT config_struct)
{
    config_struct_ = config_struct;
}

void cSupplyrsusb::set_device_name(std::string name)
{
    config_struct_.device_name = name;
}

void cSupplyrsusb::stop_device() {
    std::cout << "cSupplyrsusb->stoping...\n"; acquireloop.request_stop();

    device->write(L"VSET1:0\n");
    Sleep(500);
    device->write(L":OUT0\r\n");
    Sleep(500);
    //device->write(L"LOC\r\n"); 
    device->close();
}

cSupplyrsusb::~cSupplyrsusb() { std::cout << "cSupplyrsusb dtor...\n"; };


