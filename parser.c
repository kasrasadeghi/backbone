
#include <stdlib.h>
#include <stdio.h>
#include "str.h"
#include <ctype.h>
#include <stdint.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <libgen.h>
#include "parser.h"

//#include <assert.h>

#define assert(exp) (exp)

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

//endregion Reader

//region struct Sexp {...}

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

//endregion

//region Sexp* parse(char* filename)

void pWhitespace(Reader* r) {
  while (hasNext(r) && isspace(peek(r))) {
    get(r);
  }
}

char* pWord(Reader* r) {
  String str = str_init();
  pWhitespace(r);
  if (!hasNext(r)) {
    fprintf(stderr, "backbone: list does not start with word.\n");
    exit(EXIT_FAILURE);
  }
  while (hasNext(r) && peek(r) != '(' && peek(r) != ')' && !isspace(peek(r))) {
    str_push(&str, get(r));
  }
  return str.list;
}

Sexp* pSexp(Reader*);

Sexp* pList(Reader* r) {
  assert (get(r) == '(');

  Sexp* curr = sexp(pWord(r));
  while (peek(r) != ')') {
    if (!hasNext(r)) {
      fprintf(stderr, "backbone: unmatched paren for list\n"); //TODO ...on line \d
      exit(EXIT_FAILURE);
    }
    pushSexp(curr, pSexp(r));
  }

  assert (get(r) == ')');
  return curr;
}

Sexp* pChar(Reader* r) {
  String str = str_init();
  assert (peek(r) == '\'');
  str_push(&str, get(r));
  // while we've not (ended the char with an unescaped apostrophe)
  while (!(peek(r) == '\'' && prev(r) != '\\')) {
    if (!hasNext(r)) {
      fprintf(stderr, "backbone: unmatched apostrophe for char\n"); //TODO ...on line \d
      exit(EXIT_FAILURE);
    }
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
    if (!hasNext(r)) {
      fprintf(stderr, "backbone: unmatched quote for string\n"); //TODO ...on line \d
      exit(EXIT_FAILURE);
    }
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
  //printf("%c: \n",peek(r)); // <- useful for debugging
  return peek(r) == '('? pList(r) : pAtom(r);
}

Sexp* pProgram(char* filename, Reader* r) {
  Sexp* program = sexp(basename(filename));
  while (hasNext(r)) {
    pushSexp(program, pSexp(r));
    pWhitespace(r);
  }
  return program;
}

Sexp* parse(char* filename) {
  Reader* r = reader(filename);
  if (r == NULL) {
    fprintf(stderr, "backbone: error reading file \"%s\": ", filename);
    perror("");
    exit(EXIT_FAILURE);
  }
  return pProgram(filename, r);
}

//endregion

void destroySexp(Sexp* s) {
  //TODO actually finish
  free(s);
}