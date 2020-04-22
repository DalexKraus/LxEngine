#include "lx.h"
#include "lxvao.h"
#include "lxshader.h"

#include "glm/gtc/matrix_transform.hpp"

#include "fileutil.h"

double dTime;
lxVao vao;
lxShader shader;
lxWindow window;
LxCamera camera;

extern void camctrl_keyboard();
extern void camctrl_mouse();

void draw(double deltaTime)
{
    //Update time
    dTime = deltaTime;
    double time = glfwGetTime();

    //Update camera
    glm::vec3 translation = { cos(time), 0, 5 + sin(time) };
    camera.setPosition(translation);


    int fps = (int) (1.0 / deltaTime);
    printf("FPS: %d\n", fps);

    //Draw
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    lxShaderStart(shader);

    lxShaderUniform u_projectionMatrix  = lxShaderGetUniformLocation(shader, "projectionMatrix");
    lxShaderUniform u_viewMatrix        = lxShaderGetUniformLocation(shader, "viewMatrix");
    lxShaderUniformMat4(u_projectionMatrix, camera.getProjection());
    lxShaderUniformMat4(u_viewMatrix,       camera.getView());

    glEnableVertexAttribArray(0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    lxShaderStop();
}

int main()
{
    printf("Starting engine ...\n");
    window = lxWindowCreate("LynxEngine", 1280, 720, false);
    lxWindowAntiAlias(window, true);
    lxWindowVsync(window, true);

    const char* shaderVsh = readFileContent("res/shaders/base.vsh");
    const char* shaderFsh = readFileContent("res/shaders/base.fsh");

    shader = lxShaderCreate();
    lxShaderCompile(shader, shaderVsh, GL_VERTEX_SHADER);
    lxShaderCompile(shader, shaderFsh, GL_FRAGMENT_SHADER);
    lxShaderLink(shader, NULL);

    float vertices[] = {
		-0.5f, 0.5f, -1.0f,
		-0.5f, -0.5f, -1.0f,
		0.5f, -0.5f, -1.0f,
		0.5f, 0.5f, -1.0f
	};

	unsigned int indices[] = {
		0, 1, 3, 3, 1, 2
	};

    vao = lxVaoCreate();
    lxVaoStoreIndicesList(vao, indices, sizeof(indices), 6);
    lxVaoStoreData(vao, 0, vertices, sizeof(vertices), 3);

    float fovY      = 75;
    float aspect    = (float) window->width / (float) window->height;
    camera          = LxCamera(fovY, aspect, 0.001f, 10.0f);

    lxWindowShow(window, &draw);
    
    lxVaoDestroy(vao);
    lxShaderDestroy(shader);
    lxWindowDestroy(window);
    return 0;
}
