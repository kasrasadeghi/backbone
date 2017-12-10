; ModuleID = 'var.c'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %_c = alloca i8, align 1
  store i8 65, i8* %_c, align 1
  %c = load i8, i8* %_c
  %3 = sext i8 %c to i32
  %$0 = call i32 (i32) @putchar(i32 %c)
  store i8 66, i8* %1, align 1
  %$1 = call i32 (i8) @putchar(i32 %5)
  ret i32 0
}

declare i32 @putchar(i32)



