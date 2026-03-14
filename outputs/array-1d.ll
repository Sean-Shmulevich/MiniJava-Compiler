; ModuleID = 'outputs/array-1d.ll'
source_filename = "outputs/array-1d.ll"

%Array1D = type { [348523040 x i32] }

@Array1D.global = global %Array1D zeroinitializer
@0 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@printf.num = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare i32 @printf(ptr, ...)

define i32 @main() {
entry:
  %arrLocal = alloca i32, align 4
  %y = load i32, ptr %arrLocal, align 4
  %i = alloca i32, align 4
  %y1 = load i32, ptr %i, align 4
  store i32 0, ptr %i, align 4
  br label %loop.cond

loop.cond:                                        ; preds = %loop.body, %entry
  %i2 = load i32, ptr %i, align 4
  %lt = icmp slt i32 %i2, 10
  %cmp_ext = zext i1 %lt to i32
  %tobool = icmp ne i32 %cmp_ext, 0
  br i1 %tobool, label %loop.body, label %loop.end

loop.body:                                        ; preds = %loop.cond
  %i3 = load i32, ptr %i, align 4
  %i4 = load i32, ptr %i, align 4
  %arr.elem = getelementptr [348523040 x i32], ptr @Array1D.global, i32 0, i32 %i4
  %arrMember.val = load i32, ptr %arr.elem, align 4
  store i32 %arrMember.val, ptr %arrLocal, align 4
  %i5 = load i32, ptr %i, align 4
  %add_result = add i32 %i5, 1
  store i32 %add_result, ptr %i, align 4
  br label %loop.cond

loop.end:                                         ; preds = %loop.cond
  store i32 0, ptr %i, align 4
  br label %loop.cond6

loop.cond6:                                       ; preds = %loop.body7, %loop.end
  %i9 = load i32, ptr %i, align 4
  %lt10 = icmp slt i32 %i9, 10
  %cmp_ext11 = zext i1 %lt10 to i32
  %tobool12 = icmp ne i32 %cmp_ext11, 0
  br i1 %tobool12, label %loop.body7, label %loop.end8

loop.body7:                                       ; preds = %loop.cond6
  %i13 = load i32, ptr %i, align 4
  %arrLocal14 = load i32, ptr %arrLocal, align 4
  %print = call i32 (ptr, ...) @printf(ptr @printf.num, i32 %arrLocal14)
  %i15 = load i32, ptr %i, align 4
  %add_result16 = add i32 %i15, 1
  store i32 %add_result16, ptr %i, align 4
  br label %loop.cond6

loop.end8:                                        ; preds = %loop.cond6
  ret i32 0
}
