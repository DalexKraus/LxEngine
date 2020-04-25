#ifndef __LIBVMF_H
#define __LIBVMF_H

#include <stdio.h>
#include <vector>

#ifndef LIBVMF
    #ifdef LIBVMF_EXPORT
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

class BrushSide
{
public:
    // The face is described by a plane which
    // is defined by 3 points in 3D-space.
    Vec3i p1, p2, p3;

    BrushSide()
    {
        p1 = Vec3i();
        p2 = Vec3i();
        p3 = Vec3i();
    }
};

class VmfBrush
{
public:
    // A brush is defined by one or more sides (faces).
    std::vector<BrushSide*> sides = std::vector<BrushSide*>();
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
LIBVMF  void        vmfFreeBrushes(vmf_t vmf);

#endif