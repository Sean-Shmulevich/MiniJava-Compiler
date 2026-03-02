; ModuleID = 'outputs/if-statements.ll'
source_filename = "outputs/if-statements.ll"

%IfStatements = type { i32 }

@IfStatements.global = global %IfStatements { i32 -1 }
@0 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@1 = private unnamed_addr constant [5 x i8] c"x>=0\00", align 1
@printf.str = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

declare i32 @printf(ptr, ...)

define i32 @main() {
entry:
  %x = alloca i32, align 4
  %y = load i32, ptr %x, align 4
  %x1 = load i32, ptr %x, align 4
  %ge = icmp sge i32 %x1, 0
  %cmp_ext = zext i1 %ge to i32
  %ifcond = icmp ne i32 %cmp_ext, 0
  br i1 %ifcond, label %then, label %else

ifcont:                                           ; preds = %else, %then
  ret i32 0

then:                                             ; preds = %entry
  %print = call i32 (ptr, ...) @printf(ptr @printf.str, ptr @1)
  br label %ifcont

else:                                             ; preds = %entry
  br label %ifcont
}

declare i32 @main.1()
