; ModuleID = 'slt.bb'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @signext(i1 %a) {
entry:
  br i1 %a, label %then0, label %post0
then0:
  ret i32 1
  br label %post0
post0:
  %$0 = add i1 %a, 1

  br i1 %$0, label %then1, label %post1
then1:
  ret i32 0
  br label %post1
post1:
  ret i32 20
}

define i32 @main(i32 %argc, i8** %argv) {
entry:
  %$1 = icmp slt i32 %argc, 2
  %$0 = call i32 (i1) @signext(i1 %$1)
  ret i32 %$0
}
