#ifndef LX_CAMERA_H
#define LX_CAMERA_H

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

#include <stdio.h>

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
        m_up        = glm::vec3(0, 1, 0);
        m_right     = glm::vec3(0, 0, 0);
        m_front     = glm::vec3(0, 0, 0);

        m_yaw   = (float) (-(3.14159) / 2.0); // PI / 2 = 90°
        m_pitch = m_roll = 0;

        m_position.x = m_position.y = m_position.z = 0;
        
        updateProjection();
        updateView();
    }

    inline void rotate(float& dYaw, float& dPitch, float& dRoll)
    {
        m_pitch += dPitch;
        m_yaw   += dYaw;
        m_roll  += dRoll;
        
        //Camera constraints
        if (m_pitch > 89.0f)
            m_pitch = 89.0f;
        if (m_pitch < -89.0f)
            m_pitch = -89.0f;
    }

    inline void translate(glm::vec3 translation)
    {
        m_position += translation;
    }

    inline void setPosition(glm::vec3 position)
    {
        m_position = position;
    }

    void            updateProjection();
    void            updateView();
    
    //TODO: Make accessors read only (const?)
    glm::vec3*      getPosition()       { return &m_position;       }
    glm::vec3*      getUp()             { return &m_up;             }
    glm::vec3*      getFront()          { return &m_front;          }
    glm::vec3*      getRight()          { return &m_right;          }

    glm::mat4*      getProjection()     { return &m_projection;     }
    glm::mat4*      getView()           { return &m_view;           }

};

#endif