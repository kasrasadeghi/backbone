; ModuleID = 'multiarg_call.bb'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @first(i32 %a, i32 %b) {
entry:
  ret i32 %a
}

define i32 @main(i32 %argc, i8** %argv) {
entry:
  %check = call i32 (i32, i32) @first(i32 2, i32 3)
  ret i32 %check
}