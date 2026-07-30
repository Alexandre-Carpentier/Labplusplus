/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     07/06/2026
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include <vector>
#include <memory>
#include <string>
#include <map>

#include "instrument.h"
#include "configManager.h"

#include "instrumentConfig.h"
#include "instrumentList.h"
#include "testSequence.h"    
#include "plot.h"

class PageView {
public:
    PageView();
    ~PageView();

    void Render();
    
    // Instrument management
    void AddInstrument(std::unique_ptr<IInstrument> instrument);
    void RemoveInstrument(size_t index);
    IInstrument* GetInstrument(size_t index);

    // Configuration
    void SaveConfiguration(const std::string& filename = "config.lua");
    void LoadConfiguration(const std::string& filename = "config.lua");

private:
    // Tab rendering methods
    void RenderInstrumentConfigTab();
    void RenderInstrumentListTab();
    void RenderTestSequenceTab();
    void RenderPlotTab();
    
    // Instrument config tab methods

    void RenderAddInstrumentDialog();
    
    // Instrument list tab methods (existing)
    void RenderInstrumentList();
    void RenderInstrumentPage();
    
    // Test sequence tab methods
    void RenderTestStepTable();
    void AddTestStep();
    void RemoveTestStep(size_t index);
    
    // Plot tab methods
    void UpdatePlotData();
    
    // Header
    void InitializeHeader();

    // Data members
    std::vector<std::unique_ptr<IInstrument>> instruments;
    int selectedInstrument;
    
    ConfigManager configManager;
    bool showAddInstrumentDialog;
    int selectedInstrumentType;
  
    std::unique_ptr<InstrumentConfigTab> instrumentConfigTab;
    std::unique_ptr<InstrumentListTab> instrumentListTab;
    std::unique_ptr<TestSequenceTab> testSequenceTab ;
    std::unique_ptr<PlotTab> plotTab ;
    
    // Test sequence data
    std::vector<TestStep> testSteps;
    int selectedStep;
    
    // Plot data
    std::vector<PlotPoint> plotData;
    double currentTime;
    bool isRunningTest;
};