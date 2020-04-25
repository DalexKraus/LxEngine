#define _CRT_SECURE_NO_WARNINGS
#include "libvmf.h"

#ifndef ERRORBOX
#ifdef _WIN32
#include <windows.h>
#define ERRORBOX(msg) MessageBoxA(0, msg, "LibVMF", 0);
#endif
#endif

char* trim(char* str);

LIBVMF vmf_t vmfOpen(const char* filePath)
{
    vmf_t vmf = (vmf_t) malloc(sizeof(struct vmf_s));

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

/*
 * Returns the value of the given key in a line.
 * Note:    The line must only contain the value and the key.
 *          Leading and trailing whitespaces are ignored.
 */ 
char* keyValue(char* line, char* key)
{
    char* trimmedLine = trim(line) +
        strlen(key) + 4;                            //Skip the key, spaces and quotes.
    trimmedLine[strlen(trimmedLine) - 1] = '\0';    //Remove the trailing quote
    return trimmedLine;
}

VmfBrush* newBrush(vmf_t vmf)
{
    VmfBrush* brushInstance = new VmfBrush();

    //Append created brush to list of brushes in the map
    vmf->brushes->push_back(brushInstance);
    return brushInstance;
}

BrushSide* newSide(VmfBrush* brush)
{
    BrushSide* sideInstance = new BrushSide();

    //Append created side to list of sides in the brush
    brush->sides.push_back(sideInstance);
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
            if (brushInstance->id == 0)
            {
                printf("LINE: %s\n", line);
            }
            
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
                    BrushSide* side = newSide(brushInstance);
                    faceCount++;
                    sscanf(value, "(%d %d %d) (%d %d %d) (%d %d %d)",
                        &side->p1.m_x, &side->p1.m_y, &side->p1.m_z,
                        &side->p2.m_x, &side->p2.m_y, &side->p2.m_z,
                        &side->p3.m_x, &side->p3.m_y, &side->p3.m_z);

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

LIBVMF void vmfFreeBrushes(vmf_t vmf)
{

}

char* trim(char* str)
{
    char* end;

    // Trim leading spaces
    while (isspace((unsigned char) *str))
        str++;

    // Return if we don't have any chars left
    if (*str == 0) 
        return str;

    // Trim trailing spaces
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char) *end))
        end--;

    // Write new null-terminator
    end[1] = '\0';

    return str;
}