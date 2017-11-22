#include <stdlib.h> // calloc, malloc, realloc, size_t
#include "str.h"

/**
 * Creates a new String.
 */
String* str_make(void) {
  String* result = (String*)malloc(sizeof(String));
  str_flush(result);
  return result;
}

String str_init(void) {
  String str = {NULL, 0, 10};
  str.list = (char*)calloc(10, sizeof(char));
  return str;
}

/**
 * Adds a character to the String, doubling size if the capacity doesn't fit it.
 */
void str_push(String* s, char c) {
  s->list[s->length] = c;
  ++s->length;
  if (s->length == s->cap) {
    s->cap *= 2;
    s->list = (char*)realloc(s->list, s->cap);
  }
  s->list[s->length] = '\0';
}

/**
 * Flushes the String, return the char* it used to point to.
 * The user is required to free the string after the last str_flush.
 * The user is required to free the result of str_flush.
 */
char* str_flush(String* s) {
  char* result = s->list;
  s->list = (char*)calloc(10, sizeof(char));
  s->length = 0;
  s->cap = 10;
  return result;
}
