(str-table
  (0 "Hello World!\00")
  (1 "Goodbye World!\00"))

(decl puts (types i8*) i32)

(def main (params (argc i32) (argv i8**)) i32
  (if (< i32 argc 3)
     (call puts (types i8*) i32 (args (str-get 0))))
  (if (>= i32 argc 3)
     (call puts (types i8*) i32 (args (str-get 1))))
  (return 0 i32))