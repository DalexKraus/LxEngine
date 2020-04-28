#include "LxWindow.hpp"

/**
 * Creates a new window with the given width, height and title. 
 */
LxWindow::LxWindow(const char* title, int width, int height, bool resizable)
    :  m_title(title), m_width(width), m_height(height), m_resizable(resizable)
{

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    glfwWindowHint(GLFW_RESIZABLE, resizable);
    glfwWindowHint(GLFW_SAMPLES, 4);

    //Create window instance
    m_glfwHandle = glfwCreateWindow(width, height, title, NULL, NULL);
    
	if (m_glfwHandle == NULL)
	{
        dbgErr("Unable to create window!");
        glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(m_glfwHandle);
	glfwSetInputMode(m_glfwHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
        dbgErr("Failed to inizialize GLAD!");
        glfwTerminate();
		return;
	}

    glViewport(0, 0, width, height);
}

/**
 * Starts displaying the specified window, and stars a render loop.
 */
void LxWindow::show(void (*draw_callback)(double deltaTime))
{
    //Enable vertical synchronization if desired
	glfwSwapInterval(m_vsync);

    double lastFrameTime = glfwGetTime();

    glClearColor(0.5f, 0.0f, 0.25f, 1.0f);

    //Enable anti-aliasing if desired
    if (m_antialias)
        glEnable(GL_MULTISAMPLE);
    else glDisable(GL_MULTISAMPLE);

    while (!glfwWindowShouldClose(m_glfwHandle))
    {
        double frameTime    = glfwGetTime();
        double deltaTime    = frameTime - lastFrameTime;
        lastFrameTime       = frameTime;
        m_deltaTime         = deltaTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        draw_callback(deltaTime);

        glfwSwapBuffers(m_glfwHandle);
		glfwPollEvents();
    }
}
