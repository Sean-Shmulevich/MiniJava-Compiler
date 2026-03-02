; ModuleID = 'outputs/helloworld.ll'
source_filename = "outputs/helloworld.ll"

%Helloworld = type { i32 }

@Helloworld.global = global %Helloworld { i32 -1 }
@0 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@1 = private unnamed_addr constant [16 x i8] c"Hello World !!!\00", align 1
@printf.str = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

declare i32 @printf(ptr, ...)

define i32 @main() {
entry:
  %print = call i32 (ptr, ...) @printf(ptr @printf.str, ptr @1)
  ret i32 0
}

declare i32 @main.1()
