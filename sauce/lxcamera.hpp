#ifndef __LXCAMERA_H
#define __LXCAMERA_H

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

class LxCamera
{
private:
    float m_fov, m_aspect;
    float m_nearZ, m_farZ;
    float m_yaw, m_pitch, m_roll;

    glm::vec3 m_position;
    glm::mat4 m_projection;
    glm::mat4 m_view;

    glm::vec3 m_front, m_up, m_right;

public:
    LxCamera(float fov, float aspect, float nearZ, float farZ)
        : m_fov(fov), m_aspect(aspect), m_nearZ(nearZ), m_farZ(farZ)
    {
        m_up    = glm::vec3(0, 1, 0);
        m_yaw   = (float) (-(3.14159) / 2.0); // PI / 2 = 90°
        updateProjection();
        updateView();
    }

    inline void     translate(glm::vec3 translation)
    {
        m_position += translation;
    }

    inline void     setPosition(glm::vec3 position)
    {
        m_position = position;
    }

    void            updateProjection();
    void            updateView();
    glm::vec3*      getPosition()       { return &m_position;       }
    glm::mat4*      getProjection()     { return &m_projection;     }
    glm::mat4*      getView()           { return &m_view;           }

};

#endif