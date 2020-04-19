#ifndef __LXDBG_H
#define __LXDBG_H

void dbgPrint(const char* msg);
void dbgErr(const char* msg);
void dbgInfo(const char* msg);

void        dbgMsgBox(const char* title, const char* message);
void        dbgErrBox(const char* msg);

//-------------------------------------------------------
//Note: symbol __func__ is defined in "lx.h"
#include "lx.h"
#ifndef _ASSERT
#define _ASSERT(check, errorText) assertBox(check, __func__, __FILE__, __LINE__, errorText);
#endif
void assertBox(bool ckSuccess, char* cFuncName, char* cFileName, int cLNum, const char* errorText);

#endif