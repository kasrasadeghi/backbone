; ModuleID = 'call_with_args.bb'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @call(i32 %argc) {
entry:
  ret i32 %argc
}

define i32 @main(i32 %argc, i8** %argv) {
entry:
  %check = call i32 (i32) @call(i32 %argc)
  ret i32 %check
}