/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     28/07/2026
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#include "testSequence.h"
#include <cstring>

TestSequenceTab::TestSequenceTab(std::vector<TestStep>& testSteps, int& selectedStep)
    : testSteps(testSteps), selectedStep(selectedStep) {
}

void TestSequenceTab::Render() {
    ImGui::Text("Test Sequence Configuration");
    ImGui::Separator();
    
    if (ImGui::Button("Add Step")) {
        AddTestStep();
    }
    
    ImGui::SameLine();
    
    if (ImGui::Button("Remove Selected") && selectedStep >= 0) {
        RemoveTestStep(selectedStep);
    }
    
    ImGui::Spacing();
    RenderTestStepTable();
}

void TestSequenceTab::RenderTestStepTable() {
    ImGui::BeginChild("TestStepTable");
    
    if (ImGui::BeginTable("TestSteps", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable)) {
        ImGui::TableSetupColumn("Step", ImGuiTableColumnFlags_WidthFixed, 50.0f);
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, 150.0f);
        ImGui::TableSetupColumn("Duration (s)", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableSetupColumn("Parameters", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();
        
        for (size_t i = 0; i < testSteps.size(); ++i) {
            ImGui::TableNextRow();
            
            ImGui::TableNextColumn();
            ImGui::Text("%zu", i + 1);
            
            ImGui::TableNextColumn();
            ImGui::PushID(static_cast<int>(i));
            char nameBuf[128];
            strncpy(nameBuf, testSteps[i].name.c_str(), sizeof(nameBuf) - 1);
            nameBuf[sizeof(nameBuf) - 1] = '\0';
            if (ImGui::InputText("##name", nameBuf, sizeof(nameBuf))) {
                testSteps[i].name = nameBuf;
            }
            
            ImGui::TableNextColumn();
            ImGui::InputDouble("##duration", &testSteps[i].duration, 0.1, 1.0, "%.2f");
            
            ImGui::TableNextColumn();
            ImGui::Text("%zu parameters", testSteps[i].physicalValues.size());
            if (ImGui::Button("Edit##params")) {
                selectedStep = static_cast<int>(i);
            }
            
            ImGui::PopID();
        }
        
        ImGui::EndTable();
    }
    
    ImGui::EndChild();
}

void TestSequenceTab::AddTestStep() {
    TestStep step;
    step.name = "Step " + std::to_string(testSteps.size() + 1);
    testSteps.push_back(step);
}

void TestSequenceTab::RemoveTestStep(size_t index) {
    if (index < testSteps.size()) {
        testSteps.erase(testSteps.begin() + index);
        selectedStep = -1;
    }
}
