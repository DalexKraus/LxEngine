#ifndef __LXSHADER_H
#define __LXSHADER_H

#include "lx.h"

//Only include the needed cglm stuff
#include "cglm/vec2.h"
#include "cglm/vec3.h"
#include "cglm/vec4.h"
#include "cglm/mat4.h"

typedef GLuint* lxShader;
typedef GLuint lxShaderUniform;

lxShader    lxShaderCreate();
void        lxShaderCompile(lxShader shader, const char* shaderSource, GLuint shaderType);
void        lxShaderLink(lxShader shader, void (*bindAttributesCallback)());

// Attribute and uniform stuff
void                        lxShaderBindAttribute(lxShader shader, int attr, const char* variableName);
inline lxShaderUniform      lxShaderGetUniformLocation(lxShader shader, const char* name) { return glGetUniformLocation(*shader, name); }

inline void lxShaderUniformBool (lxShaderUniform uniform, bool  value) { glUniform1i(uniform, value); }
inline void lxShaderUniformInt  (lxShaderUniform uniform, int   value) { glUniform1i(uniform, value); }
inline void lxShaderUniformFloat(lxShaderUniform uniform, float value) { glUniform1f(uniform, value); }
inline void lxShaderUniformVec2 (lxShaderUniform uniform, vec2  value) { glUniform2fv(uniform, 1, &value[0]); }
inline void lxShaderUniformVec3 (lxShaderUniform uniform, vec3  value) { glUniform3fv(uniform, 1, &value[0]); }
inline void lxShaderUniformVec4 (lxShaderUniform uniform, vec4  value) { glUniform4fv(uniform, 1, &value[0]); }
inline void lxShaderUniformMat4 (lxShaderUniform uniform, mat4  value) { glUniformMatrix4fv(uniform, 1, GL_FALSE, &value[0][0]); }

void        lxShaderDestroy(lxShader shader);
void        lxShaderStart(lxShader shader);
void        lxShaderStop();

#endif