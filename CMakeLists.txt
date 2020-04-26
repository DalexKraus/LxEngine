cmake_minimum_required(VERSION 3.13)
project(LynxEngine VERSION 1.1)

#Disable in source-builds
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

set(LX_BIN_DIR "${CMAKE_BINARY_DIR}../bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${LX_BIN_DIR})

# --------------------  Sources --------------------
file(GLOB_RECURSE CXX_SOURCES RELATIVE  ${CMAKE_SOURCE_DIR} "src/*.cpp")
file(GLOB_RECURSE C_SOURCES RELATIVE    ${CMAKE_SOURCE_DIR} "src/*.c")
add_executable(lxengine ${CXX_SOURCES}  ${C_SOURCES})

# -------------------- Libraries --------------------
# GLFW
add_subdirectory("lib/glfw3")
include_directories("include/")
target_link_libraries(lxengine glfw ${GLFW_LIBRARIES})

# LibVMF
add_subdirectory("lib/libvmf")
target_link_libraries(lxengine libvmf "libvmf")

set_target_properties(libvmf # TODO: add other libs here as well
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${LX_BIN_DIR}"
)
