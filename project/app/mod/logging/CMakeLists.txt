#############################################################################
# Author:      Alexandre CARPENTIER
# Modified by:
# Created:     20/10/25
# Copyright:   (c) Alexandre CARPENTIER
# Licence:     LGPL-2.1-or-later
#############################################################################

#########################################################################
# go modern
cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 23)

# Activez Rechargement à chaud pour les compilateurs MSVC si cela est pris en charge.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project(logging VERSION 1.5.7 DESCRIPTION "Logging library - (c) Alexandre CARPENTIER - LGPL-2.1-or-later" HOMEPAGE_URL https://github.com/Alexandre-Carpentier/Labplusplus LANGUAGES CXX )

message("[*] Building ${PROJECT_NAME} v${PROJECT_VERSION}")

add_library(logging STATIC main.cpp) 
target_sources(logging PRIVATE main.cpp)

message("[*] Building ${PROJECT_NAME} complete")