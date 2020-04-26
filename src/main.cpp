#include "lx.hpp"
#include "lxvao.hpp"
#include "lxshader.hpp"

#include "glm/gtc/matrix_transform.hpp"

#include "fileutil.h"

#include "libvmf/libvmf.h"

double dTime;
lxVao vao;
lxShader shader;
LxWindow* lxwindow;
LxCamera* camera;

extern void camctrl_init();
extern void camctrl_keyboard(GLFWwindow* window);
extern void camctrl_mouse(GLFWwindow* window, double xpos, double ypos);

void draw(double deltaTime)
{
    //Update time
    dTime = deltaTime;
    double time = glfwGetTime();

    //Check for input
    camctrl_keyboard(lxwindow->handle());

    //Update camera
    //glm::vec3 translation = glm::vec3(cos(time), 0, 5 + sin(time));
    camera->updateView();

    int fps = (int) (1.0 / deltaTime);
    printf("FPS: %d\n", fps);

    //Draw
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    lxShaderStart(shader);
    lxShaderUniform u_projectionMatrix  = lxShaderGetUniformLocation(shader, "projectionMatrix");
    lxShaderUniform u_viewMatrix        = lxShaderGetUniformLocation(shader, "viewMatrix");
    lxShaderUniformMat4(u_projectionMatrix, camera->getProjection());
    lxShaderUniformMat4(u_viewMatrix,       camera->getView());

    glEnableVertexAttribArray(0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    lxShaderStop();
}

int main()
{
    printf("Starting engine ...\n");

    /* Load map */
    vmf_t map = vmfOpen("maps/test.vmf");
    vmfLoadBrushes(map);


    vmfClose(map);
    vmfFree(map);

    /* Create window */
    lxwindow = new LxWindow("LynxEngine", 1280, 720, false);
    lxwindow->antiAlias(true);
    lxwindow->useVsync(true);

    const char* shaderVsh = readFileContent("res/shaders/base.vsh");
    const char* shaderFsh = readFileContent("res/shaders/base.fsh");

    shader = lxShaderCreate();
    lxShaderCompile(shader, shaderVsh, GL_VERTEX_SHADER);
    lxShaderCompile(shader, shaderFsh, GL_FRAGMENT_SHADER);
    lxShaderLink(shader, NULL);

    float vertices[] = {
		-0.5f, 0.5f, -1.0f,     //upper left
		-0.5f, -0.5f, -1.0f,    //lower left
		0.5f, -0.5f, -1.0f,     //lower right
		0.5f, 0.5f, -1.0f,      //upper right
	};

	unsigned int indices[] = {
	    0, 1, 3, 3, 1, 2
	};

    vao = lxVaoCreate();
    lxVaoStoreIndicesList(vao, indices, sizeof(indices), 6);
    lxVaoStoreData(vao, 0, vertices, sizeof(vertices), 3);

    float fovY      = 120;
    float aspect    = (float) lxwindow->width() / (float) lxwindow->height();
    camera          = new LxCamera(fovY, aspect, 0.001f, 1000.0f);
    camera->translate(glm::vec3(0,0,5));

    //Camera callbacks
    glfwSetCursorPosCallback(lxwindow->handle(), &camctrl_mouse);
    camctrl_init();

    lxwindow->show(&draw);
    
    lxVaoDestroy(vao);
    lxShaderDestroy(shader);
    delete lxwindow;

    return 0;
}
