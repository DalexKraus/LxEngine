#include "lxdbg.hpp"
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <Windows.h>
#define MSGBOX(msg, title) MessageBoxA(0, msg, title, 0);
#else
#define MSGBOX(msg, title) dbgErr("Unable to display message box, as host os is not windows.")
#endif

void dbgPrint(const char* msg)
{
    printf("%s\n", msg);
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
    MSGBOX(0, message, title, 0);
}

void dbgErrBox(const char* msg)
{
    dbgMsgBox("Error", msg);
}

void assertBox(bool ckSuccess, DBGTXTPRM cFuncName, DBGTXTPRM cFileName, int cLNum, DBGTXTPRM errorText)
{
    if (ckSuccess) return;

    char buffer[2048];
    sprintf(buffer, "Assert Failed for function '%s'. File: %s:%d\n\nText: %s",
            cFuncName, cFileName, cLNum, errorText);

    dbgMsgBox("Engine Error", buffer);
    dbgErr(buffer);

    exit(-1);
}