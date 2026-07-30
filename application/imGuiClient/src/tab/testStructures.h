/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     28/07/2026
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include <string>
#include <vector>

struct PhysicalValue {
    std::string name;
    double value;
};

struct TestStep {
    std::string name;
    double duration;
    std::vector<PhysicalValue> physicalValues;
};

struct PlotPoint {
    double time;
    double value;
};
