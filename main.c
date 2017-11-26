#include <stdlib.h>
#include <stdio.h>
#include "str.h"
#include <ctype.h>
#include <stdint.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <assert.h>

//#define assert(exp) (exp)

//region struct Reader {...}

typedef struct Reader {
  char* file;
  size_t size;
  size_t offset;
} Reader;

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
  r->file = (char*) mmap(NULL, r->size, PROT_READ, MAP_FILE|MAP_PRIVATE, fd, r->offset);
  return r;
}

int hasNext(Reader* r) {
  return r->size > r->offset;
}

char get(Reader* r) {
  assert(hasNext(r));
  return r->file[r->offset++];
}

char peek(Reader* r) {
  return r->file[r->offset];
}

char prev(Reader* r) {
  assert(r->offset != 0);
  return r->file[r->offset - 1];
}

//endregion Reader

typedef struct Sexp {
  char* value;
  struct Sexp** list;
  size_t length;
  size_t cap;
} Sexp;

Sexp* sexp(char* value) {
  Sexp* result = calloc(1, sizeof(Sexp));
  result->value = value;
  result->list = calloc(2, sizeof(Sexp*));
  result->length = 0;
  result->cap = 2;
  return result;
};

void printSexp(Sexp* s, int l) {
  for (int i = 0; i < l; ++i) {
    printf("  ");
  }
  printf("%s\n", s->value);
  for (int i = 0; i < s->length; ++i) {
    printSexp(s->list[i], l + 1);
  }
}

void pushSexp(Sexp* s, Sexp* child) {
  s->list[s->length] = child;
  ++s->length;
  if (s->length == s->cap) {
    s->cap *= 2;
    s->list = realloc(s->list, s->cap * sizeof(Sexp*));
  }
}

void pWhitespace(Reader* r) {
  while (hasNext(r) && isspace(peek(r))) {
    get(r);
  }
}

char* pWord(Reader* r) {
  String str = str_init();
  while (peek(r) != '(' && peek(r) != ')' && !isspace(peek(r))) {
    str_push(&str, get(r));
  }
  return str.list;
}

Sexp* pSexp(Reader*);

Sexp* pList(Reader* r) {
  char test = get(r);
  assert (test == '(');

  Sexp* curr = sexp(pWord(r));
  while (peek(r) != ')') {
    pushSexp(curr, pSexp(r));
  }

  test = get(r);
  assert (test == ')');
  return curr;
}

Sexp* pChar(Reader* r) {
  String str = str_init();
  assert (peek(r) == '\'');
  str_push(&str, get(r));
  // while we've not (ended the char with an unescaped apostrophe)
  while (!(peek(r) == '\'' && prev(r) != '\\')) {
    str_push(&str, get(r));
  }
  assert (peek(r) == '\'');
  str_push(&str, get(r));
  return sexp(str.list);
}

Sexp* pString(Reader* r) {
  String str = str_init();
  assert (peek(r) == '\"');
  str_push(&str, get(r));
  // while we've not (ended the string with an unescaped quote)
  while (!(peek(r) == '\"' && prev(r) != '\\')) {
    str_push(&str, get(r));
  }
  assert (peek(r) == '\"');
  str_push(&str, get(r));
  return sexp(str.list);
}

Sexp* pAtom(Reader* r) {
  return peek(r) == '\'' ? pChar(r) :
         peek(r) == '"' ? pString(r) :
         sexp(pWord(r));
}

Sexp* pSexp(Reader* r) {
  pWhitespace(r);
  printf("%c: \n",peek(r));
  return peek(r) == '('? pList(r) : pAtom(r);
}

Sexp* pProgram(Reader* r) {
  Sexp* program = sexp("programName");
  while (hasNext(r)) {
    pushSexp(program, pSexp(r));
    pWhitespace(r);
  }
  return program;
}

void destroySexp(Sexp* s) {
  //TODO actually finish
  free(s);
}

int main(int argc, char *argv[]) {
  if (argc == 1) {
    printf("Usage: %s <file>\n", argv[0]);
    return 1;
  }
  char* filename = argv[1];
  Reader* r = reader(filename);
  if (r == NULL) {
    fprintf(stderr, "backbone: error making reader for file: %s\n", filename);
    return EXIT_FAILURE;
  }
  Sexp* program = pProgram(r);
  printSexp(program, 0);
  destroySexp(program);
}
