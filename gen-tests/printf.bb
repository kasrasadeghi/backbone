(str-table
  (0 "Basic{%d, %ld, %c}\0A\00"))

(decl printf (types i8* ...) i32)

(def main (params (argc i32) (argv i8**)) i32
  (call-vargs printf (types i8* i32 i64 i8) i32 (args (str-get 0) 1 2 99))
  (return 0 i32))