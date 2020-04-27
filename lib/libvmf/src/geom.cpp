#include "geom.hpp"
#include "glm/glm.hpp"

#include <algorithm>

// Used to avoid checking for an exact number
// (We use a region instead [-EPS;+EPS])
#define EPSILON 0.005

void calculateNormal(glm::vec3* a, glm::vec3* b, glm::vec3* c, glm::vec3* dest)
{
    *dest = glm::normalize(glm::cross(*b - *a, *c - *a));
}

/*
 * Rounds both values before comparing them.
 */ 
bool roundEqual(glm::vec3* a, glm::vec3* b)
{
    return round(a->x) == round(b->x) && round(a->y) == round(b->y) && round(a->z) == round(b->z);
}

/*
 * Calculates the given face by constructing the face's normal and distance from the origin
 */
void calculateFace(Face* face)
{
    // Calculate plane normal
    calculateNormal(&face->p1, &face->p2, &face->p3, &face->normal);

    // Calculate plane offset (From equation "n*P+d=0" or "ax+by+cz+d=0 => d = -ax - by - cz")
    // (Using the second one here)
    face->d = -face->normal.x * face->p1.x - face->normal.y * face->p1.y - face->normal.z * face->p1.z;
}

/*
 * Calculates the intersection between three planes in 3D space.
 */
bool getIntersection(Face* a, Face* b, Face* c, glm::vec3* dest)
{
    float denom = (float) glm::dot(a->normal, glm::cross(b->normal, c->normal));
    if (denom < EPSILON && denom > -EPSILON)
        return false;

    *dest = -a->d * glm::cross(b->normal, c->normal);
    *dest -= b->d * glm::cross(c->normal, a->normal);
    *dest -= c->d * glm::cross(a->normal, b->normal);
    *dest *= (1.0f / denom);
    return true;
}

/*
 * Where is the given point located relative to the given plane?
 */
PlaneLoc classifyPoint(Face* plane, glm::vec3* point)
{
    float dotProd = glm::dot(plane->normal, *point) + plane->d;

    if (dotProd >= EPSILON)
        return FRONT;
    if (dotProd <= -EPSILON)
        return BACK;
    else return ON_PLANE;
}

/*
 * Checks whether or not the face already contains the given vertex.
 */ 
bool containsVertex(Face* face, glm::vec3* vert)
{
    bool contained = false;
    for (int i = 0; i < face->vertices.size() && !contained; i++)
    {
        if (roundEqual(&(face->vertices[i]), vert)) contained = true;
    }

    return contained;
}

void sortVertices(Face* face)
{
    size_t vertexCount = face->vertices.size();
    if (vertexCount < 3) return;
    
    for (int i = 0; i < vertexCount - 2; i++)
    {
        glm::vec3 vertex = face->vertices.at(i);
        glm::vec3 centerToVertex = glm::normalize(vertex - face->center);

        double smallestAngle = -1;
        int smallestIdx = -1;

        // This plane will divide the vertices into the ones which are
        // more and less than 180° apart from the current vertex.
        // (It is not actually a face which will be visible later on)
        Face splitPlane;
        splitPlane.p1 = face->center;
        splitPlane.p2 = face->center + face->normal;
        splitPlane.p3 = face->center + centerToVertex;
        calculateFace(&splitPlane);

        //TODO: check the condition. i or i + 1?
        for (int j = i + 1; j < vertexCount; j++)
        {
            //The candidate is the possible next vertex
            glm::vec3 candidate = face->vertices[j];

            if (classifyPoint(&splitPlane, &candidate) != BACK)
            {
                glm::vec3 centerToCandidate = glm::normalize(candidate - face->center);
                double angle = glm::dot(centerToVertex, centerToCandidate);

                if (angle > smallestAngle)
                {
                    smallestAngle = angle;
                    smallestIdx = j;
                }
            }
        }

        // Swap the next vertex in the row with the
        // one we picked (only if we found a smallest one)
        if (smallestIdx != -1)
        {
            glm::vec3 temp = face->vertices[i + 1];
            face->vertices[i + 1] = face->vertices[smallestIdx];
            face->vertices[smallestIdx] = temp;
        }
    }

    // Reverse the order of the vertices if the face is facing in the wrong direction.
    // This is important to keep back-face culling working, as it is determined by the order
    // in which we draw our vertices.
    glm::vec3 normal;
    calculateNormal(&(face->vertices[0]), &(face->vertices[1]), &(face->vertices[2]), &normal);
    if (glm::dot(face->normal, normal) < -EPSILON)
    {
        std::reverse(face->vertices.begin(), face->vertices.end());
    }
}