#############################################################################
# Author:      Alexandre CARPENTIER
# Modified by:
# Created:     24/10/25
# Copyright:   (c) Alexandre CARPENTIER
# Licence:     LGPL-2.1-or-later
#############################################################################

#go modern
cmake_minimum_required(VERSION 3.20)
set(CMAKE_VERSION_CXX 23)

#MSVC hot reload
if(WIN32)
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()
endif()

project(
labapp
DESCRIPTION "Tests and measurments software - (c) Alexandre CARPENTIER - LGPL-2.1-or-later"
HOMEPAGE_URL https://github.com/Alexandre-Carpentier/Labplusplus
LANGUAGES CXX
)
message("[*] Configuring ${PROJECT_NAME}...")

#############################################################################
# wxWidgets
message("[*] find package wxWidgets")
find_package(wxWidgets REQUIRED COMPONENTS net core base gl)
if(wxWidgets_USE_FILE) 
    include(${wxWidgets_USE_FILE})
endif()
message("[*] wxWidgets found")

#############################################################################
# includes

include_directories("C:\\Program Files\\IVI Foundation\\VISA\\Win64\\Include")
include_directories(cycles_controler)
include_directories(main_window)
include_directories(main_window/config)
include_directories(main_window/config/pressure)
include_directories(main_window/config/voltage/daq)
include_directories(main_window/config/voltage/daq/daqmx)
include_directories(main_window/config/voltage/daq/keithley_6510)
include_directories(main_window/config/voltage/dc_supply)
include_directories(main_window/config/voltage/dc_supply/keithley_2280s)
include_directories(main_window/config/voltage/dc_supply/radiospare_rp500)
include_directories(main_window/config/voltage/oscope)
include_directories(main_window/config/voltage/oscope/Keysight_dsox1202g)
include_directories(main_window/footer)
include_directories(main_window/open)
include_directories(main_window/plot)
include_directories(main_window/table)
include_directories(measurements)
include_directories(measurements/physical_types)
include_directories(measurements/physical_types/pressure)
include_directories(measurements/physical_types/voltage)
include_directories(measurements/physical_types/voltage/daq)
include_directories(measurements/physical_types/voltage/daq/keithley_6510)
include_directories(measurements/physical_types/voltage/daq/ni_daqmx)
include_directories(measurements/physical_types/voltage/dc_supply)
include_directories(measurements/physical_types/voltage/oscopes)
include_directories(measurements/protocols)
include_directories(measurements/protocols/native)
include_directories(measurements/protocols/native/win32_rs232)
include_directories(measurements/protocols/native/win32_usb)
include_directories(measurements/protocols/native/win32_winsock)
include_directories(measurements/protocols/visa)
include_directories(measurements/protocols/visa/cross_rs232)
include_directories(measurements/protocols/visa/cross_tcpip)
include_directories(measurements/protocols/visa/cross_usb)
include_directories(mod)
include_directories(mod/logging)
include_directories(mod/plugins)
include_directories(res)
include_directories(tools)
include_directories(tools/serialize)
include_directories(tools/device_monitor)

#############################################################################
# build executable

if (MSVC)
#set (CMAKE_CXX_FLAGS "/MT /DWIN32 /D_WINDOWS /W3 /GR /Zi /EHsc")
#set (CMAKE_CXX_FLAGS_DEBUG "/EHsc /D_DEBUG /MTd /Ob0 /Od")
#set (CMAKE_CXX_FLAGS_RELEASE "/EHsc /MT /Ox /Ob2 /Oi /Ot /Oy /GL /DNDEBUG")
#set_property(TARGET labapp APPEND_STRING
#  PROPERTY LINK_FLAGS " /NODEFAULTLIB:msvcrtd /NODEFAULTLIB:msvcprtd")
endif ()

add_subdirectory(mod)

# Find every .cpp and .h 
file(GLOB_RECURSE APP_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS
    *.cpp
    *.c
)

# Remove plugin sources from the main application build
list(REMOVE_ITEM APP_SOURCES mod/plugins/cLoader.cpp mod/plugins/cDeviceBuilder.cpp mod/plugins/cLua.cpp)

file(GLOB_RECURSE APP_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS
    *.h
    *.hpp
)

file(GLOB_RECURSE APP_RES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS
    *.png
    *.bmp
    *.rc 
)

add_executable(labapp ${APP_SOURCES} ${APP_HEADERS} ${APP_RES})

# copy icon to output dir so runtime find "icon.ico"
add_custom_command(TARGET labapp POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/project/app/icon.ico"
    $<TARGET_FILE_DIR:labapp>
)

#############################################################################
# linking
target_include_directories(labapp PRIVATE ${LUA_INCLUDE_DIR} "C:\\DEV\\vcpkg\\packages\\lua_x64-windows-static\\include")
target_link_libraries(labapp PRIVATE ${wxWidgets_LIBRARIES} ${LUA_LIBRARIES} logging "Winusb.lib" "C:\\DEV\\vcpkg\\packages\\lua_x64-windows-static\\lib\\lua.lib")

message("[*] ${PROJECT_NAME} built.")