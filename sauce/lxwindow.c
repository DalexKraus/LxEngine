#include "lx.h"

/**
 * Creates a new window with the given width, height and title. 
 */
lxWindow lxWindowCreate(const char* title, int width, int height, bool resizable)
{
    //Allocate lxWindow
    lxWindow window = malloc(sizeof(lxWindow));

    glfwInit();

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    glfwWindowHint(GLFW_RESIZABLE, resizable);
    glfwWindowHint(GLFW_SAMPLES, 4);

    //Create window instance
    GLFWwindow* windowHandle = glfwCreateWindow(width, height, title, NULL, NULL);
    
    //Apply parameters to lxwindow instance
    window->glfwHandle  = windowHandle;
    window->width       = width;
    window->height      = height;
    window->title       = title;
    window->vSync       = false;
    window->antiAlias   = false;

	if (windowHandle == NULL)
	{
        dbgErr("Unable to create window!");
        lxWindowDestroy(window);
		return 0;
	}

	glfwMakeContextCurrent(windowHandle);
	glfwSetInputMode(windowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
        dbgErr("Failed to inizialize GLAD!");
        lxWindowDestroy(window);
		return 0;
	}

    glViewport(0, 0, width, height);

    return window;
}

/**
 * Starts displaying the specified window, and stars a render loop.
 */
void lxWindowShow(lxWindow window, void (*draw_callback)(double deltaTime))
{
    _ASSERT(window          != NULL, "The window instance must not be null!");
    _ASSERT(draw_callback   != NULL, "The draw callback must not be null!");

    //Enable vertical synchronization if desired
	glfwSwapInterval(window->vSync);

    double lastFrameTime = glfwGetTime();

    glClearColor(0.5f, 0.0f, 0.25f, 1.0f);

    //Enable anti-aliasing if desired
    if (window->antiAlias)
        glEnable(GL_MULTISAMPLE);
    else glDisable(GL_MULTISAMPLE);

    while (!glfwWindowShouldClose(window->glfwHandle))
    {
        double frameTime    = glfwGetTime();
        double deltaTime    = frameTime - lastFrameTime;
        lastFrameTime       = frameTime;
        window->deltaTime   = deltaTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        draw_callback(deltaTime);

        glfwSwapBuffers(window->glfwHandle);
		glfwPollEvents();
    }
}

/**
 * Destroys the created window and releases it's memory. 
 */
void lxWindowDestroy(lxWindow window)
{
    _ASSERT(window != NULL, "The window instance must not be null!");
    glfwDestroyWindow(window->glfwHandle);
    glfwTerminate();
    free(window);
}

/**
 * Applies the v-sync setting to the specified window.
 * Note: Applying this setting while the display is showing, will not do anything until #lxWindowShow() is called.
 */
void lxWindowVsync(lxWindow window, bool useVsync)
{
    _ASSERT(window != NULL, "The window instance must not be null!");
    window->vSync = true;
}

/**
 * Enables anti aliasing for the specified window.
 * Note: Applying this setting while the display is showing, will not do anything until #lxWindowShow() is called.
 */
void lxWindowAntiAlias(lxWindow window, bool antiAlias)
{
    _ASSERT(window != NULL, "The window instance must not be null!");
    window->antiAlias = antiAlias;
}