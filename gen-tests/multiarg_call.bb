(def first (params (a i32) (b i32)) i32
  (return a i32))

(def main (params (argc i32) (argv i8**)) i32
  (let check (call first (types i32 i32) i32 (args 2 3)))
  (return check i32))