 /*
 * The following source code is based on the following paper:
 * https://github.com/stefanha/map-files/blob/master/MAPFiles.pdf
 * 
 * The described principles are implemented in a slightly different matter,
 * to fit the engine's needs. 
 * 
 * @author Kraus David
 * @date 24.04.2020
 */ 

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <vector>
#include <cmath>
#include <algorithm>

#include "glm/glm.hpp"

// Used to avoid checking for an exact number
// (We use a region instead [-EPS;+EPS])
#define EPSILON 0.05

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

//Used to determine the location of 
//a point relative to a plane.
typedef enum
{ 
    ON_PLANE, FRONT, BACK
} PlaneLoc;

/* Plane-creation stuff */
void        calculatePlane(plane_t& plane);
void        calculateNormal(glm::vec3& a, glm::vec3& b, glm::vec3* dest);

/* Plane usage */
bool        getIntersection(plane_t a, plane_t b, plane_t c, glm::vec3* dest);
PlaneLoc    pointLocationOnPlane(plane_t& plane, glm::vec3* point);

/* Vertex management */
bool        containsVertex(face_t face, glm::vec3* vert);
void        sortVertices(face_t& face);

int main()
{
    std::vector<plane_t> planes = std::vector<plane_t>();

    // ----------------------------------------------------------------------
    // Read planes file
    FILE* fd = fopen("planes", "r");
    
    while (!feof(fd))
    {
        plane_t plane;
        plane.p1 = plane.p2 = plane.p3 = glm::vec3(); //Initialize our plane vectors

        // The vectors in a .vmf file are described using integers,
        // but we need them to contain float-components.
        vec3i_t ip1, ip2, ip3;
        fscanf(fd, "(%d %d %d) (%d %d %d) (%d %d %d)\n",
                    &ip1.x, &ip1.y, &ip1.z,
                    &ip2.x, &ip2.y, &ip2.z,
                    &ip3.x, &ip3.y, &ip3.z);

        //Convert them to glm float-vectors
        plane.p1 = glm::vec3(ip1.x, ip1.y, ip1.z);
        plane.p2 = glm::vec3(ip2.x, ip2.y, ip2.z);
        plane.p3 = glm::vec3(ip3.x, ip3.y, ip3.z);

        //Calculate our plane using those three points
        calculatePlane(plane);

        printf("PLANE #%d: (%2d %2d %2d) * (x, y, z) + %d\n",
            (int) planes.size(),
            (int) plane.normal.x, (int) plane.normal.y, (int) plane.normal.z,
            (int) plane.d);

        planes.push_back(plane);
    }
    fclose(fd);
    printf("==> Read %d planes\n\n", (int) planes.size());

    // ----------------------------------------------------------------------
    // Math
    face_t* faces = (face_t*) calloc(sizeof(face_t), planes.size());

    //Collect all vertices
    for (int i = 0; i < planes.size(); i++)
    {
        faces[i].vertices = std::vector<glm::vec3>();
        faces[i].center = glm::vec3();
        faces[i].normal = planes.at(i).normal;
        for (int j = 0; j < planes.size(); j++)
        {
            for (int k = 0; k < planes.size(); k++)
            {
                if (i != j && i != k && j != k)
                {
                    plane_t a = planes.at(i);
                    plane_t b = planes.at(j);
                    plane_t c = planes.at(k);

                    glm::vec3 intersection;
                    bool intersectionFound = getIntersection(a, b, c, &intersection);
                    if (!intersectionFound)
                        continue;

                    bool legal = true;
                    for (int m = 0; m < planes.size() && legal;  m++)
                    {
                        plane_t plane = planes.at(i);
                        if (pointLocationOnPlane(plane, &intersection) != ON_PLANE)
                        {
                            legal = false;
                        }
                    }

                    if (!legal)
                        printf("Ignoring illegal vertex ...\n");

                    if (legal)
                    {
                        if (!containsVertex(faces[i], &intersection))
                        {
                            faces[i].vertices.push_back(intersection);
                            faces[i].center += intersection;
                        }
                    }
                }
            }
        }

        faces[i].center *= (1.0f / (float) faces[i].vertices.size());

        //Sort vertices for each face
        sortVertices(faces[i]);
    }
    // ----------------------------------------------------------------------
    // Print and Write
    fd = fopen("geom", "w");
    for (int i = 0; i < planes.size(); i++)
    {
        face_t face = faces[i];
        printf("\nFACE #%d (%d verts):\n", i, (int) face.vertices.size());
        fprintf(fd, "FACE %d:", i);

        for (auto& point : face.vertices)
        {
            printf("VERTEX %d %d %d\n", (int) point.x, (int) point.y, (int) point.z);
            fprintf(fd, " %d %d %d", (int) point.x, (int) point.y, (int) point.z);
        }
        fprintf(fd, "\n");
    }
    fclose(fd);
    // ----------------------------------------------------------------------

    getchar();
    return 0;
}

