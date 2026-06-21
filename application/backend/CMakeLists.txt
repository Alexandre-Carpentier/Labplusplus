############################################################
# Author:      Alexandre CARPENTIER
# Modified by:
# Created:     08/05/26
# Copyright:   (c) Alexandre CARPENTIER
# Licence:     LGPL-2.1-or-later
############################################################

############################################################
# go modern cpp
set(CMAKE_CXX_STANDARD 23)

############################################################
# project
project(backend)

#########################################################################
# Fetch external dependencies
include(FetchContent)

#########################################################################
# Fetch Lua source code
FetchContent_Declare(
  lua
  GIT_REPOSITORY https://github.com/lua/lua
  GIT_TAG v5.4.7
)
FetchContent_MakeAvailable(lua)

# Manually build Lua as a library since it doesn't have CMakeLists.txt
file(GLOB LUA_SOURCES 
  ${lua_SOURCE_DIR}/*.c
)
# Remove standalone executables and amalgamation file
list(REMOVE_ITEM LUA_SOURCES 
  ${lua_SOURCE_DIR}/lua.c 
  ${lua_SOURCE_DIR}/luac.c
  ${lua_SOURCE_DIR}/onelua.c
)

add_library(lua STATIC ${LUA_SOURCES})
target_include_directories(lua PUBLIC ${lua_SOURCE_DIR})

# Define LUA_USE_WINDOWS for Windows build
if(WIN32)
  target_compile_definitions(lua PRIVATE LUA_USE_WINDOWS)
endif()

#########################################################################
# Fetch cppDaq source code
#FetchContent_Declare(
#  cppDaq
#  GIT_REPOSITORY https://github.com/Alexandre-Carpentier/cppDaq
#)
#FetchContent_MakeAvailable(cppDaq)

#########################################################################
# Fetch cppVisa source code
#FetchContent_Declare(
#  cppVisa
#  GIT_REPOSITORY https://github.com/Alexandre-Carpentier/cppVisa
 # GIT_TAG main  # Changed from default 'master' to 'main'
#)
#FetchContent_MakeAvailable(cppVisa)


#########################################################################
# LIB
add_library(backend-static
	src/main.cpp
)

#target_include_directories(backend-static PUBLIC 
#	${cppdaq_SOURCE_DIR}/app/src
#)

target_include_directories(backend-static PUBLIC 
	${lua_SOURCE_DIR}
)

#target_link_libraries(backend-static PUBLIC cppDaqStatic )
target_link_libraries(backend-static PUBLIC lua)
target_link_libraries(backend-static PRIVATE protobuf_lib_backend)

#########################################################################
# EXECUTABLE
add_executable(backend
	src/main.cpp
)

target_include_directories(backend PUBLIC 
	${cppdaq_SOURCE_DIR}/app/src
)

target_include_directories(backend PUBLIC 
	${lua_SOURCE_DIR}
)

#target_link_libraries(backend PRIVATE cppDaqStatic)
target_link_libraries(backend PRIVATE  lua)
target_link_libraries(backend PRIVATE protobuf_lib_backend)

message("[*] Building backend executable...\n")