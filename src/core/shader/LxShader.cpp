#include "LxShader.hpp"
#include "util/lxdbg.hpp"

bool checkError(int programId, GLuint errorType);

/**
 * Creates a new shader instance and copies the shader file paths.
 */
lxShader lxShaderCreate()
{
    lxShader shader = (lxShader) malloc(sizeof(lxShader));
    *shader = glCreateProgram();
    return shader;
}

/**
 * Compiles and attaches a shader to the shader program instance.
 */
void lxShaderCompile(lxShader shader, const char* shaderSource, GLuint shaderType)
{
    _ASSERT(shader          != NULL, "The shader must not be null!");
    _ASSERT(shaderSource    != NULL, "The shader source must not be null!");
    _ASSERT(   shaderType == GL_VERTEX_SHADER
            || shaderType == GL_FRAGMENT_SHADER 
            || shaderType == GL_GEOMETRY_SHADER
            || shaderType == GL_COMPUTE_SHADER,
            "Invalid shader type!");

    GLuint shaderId = glCreateShader(shaderType);
    //Atttach the shader source
    glShaderSource(shaderId, 1, &shaderSource, NULL);
    //Compile the shader
    glCompileShader(shaderId);

    //Check for any compilation errors
	if (checkError(GL_COMPILE_STATUS, shaderId))
	{
		glDeleteShader(shaderId);
		return;
	}

    //Attach shader program
    glAttachShader(*shader, shaderId);

    dbgInfo("Shader compilation finished.");

    //Normally, we need to delete the individual shaders at this point,
    //as the program has already been linked. But as they get deleted when the
    //shader is eventually deleted using #lxShaderDestroy(),
    //there's no point in doing that.
}

/**
 * Links the previously compiled shaders to a shader program.
 */
void lxShaderLink(lxShader shader, void (*bindAttributesCallback)())
{
    _ASSERT(shader != NULL, "The shader must not be null!");

    if (bindAttributesCallback != NULL)
        bindAttributesCallback();

    //Default attributes
    lxShaderBindAttribute(shader, 0, "aPosition");

    //Linking stage
    glLinkProgram(*shader);

    //Check for any linking errors
    if (checkError(GL_LINK_STATUS, *shader))
    {
        dbgErr("Shader linking failed");
    }
    else dbgInfo("Shader linking done.");
}

/**
 * Binds the given attribute number to the specified variable name.
 */
void lxShaderBindAttribute(lxShader shader, int attr, const char* variableName)
{
    glBindAttribLocation(*shader, attr, variableName);
}

void lxShaderDestroy(lxShader shader)
{
    _ASSERT(shader != NULL, "The shader must not be null!");
    glDeleteProgram(*shader);
    free(shader);
}

void lxShaderStart(lxShader shader)
{
    _ASSERT(shader != NULL, "The shader must not be null!");
    glUseProgram(*shader);
}

void lxShaderStop()
{
    glUseProgram(0);
}

//--------------------------------------------------

bool checkError(int programId, GLuint errorType)
{
    int success;
    glGetShaderiv(programId, errorType, &success);
    if (!success) {
        char errorLog[1024] = "Shader compilation failed!\n\nError Log:\n";
        //Shift buffer by 64 bytes, so we can insert the message which is already contained.
        glGetShaderInfoLog(programId, sizeof(errorLog) - 64, NULL, errorLog + sizeof(64));
        dbgErrBox(errorLog);
    }

    //TODO: check this statement
    return !success;
}