#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <vector>
#include <cmath>

#include "glm/glm.hpp"

#define EPSILON 0.05
 
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
};
typedef struct s_poly_t poly_t;

bool getIntersection(plane_t a, plane_t b, plane_t c, glm::vec3* dest);
bool containsVertex(poly_t poly, glm::vec3* vert);

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

        // Calculate plane normal
        glm::vec3 p1tp2 = plane.p2 - plane.p1;
        glm::vec3 p1tp3 = plane.p3 - plane.p1;
        plane.normal = glm::normalize(glm::cross(p1tp2, p1tp3));

        // Calculate plane offset (From equation "n*P+d=0" or "ax+by+cz+d=0")
        // (Using the second one here)
        plane.d = -plane.normal.x * plane.p1.x - plane.normal.y * plane.p1.y - plane.normal.z * plane.p1.z;

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
                        float t = glm::dot(plane.normal, intersection) + plane.d;
                        if (t >= EPSILON && t <= -EPSILON)
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
                        }
                    }
                }
            }
        }
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

bool containsVertex(poly_t poly, glm::vec3* vert)
{
    bool contained = false;
    for (int i = 0; i < poly.vertices.size() && !contained; i++)
    {
        if (roundEqual(&poly.vertices.at(i), vert)) contained = true;
    }

    return contained;
}