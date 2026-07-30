#pragma once
#include <iostream>
// Function to get font path based on the operating system
const char* GetSystemFontPath() {
#if defined(_WIN32)
    // Windows - Segoe UI (police moderne de Windows)
    static const char* windowsFonts[] = {
        "C:\\Windows\\Fonts\\segoeui.ttf",      // Segoe UI (Windows 7+)
        "C:\\Windows\\Fonts\\arial.ttf",        // Arial (fallback)
        "C:\\Windows\\Fonts\\verdana.ttf"       // Verdana (fallback)
    };

    for (const char* font : windowsFonts) {
        FILE* f = fopen(font, "rb");
        if (f) {
            fclose(f);
            return font;
        }
    }
#elif defined(__APPLE__)
    // macOS - San Francisco (système) ou Helvetica Neue
    static const char* macFonts[] = {
        "/System/Library/Fonts/SFNS.ttf",                           // San Francisco (macOS 10.11+)
        "/System/Library/Fonts/SFNSText.ttf",                       // San Francisco Text
        "/System/Library/Fonts/HelveticaNeue.ttc",                  // Helvetica Neue
        "/System/Library/Fonts/Helvetica.ttc"                       // Helvetica
    };

    for (const char* font : macFonts) {
        FILE* f = fopen(font, "rb");
        if (f) {
            fclose(f);
            return font;
        }
    }
#else
    // Linux - chercher les polices communes
    static const char* linuxFonts[] = {
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",          // Debian/Ubuntu
        "/usr/share/fonts/dejavu/DejaVuSans.ttf",                   // Fedora/RedHat
        "/usr/share/fonts/TTF/DejaVuSans.ttf",                      // Arch
        "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf", // Liberation Sans
        "/usr/share/fonts/liberation/LiberationSans-Regular.ttf",
        "/usr/share/fonts/truetype/ubuntu/Ubuntu-R.ttf"             // Ubuntu
    };

    for (const char* font : linuxFonts) {
        FILE* f = fopen(font, "rb");
        if (f) {
            fclose(f);
            return font;
        }
    }
#endif

    return nullptr; // No font found
}

void setStyle(GLFWwindow* window)
{
    // Setup style
    ImGui::StyleColorsDark();

    // Customize style for softer dark theme
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.25f, 0.25f, 0.28f, 0.5f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.22f, 1.0f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.27f, 1.0f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.30f, 0.33f, 1.0f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.18f, 0.18f, 0.20f, 1.0f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.25f, 0.25f, 0.27f, 1.0f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.30f, 0.30f, 0.33f, 1.0f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.35f, 0.35f, 0.38f, 1.0f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.28f, 1.0f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.35f, 0.35f, 0.38f, 1.0f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.40f, 0.40f, 0.43f, 1.0f);
    style.Colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.0f);

    // Configuration police système
    ImGuiIO& io = ImGui::GetIO();

    // Récupérer le scaling DPI
    float xscale, yscale;
    glfwGetWindowContentScale(window, &xscale, &yscale);
    float dpi_scale = (xscale + yscale) / 2.0f;

    // Configuration pour un rendu net
    ImFontConfig config;
    config.OversampleH = 2;
    config.OversampleV = 2;
    config.PixelSnapH = true;

    float font_size = 16.0f * dpi_scale;

    // Essayer de charger la police système
    const char* systemFont = GetSystemFontPath();
    if (systemFont) {
        //std::cout << "[*] Using system font: " << systemFont << "\n";
        io.Fonts->AddFontFromFileTTF(systemFont, font_size, &config);
    }
    else {
        std::cerr << "[!] System font not found, using default font\n";
        io.Fonts->AddFontDefault(&config);
    }
}
