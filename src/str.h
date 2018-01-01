#include <stdlib.h> // calloc, malloc, realloc, size_t

#ifndef __STR_H__
#define __STR_H__

typedef struct {
  char* list;
  size_t length;
  size_t cap;
} String;

/**
 * Uses return-value optimization to default-ctor a stack-local String.
 */
String initStr(void);

/**
 * Adds a character to the String, doubling size if the capacity doesn't fit it.
 */
void pushStr(String* s, char c);

/**
 * Create a unique char* copy for another char* in order to simplify destruction
 */
char* copyStr(char* value);

/**
 * Creates a string from arguments like printf.
 */
char* makeStr(const char* fmt, ...);

#endif
