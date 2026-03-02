; ModuleID = 'outputs/parameters.ll'
source_filename = "outputs/parameters.ll"

%Parameters = type { i32 }

@Parameters.global = global %Parameters { i32 -1 }
@0 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@printf.num = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@printf.num.3 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare i32 @printf(ptr, ...)

define i32 @product() {
entry:
  ret void
}

declare i32 @product.1()

define i32 @main() {
entry:
  %p = alloca i32, align 4
  %y = load i32, ptr %p, align 4
  %x = alloca i32, align 4
  %y1 = load i32, ptr %x, align 4
  store i32 0, ptr %p, align 4
  %p2 = load i32, ptr %p, align 4
  %ELLLO = call i32 @product(i32 %p2, i32 3, i32 4)
  store i32 %ELLLO, ptr %x, align 4
  %p3 = load i32, ptr %p, align 4
  %print = call i32 (ptr, ...) @printf(ptr @printf.num, i32 %p3)
  %x4 = load i32, ptr %x, align 4
  %print5 = call i32 (ptr, ...) @printf(ptr @printf.num.3, i32 %x4)
  ret i32 0
}

declare i32 @main.2()
