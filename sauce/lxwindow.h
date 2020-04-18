#ifndef __LXWINDOW_H
#define __LXWINDOW_H

#include "lx.h"

struct lxWindowData
{
    bool resizable;
    int width;
    int height;
    const char* title;
    GLFWwindow* glfwHandle;
};
typedef struct lxWindowData* lxWindow;


// Functions
lxWindow    lxWindowCreate(const char* title, int width, int height, bool resizable);
void        lxWindowShow(lxWindow window, void (*draw_callback)(float deltaTime));
void        lxWindowDestroy(lxWindow window);

#endif