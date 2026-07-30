/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     27/07/2026
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#include "instrumentFactory.h"

InstrumentFactory& InstrumentFactory::GetInstance() {
    static InstrumentFactory instance;
    return instance;
}

void InstrumentFactory::RegisterInstrument(const std::string& type, InstrumentCreator creator) {
    creators[type] = creator;
}

std::unique_ptr<IInstrument> InstrumentFactory::CreateInstrument(const std::string& type) {
    auto it = creators.find(type);
    if (it != creators.end()) {
        return it->second();
    }
    return nullptr;
}

std::vector<std::string> InstrumentFactory::GetRegisteredTypes() const {
    std::vector<std::string> types;
    for (const auto& pair : creators) {
        types.push_back(pair.first);
    }
    return types;
}