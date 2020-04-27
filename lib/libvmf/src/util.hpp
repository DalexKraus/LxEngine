#ifndef __UTIL_H
#define __UTIL_H

/*
 * Removes any leading and trailing whitespaces.
 */
char* trim(char* str);

/*
 * Returns the value of the given key in a line.
 * Note:    The line must only contain the value and the key.
 *          Leading and trailing whitespaces are ignored.
 */ 
char* keyValue(char* line, const char* key);

#endif