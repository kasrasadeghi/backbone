(str-table
  (0 "Hello World!\00"))

(decl puts (types i8*) i32)

(def main (params) i32
  (if true
    (let ignore-true (call puts (types i8*) i32 (args (str-get 0)))))
  (if false
    (let ignore-false (call puts (types i8*) i32 (args (str-get 0)))))
  (return 0 i32))