; ModuleID = 'cscccc.bb'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @first(i32 %a, i32 %b) {
entry:
  ret i32 %a
}

define i32 @i(i32 %a) {
entry:
  ret i32 %a
}

define i32 @main() {
entry:
  %$3 = call i32 (i32) @i(i32 4)
  %$2 = call i32 (i32) @i(i32 %$3)
  %$1 = call i32 (i32) @i(i32 %$2)
  %$0 = call i32 (i32) @i(i32 %$1)
  %$7 = call i32 (i32) @i(i32 7)
  %$6 = call i32 (i32) @i(i32 %$7)
  %$5 = call i32 (i32) @i(i32 %$6)
  %$4 = call i32 (i32) @i(i32 %$5)
  %ignore = call i32 (i32, i32) @first(i32 %$0, i32 %$4)
  ret i32 0
}
