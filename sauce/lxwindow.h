#ifndef __LXWINDOW_H
#define __LXWINDOW_H

#include "lx.h"

struct lxWindowData
{
    bool resizable;
    bool vSync;
    bool antiAlias;
    int width;
    int height;
    const char* title;
    GLFWwindow* glfwHandle;
    double deltaTime;
};
typedef struct lxWindowData* lxWindow;


// Functions
lxWindow    lxWindowCreate(const char* title, int width, int height, bool resizable);
void        lxWindowShow(lxWindow window, void (*draw_callback)(double deltaTime));
void        lxWindowDestroy(lxWindow window);

void        lxWindowVsync(lxWindow window, bool useVsync);
void        lxWindowAntiAlias(lxWindow window, bool antiAlias);

#endif