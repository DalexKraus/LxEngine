#include "lx.h"
#include "lxvao.h"
#include "lxshader.h"
#include "fileutil.h"

#include "cglm/cglm.h"

lxVao vao;
lxShader shader;
mat4 projectionMatrix;

void draw(double deltaTime)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    lxShaderStart(shader);

    lxShaderUniform u_projectionMatrix = lxShaderGetUniformLocation(shader, "projectionMatrix");
    lxShaderUniformMat4(u_projectionMatrix, projectionMatrix);

    glEnableVertexAttribArray(0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);    

    lxShaderStop();
}

int main()
{
    printf("Starting engine ...\n");
    lxWindow window = lxWindowCreate("LynxEngine", 480, 270, false);
    lxWindowVsync(window, true);
    lxWindowAntiAlias(window, true);

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

    vec3 translation = { 0, 0, -5.0 };

    float fovY      = 75;
    float aspect    = (float) window->width / (float) window->height;
    glm_perspective(fovY, aspect, 0.01f, 10.0f, projectionMatrix);
    glm_translate_to(projectionMatrix, translation, projectionMatrix);

    lxWindowShow(window, &draw);
    
    lxVaoDestroy(vao);
    lxShaderDestroy(shader);
    lxWindowDestroy(window);
    return 0;
}
