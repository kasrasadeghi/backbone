; ModuleID = 'struct.c'
source_filename = "struct.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.Basic = type { i32, i64, i8 }

@.str = private unnamed_addr constant [20 x i8] c"Basic{%d, %ld, %c}\0A\00", align 1

define noalias %struct.Basic* @makeBasic(i32 %a, i64 %b, i8 %c) {
  ; Basic* r = calloc(1, sizeof(Basic))
  %_r = call i8* @calloc(i64 1, i64 24)
  %r = bitcast i8* %_r to %struct.Basic*

  ; r->a = a
  %ra = getelementptr inbounds %struct.Basic, %struct.Basic* %r, i32 0, i32 0
  store i32 %a, i32* %ra, align 8

  ; r->b = b
  %rb = getelementptr inbounds %struct.Basic, %struct.Basic* %r, i32 0, i32 1
  store i64 %b, i64* %rb, align 8

  ; r->c = c
  %rc = getelementptr inbounds %struct.Basic, %struct.Basic* %r, i32 0, i32 2
  store i8 %c, i8* %rc, align 8

  ; return r
  ret %struct.Basic* %r
}

declare noalias i8* @calloc(i64, i64)

define i32 @main() {
  %1 = call %struct.Basic* @makeBasic(i32 1, i64 2, i8 99)
  %2 = getelementptr inbounds %struct.Basic, %struct.Basic* %1, i64 0, i32 0
  %3 = load i32, i32* %2, align 8
  %4 = getelementptr inbounds %struct.Basic, %struct.Basic* %1, i64 0, i32 1
  %5 = load i64, i64* %4, align 8
  %6 = getelementptr inbounds %struct.Basic, %struct.Basic* %1, i64 0, i32 2
  %7 = load i8, i8* %6, align 8
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str,
  i64 0, i64 0), i32 %3, i64 %5, i8 %7)
  ret i32 0
}

declare i32 @printf(i8*, ...)



