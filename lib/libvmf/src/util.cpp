#include "util.hpp"

#include <ctype.h>
#include <string.h>

char* trim(char* str)
{
    char* end;

    // Trim leading spaces
    while (isspace((unsigned char) *str))
        str++;

    // Return if we don't have any chars left
    if (*str == 0) 
        return str;

    // Trim trailing spaces
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char) *end))
        end--;

    // Write new null-terminator
    end[1] = '\0';

    return str;
}

char* keyValue(char* line, const char* key)
{
    char* trimmedLine = trim(line) +
        strlen(key) + 4;                            //Skip the key, spaces and quotes.
    trimmedLine[strlen(trimmedLine) - 1] = '\0';    //Remove the trailing quote
    return trimmedLine;
}