
#ifndef __READER_H__
#define __READER_H__

#include <stdlib.h>

typedef struct Reader {
  char* file;
  size_t size;
  size_t offset;
} Reader;

Reader* reader(char* filename);
int hasNext(Reader* r);
char get(Reader* r);
char peek(Reader* r);
char prev(Reader* r);
void reset(Reader* r);

#endif