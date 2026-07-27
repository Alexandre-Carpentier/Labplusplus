/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     07/06/2026
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "instrument.h"
#include <vector>
#include <memory>

class PageView {
public:
    PageView();
    ~PageView();

    void Render();
    
    void AddInstrument(std::unique_ptr<IInstrument> instrument);
    void RemoveInstrument(size_t index);
    IInstrument* GetInstrument(size_t index);

private:
    void RenderInstrumentList();
    void RenderInstrumentPage();

    std::vector<std::unique_ptr<IInstrument>> instruments;
    int selectedInstrument;
};