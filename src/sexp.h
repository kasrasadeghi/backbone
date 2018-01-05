#ifndef __SEXP_H__
#define __SEXP_H__

#include <stdlib.h>

typedef struct Sexp {
  char* value;
  struct Sexp** list;
  size_t length;
  size_t cap;
} Sexp;

Sexp* sexp(char* value);
Sexp* makeSexp(char* value, size_t length);
Sexp* copySexp(Sexp* s);
void destroySexp(Sexp* s);

size_t indexOfSexp(Sexp* parent, Sexp* child);
void insertSexp(Sexp* parent, Sexp* stmt, size_t csi);
void incrementLength(Sexp* s);

void printSexp(Sexp*);
void pushSexp(Sexp* s, Sexp* child);

//region grammar

int isTopLevel (Sexp* s);
int isStrTable (Sexp* s);
int isStruct   (Sexp* s);
int isDecl     (Sexp* s);
int isDef      (Sexp* s);

int isStmt     (Sexp* s);
int isCallLike (Sexp* s);
int isLet      (Sexp* s);
int isReturn   (Sexp* s);
int isIf       (Sexp* s);
int isStore    (Sexp* s);
int isAuto     (Sexp* s);

int isCall     (Sexp* s);
int isCallTail (Sexp* s);
int isCallVargs(Sexp* s);
int isBecome   (Sexp* s);

int isExpr     (Sexp* s);
int isLoad     (Sexp* s);
int isIndex    (Sexp* s);
int isCast     (Sexp* s);

int isValue    (Sexp* s);
int isStrGet   (Sexp* s);
int isLiteral  (Sexp* s);

int isMathBinop(Sexp* s);
int isAdd      (Sexp* s);

int isIcmp     (Sexp* s);

int isBool     (Sexp* s);

//endregion

#endif