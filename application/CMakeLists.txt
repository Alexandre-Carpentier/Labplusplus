############################################################
# Author:      Alexandre CARPENTIER
# Modified by:
# Created:     08/05/26
# Copyright:   (c) Alexandre CARPENTIER
# Licence:     LGPL-2.1-or-later
############################################################

############################################################
# go modern cpp
cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 23)

############################################################
# project
project(labpp)

#########################################################################
# Package gRPC
find_package(gRPC CONFIG REQUIRED)
find_package(Protobuf CONFIG REQUIRED)

#########################################################################
# gRPC configuration
set(PROTO_FILES ${CMAKE_CURRENT_SOURCE_DIR}/proto/instrumentMsg.proto)
set(PROTO_IMPORT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/proto)

add_library(protobuf_lib_backend ${PROTO_FILES})
target_include_directories(protobuf_lib_backend PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(protobuf_lib_backend gRPC::grpc++)
protobuf_generate(
  TARGET protobuf_lib_backend
  LANGUAGE cpp
  IMPORT_DIRS ${PROTO_IMPORT_DIR}
)

get_property(grpc_cpp_plugin_location
  TARGET gRPC::grpc_cpp_plugin
  PROPERTY LOCATION
)

protobuf_generate(
  TARGET protobuf_lib_backend
  LANGUAGE grpc 
  IMPORT_DIRS ${PROTO_IMPORT_DIR}
  GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc
  PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}"
)

############################################################
# sub projects
add_subdirectory(backend)
add_subdirectory(imGuiClient)
add_subdirectory(tests)

