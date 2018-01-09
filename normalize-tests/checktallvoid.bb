(def v (params (a i32)) void
  (call printf (types i8* i32) i32 (args (str-get 0) a))
  (return void))