#include "lxdbg.h"

void dbgPrint(const char* msg)
{
    printf(msg);
}

void dbgErr(const char* msg)
{
    printf("[ERROR] %s\n", msg);
}

void dbgInfo(const char* msg)
{
    printf("[INFO] %s\n", msg);
}

void dbgMsgBox(const char* title, const char* message)
{
    #ifdef _WIN32
    MessageBox(0, message, title, 0);
    #else
    dbgErr("Unable to display message box, as host os is not windows.");
    #endif    
}

void dbgErrBox(const char* msg)
{
    dbgMsgBox("Error", msg);
}

void assertBox(bool ckSuccess, char* cFuncName, char* cFileName, int cLNum, const char* errorText)
{
    if (ckSuccess) return;

    char buffer[2048];
    sprintf(buffer, "Assert Failed for function '%s'. File: %s:%d\n\nText: %s",
            cFuncName, cFileName, cLNum, errorText);

    dbgMsgBox("Engine Error", buffer);
    dbgErr(buffer);

    exit(-1);
}