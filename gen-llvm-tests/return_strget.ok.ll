; ModuleID = 'return_strget.bb'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str.0 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1

declare i32 @puts(i8*)

define i8* @some() {
entry:
  ret i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.0, i64 0, i64 0)
}

define i32 @main(i32 %argc, i8** %argv) {
entry:
  %k = call i8* () @some()
  %ignore = call i32 (i8*) @puts(i8* %k)
  ret i32 0
}
