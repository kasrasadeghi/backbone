#include <stdlib.h> // calloc, malloc, realloc, size_t

#ifndef __STR_H__
#define __STR_H__

typedef struct {
  char* list;
  size_t length;
  size_t cap;
} String;

/**
 * Creates a new String.
 */
String* str_make(void);

/**
 * Adds a character to the String, doubling size if the capacity doesn't fit it.
 */
void str_push(String* s, char c);

/**
 * Flushes the String, return the char* it used to point to.
 * The user is required to free the result of str_flush.
 */
char* str_flush(String* s);

#endif
