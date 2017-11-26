#include "map.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#define DEFAULT_SIZE 4
#define SCALE 3/2

Map *map_create() {
    Map *map = malloc(sizeof(Map));
    map->capacity = DEFAULT_SIZE;
    map->size = 0;
    map->entries = (Entry*) malloc(map->capacity * sizeof(Entry));
    return map;
}

Sexp *map_get(Map *map, char *key) {
    assert(key);
    for (int i=0; i<map->size; i++)
        if (strcmp(map->entries[i].key, key) == 0)
            return map->entries[i].s;
    return NULL;
}

void map_put(Map *map, char *key, Sexp *s) {
    if (map->size == map->capacity) {
        map->capacity = map->capacity * SCALE;
        map->entries = (Entry*) realloc(map->entries, map->capacity * sizeof(Entry));
    }
    map->entries[map->size] = (Entry) {key, s};
    map->size++;
}
