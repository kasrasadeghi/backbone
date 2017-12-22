
#include <stdlib.h>
#include <stdio.h>
#include "str.h"
#include <ctype.h>
#include <stdint.h>
#include <libgen.h>
#include "sexp.h"
#include "parser.h"
#include "reader.h"

#include <assert.h>

//#define assert(exp) (exp)

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
    pWhitespace(r);
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
  if (peek(r) == '\'') return pChar(r);
  if (peek(r) == '"' ) return pString(r);
  return sexp(pWord(r));
}

Sexp* pSexp(Reader* r) {
  pWhitespace(r);
  //printf("%c: \n",peek(r)); // <- useful for debugging
  if (peek(r) == '(') return pList(r);
  return pAtom(r);
}

Sexp* pProgram(char* filename, Reader* r) {
  Sexp* program = sexp(basename(filename));
  while (hasNext(r)) {
    pushSexp(program, pSexp(r));
    pWhitespace(r);
  }
  return program;
}

/* assume: r is fresh */
void removeComments(Reader* r) {
  int mode = 0;
  while (hasNext(r)) {
    char c = get(r);
    if (mode == -1) {
      if (c == '\n') {
        //TODO end a comment
        mode = 0;
        continue;
      } else {
        // continue to comment out stuff
        r->file[r->offset - 1] = ' ';
        continue;
      }
    }
    if (mode == 0) {
      if (c == '\"') {
        mode = 1;
        continue;
      }
      if (c == '\'') {
        mode = 2;
        continue;
      }
      if (c == ';') {
        mode = -1;
        //TODO start a comment
        r->file[r->offset - 1] = ' ';
        continue;
      }
    } else if (mode == 1 && c == '\"') {
      mode = 0;
      continue;
    } else if (mode == 2 && c == '\'') {
      mode = 0;
      continue;
    }
  }

  reset(r);
}

Sexp* parse(char* filename) {
  Reader* r = reader(filename);
  if (r == NULL) {
    fprintf(stderr, "backbone: error reading file \"%s\": ", filename);
    perror("");
    exit(EXIT_FAILURE);
  }
  removeComments(r);
  return pProgram(filename, r);
}

//endregion
