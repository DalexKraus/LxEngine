#include "lx.h"
#include "lxvao.h"
#include "lxshader.h"
#include "lxcamera.h"

#include "fileutil.h"

lxVao vao;
lxShader shader;
lxCamera camera;

void draw(double deltaTime)
{
    //Update time
    //Update camera
    double time = glfwGetTime();
    float x = (float) cos(time) * 5;
    float z = (float) sin(time) * 5; 

    vec3 translation = { x, 0, z };

    lxCameraPosition(camera, translation);
    lxCameraUpdateView(camera);

    int fps = (int) (1.0 / deltaTime);
    printf("FPS: %d\n", fps);

    //Draw
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    lxShaderStart(shader);

    lxShaderUniform u_projectionMatrix  = lxShaderGetUniformLocation(shader, "projectionMatrix");
    lxShaderUniform u_viewMatrix        = lxShaderGetUniformLocation(shader, "viewMatrix");
    lxShaderUniformMat4(u_projectionMatrix, camera->projectionMatrix);
    lxShaderUniformMat4(u_viewMatrix,       camera->viewMatrix);

    glEnableVertexAttribArray(0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    lxShaderStop();
}

int main()
{
    printf("Starting engine ...\n");
    lxWindow window = lxWindowCreate("LynxEngine", 1280, 720, false);
    lxWindowAntiAlias(window, true);

    const char* shaderVsh = readFileContent("res/shaders/base.vsh");
    const char* shaderFsh = readFileContent("res/shaders/base.fsh");

    shader = lxShaderCreate();
    lxShaderCompile(shader, shaderVsh, GL_VERTEX_SHADER);
    lxShaderCompile(shader, shaderFsh, GL_FRAGMENT_SHADER);
    lxShaderLink(shader, NULL);

    float vertices[] = {
		-0.5f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f
	};

	unsigned int indices[] = {
		0, 1, 3, 3, 1, 2
	};

    vao = lxVaoCreate();
    lxVaoStoreIndicesList(vao, indices, sizeof(indices), 6);
    lxVaoStoreData(vao, 0, vertices, sizeof(vertices), 3);

    float fovY      = 75;
    float aspect    = (float) window->width / (float) window->height;
    camera = lxCameraCreate(fovY, aspect, 0.01f, 10.0f);
    lxCameraUpdateProjection(camera);

    lxWindowShow(window, &draw);
    
    lxVaoDestroy(vao);
    lxCameraDestroy(camera);
    lxShaderDestroy(shader);
    lxWindowDestroy(window);
    return 0;
}
