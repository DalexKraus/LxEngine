cmake_minimum_required(VERSION 3.13)

message("===> Building LibPlaneGeometry ...")
project(libPlaneGeom VERSION 1.2)

#Disable in source-builds
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# We want to export our function definitions
add_compile_definitions(LIBPLANEGEOM_EXPORT)

#Include dir
include_directories("include/")

add_library(libPlaneGeom SHARED "src/planegeom.cpp")