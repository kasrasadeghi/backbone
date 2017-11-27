

typedef struct Sexp {
  char* value;
  struct Sexp** list;
  size_t length;
  size_t cap;
} Sexp;

void printSexp(Sexp*, int l);

void destroySexp(Sexp*);
Sexp* parse(char* filename);