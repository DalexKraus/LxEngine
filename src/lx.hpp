#ifndef __LXENGINE_H
#define __LXENGINE_H

#define _CRT_SECURE_NO_WARNINGS

//Standard libraries
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef _WIN32
#include <Windows.h>
#define __func__ __FUNCTION__
#endif

//LXEngine debugging util
#include "util/lxdbg.hpp"

//GLAD and GLFW includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//LXEngine Window and camera
#include "LxWindow.hpp"
#include "LxCamera.hpp"

#endif