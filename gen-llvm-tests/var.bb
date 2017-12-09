(decl printf (i8* ...) i32)

(str-table
  (0 "c = %c\0A\00"))

(def main (params) i32
  (var c i8)
  (store 65 i8 c)
  (call printf (types i8* i8) i32 (args (str-get 0) (load i8 c)))
  (ret 0 i32))
