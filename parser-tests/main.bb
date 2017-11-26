(str-table
  (0 "Usage: backbone parse <file>\n"))

(def main (paramlist (argc int) (argv char**)) int
  (if (< argc 3)
    (call puts (char*-from-str 0))
    (return 1))
  (= filename_ptr
