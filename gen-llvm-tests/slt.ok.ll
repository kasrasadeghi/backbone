; ModuleID = 'slt.bb'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main(i32 %argc, i8** %argv) {
entry:
  %$0 = icmp slt i32 %argc, 2
  ret i32 %$0
}
