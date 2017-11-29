; ModuleID = 'print_int.c'
source_filename = "print_int.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare i32 @printf(i8*, ...)

define void @print_int(i32) {
  %printf_str = getelementptr inbounds [4 x i8], [4 x i8]* @.str, i64 0, i64 0
  %why_does_printf_return_int = tail call i32 (i8*, ...) @printf(i8* %printf_str, i32 %0)
  br label %1
  ret void
}

define i32 @main() {
  tail call void @print_int(i32 5)
  ret i32 0
}
