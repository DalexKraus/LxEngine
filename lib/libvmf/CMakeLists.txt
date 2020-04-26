cmake_minimum_required(VERSION 3.13)

message("===> Building LibVMF ...")
project(libvmf VERSION 1.1)

#Disable in source-builds
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# We want to export our function definitions
add_compile_definitions(LIBVMF_EXPORT)

#Include dir
include_directories("include/")

add_library(libvmf SHARED "src/libvmf.cpp")