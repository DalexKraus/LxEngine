#ifndef __LXSHADER_H
#define __LXSHADER_H

#include "lx.h"

typedef GLuint* lxShader;

lxShader    lxShaderCreate();
void        lxShaderCompile(lxShader shader, const char* shaderSource, GLuint shaderType);
void        lxShaderLink(lxShader shader, void (*bindAttributesCallback)());
void        lxShaderBindAttribute(lxShader shader, int attr, const char* variableName);

void        lxShaderDestroy(lxShader shader);
void        lxShaderStart(lxShader shader);
void        lxShaderStop();

#endif