; ModuleID = 'tallvoid.bb'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str.0 = private unnamed_addr constant [13 x i8] c"voidcall a:%d", align 1

define i32 @i(i32 %c) {
entry:
  ret i32 %c
}

declare i32 @printf(i8*, ...)

define void @v(i32 %a) {
entry:
  %$0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.0, i64 0, i64 0), i32 %a)
  ret void
}

define i32 @main(i32 %argc, i8** %argv) {
entry:
  %$0 = call i32 (i32) @i(i32 5)
  call void (i32) @v(i32 %$0)
  ret i32 0
}
