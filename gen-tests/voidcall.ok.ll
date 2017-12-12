; ModuleID = 'voidcall.bb'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str.0 = private unnamed_addr constant [12 x i8] c"Hello World\00", align 1

declare i32 @puts(i8*)

define void @voidcall() {
entry:
  %$0 = call i32 (i8*) @puts(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.0, i64 0, i64 0))
  ret void
}

define i32 @main() {
entry:
  call void () @voidcall()
  ret i32 0
}
