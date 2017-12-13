(str-table
  (0 "voidcall a:%d"))

(def i (params (c i32)) i32
  (return c i32))

(decl printf (types i8* ...) i32)

(def v (params (a i32)) void
  (call printf (types i8* i32) i32 (args (str-get 0) a))
  (return void))

(def main (params (argc i32) (argv i8**)) i32
  (call v (types i32) void (args (call i (types i32) i32 (args 5))))
  (return 0 i32))