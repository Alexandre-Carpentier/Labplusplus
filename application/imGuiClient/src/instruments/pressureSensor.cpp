/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     19/06/2026
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#include "pressureSensor.h"
#include "../instrumentFactory.h"
#include <imgui.h>
#include <cstring>
#include <sstream>

PressureSensor::PressureSensor() 
    : pressure(0.0), slider1(0.0f), isRunning(false) {
}

void PressureSensor::Render() {
    ImGui::Text("Pressure Sensor");
    ImGui::Separator();
    
    if (ImGui::Button(isRunning ? "Stop" : "Start")) {
        if (isRunning) Stop();
        else Start();
    }
    
    ImGui::SliderFloat("Pressure", &slider1, 0.0f, 100.0f);
    ImGui::Text("Current: %.2f kPa", pressure);
}

InstrumentStatus PressureSensor::Start() {
    isRunning = true;
    return InstrumentStatus::OK;
}

InstrumentStatus PressureSensor::Configure(const char* config) {
    // Parse config string if needed
    return InstrumentStatus::OK;
}

InstrumentStatus PressureSensor::Stop() {
    isRunning = false;
    return InstrumentStatus::OK;
}

InstrumentStatus PressureSensor::Read(void* data, size_t size) {
    if (size < sizeof(double)) return InstrumentStatus::ErrorIO;
    std::memcpy(data, &pressure, sizeof(double));
    return InstrumentStatus::OK;
}

InstrumentStatus PressureSensor::Write(const void* data, size_t size) {
    if (size < sizeof(double)) return InstrumentStatus::ErrorIO;
    std::memcpy(&pressure, data, sizeof(double));
    return InstrumentStatus::OK;
}

std::string PressureSensor::SerializeToLua() const {
    std::ostringstream oss;
    oss << "      pressure = " << pressure << ",\n";
    oss << "      slider1 = " << slider1 << ",\n";
    oss << "      isRunning = " << (isRunning ? "true" : "false") << "\n";
    return oss.str();
}

bool PressureSensor::DeserializeFromLua(const std::string& luaCode) {
    // Simple parsing
    std::istringstream iss(luaCode);
    std::string line;
    
    while (std::getline(iss, line)) {
        if (line.find("pressure") != std::string::npos) {
            sscanf(line.c_str(), " pressure = %lf", &pressure);
        } else if (line.find("slider1") != std::string::npos) {
            sscanf(line.c_str(), " slider1 = %f", &slider1);
        } else if (line.find("isRunning") != std::string::npos) {
            isRunning = (line.find("true") != std::string::npos);
        }
    }
    
    return true;
}

// Manual registration (replace REGISTER_INSTRUMENT macro)
namespace {
    struct PressureSensorRegistrar {
        PressureSensorRegistrar() {
            InstrumentFactory::GetInstance().RegisterInstrument(
                "PressureSensor",
                []() -> std::unique_ptr<IInstrument> {
                    return std::unique_ptr<IInstrument>(new PressureSensor());
                }
            );
        }
    };
    static PressureSensorRegistrar g_PressureSensorRegistrar;
}