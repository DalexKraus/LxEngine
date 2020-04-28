#ifndef LX_RENDERER_H
#define LX_RENDERER_H

#include "glm/mat4x4.hpp"

class LxRenderer
{
public:
    virtual void update (const double& deltaTime) = 0;
    virtual void draw   (const glm::mat4* projectionMatrix) = 0;
};

#endif