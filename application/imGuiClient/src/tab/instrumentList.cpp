/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     28/07/2026
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#include "instrumentList.h"
#include "instrument.h"

InstrumentListTab::InstrumentListTab(std::vector<std::unique_ptr<IInstrument>>& instruments, int& selectedInstrument)
    : instruments(instruments), selectedInstrument(selectedInstrument) {
}

void InstrumentListTab::Render() {
    RenderInstrumentList();
    RenderInstrumentPage();
}

void InstrumentListTab::RenderInstrumentList() {
    ImGui::BeginChild("InstrumentList", ImVec2(200, 0), true);
    
    for (size_t i = 0; i < instruments.size(); ++i) {
        if (ImGui::Selectable(instruments[i]->GetName(), selectedInstrument == static_cast<int>(i))) {
            selectedInstrument = static_cast<int>(i);
        }
        
        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Remove")) {
                instruments.erase(instruments.begin() + i);
                if (selectedInstrument >= static_cast<int>(instruments.size())) {
                    selectedInstrument = static_cast<int>(instruments.size()) - 1;
                }
            }
            ImGui::EndPopup();
        }
    }
    
    ImGui::EndChild();
}

void InstrumentListTab::RenderInstrumentPage() {
    ImGui::SameLine();
    ImGui::BeginChild("InstrumentPage");
    
    if (selectedInstrument >= 0 && selectedInstrument < static_cast<int>(instruments.size())) {
        instruments[selectedInstrument]->Render();
    } else {
        ImGui::Text("No instrument selected");
    }
    
    ImGui::EndChild();
}
