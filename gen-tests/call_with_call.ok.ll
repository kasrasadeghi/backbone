; ModuleID = 'call_with_call.bb'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @iden(i32 %c) {
entry:
  ret i32 %c
}

define i32 @main(i32 %argc, i8** %argv) {
entry:
  %$0 = call i32 (i32) @iden(i32 %argc)
  %check = call i32 (i32) @iden(i32 %$0)
  ret i32 %check
}
