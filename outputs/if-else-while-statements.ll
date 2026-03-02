; ModuleID = 'outputs/if-else-while-statements.ll'
source_filename = "outputs/if-else-while-statements.ll"

%IfElseWhile = type { i32 }

@IfElseWhile.global = global %IfElseWhile { i32 -1 }
@0 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@1 = private unnamed_addr constant [4 x i8] c"x=?\00", align 1
@2 = private unnamed_addr constant [4 x i8] c">10\00", align 1
@3 = private unnamed_addr constant [3 x i8] c">5\00", align 1
@4 = private unnamed_addr constant [4 x i8] c"<=5\00", align 1
@5 = private unnamed_addr constant [3 x i8] c"y=\00", align 1
@printf.str = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@scanf.int = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@printf.str.2 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@printf.str.3 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@printf.str.4 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@printf.str.5 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@printf.num = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare i32 @printf(ptr, ...)

define i32 @main() {
entry:
  %x = alloca i32, align 4
  %y = load i32, ptr %x, align 4
  %y1 = alloca i32, align 4
  %y2 = load i32, ptr %y1, align 4
  %print = call i32 (ptr, ...) @printf(ptr @printf.str, ptr @1)
  %readln = call i32 (ptr, ...) @scanf(ptr @scanf.int, ptr %x)
  %x3 = load i32, ptr %x, align 4
  %gt = icmp sgt i32 %x3, 10
  %cmp_ext = zext i1 %gt to i32
  %ifcond = icmp ne i32 %cmp_ext, 0
  br i1 %ifcond, label %then, label %else

ifcont:                                           ; preds = %else10, %then9
  store i32 1, ptr %x, align 4
  store i32 0, ptr %y1, align 4
  %print13 = call i32 (ptr, ...) @printf(ptr @printf.str.5, ptr @5)
  %y14 = load i32, ptr %y1, align 4
  %print15 = call i32 (ptr, ...) @printf(ptr @printf.num, i32 %y14)
  ret i32 0

elseif:                                           ; preds = %else
  %x5 = load i32, ptr %x, align 4
  %gt6 = icmp sgt i32 %x5, 5
  %cmp_ext7 = zext i1 %gt6 to i32
  %ifcond8 = icmp ne i32 %cmp_ext7, 0
  br i1 %ifcond8, label %then9, label %else10

then:                                             ; preds = %entry
  %print4 = call i32 (ptr, ...) @printf(ptr @printf.str.2, ptr @2)
  ret i32 0

else:                                             ; preds = %entry
  br label %elseif

then9:                                            ; preds = %elseif
  %print11 = call i32 (ptr, ...) @printf(ptr @printf.str.3, ptr @3)
  br label %ifcont

else10:                                           ; preds = %elseif
  %print12 = call i32 (ptr, ...) @printf(ptr @printf.str.4, ptr @4)
  br label %ifcont
}

declare i32 @main.1()

declare i32 @scanf(ptr, ...)
