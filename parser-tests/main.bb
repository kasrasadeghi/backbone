(str-table
  (0 "Usage: backbone parse <file>\n"))

(struct Sexp
  (value char*)
  (list Sexp**)
  (len u32)
  (cap u32))

(def main ((argc i32) (argv i8**)) i32
  (if (< i32 argc 3)
    (call puts (i8*-from-str 0))
    (return 1 i32))
  (let filename i8* (load i8* i8** (index i8* i8** argv 2)))
  (let program Sexp* (call parse Sexp* ((filename i8*))))
  (call printSexp void ((program Sexp*) (0 i32)))
  (return 0 i32))