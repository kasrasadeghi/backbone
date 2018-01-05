; ModuleID = 'if.bb'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str.0 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1

declare i32 @puts(i8*)

define i32 @main() {
entry:
  br i1 true, label %then0, label %post0
then0:
  %ignore = call i32 (i8*) @puts(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.0, i64 0, i64 0))
  br label %post0
post0:
  ret i32 0
}
