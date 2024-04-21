#include "cUSB6001.h"

#include "cMeasurement.h"
#include <string>

cUsb6001::cUsb6001()
{
    std::cout << "cUsb6001 ctor...\n";
    analog_taskHandle = nullptr;
    digital_taskHandle = nullptr;

    result.buffer;
    result.buffer_size = 0;
};

std::string cUsb6001::device_name() { return std::string("NI-DAQ"); }
MEAS_TYPE cUsb6001::device_type() { return DAQ_INSTR; };
size_t cUsb6001::chan_count()
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

int cUsb6001::launch_device(CURRENT_DEVICE_CONFIG_STRUCT config_struct)
{
    std::cout << "[*] cUsb6001->launching...\n";
    config_struct_ = config_struct;

    // Setup DAQmx

    if (0 != DAQmxSelfTestDevice(config_struct_.device_name))
    {
        MessageBox(0, 0, L"DAQmxSelfTestDevice Failed", 0);
    }

    // Check compatible measurement

    int32 meas_types[64]; memset(meas_types, 0, sizeof(meas_types)); // officially 29 different type is supported by NI we put 64 to be sure there is enought space for further NI API updates
    if (0 != DAQmxGetDevAISupportedMeasTypes(config_struct_.device_name, meas_types, sizeof(meas_types)))
    {
        // DAQmx_Val_Voltage 	    10322 	Voltage measurement.
        // DAQmx_Val_Temp_TC 	    10303 	Temperature measurement using a thermocouple.
        // DAQmx_Val_Temp_Thrmstr   10302 	Temperature measurement using a thermistor.     
        bool bvolt = false; bool btc = false; bool bthm = false;

        for (auto& meastype : meas_types)
        {
            if (meastype == DAQmx_Val_Voltage)
                bvolt = true;

            if (meastype == DAQmx_Val_Temp_TC)
                btc = true;

            if (meastype == DAQmx_Val_Temp_Thrmstr)
                bthm = true;
        }

        if (!(bvolt == true && btc == true && bthm == true))
        {
            MessageBox(0, L"Be carefull your DAQ system desn't support Voltage,Thermocouple and Thermistor measurements.", L"DAQmxGetDevAISupportedMeasTypes Warnings", 0);
        }
    }

    char producttype[256] = "";
    DAQmxGetDevProductType(config_struct_.device_name, producttype, sizeof(producttype));

    char modulenames[256] = "";
    DAQmxGetDevChassisModuleDevNames(config_struct_.device_name, modulenames, sizeof(modulenames));

    int analog_count = 0;
    DAQret = DAQmxCreateTask("", &analog_taskHandle);
    if (0 != DAQret)
    {
        MessageBox(0, 0, L"DAQmxCreateTask for analog channels Failed", 0);
    }

    int digital_count = 0;
    DAQret = DAQmxCreateTask("", &digital_taskHandle);
    if (0 != DAQret)
    {
        MessageBox(0, 0, L"DAQmxCreateTask for digital channels Failed", 0);
    }

    for (int c = 0; c < config_struct_.chan_number; c++)
    {
        if (config_struct_.channel_enabled[c] == true)
        {
            std::string name;
            std::string chan;

            wxString wxName = config_struct_.channel_name.at(c);
            name = wxName.ToStdString();

            wxString wxChan = config_struct_.channel_physical_name[c];
            chan = wxChan.ToStdString();


            if (config_struct_.channel_mode.at(c) == CHANANALOG)
            {
                analog_count++;
                char* Cscalename = NULL;
                int DAQ_customscale = DAQmx_Val_Volts;
                if (config_struct_.channel_linearize[c].compare("No") != 0)
                {
                    DAQ_customscale = DAQmx_Val_Volts;

                    std::string scale_name = config_struct_.channel_linearize[c].ToStdString();
                    Cscalename = new char[scale_name.length() + 1];
                    strcpy(Cscalename, scale_name.c_str());
                    double scale_slope = 0.0;
                    config_struct_.channel_linearize_slope[c].ToCDouble(&scale_slope);
                    double scale_intercept = 0.0;
                    config_struct_.channel_linearize_shift[c].ToCDouble(&scale_intercept);
                    /*
                    DAQret = DAQmxCreateLinScale(scale_name.c_str(), scale_slope, scale_intercept, DAQmx_Val_Volts, config_struct_.channel_linearize_unit[c].c_str());
                    if (0 != DAQret)
                    {
                        MessageBox(0, 0, L"DAQmxCreateLinScale Failed", 0);
                    }
                    */
                }

                // Voltage
                // 
                // Create an appropriate chan with DAQmxCreateAIVoltageChan()
                if (config_struct_.channel_type[c].compare(L"Voltage") == 0)
                {
                    int DAQ_pinmode = DAQmx_Val_RSE;
                    if (config_struct_.channel_mode_type[c].compare("Grounded") == 0)
                    {
                        DAQ_pinmode = DAQmx_Val_RSE;
                    }
                    if (config_struct_.channel_mode_type[c].compare("Differential") == 0)
                    {
                        DAQ_pinmode = DAQmx_Val_Diff;
                    }
                    if (config_struct_.channel_mode_type[c].compare("NRSE") == 0)
                    {
                        DAQ_pinmode = DAQmx_Val_NRSE;
                    }
                    if (config_struct_.channel_mode_type[c].compare("Pseudo Diff") == 0)
                    {
                        DAQ_pinmode = DAQmx_Val_PseudoDiff;
                    }

                    double min_range = 0.0;
                    config_struct_.channel_min[c].ToCDouble(&min_range);
                    double max_range = 0.0;
                    config_struct_.channel_max[c].ToCDouble(&max_range);


                    DAQret = DAQmxCreateAIVoltageChan(
                        analog_taskHandle,
                        chan.c_str(), // DEV1/ai0
                        name.c_str(), // Chan0
                        DAQ_pinmode, // RSE/Differential/...
                        min_range, // -10V
                        max_range, // +10V
                        DAQmx_Val_Volts, // return Volt or custom scale
                        NULL);

                    if (0 != DAQret)
                    {
                        MessageBox(0, 0, L"DAQmxCreateAIVoltageChan() Failed", 0);
                        DAQmxClearTask(analog_taskHandle);
                        analog_taskHandle = nullptr;
                        return 0;
                    }
                }

                // Thermocouple
                // 
                // Create an appropriate chan with DAQmxCreateAIThrmcplChan()
                if (config_struct_.channel_type[c].compare(L"Thermocouple") == 0)
                {
                    DAQret = DAQmxCreateAIThrmcplChan(
                        analog_taskHandle,
                        chan.c_str(),           // DEV1/ai0
                        name.c_str(),           // Chan0
                        -100.0,                 // -100°C min
                        200.0,                  // 200°C max
                        DAQmx_Val_DegC,         // We are European °C
                        DAQmx_Val_T_Type_TC,    // TODO: add different type in the panel
                        DAQmx_Val_BuiltIn,      // Built in cold junction measurement because it is easier
                        0,                      // not used
                        ""                      // not used
                    );

                    if (0 != DAQret)
                    {
                        MessageBox(0, 0, L"DAQmxCreateAIThrmcplChan() Failed", 0);
                        DAQmxClearTask(analog_taskHandle);
                        analog_taskHandle = nullptr;
                        return 0;
                    }

                }

                // Thermistance
                // 
                // Create an appropriate chan with DAQmxCreateAIThrmcplChan()
                if (config_struct_.channel_type[c].compare(L"Thermistance") == 0)
                {
                    DAQret = DAQmxCreateAIThrmstrChanIex(
                        analog_taskHandle,
                        chan.c_str(),           // DEV1/ai0
                        name.c_str(),           // Chan0
                        -100,                   // -100°C min
                        200,                    // 200°C max
                        DAQmx_Val_DegC,
                        DAQmx_Val_2Wire,
                        DAQmx_Val_Internal,
                        0.001,                  // Current to flow into thermistance
                        0.1,                    // Steinhart equation A
                        0.2,                    // Steinhart equation B
                        0.3                     // Steinhart equation C
                    );

                    if (0 != DAQret)
                    {
                        MessageBox(0, 0, L"DAQmxCreateAIThrmstrChanIex() Failed", 0);
                        DAQmxClearTask(analog_taskHandle);
                        analog_taskHandle = nullptr;
                        return 0;
                    }

                }

            }
            else if (config_struct_.channel_mode.at(c) == CHANDIGITAL)
            {
                digital_count++;
                // Digital in
                // 
                // Create an appropriate chan with DAQmxCreateDIChan()
                if (config_struct_.digital_channel_type[c].compare(L"Input") == 0)
                {
                    DAQret = DAQmxCreateDIChan(
                        digital_taskHandle,
                        chan.c_str(),           // DEV1/ai0
                        name.c_str(),           // Digital 0
                        DAQmx_Val_ChanPerLine   // lineGrouping:  DAQmx_Val_ChanPerLine/ DAQmx_Val_ChanForAllLines              
                    );

                    if (0 != DAQret)
                    {
                        MessageBox(0, 0, L"DAQmxCreateDIChan() Failed", 0);
                        DAQmxClearTask(digital_taskHandle);
                        digital_taskHandle = nullptr;
                        return 0;
                    }
                }
                // Digital out
                // 
                // Create an appropriate chan with DAQmxCreateDOChan()
                if (config_struct_.digital_channel_type[c].compare(L"Output") == 0)
                {
                    DAQret = DAQmxCreateDOChan(
                        digital_taskHandle,
                        chan.c_str(),           // DEV1/ai0
                        name.c_str(),           // Digital 0
                        DAQmx_Val_ChanPerLine   // lineGrouping:  DAQmx_Val_ChanPerLine/ DAQmx_Val_ChanForAllLines              
                    );

                    if (0 != DAQret)
                    {
                        MessageBox(0, 0, L"DAQmxCreateDOChan() Failed", 0);
                        DAQmxClearTask(digital_taskHandle);
                        digital_taskHandle = nullptr;
                        return 0;
                    }
                }


            }
        }
    }

    if (analog_count > 0)
    {
        DAQret = DAQmxStartTask(analog_taskHandle);
        if (0 != DAQret)
        {
            MessageBox(0, 0, L"DAQmxStartTask Failed", 0);
            DAQmxStopTask(analog_taskHandle);
            DAQmxClearTask(analog_taskHandle);
            analog_taskHandle = nullptr;
        }
    }
    if (digital_count > 0)
    {
        DAQret = DAQmxStartTask(digital_taskHandle);
        if (0 != DAQret)
        {
            MessageBox(0, 0, L"DAQmxStartTask Failed", 0);
            DAQmxStopTask(digital_taskHandle);
            DAQmxClearTask(digital_taskHandle);
            digital_taskHandle = nullptr;
        }
    }

    size_t nb_sig = chan_count();
    std::cout << "[*] result.buffer_size set to " << nb_sig << " in Usb6001.cpp\n";
    result.buffer_size = nb_sig;

    std::cout << "[*] DAQmxStartTask() in Usb6001.cpp\n";

    return 0;
}

