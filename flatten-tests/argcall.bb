(def call (params (argc i32)) i32
  (return argc i32))

(def main (params (argc i32) (argv i8**)) i32
  (let check (call call (types i32) i32 (args (argc))))
  (return check i32))