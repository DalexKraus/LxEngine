#include <stdio.h>
#include "libvmf.h"

int main()
{
    vmf_t vmf = vmfOpen("map.vmf");
    
    char skyname[50];
    vmfLoadSkyname(vmf);
    vmfLoadBrushes(vmf);

    printf("SKYNAME: %s\n", vmf->skyName);

    vmfClose(vmf);
    vmfFree(vmf);

    getchar();
    return 0;
}