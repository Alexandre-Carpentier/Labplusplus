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

std::string cPacecom::device_name() { 
    return config_struct_.device_name.ToStdString(); 
}

MEAS_TYPE cPacecom::device_type() { return PRESSURE_CONTROLER_INSTR; };

size_t cPacecom::chan_count()
{
    size_t nb_sig = 1;
    return nb_sig;
}

int cPacecom::launch_device(CURRENT_DEVICE_CONFIG_STRUCT config_struct)
{
    config_struct_ = config_struct;
    readpoint = 0.0;
    setpoint = 0.0;

    // Create the proper implementation of cProtocol with a factory method
    //std::unique_ptr<cProtocol> device = factory.make(PROTOCOLENUM::VISATCP, L"TCPIP0::169.254.254.001::inst0::INSTR");
    //std::unique_ptr<cProtocol> device = factory.make(PROTOCOLENUM::VISASERIAL, L"\\\\.\\COM20");
    
    device = factory.make(PROTOCOLENUM::VISASERIAL, L"ASRL*::INSTR");
    //device = factory.make(PROTOCOLENUM::VISASERIAL, config_struct_.device_addr.ToStdWstring());

    // TODO: test device not null

    err = device->init();

    if (err.err_code != 0)
    {
        MessageBox(GetFocus(), L"Failed to launch cPacecom at init()", L"FAIL", S_OK);
        return -1;
    }
    // Reset the device
    device->write(L"*RST\n");

    // init PACE6000 specific command
    device->write(L"SOUR:PRES 0\n"); // Set to 1 bar

    device->write(L":UNIT BAR\n");
    device->write(L":SOUR:SLEW:MODE max\n");
    device->write(L":OUTP 1\n");

    // acquizition loop
    acquireloop = std::jthread(&cPacecom::acquire, this);

    return 0;
}

void cPacecom::acquire()
{
    double setpoint_saved = 0.0;
    auto st = acquireloop.get_stop_token();
    while (!st.stop_requested())
    {
        assert(setpoint > -1.0 );
        assert(setpoint < 20.0 );
        assert(setpoint_saved > -1.0 );
        assert(setpoint_saved < 20.0 );

        if (setpoint != setpoint_saved)
        {

            // PACE 6000 react with \r or \n or \r\n
            // Be carefull to interact with the good end char...

            std::cout << "[*] New set point: " << setpoint <<"\n";
            std::wstring cmd;
            cmd = std::format(L"SOUR:PRES {}\r\n", setpoint);
            device->write(cmd); // Set to value
            //device->write(L"SOUR:PRES 0\r\n");
            setpoint_saved = setpoint;  
        }

        std::wstring msg;
        device->write(L":SENS?\r\n"); // Set to value
        device->read(msg);

        // Valid reading?
        // ex ":SENS:PRES - 0.0002771\r"
        //assert(msg.size() > 1 );
        //assert(msg.size() < 30 );

        if (msg.compare(0, wcslen(L":SENS:PRES"), L":SENS:PRES") == 0)
        {
            // extract floating point

            // C style
            //std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> converter;
            //std::string msg_utf8 = converter.to_bytes(msg);
            //char temp_buff[512] = "";
            //strncpy(temp_buff, msg_utf8.c_str(), msg_utf8.size());
            //int pos = 10;
            //int i = 0;
            //while (temp_buff[pos + i] != '\r')
            //{
            //    temp_buff[i] = temp_buff[pos + i];
            //    i++;
            //}
            //temp_buff[i] = '\0';
            //readpoint = atof(temp_buff);
            // C style end

            // C++ style
            std::wstring wValue = msg.substr(wcslen(L":SENS:PRES"));

            //assert(wValue.length() == 0 );
            //assert(wValue.length() < 20 );

            std::string utf8 = ConvertWideToUtf8(wValue);
            char* p = nullptr;
            readpoint = strtod(utf8.c_str(), &p);

            if (p[0] == '\r')
                std::cout << "[*] CR terminated.\n";

            if (p[0] == '\n')
                std::cout << "[*] LF terminated.\n";

            if (p == utf8.c_str())
            {
                std::cout << "[!] packet corrupted.\n";
                readpoint = 0;
            }    

            if (readpoint > 20)
                readpoint = 0;

            if (readpoint < -1)
                readpoint = 0;

            //readpoint = std::stof(wValue);


            assert(readpoint > -1.0);
            assert(readpoint < 20.0 );

            // C++ style end
            
            std::cout << "[*] read pressure: " << readpoint << " bar\n";
            Sleep(100);

            // skip end loop
            continue; 
        }
       
        // if reading issue -> assuming readpoint is the setpoint
        
        std::cout << "setpoint " << setpoint << "setpoint_saved " << setpoint_saved << "\n";
        readpoint = setpoint;
        Sleep(500);
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
    assert(value > -1.0 );
    assert(value <  20.0 );
    setpoint = value;
}

void cPacecom::set_configuration_struct(CURRENT_DEVICE_CONFIG_STRUCT config_struct)
{
    config_struct_ = config_struct;
}

void cPacecom::set_device_name(std::string name)
{
    config_struct_.device_name = name;
}

void cPacecom::stop_device() {
    std::cout << "cPacesim->stoping...\n"; acquireloop.request_stop();

    device->write(L"SOUR:PRES 0\r\n"); // Set to value
    Sleep(3000);
    device->write(L":OUTP 0\r\n"); // Set to value
    device->write(L"LOC\r\n"); // Set to value
    device->close();
}

cPacecom::~cPacecom() { std::cout << "cPacesim dtor...\n"; };


