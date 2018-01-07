(def iden (params (i i32)) i32
  (return i i32))

(def main (params (argc i32) (argv i8**)) i32
  (return (call iden (types i32) i32 (args (argc))) i32))