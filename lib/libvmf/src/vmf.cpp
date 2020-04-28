 /*
 * Some of the code here is based on the following paper:
 * https://github.com/stefanha/map-files/blob/master/MAPFiles.pdf
 * 
 * The described principles are implemented in a slightly different matter,
 * to fit the engine's needs. 
 * 
 * @author Kraus David
 * @date 24.04.2020
 */ 

#define _CRT_SECURE_NO_WARNINGS
#include "vmf.hpp"
#include "util.hpp"
#include "geom.hpp"

#ifndef ERRORBOX
#ifdef _WIN32
#include <windows.h>
#define ERRORBOX(msg) MessageBoxA(0, msg, "LibVMF", 0);
#endif
#endif

LIBVMF vmf_t vmfOpen(const char* filePath)
{
    vmf_t vmf = (vmf_t) malloc(sizeof(struct vmf_s));

    printf("[LibVMF] Reading '%s' ...\n", filePath);
    vmf->f = fopen(filePath, "r");
    vmf->brushes = new std::vector<VmfBrush*>();
    vmf->skyName = (const char*) calloc(VMF_MAX_SKYN_LEN, sizeof(char));
    return vmf;
}

LIBVMF void vmfClose(vmf_t vmf)
{
    if (vmf == 0 || vmf->f == 0)
        return;
    
    fclose(vmf->f);
    vmf->f = 0;
}

LIBVMF void vmfFree(vmf_t& vmf)
{
    if (vmf == 0)
        return;
    
    if (vmf->f != 0)
    {
        ERRORBOX("WARN: The VMF file instance has not been closed.\nClosing now ...");
        vmfClose(vmf);
    }

    //Delete all brush instances
    delete vmf->brushes;

    free((void*) vmf->skyName);

    free(vmf);
    vmf = 0;
}

VmfBrush* newBrush(vmf_t vmf)
{
    VmfBrush* brushInstance = new VmfBrush();

    //Append created brush to list of brushes in the map
    vmf->brushes->push_back(brushInstance);
    return brushInstance;
}

Face* newSide(VmfBrush* brush)
{
    Face* sideInstance = new Face();

    //Append created side to list of sides in the brush
    brush->faces.push_back(sideInstance);
    return sideInstance;
}

LIBVMF void vmfLoadSkyname(vmf_t vmf)
{
    if (vmf == 0)
        return;
    
    FILE* fd = vmf->f;
    rewind(fd);
    
    //Seek to the "skyname" key in the vmf file
    bool found = false;
    char line[VMF_MAX_LINE_LEN];
    while (!feof(fd) && !found)
    {
        fgets(line, VMF_MAX_LINE_LEN, fd);
        found = (strstr(line, VMF_KEY_SKYNAME) != NULL);
    }   

    if (found)
    {
        char* value = keyValue(line, VMF_KEY_SKYNAME);
        strcpy((char*) vmf->skyName, value);
    }
}

LIBVMF void vmfLoadBrushes(vmf_t vmf)
{
    if (vmf == 0)
        return;
    
    FILE* fd = vmf->f;
    rewind(fd);

    printf("Loading brushes from VMF ...");

    //Seek to the first "solid" key in the vmf file
    char line[VMF_MAX_LINE_LEN];
    while (!feof(fd))
    {
        fgets(line, VMF_MAX_LINE_LEN, fd);
        if (strstr(line, VMF_KEY_SOLID) != NULL)
        {
            // We reached the 'solid' keyword
            // Skip the next '{'
            fgets(line, VMF_MAX_LINE_LEN, fd);

            // Allocate a new brush instance
            VmfBrush* brushInstance = newBrush(vmf);

            // Extract solid's id
            fgets(line, VMF_MAX_LINE_LEN, fd);
            char* value = keyValue(line, VMF_KEY_ID);
            brushInstance->id = atoi(value);
            
            printf("\n\tBrush #%d faces: ", brushInstance->id);

            int faceCount = 0;
            // Read individual faces until we come across the "editor" tag,
            // which signals us that the brush has ended in the file.
            while (!feof(fd))
            {
                fgets(line, VMF_MAX_LINE_LEN, fd);
                if (strstr(line, VMF_KEY_SIDE) != NULL)
                {
                    // Skip the next '{' an the face's id
                    fgets(line, VMF_MAX_LINE_LEN, fd);
                    fgets(line, VMF_MAX_LINE_LEN, fd);

                    //Read the plane vectors
                    fgets(line, VMF_MAX_LINE_LEN, fd);
                    char* value = keyValue(line, VMF_KEY_PLANE);
                    
                    // Allocate a new face and store the vectors
                    Face* side = newSide(brushInstance);
                    faceCount++;
                    sscanf(value, "(%f %f %f) (%f %f %f) (%f %f %f)",
                        &side->p1.x, &side->p1.z, &side->p1.y,
                        &side->p2.x, &side->p2.z, &side->p2.y,
                        &side->p3.x, &side->p3.z, &side->p3.y);

                    printf("%d ", faceCount);
                }
                else if (strstr(line, VMF_KEY_EDITOR) != NULL)
                {
                    break;
                }
            }
        }
    }
    printf("\n==> Loaded %d brush(es)\n", (int) vmf->brushes->size());
}

