(str-table
    (0 "../examples/hello.kl"))

(def main (hi) int
  (let filename char* (str-to-char* 0))
  (let r Reader* (call reader (filename))))