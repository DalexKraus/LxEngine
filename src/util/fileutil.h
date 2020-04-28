#ifndef FILEUTIL_H
#define FILEUTIL_H

#include <stdio.h>
#include <stdlib.h>


#ifdef __cplusplus
extern "C" {
#endif

const char* readFileContent(const char* fileName);

#ifdef __cplusplus
}
#endif

#endif