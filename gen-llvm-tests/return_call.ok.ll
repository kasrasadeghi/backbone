; ModuleID = 'return_call.bb'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @iden(i32 %i) {
entry:
  ret i32 %i
}

define i32 @main(i32 %argc, i8** %argv) {
entry:
  %$0 = call i32 (i32) @iden(i32 %argc)
  ret i32 %$0
}
