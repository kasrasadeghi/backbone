(str-table
  (0 "Basic{%d, %ld, %c}\0A\00"))

(decl printf (types i8* ...) i32)

(def main (params (argc i32) (argv i8**)) i32
  (let a (i32 1))
  (call-vargs printf (types i8* i32 i64 i8) i32 (args
    (load i32 (index t Basic 0))
    (load i64 (index t Basic 1))
    (load i8  (index t Basic 2))))
  (return 0 i32))