void calculateNormal(glm::vec3& a, glm::vec3& b, glm::vec3& c, glm::vec3* dest)
{
    *dest = glm::normalize(glm::cross(b - a, c - a));
}

void calculatePlane(plane_t& plane)
{
    // Calculate plane normal
    calculateNormal(plane.p1, plane.p2, plane.p3, &plane.normal);

    // Calculate plane offset (From equation "n*P+d=0" or "ax+by+cz+d=0 => d = -ax - by - cz")
    // (Using the second one here)
    plane.d = -plane.normal.x * plane.p1.x - plane.normal.y * plane.p1.y - plane.normal.z * plane.p1.z;
}

bool getIntersection(plane_t a, plane_t b, plane_t c, glm::vec3* dest)
{
    float denom = (float) glm::dot(a.normal, glm::cross(b.normal, c.normal));
    if (denom < EPSILON && denom > -EPSILON)
        return false;

    *dest = -a.d * glm::cross(b.normal, c.normal);
    *dest -= b.d * glm::cross(c.normal, a.normal);
    *dest -= c.d * glm::cross(a.normal, b.normal);
    *dest *= (1.0f / denom); 

    return true;
}

bool roundEqual(glm::vec3* a, glm::vec3* b)
{
    return round(a->x) == round(b->x) && round(a->y) == round(b->y) && round(a->z) == round(b->z);
}

PlaneLoc pointLocationOnPlane(plane_t& plane, glm::vec3* point)
{
    float dotProd = glm::dot(plane.normal, *point) + plane.d;

    if (dotProd >= EPSILON)
        return FRONT;
    if (dotProd <= -EPSILON)
        return BACK;
    else return ON_PLANE;
}

bool containsVertex(face_t face, glm::vec3* vert)
{
    bool contained = false;
    for (int i = 0; i < face.vertices.size() && !contained; i++)
    {
        if (roundEqual(&face.vertices.at(i), vert)) contained = true;
    }

    return contained;
}

/*
 * Sorts the vertices of the given face clockwise.
 */
void sortVertices(face_t& face)
{
    size_t vertexCount = face.vertices.size();
    for (int i = 0; i < vertexCount - 2; i++)
    {
        glm::vec3 vertex = face.vertices.at(i);
        glm::vec3 centerToVertex = glm::normalize(vertex - face.center);

        double smallestAngle = -1;
        int smallestIdx = -1;

        //This plane will divide the vertices into the ones with more and less than 180°.
        plane_t splitPlane;
        splitPlane.p1 = face.center;
        splitPlane.p2 = face.center + face.normal;
        splitPlane.p3 = face.center + centerToVertex;
        calculatePlane(splitPlane);

        //TODO: check the condition. i or i + 1?
        for (int j = i + 1; j < vertexCount; j++)
        {
            //The candidate is the possible next vertex
            glm::vec3 candidate = face.vertices.at(j);

            if (pointLocationOnPlane(splitPlane, &candidate) != BACK)
            {
                glm::vec3 centerToCandidate = glm::normalize(candidate - face.center);
                double angle = glm::dot(centerToVertex, centerToCandidate);

                if (angle > smallestAngle)
                {
                    smallestAngle = angle;
                    smallestIdx = j;
                }
            }
        }

        // Swap the next vertex in the row with the
        // one we picked
        // TODO: Replace all .at() with the index-operator ([])
        glm::vec3 temp = face.vertices[i + 1];
        face.vertices[i + 1] = face.vertices[smallestIdx];
        face.vertices[smallestIdx] = temp;
    }

    // Reverse the order of the vertices if the face is facing in the wrong direction.
    // This is important to keep back-face culling working, as it is determined by the order
    // in which we draw our vertices.
    glm::vec3 normal;
    calculateNormal(face.vertices[0], face.vertices[1], face.vertices[2], &normal);
    if (glm::dot(face.normal, normal) < -EPSILON)
    {
        std::reverse(face.vertices.begin(), face.vertices.end());
    }
}
