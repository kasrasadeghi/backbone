(def iden (params (c i32)) i32
  (return c i32))

(def main (params (argc i32) (argv i8**)) i32
  (let check
    (call iden (types i32) i32 (args
      (call iden (types i32) i32 (args (argc))))))
  (return check i32))