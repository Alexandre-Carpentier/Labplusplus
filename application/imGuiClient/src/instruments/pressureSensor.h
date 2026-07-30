/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     19/06/2026
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "../instrument.h"

class PressureSensor : public IInstrument {
public:
    PressureSensor();
    ~PressureSensor() override = default;

    const char* GetName() const override { return "Pressure Sensor"; }
    const char* GetType() const override { return "PressureSensor"; }
    void Render() override;
    
    InstrumentStatus Start() override;
    InstrumentStatus Configure(const char* config) override;
    InstrumentStatus Stop() override;
    InstrumentStatus Read(void* data, size_t size) override;
    InstrumentStatus Write(const void* data, size_t size) override;

    std::string SerializeToLua() const override;
    bool DeserializeFromLua(const std::string& luaCode) override;

private:
    double pressure;
    float slider1;
    bool isRunning;
};