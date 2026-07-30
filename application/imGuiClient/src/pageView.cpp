/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     07/06/2026
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#include "pageView.h"

#include <iostream>
#include <cmath>

#include <imgui.h>
#include <implot.h>

#include "instrumentFactory.h"
#include "testStructures.h"



PageView::PageView() 
    : selectedInstrument(0), 
      showAddInstrumentDialog(false),   
      selectedInstrumentType(0),
      selectedStep(-1),
      currentTime(0.0),
      isRunningTest(false)
{

    LoadConfiguration();
    
    // Add default test step
    AddTestStep();

    // Initialize tabs
    instrumentConfigTab = std::make_unique<InstrumentConfigTab>(instruments);
    instrumentListTab = std::make_unique<InstrumentListTab>(instruments, selectedInstrument);
    testSequenceTab = std::make_unique<TestSequenceTab>(testSteps, selectedStep);
    plotTab = std::make_unique<PlotTab>(plotData, currentTime, isRunningTest);
}

PageView::~PageView() {
    SaveConfiguration();
}

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

void PageView::SaveConfiguration(const std::string& filename) {
    if (configManager.SaveConfig(filename, instruments)) {
        std::cout << "[*] Configuration saved successfully." << std::endl;
    }
}

void PageView::LoadConfiguration(const std::string& filename) {
    auto loadedInstruments = configManager.LoadConfig(filename);
    if (!loadedInstruments.empty()) {
        instruments = std::move(loadedInstruments);
        std::cout << "Configuration loaded successfully" << std::endl;
    }
}

void PageView::Render() {
    ImGuiViewport* viewport = ImGui::GetMainViewport();

	// Adjust the window to fill the entire viewport
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, viewport->Size.y));
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | 
                                     ImGuiWindowFlags_NoCollapse |
                                     ImGuiWindowFlags_NoResize | 
                                     ImGuiWindowFlags_NoMove |
                                     ImGuiWindowFlags_NoBringToFrontOnFocus | 
                                     ImGuiWindowFlags_NoNavFocus;

    ImGui::Begin("MainWindow", nullptr, window_flags);

    if (ImGui::BeginTabBar("MainTabBar", ImGuiTabBarFlags_None)) {
        
        if (ImGui::BeginTabItem("Instrument Config")) {
            RenderInstrumentConfigTab();
            ImGui::EndTabItem();
        }
        
        if (ImGui::BeginTabItem("Instruments")) {
            RenderInstrumentListTab();
            ImGui::EndTabItem();
        }
        
        if (ImGui::BeginTabItem("Test Sequence")) {
            RenderTestSequenceTab();
            ImGui::EndTabItem();
        }
        
        if (ImGui::BeginTabItem("Plot")) {
            RenderPlotTab();
            ImGui::EndTabItem();
        }
        
        ImGui::EndTabBar();
    }

    if (showAddInstrumentDialog) {
        RenderAddInstrumentDialog();
    }

    ImGui::End();
}

// ============================================================================
// TAB 1: Instrument Config
// ============================================================================
void PageView::RenderInstrumentConfigTab() {

    if (instrumentConfigTab) {
        instrumentConfigTab->Render();
    }
}

// ============================================================================
// TAB 2: Instrument List
// ============================================================================
void PageView::RenderInstrumentListTab() {
    if (instrumentListTab) {
        instrumentListTab->Render();
    }
}

void PageView::RenderInstrumentList() {
    // Deprecated - now handled by InstrumentListTab
}

void PageView::RenderInstrumentPage() {
    // Deprecated - now handled by InstrumentListTab
}

// ============================================================================
// TAB 3: Test Sequence
// ============================================================================
void PageView::RenderTestSequenceTab() {
    if (testSequenceTab) {
        testSequenceTab->Render();
    }
}

void PageView::RenderTestStepTable() {
    // Deprecated - now handled by TestSequenceTab
}

void PageView::AddTestStep() {
    TestStep step;
    step.name = "Step " + std::to_string(testSteps.size() + 1);
    testSteps.push_back(step);
}

void PageView::RemoveTestStep(size_t index) {
    if (index < testSteps.size()) {
        testSteps.erase(testSteps.begin() + index);
        selectedStep = -1;
    }
}

// ============================================================================
// TAB 4: Plot
// ============================================================================
void PageView::RenderPlotTab() {
    if (plotTab) {
        plotTab->Render();
    }
}

void PageView::UpdatePlotData() {
    // Deprecated - now handled by PlotTab
}

void PageView::RenderAddInstrumentDialog() {
    ImGui::OpenPopup("Add Instrument");
    
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    
    if (ImGui::BeginPopupModal("Add Instrument", &showAddInstrumentDialog, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Select instrument type:");
        ImGui::Separator();
        
        auto types = InstrumentFactory::GetInstance().GetRegisteredTypes();
        
        for (size_t i = 0; i < types.size(); ++i) {
            if (ImGui::Selectable(types[i].c_str(), selectedInstrumentType == static_cast<int>(i))) {
                selectedInstrumentType = static_cast<int>(i);
            }
        }
        
        ImGui::Separator();
        
        if (ImGui::Button("Create", ImVec2(120, 0))) {
            if (selectedInstrumentType >= 0 && selectedInstrumentType < static_cast<int>(types.size())) {
                auto instrument = InstrumentFactory::GetInstance().CreateInstrument(types[selectedInstrumentType]);
                if (instrument) {
                    AddInstrument(std::move(instrument));
                }
            }
            showAddInstrumentDialog = false;
        }
        
        ImGui::SameLine();
        
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            showAddInstrumentDialog = false;
        }
        
        ImGui::EndPopup();
    }
}