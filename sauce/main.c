#include "lx.h"

void draw(float deltaTime)
{
    
}

int main()
{
    printf("Starting engine ...\n");
    
    lxWindow window = lxWindowCreate("LynxEngine", 1280, 720, false);
    lxWindowShow(window, &draw);
    lxWindowDestroy(window);
    return 0;
}
