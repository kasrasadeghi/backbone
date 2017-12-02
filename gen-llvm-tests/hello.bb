(str-table
  (0 "Hello World!\00"))

(decl puts (types i8*) i32)

(def main () i32
  (let ignore (call puts (types i8*) i32 (str-get 0)))
  (return 0 i32))