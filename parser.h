

typedef struct Sexp Sexp;
void printSexp(Sexp*, int l);

void destroySexp(Sexp*);
Sexp* parse(char* filename);