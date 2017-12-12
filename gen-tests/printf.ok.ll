; ModuleID = 'printf.bb'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str.0 = private unnamed_addr constant [20 x i8] c"Basic{%d, %ld, %c}\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main(i32 %argc, i8** %argv) {
entry:
  %$0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @str.0, i64 0, i64 0), i32 1, i64 2, i8 99)
  ret i32 0
}
