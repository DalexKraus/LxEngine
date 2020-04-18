#ifndef __LXDBG_H
#define __LXDBG_H

#include "lx.h"

inline void dbgPrint(const char* msg)
{
    printf(msg);
}

inline void dbgErr(const char* msg)
{
    printf("[ERROR] %s\n", msg);
}

inline void dbgInfo(const char* msg)
{
    printf("[INFO] %s\n", msg);
}

//-------------------------------------------------------
//Note: symbol __func__ is defined in "lx.h"
#define _ASSERT(check, errorText) assertBox(check, __func__, __FILE__, __LINE__, errorText);

static void assertBox(bool ckSuccess, char* cFuncName, char* cFileName, int cLNum, const char* errorText)
{
    if (ckSuccess) return;

    char buffer[2048];
    sprintf(buffer, "Assert Failed for function '%s'. File: %s:%d\n\nText: %s",
            cFuncName, cFileName, cLNum, errorText);

    #ifdef _WIN32
    MessageBox(0, buffer, "Engine Error", 0);
    #endif

    dbgErr(buffer);

    exit(-1);
}

#endif