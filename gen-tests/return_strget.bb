(str-table
  (0 "Hello World!\00"))

(decl puts (types i8*) i32)

(def some (params) i8*
  (return (str-get 0) i8*))

(def main (params (argc i32) (argv i8**)) i32
  (let k (call some (types) i8* (args)))
  (let ignore (call puts (types i8*) i32 (args k)))
  (return 0 i32))
