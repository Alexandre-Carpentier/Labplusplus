/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     19/06/2026
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include <cstddef>
#include <string>

enum class InstrumentStatus {
    OK = 0,
    ErrorInit = -1,
    ErrorConfig = -2,
    ErrorIO = -3
};

class IInstrument {
public:
    virtual ~IInstrument() = default;

    // Basic interface
    virtual const char* GetName() const = 0;
    virtual const char* GetType() const = 0;
    virtual void Render() = 0;
    
    // Control methods
    virtual InstrumentStatus Start() = 0;
    virtual InstrumentStatus Configure(const char* config) = 0;
    virtual InstrumentStatus Stop() = 0;
    virtual InstrumentStatus Read(void* data, size_t size) = 0;
    virtual InstrumentStatus Write(const void* data, size_t size) = 0;

    // Serialization methods
    virtual std::string SerializeToLua() const = 0;
    virtual bool DeserializeFromLua(const std::string& luaCode) = 0;
};