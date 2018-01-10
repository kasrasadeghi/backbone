(def signext (params (a i1)) i32
  (if a
    (return 1 i32))

  ; this if statement is an else statement because
  (if (+ i1 a 1)  ; this is a binary not
    (return 0 i32))
  (return 20 i32))

(def main (params (argc i32) (argv i8**)) i32
  (return
    (call signext (types i1) i32 (args (< i32 argc 2)))
    i32))
