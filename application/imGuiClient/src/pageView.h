/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     07/06/2026
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#pragma once

class PageView {
public:
    PageView();
    ~PageView();

    void Render();

private:
    void RenderInstrumentList();
    void RenderInstrumentPage();
    void RenderInstrument1();
    void RenderInstrument2();
    void RenderInstrument3();

    int selectedInstrument;
    
    float slider1;
    float slider2;
    float slider3;
    int counter1;
    int counter2;
    bool checkbox1;
    bool checkbox2;
    bool checkbox3;
    char textBuffer1[256];
    char textBuffer2[256];

    double s_pressure;
};
