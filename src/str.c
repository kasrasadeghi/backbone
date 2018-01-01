#include <stdlib.h> // calloc, malloc, realloc, size_t
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "str.h"

String initStr(void) {
  String str = {NULL, 0, 10};
  str.list = malloc(10 * sizeof(char));
  str.list[0] = 0;
  return str;
}

void pushStr(String* s, char c) {
  s->list[s->length] = c;
  ++s->length;
  if (s->length == s->cap) {
    s->cap *= 2;
    s->list = (char*)realloc(s->list, s->cap);
  }
  s->list[s->length] = '\0';
}

char* copyStr(char* value) {
  const size_t len = strlen(value) + 1;
  char* unique_str = malloc(len);
  strncpy(unique_str, value, len);
  return unique_str;
}

char* makeStr(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  char* cache = calloc(256, 1);
  size_t len = (size_t) vsnprintf(cache, 256, fmt, args);
  va_end(args);
  char* result = calloc(len + 1, 1);
  strncpy(result, cache, len + 1);
  free(cache);
  return result;
}
