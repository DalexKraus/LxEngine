#ifndef __LIBVMF_H
#define __LIBVMF_H

#include <stdio.h>
#include <vector>

#include "glm/vec3.hpp"

#ifndef LIBVMF
    #ifdef LIB_EXPORT
        #define LIBVMF __declspec(dllexport)
    #else
        #ifdef TEST
            #define LIBVMF
        #else
            #define LIBVMF __declspec(dllimport)
        #endif
    #endif
#endif

#define VMF_MAX_LINE_LEN    512
#define VMF_MAX_SKYN_LEN    64
#define VMF_KEY_SKYNAME     "skyname"
#define VMF_KEY_SOLID       "solid"
#define VMF_KEY_EDITOR      "editor"
#define VMF_KEY_SIDE        "side"
#define VMF_KEY_ID          "id"
#define VMF_KEY_PLANE       "plane"

//Used to determine the location of 
//a point relative to a plane.
typedef enum
{ 
    ON_PLANE, FRONT, BACK
} PlaneLoc;

// Very basic vector class with int components
class Vec3i
{
public:
    int m_x, m_y, m_z;
    
    Vec3i()
    {
        m_x = m_y = m_z = 0;
    }
};

class Face
{
public:
    // The face is described by a plane which
    // is defined by 3 points in 3D-space.
    glm::vec3 p1, p2, p3;

    glm::vec3 normal;
    glm::vec3 center;
    float d = 0; // From the plane equation "ax + by + cz + d = 0"

    std::vector<glm::vec3> vertices;

    Face()
    {
        p1 = glm::vec3();
        p2 = glm::vec3();
        p3 = glm::vec3();
        normal = glm::vec3();
        center = glm::vec3();
        vertices = std::vector<glm::vec3>();
    }
};

class VmfBrush
{
public:
    // A brush is defined by one or more sides (faces).
    std::vector<Face*> faces = std::vector<Face*>();
    int id;
};

struct vmf_s
{
    FILE* f;
    std::vector<VmfBrush*>* brushes;
    const char* skyName;
};
typedef struct vmf_s* vmf_t;

/* Exported library functions */
LIBVMF  vmf_t       vmfOpen(const char* filePath);
LIBVMF  void        vmfClose(vmf_t vmf);
LIBVMF  void        vmfFree(vmf_t& vmf);

LIBVMF  void        vmfLoadSkyname(vmf_t vmf);
LIBVMF  void        vmfLoadBrushes(vmf_t vmf);
LIBVMF  void        vmfPopulateVertices(vmf_t vmf);

LIBVMF  size_t      vmfGetVertexSize(VmfBrush* brush);
LIBVMF  void        vmfCopyVertexData(VmfBrush* brush, float* dest);

#endif