#include "lx.hpp"

#include "glm/vec3.hpp"

extern LxWindow* lxwindow;
extern LxCamera* camera;

float lastX, lastY;
float roll = 0;

#define CAM_SPEED       5.0f
#define CAM_SENSITIVITY 0.0005f

void camctrl_init()
{
    double x, y;
    glfwGetCursorPos(lxwindow->handle(), &x, &y);
    lastX = (float) x;
    lastY = (float) y;
}

void camctrl_keyboard(GLFWwindow* window)
{

    float cameraSpeed = (float) (CAM_SPEED * lxwindow->deltaTime());

    glm::vec3* cameraFront = camera->getFront();
    glm::vec3* cameraRight = camera->getRight();

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->translate(cameraSpeed * *cameraFront);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->translate(-cameraSpeed * *cameraFront);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->translate(*cameraRight * cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->translate(*cameraRight * -cameraSpeed);
}

void camctrl_mouse(GLFWwindow* window, double xpos, double ypos)
{
    float xOffset = (float) (lastX - xpos);
    float yOffset = (float) (ypos - lastY); // Reversed because y range from bottom to top

    lastX = (float) xpos;
    lastY = (float) ypos;

    xOffset *= CAM_SENSITIVITY;
    yOffset *= CAM_SENSITIVITY;

    camera->rotate(xOffset, yOffset, roll);
}