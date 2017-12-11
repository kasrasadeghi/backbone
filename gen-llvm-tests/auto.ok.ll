; ModuleID = 'var.bb'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

declare i32 @printf(...)

@str.0 = private unnamed_addr constant [8 x i8] c"c = %c\0A\00", align 1

define i32 @main() {
entry:
  %c = alloca i8
  store i8 65, i8* %c
  %$1 = load i8, i8* %c
  %$0 = call i32 (...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.0, i64 0, i64 0), i8 %$1)
  ret i32 0
}
