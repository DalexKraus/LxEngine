#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <vector>
#include <cmath>
#include <algorithm>

#include "glm/glm.hpp"

#define EPSILON 0.05
 
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

typedef struct
{
    int x, y, z;
} vec3i_t;

typedef struct
{
    vec3i_t ip1, ip2, ip3;   //Integer coordinate vectors
    glm::vec3 p1, p2, p3;           //Floating point glm-vectors
    glm::vec3 normal;
    float d;
} plane_t;

struct s_poly_t
{
    std::vector<glm::vec3> vertices;
    glm::vec3 center;
    glm::vec3 normal; //TODO: add a plane_t struct to this struct.
                      //TODO: Then we would only have one array of polygons, as one face = one polygon.
};
typedef struct s_poly_t poly_t;

typedef enum
{ 
    ON_PLANE, FRONT, BACK
} PlaneLoc;

void calculateNormal(glm::vec3& a, glm::vec3& b, glm::vec3* dest);
void calculatePlane(plane_t& plane);
bool getIntersection(plane_t a, plane_t b, plane_t c, glm::vec3* dest);
PlaneLoc pointLocationOnPlane(plane_t& plane, glm::vec3* point);
bool containsVertex(poly_t poly, glm::vec3* vert);
void sortVertices(poly_t& polygon);

int main()
{
    std::vector<plane_t> planes = std::vector<plane_t>();

    //Read planes file
    FILE* fd = fopen("planes", "r");
    
    while (!feof(fd))
    {
        plane_t plane;
        fscanf(fd, "(%d %d %d) (%d %d %d) (%d %d %d)\n",
                    &plane.ip1.x, &plane.ip1.y, &plane.ip1.z,
                    &plane.ip2.x, &plane.ip2.y, &plane.ip2.z,
                    &plane.ip3.x, &plane.ip3.y, &plane.ip3.z);

        plane.p1 = glm::vec3(plane.ip1.x, plane.ip1.y, plane.ip1.z);
        plane.p2 = glm::vec3(plane.ip2.x, plane.ip2.y, plane.ip2.z);
        plane.p3 = glm::vec3(plane.ip3.x, plane.ip3.y, plane.ip3.z);

        calculatePlane(plane);

        printf("PLANE: (%d %d %d)*(x, y, z) + %d\n",
            (int) plane.normal.x, (int) plane.normal.y, (int) plane.normal.z,
            (int) plane.d);

        planes.push_back(plane);
    }
    fclose(fd);
    printf("==> Loaded %d planes\n\n", (int) planes.size());

    // ----------------------------------------------------------------------
    // Math
    poly_t* polygons = (poly_t*) calloc(sizeof(poly_t), planes.size());

    for (int i = 0; i < planes.size(); i++)
    {
        polygons[i].vertices = std::vector<glm::vec3>();
        polygons[i].center = glm::vec3();
        polygons[i].normal = planes.at(i).normal;
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
                    for (int m = 0; m < planes.size() && legal; m++)
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
                        if (!containsVertex(polygons[i], &intersection))
                        {
                            polygons[i].vertices.push_back(intersection);
                            polygons[i].center += intersection;
                        }
                    }
                }
            }
        }

        polygons[i].center *= (1.0f / (float) polygons[i].vertices.size());

        //Sort vertices for each polygon
        sortVertices(polygons[i]);
    }
    // ----------------------------------------------------------------------
    // Print and Write
    fd = fopen("geom", "w");
    for (int i = 0; i < planes.size(); i++)
    {
        poly_t polygon = polygons[i];
        printf("\nPOLYGON #%d (%d verts):\n", i, (int) polygon.vertices.size());
        fprintf(fd, "Polygon %d:", i);

        for (auto& point : polygon.vertices)
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

bool containsVertex(poly_t poly, glm::vec3* vert)
{
    bool contained = false;
    for (int i = 0; i < poly.vertices.size() && !contained; i++)
    {
        if (roundEqual(&poly.vertices.at(i), vert)) contained = true;
    }

    return contained;
}

void sortVertices(poly_t& polygon)
{
    size_t vertexCount = polygon.vertices.size();
    for (int i = 0; i < vertexCount - 2; i++)
    {
        glm::vec3 vertex = polygon.vertices.at(i);
        glm::vec3 centerToVertex = glm::normalize(vertex - polygon.center);

        double smallestAngle = -1;
        int smallestIdx = -1;

        //This plane will divide the vertices into the ones with more and less than 180°.
        plane_t splitPlane;
        splitPlane.p1 = polygon.center;
        splitPlane.p2 = polygon.center + polygon.normal;
        splitPlane.p3 = polygon.center + centerToVertex;
        calculatePlane(splitPlane);

        //TODO: check the condition. i or i + 1?
        for (int j = i + 1; j < vertexCount; j++)
        {
            //The candidate is the possible next vertex
            glm::vec3 candidate = polygon.vertices.at(j);

            if (pointLocationOnPlane(splitPlane, &candidate) != BACK)
            {
                glm::vec3 centerToCandidate = glm::normalize(candidate - polygon.center);
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
        glm::vec3 temp = polygon.vertices[i + 1];
        polygon.vertices[i + 1] = polygon.vertices[smallestIdx];
        polygon.vertices[smallestIdx] = temp;
    }

    // Reverse the order of the vertices if the polygon is facing in the wrong direction.
    // This is important to keep back-face culling working, as it is determined by the order
    // in which we draw our vertices.
    glm::vec3 normal;
    calculateNormal(polygon.vertices[0], polygon.vertices[1], polygon.vertices[2], &normal);
    if (glm::dot(polygon.normal, normal) < -EPSILON)
    {
        std::reverse(polygon.vertices.begin(), polygon.vertices.end());
    }
}