/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     28/07/2026
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "testStructures.h"
#include <imgui.h>
#include <vector>

class TestSequenceTab {
public:
    TestSequenceTab(std::vector<TestStep>& testSteps, int& selectedStep);
    ~TestSequenceTab() = default;
    
    void Render();
    
private:
    void RenderTestStepTable();
    void AddTestStep();
    void RemoveTestStep(size_t index);
    
    std::vector<TestStep>& testSteps;
    int& selectedStep;
};