DATAS cUsb6001::read()
{ 
    int32 chan_type = 0;
    char buff[256]=""; 
    double timeout_s = 2.0;

    int position = 0;
    size_t AI_number = 0;
    size_t DI_number = 0;
    for (auto access : config_struct_.channel_mode)
    {
        if (access == CHANANALOG)
        {
            if (config_struct_.channel_enabled.at(position))
            {
                AI_number++;
            }      
        }
        if (access == CHANDIGITAL)
        {
            if (config_struct_.channel_enabled.at(position))
            {
                DI_number++;
            }
        }         
        position++;
    }

    //DAQmxGetTaskName(taskHandle, buff, 256);// "_unnamedTask<0>"
    DAQret = DAQmxGetTaskChannels(analog_taskHandle, buff, 256);// "Digital0" 
    DAQret = DAQmxGetAIMeasType(analog_taskHandle, buff, &chan_type);

    if (chan_type == DAQmx_Val_Voltage)
    {
        
        int32 read_nb = 0;
        DAQret = DAQmxReadAnalogF64(analog_taskHandle, sample_number, timeout_s, DAQmx_Val_GroupByChannel, multiple_data, result.buffer_size * sample_number, &read_nb, NULL); // Read multiple sample
        if (read_nb != sample_number)
        {
            std::cout << "[!] DAQmxReadAnalogF64() read issue in Usb6001.cpp\n";
        }

        if (0 != DAQret)
        {
            MessageBox(0, 0, L"DAQmxReadAnalogF64 Failed", 0);
            DAQmxClearTask(analog_taskHandle);
            //TODO: exit
        }

        // averaging
        for (int k = 0; k < AI_number; k++)
        {
            result.buffer[k] = 0.0;
            for (int i = sample_number * k; i < sample_number * (k + 1); i++)
            {
                result.buffer[k] += multiple_data[i];
            }
            result.buffer[k] = result.buffer[k] / sample_number;
        }

        // scaling
        for (int j = 0; j < AI_number; j++)
        {
            double a = 0.0;
            double b = 0.0;
            config_struct_.channel_linearize_slope[j].ToCDouble(&a);
            config_struct_.channel_linearize_shift[j].ToCDouble(&b);
            result.buffer[j] = a * result.buffer[j] + b;
        }
    }
    

    {

        uInt8 read_buffer[48];
        memset(read_buffer, 0, 48);
        int32 sample_read = 0;
        int32 bytes_per_samples = 0;

        // Digital input
        DAQret = DAQmxReadDigitalLines(digital_taskHandle, 1, timeout_s, DAQmx_Val_GroupByChannel, read_buffer, DI_number, &sample_read, &bytes_per_samples, NULL);
        for (int i=0; i< sample_read; i++)
        {
            std::cout << "byte: " << read_buffer[i] << "\n";
            result.buffer[AI_number+ i] = read_buffer[i];
        }
    }
    return result;
}

