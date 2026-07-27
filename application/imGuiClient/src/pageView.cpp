/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     07/06/2026
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#include "pageView.h"
#include "instruments/pressureSensor.h"
#include <imgui.h>

PageView::PageView() : selectedInstrument(0) {
    // Add default instruments
    AddInstrument(std::make_unique<PressureSensor>());
}

PageView::~PageView() = default;

void PageView::AddInstrument(std::unique_ptr<IInstrument> instrument) {
    instruments.push_back(std::move(instrument));
}

void PageView::RemoveInstrument(size_t index) {
    if (index < instruments.size()) {
        instruments.erase(instruments.begin() + index);
        if (selectedInstrument >= static_cast<int>(instruments.size())) {
            selectedInstrument = static_cast<int>(instruments.size()) - 1;
        }
    }
}

IInstrument* PageView::GetInstrument(size_t index) {
    if (index < instruments.size()) {
        return instruments[index].get();
    }
    return nullptr;
}

void PageView::Render() {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    // Create the main windows
    ImGui::Begin("MainWindow", nullptr, window_flags);

    RenderInstrumentList();
    RenderInstrumentPage();

    // Close the main windows
    ImGui::End();
}

void PageView::RenderInstrumentList() {
    ImGui::BeginChild("InstrumentList", ImVec2(150, 0), true);
    
    for (size_t i = 0; i < instruments.size(); ++i) {
        if (ImGui::Selectable(instruments[i]->GetName(), selectedInstrument == static_cast<int>(i))) {
            selectedInstrument = static_cast<int>(i);
        }
    }
    
    ImGui::EndChild();
}

void PageView::RenderInstrumentPage() {
    ImGui::SameLine();
    ImGui::BeginChild("InstrumentPage");
    
    if (selectedInstrument >= 0 && selectedInstrument < static_cast<int>(instruments.size())) {
        instruments[selectedInstrument]->Render();
    }
    
    ImGui::EndChild();
}