; ModuleID = 'struct.c'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [11 x i8] c"value: %d\0A\00"

define void @make0(i32* %i) {
entry:
  %val = load i32, i32* %i
  %cond = icmp ne i32 %val, 0
  br i1 %cond, label %loop, label %end

loop:
  %inc = add i32 %val, 1
  store i32 %inc, i32* %i
  br label %entry

end:
  ret void
}

define void @alsoMake0(i32* %i) {
entry:
  %val = load i32, i32* %i
  %cond = icmp eq i32 %val, 0
  br i1 %cond, label %return, label %recurse

recurse:
  %value = add i32 %val, 1
  store i32 %value, i32* %i
  musttail call void (i32*) @alsoMake0(i32* %i)
  ret void

return:
  ret void
}

define i32 @main() {
entry:
  %i = alloca i32
  store i32 1, i32* %i
  call void (i32*) @alsoMake0(i32* %i)
  %val = load i32, i32* %i
  %ignore = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str, i32 0, i32 0), i32 %val)
  ret i32 0
}

declare i32 @printf(i8*, ...)
