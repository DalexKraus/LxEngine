#ifndef __GEOM_H
#define __GEOM_H

#include "vmf.hpp"

/* Plane-creation stuff */
void calculateFace(Face* face);

/* Plane usage */
bool        getIntersection(Face* a, Face* b, Face* c, glm::vec3* dest);
PlaneLoc    classifyPoint(Face* plane, glm::vec3* point);   // Where is the point relative to the given plane?

/* Vertex management */
bool        containsVertex(Face* face, glm::vec3* vert);
void        sortVertices(Face* face);
#endif