(def main (params) i32
  (if (call i (types i1) i1 (args true))
    (let ignore (call puts (types i8*) i32 (args (str-get 0)))))
  (return 0 i32))