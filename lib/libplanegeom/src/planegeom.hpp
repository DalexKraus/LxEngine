#ifndef __LIBPLANEGEOM_H
#define __LIBPLANEGEOM_H

#ifndef LIBPLANEGEOM
    #ifdef LIB_EXPORT
        #define LIBPLANEGEOM __declspec(dllexport)
    #else
        #ifdef TEST
            #define LIBPLANEGEOM
        #else
            #define LIBPLANEGEOM __declspec(dllimport)
        #endif
    #endif
#endif

#endif