(str-table
  (0 "Usage: backbone parse <file>\n"))

(def main (paramlist (argc int) (argv char**)) int
  (if (< argc 3)
    (call (puts int) (arglist ((char*-from-str 0) char*)))
    (return 1))
  (let filename (load char** (index (argv char**) 2)))
  (let program (call parse Sexp* (arglist (filename char*)))))