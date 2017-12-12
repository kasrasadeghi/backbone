(struct Basic
  (a i32))

(str-table
  (0 "Basic{a = %d}\0A\00"))

(decl calloc (types i64 i64) i8*)

(def makeBasic (params (a i32)) Basic*
  (let r (cast i8* Basic* (call calloc (types i64 i64) i8* (args 1 8))))
  (store a i32 (index r Basic 0))
  (return r Basic*))

(decl printf (types i8* ...) i32)

(def main (params (argc i32) (argv i8**)) i32
  (let t (call makeBasic (types i32) Basic* (args 7)))
  (call-vargs printf (types i8* i32) i32 (args
    (str-get 0)
    (load i32 (index t Basic 0))))
  (return 0 i32))