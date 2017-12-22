#include "reader.h"
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <assert.h>

size_t fileSize(char* filename) {
  struct stat st;
  stat(filename, &st);
  if (st.st_size < 0) {
    fprintf(stderr, "file has negative size???");
    exit(2);
  }
  return (size_t)st.st_size;
}

Reader* reader(char* filename) {
  int fd = open(filename, O_RDONLY, 0);
  if (fd == -1) {
    return NULL;
  }
  Reader* r = calloc(1, sizeof(Reader));
  r->offset = 0;
  r->size = fileSize(filename);
  r->file = (char*) mmap(NULL, r->size, PROT_READ|PROT_WRITE, MAP_FILE|MAP_PRIVATE, fd, r->offset);
  return r;
}

int hasNext(Reader* r) {
  return r->size > r->offset;
}

char get(Reader* r) {
  assert (hasNext(r));
  return r->file[r->offset++];
}

char peek(Reader* r) {
  return r->file[r->offset];
}

char prev(Reader* r) {
  assert (r->offset != 0);
  return r->file[r->offset - 1];
}

void reset(Reader* r) {
  r->offset = 0;
}