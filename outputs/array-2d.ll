; ModuleID = 'outputs/array-2d.ll'
source_filename = "outputs/array-2d.ll"

%Array2D = type { i32 }

@Array2D.global = global %Array2D { i32 -1 }
@0 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

declare i32 @printf(ptr, ...)

define i32 @main() {
entry:
  %j = alloca i32, align 4
  %y = load i32, ptr %j, align 4
  %i = alloca i32, align 4
  %y1 = load i32, ptr %i, align 4
  ret i32 0
}

declare i32 @main.1()
