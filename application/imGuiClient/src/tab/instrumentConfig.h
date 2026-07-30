/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     28/07/2026
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include <imgui.h>
#include <vector>
#include <memory>
#include <string>

class IInstrument;
class InstrumentFactory;

class InstrumentConfigTab {
public:
    InstrumentConfigTab(std::vector<std::unique_ptr<IInstrument>>& instruments);
    ~InstrumentConfigTab() = default;
    
    void Render();
    
private:
    std::vector<std::unique_ptr<IInstrument>>& instruments;
    int selectedType;
};

