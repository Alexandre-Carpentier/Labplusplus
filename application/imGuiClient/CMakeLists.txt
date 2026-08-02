#############################################################################
# Author:      Alexandre CARPENTIER
# Modified by:
# Created:     07/06/2026
# Copyright:   (c) Alexandre CARPENTIER
# Licence:     LGPL-2.1-or-later
#############################################################################
cmake_minimum_required(VERSION 3.10)
project(labpp LANGUAGES CXX C)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(FetchContent)

# Setup OpenGL
cmake_policy(SET CMP0072 NEW)
find_package(OpenGL REQUIRED)

# Setup GLFW
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY "https://github.com/glfw/glfw"
    GIT_TAG "3.3.8"
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glfw)

# Fetch ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.89.9
)

FetchContent_Declare(
    implot
    GIT_REPOSITORY https://github.com/epezent/implot.git
    GIT_TAG v0.16
)

FetchContent_MakeAvailable(imgui implot)

# ImGui library
add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC 
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(imgui PUBLIC glfw)

# ImPlot library
add_library(implot STATIC
    ${implot_SOURCE_DIR}/implot.cpp
    ${implot_SOURCE_DIR}/implot_items.cpp
    ${implot_SOURCE_DIR}/implot_demo.cpp
)

target_include_directories(implot PUBLIC ${implot_SOURCE_DIR})
target_link_libraries(implot PUBLIC imgui)

# Main executable
add_executable(labpp 
    src/main.cpp
    src/pageView.cpp
    src/pageView.h
    src/instrument.cpp
    src/instrument.h
    src/instrumentFactory.cpp
    src/instrumentFactory.h
    src/configManager.cpp
    src/configManager.h

    src/instruments/pressureSensor.cpp
    src/instruments/pressureSensor.h

    src/appEvents/onResizeWindow.h
    src/appEvents/onCloseWindow.h
    src/appEvents/onError.h

    src/style.h

    
    
    src/tab/instrumentList.cpp
    src/tab/instrumentList.h
    src/tab/testSequence.cpp
    src/tab/testSequence.h
    src/tab/plot.cpp
    src/tab/plot.h

    src/tab/testStructures.h
)

# Include directories
target_include_directories(labpp PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/instruments
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tab
    ${CMAKE_CURRENT_SOURCE_DIR}/src/appEvents

    ${CMAKE_SOURCE_DIR}/out/build/x64-debug/_deps/lua-src
)

# Link libraries
target_link_libraries(labpp PRIVATE 
    imgui 
    implot 
    glfw 
    lua
    ${OPENGL_LIBRARIES}
)
