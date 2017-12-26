### Sexp Structure and Invariants

The Sexp's structure is the following:

struct Sexp {
  char* value;
  struct Sexp** list;
  size_t length;
  size_t cap;
}

### List Structure

The list is the same kind of list used in `str.h`. It is a contiguous block of memory
filled with `Sexp*`s.

### Double-Free Safety

In order for `destroySexp()` to work, I assume that all values and Sexp's 
are unique. This is to avoid double-frees. In essence, we are assuming that
all elements of the tree are `unique_ptr`s.

This assumption leads to the following repercussions:
 - you cannot assign a constant string to a Sexp without copying it into memory first
 - you cannot create a string and then have two Sexp's point to it, for it must be unique 
   - copy the string into a new char*, and then assign the second Sexp's value to that