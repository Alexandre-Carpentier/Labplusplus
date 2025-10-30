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

project(
mod
DESCRIPTION "Tests and measurments software - (c) Alexandre CARPENTIER - LGPL-2.1-or-later"
HOMEPAGE_URL https://github.com/Alexandre-Carpentier/Labplusplus
LANGUAGES CXX
)

# Activez Rechargement à chaud pour les compilateurs MSVC si cela est pris en charge.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

message("[*] Configuring ${PROJECT_NAME}...")
add_subdirectory(logging)
add_subdirectory(plugins)

message("[*] ${PROJECT_NAME} built.")
