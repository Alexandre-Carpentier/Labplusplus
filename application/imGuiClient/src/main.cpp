/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     07/06/2026
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include "pageView.h"
#include <GLFW/glfw3.h>
#include <iostream>

// Callback to handle GLFW errors
void glfw_error_callback(int error, const char* description) { std::cerr << "GLFW Error " << error << ": " << description << std::endl; }

// Redraw content windows during window resizing
static PageView* g_pageView = nullptr;
void window_refresh_callback(GLFWwindow* window) {
    if (!g_pageView) return;
    
    // Start frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // PageView window (fullscreen)
    g_pageView->Render();

    // Render
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Swap buffers
    glfwSwapBuffers(window);
}

// Fonction pour obtenir le chemin de la police système
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
    
    return nullptr; // Aucune police système trouvée
}

int main() {
    // Setup error callback
    glfwSetErrorCallback(glfw_error_callback);

    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Setup OpenGL version
#if defined(__APPLE__)
    // GL 3.2 + GLSL 150 (MacOS)
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on MacOS
#else
    // GL 3.0 + GLSL 130 (Windows and Linux)
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

    // Create window
    GLFWwindow* window = glfwCreateWindow(1200, 800, "ImPlot Example", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();

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
    } else {
        std::cerr << "[!] System font not found, using default font\n" ;
        io.Fonts->AddFontDefault(&config);
    }

    // Setup backend
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Create PageView instance
    PageView pageView;
    g_pageView = &pageView;

	// install refresh callback to handle window resizing and redraw content
    glfwSetWindowRefreshCallback(window, window_refresh_callback);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // PageView window (fullscreen)
        pageView.Render();

        // Render
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffers
        glfwSwapBuffers(window);
    }

    // Cleanup
    g_pageView = nullptr;
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}