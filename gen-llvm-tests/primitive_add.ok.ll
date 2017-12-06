; ModuleID = 'primitive_add.bb'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main(i32 %argc, i8** %argv) {
entry:
  %c = add i32 2, 3

  ret i32 %c
}
