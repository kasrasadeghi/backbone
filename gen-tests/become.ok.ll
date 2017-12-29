; ModuleID = 'become.bb'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @i(i32 %a) {
entry:
  ret i32 %a
}

define i32 @main() {
entry:
  %$1 = call i32 (i32) @i(i32 0)
  %$0 = musttail call i32 (i32) @i(i32 %$1)
  ret i32 %$0
}
