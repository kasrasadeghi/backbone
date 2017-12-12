; ModuleID = 'if_expr.bb'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str.0 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@str.1 = private unnamed_addr constant [15 x i8] c"Goodbye World!\00", align 1

declare i32 @puts(i8*)

define i32 @main(i32 %argc, i8** %argv) {
entry:
  %$0 = icmp slt i32 %argc, 3
  br i1 %$0, label %then0, label %post0
then0:
  %$1 = call i32 (i8*) @puts(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.0, i64 0, i64 0))
  br label %post0
post0:
  %$2 = icmp sge i32 %argc, 3
  br i1 %$2, label %then1, label %post1
then1:
  %$3 = call i32 (i8*) @puts(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.1, i64 0, i64 0))
  br label %post1
post1:
  ret i32 0
}
