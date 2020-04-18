cmake_minimum_required(VERSION 3.13)
project(LynxEngine VERSION 1.1)

#Disable in source-builds
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

#Additional packages
find_package(Threads)

# --------------------
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "sauce/*.c")
add_executable(lxengine ${SOURCES})

#Libraries
add_subdirectory(lib/glfw3)
include_directories(include)
target_link_libraries(lxengine glfw ${GLFW_LIBRARIES})

target_link_libraries(lxengine ${CMAKE_THREAD_LIBS_INIT})