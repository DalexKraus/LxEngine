cmake_minimum_required(VERSION 3.13)
project(libvmfgeom VERSION 1.1)

#Disable in source-builds
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# --------------------  Sources --------------------
file(GLOB_RECURSE CXX_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")

include_directories("include/")
add_executable(libvmfgeom ${CXX_SOURCES})