void cUsb6001::set(double* value, size_t length)
{
    // TODO
}

void cUsb6001::stop_device()
{
    std::cout << "cUsb6001->stoping...\n";
    if (analog_taskHandle != nullptr)
    {
        DAQmxStopTask(analog_taskHandle);
        DAQmxClearTask(analog_taskHandle);
        std::cout << "[*] DAQmxStopTask() in Usb6001.cpp\n";
        std::cout << "[*] DAQmxClearTask() in Usb6001.cpp\n";
        analog_taskHandle = nullptr;
    }
    if (digital_taskHandle != nullptr)
    {
        DAQmxStopTask(digital_taskHandle);
        DAQmxClearTask(digital_taskHandle);
        std::cout << "[*] DAQmxStopTask() in Usb6001.cpp\n";
        std::cout << "[*] DAQmxClearTask() in Usb6001.cpp\n";
        digital_taskHandle = nullptr;
    }
}

cUsb6001::~cUsb6001()
{
    std::cout << "cUsb6001 dtor...\n";
    if (analog_taskHandle != nullptr)
    {
        DAQmxStopTask(analog_taskHandle);
        DAQmxClearTask(analog_taskHandle);
        std::cout << "[*] DAQmxStopTask() in Usb6001.cpp\n";
        std::cout << "[*] DAQmxClearTask() in Usb6001.cpp\n";
        analog_taskHandle = nullptr;
    }
    if (digital_taskHandle != nullptr)
    {
        DAQmxStopTask(digital_taskHandle);
        DAQmxClearTask(digital_taskHandle);
        std::cout << "[*] DAQmxStopTask() in Usb6001.cpp\n";
        std::cout << "[*] DAQmxClearTask() in Usb6001.cpp\n";
        digital_taskHandle = nullptr;
    }
};


