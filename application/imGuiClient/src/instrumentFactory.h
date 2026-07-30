/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     27/07/2026
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "instrument.h"
#include <memory>
#include <string>
#include <functional>
#include <unordered_map>
#include <vector>

// Factory function type
using InstrumentCreator = std::function<std::unique_ptr<IInstrument>()>;

class InstrumentFactory {
public:
    static InstrumentFactory& GetInstance();

    // Register a new instrument type
    void RegisterInstrument(const std::string& type, InstrumentCreator creator);

    // Create an instrument by type name
    std::unique_ptr<IInstrument> CreateInstrument(const std::string& type);

    // Get list of all registered instrument types
    std::vector<std::string> GetRegisteredTypes() const;

private:
    InstrumentFactory() = default;
    std::unordered_map<std::string, InstrumentCreator> creators;
};