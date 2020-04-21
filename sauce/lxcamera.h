#ifndef __LXCAMERA_H
#define __LXCAMERA_H

#include "cglm/vec3.h"
#include "cglm/mat4.h"
#include "lxinputmanager.h"

struct lxCameraData
{
    //Generic information
    float fov;
    float aspect;
    float nearZ;
    float farZ;
    float yaw;
    float pitch;
    float roll;
    
    vec3 upVector, frontVector, rightVector, lookAt;
    vec3 position;
    mat4 viewMatrix;

    mat4 projectionMatrix;
};
typedef struct lxCameraData* lxCamera;

lxCamera        lxCameraCreate(float fov, float aspect, float nearZ, float farZ);
void            lxCameraDestroy(lxCamera camera);
void            lxCameraInputManager(lxInputManager mgr);
void            lxCameraUpdateProjection(lxCamera camera);
void            lxCameraUpdateView(lxCamera camera);

inline void     lxCameraTranslate(lxCamera camera, vec3 translation)
{
    glm_vec3_add(camera->position, translation, camera->position);
}

inline void     lxCameraPosition(lxCamera camera, vec3 position)
{
    glm_vec3_copy(position, camera->position);
}

#endif