/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     28/07/2026
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#include "instrumentConfig.h"
#include "instrumentFactory.h"

InstrumentConfigTab::InstrumentConfigTab(std::vector<std::unique_ptr<IInstrument>>& instruments)
    : instruments(instruments), selectedType(0) {
}

void InstrumentConfigTab::Render() {
    ImGui::Text("Create and configure instruments");
    ImGui::Separator();
    
    ImGui::BeginChild("InstrumentCreator");
    
    ImGui::Text("Available Instrument Types:");
    ImGui::Separator();
    
    auto types = InstrumentFactory::GetInstance().GetRegisteredTypes();
    
    for (size_t i = 0; i < types.size(); ++i) {
        if (ImGui::Selectable(types[i].c_str(), selectedType == static_cast<int>(i))) {
            selectedType = static_cast<int>(i);
        }
    }
    
    ImGui::Spacing();
    
    if (ImGui::Button("Create Instrument", ImVec2(200, 0))) {
        if (selectedType >= 0 && selectedType < static_cast<int>(types.size())) {
            auto instrument = InstrumentFactory::GetInstance().CreateInstrument(types[selectedType]);
            if (instrument) {
                instruments.push_back(std::move(instrument));
            }
        }
    }
    
    ImGui::Separator();
    ImGui::Text("Created Instruments: %zu", instruments.size());
    
    ImGui::EndChild();
}
