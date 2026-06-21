/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     07/06/2026
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////

#include "pageView.h"
#include "imgui.h"
#include <cstring>
#include <future>

PageView::PageView() 
    : selectedInstrument(0)
    , slider1(0.0f)
    , slider2(50.0f)
    , slider3(100.0f)
    , counter1(0)
    , counter2(10)
    , checkbox1(false)
    , checkbox2(true)
    , checkbox3(false) {
    std::memset(textBuffer1, 0, sizeof(textBuffer1));
    std::memset(textBuffer2, 0, sizeof(textBuffer2));
    std::strcpy(textBuffer1, "Texte exemple 1");
    std::strcpy(textBuffer2, "Texte exemple 2");
}

PageView::~PageView() {
}

void PageView::Render() {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | 
                                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                     ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    


    ImGui::Begin("Gestion des Instruments", nullptr, window_flags);
    
    ImGui::BeginChild("Sections", ImVec2(ImGui::GetContentRegionAvail().x * 0.25f, 0), true);

    ImGui::BeginChild("InstrumentList", ImVec2(ImGui::GetContentRegionAvail().x * 0.25f, 0), true);
    RenderInstrumentList();
    ImGui::EndChild();
    
    ImGui::SameLine();
    
    ImGui::BeginChild("InstrumentPage", ImVec2(0, 0), true);
    RenderInstrumentPage();
    ImGui::EndChild();

    ImGui::EndChild();
    
    ImGui::End();
}

void PageView::RenderInstrumentList() {
    ImGui::Text("Liste des Instruments");
    ImGui::Separator();
    
    if (ImGui::Selectable("Instrument 1", selectedInstrument == 0)) {
        selectedInstrument = 0;
    }
    if (ImGui::Selectable("Instrument 2", selectedInstrument == 1)) {
        selectedInstrument = 1;
    }
    if (ImGui::Selectable("Instrument 3", selectedInstrument == 2)) {
        selectedInstrument = 2;
    }
}

void PageView::RenderInstrumentPage() {
    switch (selectedInstrument) {
        case 0:
            RenderInstrument1();
            break;
        case 1:
            RenderInstrument2();
            break;
        case 2:
            RenderInstrument3();
            break;
        default:
            ImGui::Text("Aucun instrument sélectionné");
            break;
    }
}

void PageView::RenderInstrument1() {
    ImGui::Text("Configuration Instrument 1");
    ImGui::Separator();
    
    ImGui::SliderFloat("Paramètre 1", &slider1, 0.0f, 100.0f);
    ImGui::SliderFloat("Paramètre 2", &slider2, 0.0f, 100.0f);
    
    ImGui::Spacing();
    ImGui::InputText("Nom", textBuffer1, sizeof(textBuffer1));
    
    ImGui::Spacing();
    ImGui::Checkbox("Option A", &checkbox1);
    ImGui::Checkbox("Option B", &checkbox2);
    
    ImGui::Spacing();
    if (ImGui::Button("Démarrer")) {
        counter1++;
    }
    ImGui::SameLine();
    ImGui::Text("Compteur: %d", counter1);
    
    ImGui::Spacing();
    ImGui::ProgressBar(slider1 / 100.0f, ImVec2(0.0f, 0.0f));
    
    ImGui::Spacing();
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "État: Actif");
    
    ImGui::Spacing();
    if (ImGui::CollapsingHeader("Paramètres avancés")) {
        ImGui::SliderFloat("Sensibilité", &slider3, 0.0f, 200.0f);
        ImGui::InputInt("Fréquence", &counter1);
    }
}

double calculate_actual_pressure()
{
    double pressure = 10.0; // Simulated pressure value
	for (int i = 0; i < 100000000; i++); // Simulate a long calculation
    {
        pressure = pressure - 0.00001;
    }
	pressure += 2.0; // Simulate some adjustments
    return pressure;
}

void PageView::RenderInstrument2() {

    std::future<double> f_pressure = std::async(std::launch::async, calculate_actual_pressure);

    ImGui::Text("Configuration Instrument 2");
    ImGui::Separator();
    
    static float values[10] = { 0.2f, 0.4f, 0.6f, 0.8f, 1.0f, 0.8f, 0.6f, 0.4f, 0.2f, 0.0f };
    ImGui::PlotLines("Signal", values, 10, 0, nullptr, 0.0f, 1.0f, ImVec2(0, 80));
    
    ImGui::Spacing();
    ImGui::InputText("Description", textBuffer2, sizeof(textBuffer2));
    
    ImGui::Spacing();
    ImGui::SliderFloat("Voltage", &slider1, -10.0f, 10.0f);
    
    ImGui::Spacing();
    ImGui::Checkbox("Mode automatique", &checkbox1);
    ImGui::Checkbox("Enregistrement", &checkbox2);
    ImGui::Checkbox("Calibration", &checkbox3);
    
    ImGui::Spacing();
    if (ImGui::Button("Calibrer")) {
        counter2++;
    }
    ImGui::SameLine();
    ImGui::Text("Calibrations: %d", counter2);
    
    ImGui::Spacing();
    const char* items[] = { "Mode 1", "Mode 2", "Mode 3", "Mode 4" };
    static int currentItem = 0;
    ImGui::Combo("Mode de mesure", &currentItem, items, 4);
    
    ImGui::Spacing();
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "État: Prêt");

    s_pressure = f_pressure.get();
    ImGui::Text("Pressure calculated %f", s_pressure);
}

void PageView::RenderInstrument3() {
    ImGui::Text("Configuration Instrument 3");
    ImGui::Separator();
    
    static float color[3] = { 1.0f, 0.5f, 0.2f };
    ImGui::ColorEdit3("Couleur LED", color);
    
    ImGui::Spacing();
    ImGui::SliderFloat("Température", &slider1, -50.0f, 150.0f);
    ImGui::SliderFloat("Pression", &slider2, 0.0f, 10.0f);
    ImGui::SliderFloat("Humidité", &slider3, 0.0f, 100.0f);
    
    ImGui::Spacing();
    ImGui::Checkbox("Alarme haute", &checkbox1);
    ImGui::Checkbox("Alarme basse", &checkbox2);
    
    ImGui::Spacing();
    if (ImGui::Button("Réinitialiser")) {
        slider1 = 20.0f;
        slider2 = 1.0f;
        slider3 = 50.0f;
        counter1 = 0;
    }
    
    ImGui::Spacing();
    ImGui::InputInt("Intervalle (ms)", &counter1);
    
    ImGui::Spacing();
    static float values[20] = {};
    for (int i = 0; i < 20; i++) {
        values[i] = slider1 + (i * 0.5f);
    }
    ImGui::PlotHistogram("Historique", values, 20, 0, nullptr, 0.0f, 200.0f, ImVec2(0, 80));
    
    ImGui::Spacing();
    ImGui::TextColored(ImVec4(0.0f, 0.5f, 1.0f, 1.0f), "État: En surveillance");
}
