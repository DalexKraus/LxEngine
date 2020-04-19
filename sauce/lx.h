#ifndef __LXENGINE_H
#define __LXENGINE_H

#define _CRT_SECURE_NO_WARNINGS

//Standard libraries
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#ifdef _WIN32
#include <Windows.h>
#define __func__ __FUNCTION__
#endif

//LXEngine debugging util
#include "lxdbg.h"

//GLAD and GLFW includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//LXEngine Window
#include "lxwindow.h"

#endif