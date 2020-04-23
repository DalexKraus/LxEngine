#include "lxcamera.hpp"
#include "lxdbg.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_precision.hpp"

//Used for calculating the right-vector
glm::vec3 worldUp = glm::vec3(0, 1, 0); 

void LxCamera::updateProjection()
{
    m_projection = glm::perspective(m_fov, m_aspect, m_nearZ, m_farZ);
}

void LxCamera::updateView()
{
    // Calculate front-vector according to yaw, pitch and roll
    m_front.x = (float) (cos(m_yaw) * cos(m_pitch));
    m_front.y = (float)  sin(m_pitch);
    m_front.z = (float) (sin(m_yaw) * cos(m_pitch));

    // Re-calculate vectors and normalize them
    m_right = glm::cross(m_front, worldUp);
    m_up    = glm::cross(m_front, m_right);

    m_front = glm::normalize(m_front);
    m_right = glm::normalize(m_right);
    m_up    = glm::normalize(m_up);

    // Re-create the view-matrix.
    m_view  = glm::lookAt(m_position, m_position + m_front, m_up);
}
