/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     28/07/2026
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#include "plot.h"
#include <algorithm>
#include <cmath>

PlotTab::PlotTab(std::vector<PlotPoint>& plotData, double& currentTime, bool& isRunningTest)
    : plotData(plotData), currentTime(currentTime), isRunningTest(isRunningTest) {
}

void PlotTab::Render() {
    ImGui::Text("Real-time Plot: y = f(t)");
    ImGui::Separator();
    
    if (isRunningTest) {
        UpdatePlotData();
    }
    
    if (ImPlot::BeginPlot("Test Data", ImVec2(-1, -1))) {
        ImPlot::SetupAxes("Time (s)", "Value");
        ImPlot::SetupAxisLimits(ImAxis_X1, 0, std::max(currentTime, 10.0), ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 100, ImGuiCond_Once);
        
        if (!plotData.empty()) {
            std::vector<double> xs, ys;
            for (const auto& point : plotData) {
                xs.push_back(point.time);
                ys.push_back(point.value);
            }
            ImPlot::PlotLine("Measurement", xs.data(), ys.data(), static_cast<int>(xs.size()));
        }
        
        ImPlot::EndPlot();
    }
}

void PlotTab::UpdatePlotData() {
    currentTime += ImGui::GetIO().DeltaTime;
    
    double value = 50.0 + 30.0 * std::sin(currentTime);
    plotData.push_back({currentTime, value});
    
    if (plotData.size() > 1000) {
        plotData.erase(plotData.begin());
    }
}
