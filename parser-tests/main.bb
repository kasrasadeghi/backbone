(str-table
  (0 "Usage: backbone parse <file>\n"))

(struct Sexp
  (value char*)
  (list Sexp**)
  (len uint)
  (cap uint))

(def main ((argc int) (argv char**)) int
  (if (< argc 3)
    (call puts (char*-from-str 0))
    (return 1))
  (let filename char* (load char** (index (argv char**) 2)))
  (let program Sexp* (call parse (filename))))