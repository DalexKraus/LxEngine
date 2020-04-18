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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    glfwWindowHint(GLFW_RESIZABLE, resizable);

    //Create window instance
    GLFWwindow* windowHandle = glfwCreateWindow(width, height, title, NULL, NULL);
    
    //Apply parameters to lxwindow instance
    window->glfwHandle  = windowHandle;
    window->width       = width;
    window->height      = height;
    window->title       = title;

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
	glfwSwapInterval(1);

    return window;
}

/**
 * Starts displaying the specified window, and stars a render loop.
 */
void lxWindowShow(lxWindow window, void (*draw_callback)(float deltaTime))
{
    _ASSERT(window          != NULL, "The window instance must not be null!");
    _ASSERT(draw_callback   != NULL, "The draw callback must not be null!");

    glClearColor(0.5f, 0.0f, 0.25f, 1.0f);
    while (!glfwWindowShouldClose(window->glfwHandle))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        draw_callback(0);

        glfwSwapBuffers(window->glfwHandle);
		glfwPollEvents();
    }
}

/**
 * Destroys the created window and releases it's memory. 
 */
void lxWindowDestroy(lxWindow window)
{
    glfwDestroyWindow(window->glfwHandle);
    glfwTerminate();
    free(window);
}