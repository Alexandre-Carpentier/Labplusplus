#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include "pageView.h"

#include <memory>

class onResizeWindow {
public:
    onResizeWindow(PageView& shared) : resizePageView(shared) {};

    // Redraw content windows during window resizing
   void window_refresh_callback(GLFWwindow* window) {

        // Start frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // PageView window (fullscreen)
        resizePageView.Render();

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
private:
    PageView& resizePageView;
};

// callback that retrieves instance from window user pointer
void window_refresh_callback(GLFWwindow* window) {
    onResizeWindow* handler = static_cast<onResizeWindow*>(glfwGetWindowUserPointer(window));
    if (handler) {
		handler->window_refresh_callback(window);
    }
}