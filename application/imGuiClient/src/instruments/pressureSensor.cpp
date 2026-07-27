/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     19/06/2026
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#include "pressureSensor.h"
#include <imgui.h>
#include <cstring>

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
    return INSTRUMENT_OK;
}

InstrumentStatus PressureSensor::Configure(const char* config) {
    // Parse config string
    return INSTRUMENT_OK;
}

InstrumentStatus PressureSensor::Stop() {
    isRunning = false;
    return INSTRUMENT_OK;
}

InstrumentStatus PressureSensor::Read(void* data, size_t size) {
    if (size < sizeof(double)) return INSTRUMENT_ERROR_IO;
    std::memcpy(data, &pressure, sizeof(double));
    return INSTRUMENT_OK;
}

InstrumentStatus PressureSensor::Write(const void* data, size_t size) {
    if (size < sizeof(double)) return INSTRUMENT_ERROR_IO;
    std::memcpy(&pressure, data, sizeof(double));
    return INSTRUMENT_OK;
}