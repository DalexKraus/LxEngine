#ifndef __LXWINDOW_H
#define __LXWINDOW_H

#include "lx.hpp"

class LxWindow
{
private:
    int m_width, m_height;
    const char* m_title;
    bool m_resizable, m_vsync, m_antialias;

    GLFWwindow* m_glfwHandle;
    double m_deltaTime;

public:
    LxWindow(const char* title, int width, int height, bool resizable);
    ~LxWindow()
    {
        glfwDestroyWindow(m_glfwHandle);
        glfwTerminate();
    }

    //Show the window
    void            show(void (*draw_callback)(double deltaTime));

    //Properties
    inline void     useVsync(bool vSync)        { m_vsync = vSync;          }
    inline void     antiAlias(bool antiAlias)   { m_antialias = antiAlias;  }
    inline int      width()                     { return m_width;           }
    inline int      height()                    { return m_height;          }
};

#endif