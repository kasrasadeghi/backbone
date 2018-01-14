(str-table
  (0 "Hello World\00"))

(decl puts (types i8*) i32)

(def voidcall (params) void
  (call puts (types i8*) i32 (args (str-get 0)))
  (return void))

(def main (params) i32
  (call voidcall (types) void (args))
  (return 0 i32))