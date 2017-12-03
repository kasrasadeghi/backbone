(def iden (params (i i32))
  (return i i32))

(def main (params (argc i32) (argv i8**)) i32
  (return (call (types i32) i32 (args (argc)))))