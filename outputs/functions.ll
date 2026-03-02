; ModuleID = 'outputs/functions.ll'
source_filename = "outputs/functions.ll"

%Functions = type { i32 }

@Functions.global = global %Functions { i32 -1 }
@0 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@printf.num = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare i32 @printf(ptr, ...)

define i32 @func() {
entry:
  ret i32 55
}

declare i32 @func.1()

define i32 @main() {
entry:
  %ELLLO = call i32 @func()
  %ELLLO1 = call i32 @func()
  %add_result = add i32 %ELLLO, %ELLLO1
  %print = call i32 (ptr, ...) @printf(ptr @printf.num, i32 %add_result)
  ret i32 0
}

declare i32 @main.2()
