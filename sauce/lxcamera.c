#include "lxcamera.h"

#include "lxdbg.h"
#include "cglm/cglm.h"

lxCamera lxCameraCreate(float fov, float aspect, float nearZ, float farZ)
{
    lxCamera camera = malloc(sizeof(struct lxCameraData));
    
    camera->fov     = fov;
    camera->aspect  = aspect;
    camera->nearZ   = nearZ;
    camera->farZ    = farZ;
    
    glm_vec3_zero(camera->position);

    // We only need to apply the identity matrix to the view matrix,
    // as the projection matrix is set first anyway.
    glm_mat4_identity(camera->viewMatrix);

    return camera;
}

void lxCameraDestroy(lxCamera camera)
{
    _ASSERT(camera != NULL, "The camera instance must not be null!");
    free(camera);
}

void lxCameraUpdateProjection(lxCamera camera)
{
    _ASSERT(camera != NULL, "The camera instance must not be null!");
    glm_perspective(camera->fov, camera->aspect, camera->nearZ, camera->farZ, camera->projectionMatrix);
}

void lxCameraUpdateView(lxCamera camera)
{
    _ASSERT(camera != NULL, "The camera instance must not be null!");

    vec3 front  = { 0, 0, 1 };
    vec3 up     = { 0, 1, 0 };
    vec3 center = { 0, 0, 0 };
    glm_lookat(camera->position, center, up, camera->viewMatrix);
}
