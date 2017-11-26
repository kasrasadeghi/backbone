#include "map.h"
#include <stdlib.h>
#include <stdio.h>

#define N 255

char *make_unique(char i) {
    char *out = malloc(2);
    out[0] = i;
    out[1] = 0;
    return out;
}

int main() {
    printf("Conducting map test suite...\n");

    Map *some = map_create();
    map_put(some, "hello", (Sexp*) 12345);
    int ret = (int) map_get(some, "hello");
    if (ret != 12345) {
        printf("Expected 12345, got %d on simple test.\n", ret);
        return 1;
    }

    Map **maps = malloc(N * sizeof(Map*));
    for (int i=0; i<N; i++)
        maps[i] = map_create();
    for (int i=0; i<N; i++) for (int j=0; j<N; j++) {
        map_put(maps[i], make_unique(j+1), (Sexp*) (5 * i + 7 * j));
    }
    for (int i=0; i<N; i++) for (int j=0; j<N; j++) {
        Sexp *value = map_get(maps[i], make_unique(j+1));
        if ((int) value != 5 * i + 7 * j) {
            printf("Invalid entry at (%d, %d) value %d, expected %d.\n", 
                   i, j, (int) value, 5 * i + 7 * j);
            return 1;
        }
    }

    printf("Passed.\n");
    return 0;
}
