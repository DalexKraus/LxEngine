#include "lxcamera.h"

#include "lxdbg.h"
#include "cglm/cglm.h"

//Used for calculating the right-vector
vec3 worldUp = { 0, 1, 0 }; 

lxCamera lxCameraCreate(float fov, float aspect, float nearZ, float farZ)
{
    lxCamera camera = malloc(sizeof(struct lxCameraData));
    
    camera->fov     = fov;
    camera->aspect  = aspect;
    camera->nearZ   = nearZ;
    camera->farZ    = farZ;
    
    glm_vec3_zero(camera->position);
    glm_vec3_zero(camera->frontVector);
    glm_vec3_zero(camera->rightVector);
    glm_vec3_zero(camera->upVector);
    glm_vec3_zero(camera->lookAt);
    camera->upVector[1] = 1; //Align vector

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

    // Calculate front-vector according to yaw, pitch and roll
    camera->frontVector[0] = (float) (cos(camera->yaw) * cos(camera->pitch));
    camera->frontVector[1] = (float) sin(camera->pitch);
    camera->frontVector[2] = (float) (sin(camera->yaw) * cos(camera->pitch));
    glm_vec3_normalize(camera->frontVector);

    // Re-calculate right-vector
    glm_vec3_cross(camera->frontVector, worldUp, camera->rightVector);
    glm_vec3_normalize(camera->rightVector);

    // Re-calculate up-vector
    glm_vec3_cross(camera->rightVector, camera->frontVector, camera->upVector);
    glm_vec3_normalize(camera->upVector);

    // Finally, re-create the view-matrix.
    glm_vec3_add(camera->position, camera->frontVector, camera->lookAt);
    glm_lookat(camera->position, camera->lookAt, camera->upVector, camera->viewMatrix);
}
