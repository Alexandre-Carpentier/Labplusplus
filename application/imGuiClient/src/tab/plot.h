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
#include <implot.h>
#include <vector>

class PlotTab {
public:
    PlotTab(std::vector<PlotPoint>& plotData, double& currentTime, bool& isRunningTest);
    ~PlotTab() = default;
    
    void Render();
    
private:
    void UpdatePlotData();
    
    std::vector<PlotPoint>& plotData;
    double& currentTime;
    bool& isRunningTest;
};

