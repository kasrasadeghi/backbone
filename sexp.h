#ifndef __SEXP_H__
#define __SEXP_H__

typedef struct Sexp {
  char* value;
  struct Sexp** list;
  size_t length;
  size_t cap;
} Sexp;

Sexp* sexp(char* value);

void printSexp(Sexp*, int l);
void destroySexp(Sexp*);
void pushSexp(Sexp* s, Sexp* child);

int isCall(Sexp* s);
int isCallVargs(Sexp* s);
int isAdd(Sexp* s);
int isIcmp(Sexp* s);
int isDef(Sexp* s);
int isDecl(Sexp* s);
int isLoad(Sexp* s);

#endif