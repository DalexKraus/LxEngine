#ifndef __LIBPLANEGEOM_H
#define __LIBPLANEGEOM_H

#ifndef LIBPLANEGEOM
    #ifdef LIB_EXPORT
        #define LIBPLANEGEOM __declspec(dllexport)
    #else
        #ifdef TEST
            #define LIBPLANEGEOM
        #else
            #define LIBPLANEGEOM __declspec(dllimport)
        #endif
    #endif
#endif

#include "glm/glm.hpp"
#include <vector>

#include "libvmf/vmf.hpp"

namespace planegeom
{

//A simple vector struct with integer components
typedef struct
{
    int x, y, z;
} vec3i_t;

typedef struct
{
    //The three points that define the plane in 3D space
    glm::vec3 p1, p2, p3;
    glm::vec3 normal;
    float d; // From the plane equation "ax + by + cz + d = 0"
} plane_t;

typedef struct
{
    // The infinite base plane of this polygon
    plane_t plane;

    std::vector<glm::vec3> vertices;
    glm::vec3 center;
    glm::vec3 normal;
} face_t;

typedef struct
{
    int id;
    face_t* faces;
} brush_t;

//Used to determine the location of 
//a point relative to a plane.
typedef enum
{ 
    ON_PLANE, FRONT, BACK
} PlaneLoc;

/* Exported library functions */
LIBPLANEGEOM    void        (int );

}     

#endif