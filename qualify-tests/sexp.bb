(struct Sexp
  (value i8*)
  (list Sexp**)
  (len u64)
  (cap u64))

(str-table
  (0 "Sexp{value: %s, len: %lu}\0A\00")
  (1 "Example Sexp"))

(decl malloc (types i64) i8*)
(decl calloc (types i64 i64) i8*)

(def makeSexp (params (value i8*) (length i64)) Sexp*
  (let r (cast i8* Sexp* (call malloc (types i64) i8* (args 32))))
  (store a i8* (index r Sexp 0))

  (if (= u64 length 0)
    (store 0 Sexp** (index r Sexp 1)))

  (if (!= u64 length 0)
    (store
      (cast i8* Sexp** (call calloc (types i64 i64) i8* (args length 32)))
      Sexp**
      (index r Sexp 1)))

  (store length u64 (index r Sexp 2))
  (store length u64 (index r Sexp 3))

  (return r Sexp*))

(decl printf (types i8* ...) i32)
(decl free (types i8*) void)

(def main (params (argc i32) (argv i8**)) i32
  (let s (call makeSexp (types i8* i64) Sexp* (args (str-get 1) 0)))
  (call-vargs printf (types i8* i8* u64) i32 (args
    (str-get 0)
    (load i8* (index s Sexp 0))
    (load u64 (index s Sexp 2))
  ))
  (call free (types i8*) void (args (cast Sexp* i8* s)))
  (return 0 i32))