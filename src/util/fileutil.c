#define _CRT_SECURE_NO_WARNINGS
#include "fileutil.h"


const char* readFileContent(const char* fileName)
{
    FILE* fd = fopen(fileName, "r");

    //Get how much memory we need to allocate
    fseek(fd, 0, SEEK_END);
    long size = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    //Allocate memory for the content
    char* buffer = (char*) calloc(size, sizeof(char));
    fread(buffer, 1, size, fd);

    fclose(fd);
    return buffer;
}