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

class IInstrument;

class InstrumentListTab {
public:
    InstrumentListTab(std::vector<std::unique_ptr<IInstrument>>& instruments, int& selectedInstrument);
    ~InstrumentListTab() = default;
    
    void Render();
    
private:
    void RenderInstrumentList();
    void RenderInstrumentPage();
    
    std::vector<std::unique_ptr<IInstrument>>& instruments;
    int& selectedInstrument;
};

 
