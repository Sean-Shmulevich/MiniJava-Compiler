; ModuleID = 'outputs/if-else.ll'
source_filename = "outputs/if-else.ll"

%IfElse = type { i32 }

@IfElse.global = global %IfElse { i32 -1 }
@0 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@1 = private unnamed_addr constant [3 x i8] c"i=\00", align 1
@scanf.int = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@printf.str = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@printf.num = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare i32 @printf(ptr, ...)

define i32 @main() {
entry:
  %x = alloca i32, align 4
  %y = load i32, ptr %x, align 4
  %i = alloca i32, align 4
  %y1 = load i32, ptr %i, align 4
  %readln = call i32 (ptr, ...) @scanf(ptr @scanf.int, ptr %x)
  %x2 = load i32, ptr %x, align 4
  %gt = icmp sgt i32 %x2, 10
  %cmp_ext = zext i1 %gt to i32
  %ifcond = icmp ne i32 %cmp_ext, 0
  br i1 %ifcond, label %then, label %else

ifcont:                                           ; preds = %else, %then
  %x4 = load i32, ptr %x, align 4
  %gt5 = icmp sgt i32 %x4, 6
  %cmp_ext6 = zext i1 %gt5 to i32
  %ifcond7 = icmp ne i32 %cmp_ext6, 0
  br i1 %ifcond7, label %then8, label %else9

then:                                             ; preds = %entry
  store i32 1, ptr %i, align 4
  br label %ifcont

else:                                             ; preds = %entry
  br label %ifcont

ifcont3:                                          ; preds = %else15, %then14, %then8
  %print = call i32 (ptr, ...) @printf(ptr @printf.str, ptr @1)
  %i16 = load i32, ptr %i, align 4
  %print17 = call i32 (ptr, ...) @printf(ptr @printf.num, i32 %i16)
  ret i32 0

elseif:                                           ; preds = %else9
  %x10 = load i32, ptr %x, align 4
  %gt11 = icmp sgt i32 %x10, 3
  %cmp_ext12 = zext i1 %gt11 to i32
  %ifcond13 = icmp ne i32 %cmp_ext12, 0
  br i1 %ifcond13, label %then14, label %else15

then8:                                            ; preds = %ifcont
  store i32 2, ptr %i, align 4
  br label %ifcont3

else9:                                            ; preds = %ifcont
  br label %elseif

then14:                                           ; preds = %elseif
  store i32 3, ptr %i, align 4
  br label %ifcont3

else15:                                           ; preds = %elseif
  store i32 4, ptr %i, align 4
  br label %ifcont3
}

declare i32 @main.1()

declare i32 @scanf(ptr, ...)
