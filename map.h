#include "parser.h"
#include <stddef.h>

typedef struct Entry {
    char *key;
    Sexp *s;
} Entry;

typedef struct Map {
    Entry *entries;
    size_t size;
    size_t capacity;
} Map;

Map *map_create();
Sexp *map_get(Map *map, char *key);
void map_put(Map *map, char *key, Sexp *s);

/*
 * parse
 * ->
 * (Sexp* char*)
 */

/*
 * Sexp
 * ->
 * (char* Sexp** size size)
 */