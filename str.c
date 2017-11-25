#include <stdlib.h> // calloc, malloc, realloc, size_t
#include "str.h"

String str_init(void) {
  String str = {NULL, 0, 10};
  str.list = malloc(10 * sizeof(char));
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