(def i (params (a i32)) i32
  (return a i32))

(def main (params) i32
  (become i (types i32) i32 (args (call i (types i32) i32 (args 0)))))