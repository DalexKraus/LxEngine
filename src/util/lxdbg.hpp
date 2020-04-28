#ifndef LX_DBG_H
#define LX_DBG_H

#include <stdbool.h>

void dbgPrint(const char* msg);
void dbgErr(const char* msg);
void dbgInfo(const char* msg);

void dbgMsgBox(const char* title, const char* message);
void dbgErrBox(const char* msg);

//-------------------------------------------------------
#ifdef _WIN32
#define __func__ __FUNCTION__
#endif

#ifndef _ASSERT
#define _ASSERT(check, errorText) assertBox(check, __func__, __FILE__, __LINE__, errorText);
#endif

#ifdef _WIN32
#define DBGTXTPRM char*
#else
#define DBGTXTPRM const char*
#endif

void assertBox(bool ckSuccess, DBGTXTPRM cFuncName, DBGTXTPRM cFileName, int cLNum, DBGTXTPRM errorText);

#endif