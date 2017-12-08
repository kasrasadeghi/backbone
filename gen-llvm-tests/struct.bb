(struct Basic
  (a i32)
  (b i64)
  (c i8))

(str-table
  (0 "Basic{%d, %ld, %c}\0A\00"))

(decl calloc (types i64 i64) i8*)

(def makeBasic (params (a i32) (b i64) (c i8)) Basic*
  (let r (cast i8* Basic* (call calloc (types i64 i64) i8* (args 1 24))))
  (store a i32 (index r Basic 0))
  (store b i64 (index r Basic 1))
  (store c i8  (index r Basic 2))
  (return r Basic*))

(decl printf (types i8* ...) i32)

(def main (params (argc i32) (argv i8**)) i32
  (let t (call makeBasic (types i32 i64 i8) Basic* (args 1 2 99)))
  (call-vargs printf (types i8* i32 i64 i8) i32 (args
    (load i32 (index t Basic 0))
    (load i64 (index t Basic 1))
    (load i8  (index t Basic 2))))
  (return 0 i32))