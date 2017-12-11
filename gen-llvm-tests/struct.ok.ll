; ModuleID = 'struct.bb'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.Basic = type { i32 }

@str.0 = private unnamed_addr constant [15 x i8] c"Basic{a = %d}\0A\00", align 1

declare i8* @calloc(i64, i64)

define %struct.Basic* @makeBasic(i32 %a) {
entry:
  %$0 = call i8* (i64, i64) @calloc(i64 1, i64 8)
  %r = bitcast i8* %$0 to %struct.Basic*
  %$1 = getelementptr inbounds %struct.Basic, %struct.Basic* %r, i32 0, i32 0
  store i32 %a, i32* %$1
  ret %struct.Basic* %r
}

declare i32 @printf(i8*, ...)

define i32 @main(i32 %argc, i8** %argv) {
entry:
  %t = call %struct.Basic* (i32) @makeBasic(i32 7)
  %$2 = getelementptr inbounds %struct.Basic, %struct.Basic* %t, i32 0, i32 0
  %$1 = load i32, i32* %$2
  %$0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.0, i64 0, i64 0), i32 %$1)
  ret i32 0
}
