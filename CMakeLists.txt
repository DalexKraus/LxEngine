cmake_minimum_required(VERSION 3.13)
project(LynxEngine VERSION 1.1)

#Disable in source-builds
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# --------------------  Sources --------------------
file(GLOB_RECURSE CXX_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")
file(GLOB_RECURSE C_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.c")

add_executable(lxengine ${C_SOURCES} ${CXX_SOURCES})

# -------------------- Libraries --------------------

# GLFW
add_subdirectory(lib/glfw3)
include_directories(include)
target_link_libraries(lxengine glfw ${GLFW_LIBRARIES})

# Linker options (For other libraries)
add_link_options(lxengine, "-L./lib")

# C-GLM (DEPRECATED)
# add_link_options(lxengine "-llibcglm")