LIBVMF void vmfPopulateVertices(vmf_t vmf)
{
    printf("[LibVMF] Populating vertices ...\n");

    for (auto* brush : *vmf->brushes)
    {
        printf("\tBrush #%d: %d faces, ", brush->id, (int) brush->faces.size());

        int vertexCount = 0;
        int illegalVertexCount = 0;
        size_t faceCount = brush->faces.size();

        //Calculate planes for each brush
        for (auto* face : brush->faces)
        {
            calculateFace(face);
        }

        //Calculate vertices for this face by intersecting with all other planes in this brush
        for (int i = 0; i < faceCount; i++)
        {
            Face* a = brush->faces[i];
            calculateFace(a);
            
            for (int j = 0; j < faceCount; j++)
            {
                for (int k = 0; k < faceCount; k++)
                {
                    if (i != j && i != k && j != k)
                    {
                        Face* b = brush->faces[j];
                        Face* c = brush->faces[k];

                        glm::vec3 intersection;
                        bool intersectionFound = getIntersection(a, b, c, &intersection);
                        if (!intersectionFound)
                            continue;

                        //Check if vertex is outside the solid
                        bool legal = true;
                        for (int m = 0; m < faceCount && legal; m++)
                        {
                            Face* face = brush->faces[i];
                            if (classifyPoint(face, &intersection) != ON_PLANE)
                            {
                                legal = false;
                            }
                        }

                        if (!legal)
                            illegalVertexCount++;

                        if (legal)
                        {
                            if (!containsVertex(a, &intersection))
                            {
                                a->vertices.push_back(intersection);
                                a->center += intersection;
                                vertexCount++;
                            }
                        }
                    }
                }
            }

            //Average the center
            brush->faces[i]->center *= (1.0f / (float) vertexCount);

            //Sort vertices for each face
            sortVertices(brush->faces[i]);
        }
        
        printf("%d vert(s), %d illegal;\n", vertexCount, illegalVertexCount);
    }
}

LIBVMF size_t vmfGetVertexSize(VmfBrush* brush)
{
    if (brush == 0)
        return 0;

    size_t vertexCount = 0;
    for (auto* face : brush->faces)
    {
        vertexCount += face->vertices.size();
    }

    // Times 3 as one vertex has 3 components (x, y, z).
    return vertexCount * sizeof(float) * 3;
}

LIBVMF void vmfCopyVertexData(VmfBrush* brush, float* dest)
{
    if (brush == 0 || dest == 0)
        return;

    unsigned int pos = 0;
    for (int faceIdx = 0; faceIdx < brush->faces.size(); faceIdx++)
    {
        Face* face = brush->faces[faceIdx];
        for (int vertIdx = 0; vertIdx < face->vertices.size(); vertIdx++)
        {
            glm::vec3 vertex = face->vertices[vertIdx];
            dest[pos++] = vertex.x / -128.0f; //Mirror y-axis by mult. with -1
            dest[pos++] = vertex.y / -128.0f;
            dest[pos++] = vertex.z / -128.0f;
        }
    }
}
