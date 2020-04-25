cmake_minimum_required(VERSION 3.13)
project(libvmf VERSION 1.1)

#Disable in source-builds
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# --------------------  Sources --------------------
file(GLOB_RECURSE CXX_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")

include_directories("include/")

add_compile_definitions(LIBVMF_EXPORT) # We want to export our function definitions

add_executable(libvmf ${CXX_SOURCES})