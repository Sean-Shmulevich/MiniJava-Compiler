; ModuleID = 'outputs/array-1d.ll'
source_filename = "outputs/array-1d.ll"

%Array1D = type { i32 }

@Array1D.global = global %Array1D { i32 -1 }
@0 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

declare i32 @printf(ptr, ...)

define i32 @main() {
entry:
  %arrLocal = alloca i32, align 4
  %y = load i32, ptr %arrLocal, align 4
  %i = alloca i32, align 4
  %y1 = load i32, ptr %i, align 4
  store i32 0, ptr %i, align 4
  store i32 0, ptr %i, align 4
  ret i32 0
}

declare i32 @main.1()
