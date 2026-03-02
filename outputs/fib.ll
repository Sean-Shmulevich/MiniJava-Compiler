; ModuleID = 'outputs/fib.ll'
source_filename = "outputs/fib.ll"

%Fib = type { i32 }

@Fib.global = global %Fib { i32 -1 }
@0 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

declare i32 @printf(ptr, ...)

define i32 @fib() {
entry:
  %ret = alloca i32, align 4
  %y = load i32, ptr %ret, align 4
  store i32 4, ptr %ret, align 4
  ret void <badref>
}

declare i32 @fib.1()

define i32 @main() {
entry:
  %i = alloca i32, align 4
  %y = load i32, ptr %i, align 4
  ret i32 0
}

declare i32 @main.2()
