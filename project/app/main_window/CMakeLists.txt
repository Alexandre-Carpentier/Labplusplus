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
main_window
DESCRIPTION "Tests and measurments software - (c) Alexandre CARPENTIER - LGPL-2.1-or-later"
HOMEPAGE_URL https://github.com/Alexandre-Carpentier/Labplusplus
LANGUAGES CXX
)
message("[*] Configuring ${PROJECT_NAME}...")

#add_executable(main_window cApp.cpp cApp.h cMain.cpp cMain.h)

message("[*] ${PROJECT_NAME} built.")