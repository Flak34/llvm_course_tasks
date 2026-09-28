; ModuleID = 'app.c'
source_filename = "app.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @compute_julia(ptr noundef %0, i32 noundef %1, i32 noundef %2, float noundef %3, float noundef %4) #0 {
  %6 = alloca ptr, align 8
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca float, align 4
  %10 = alloca float, align 4
  %11 = alloca i32, align 4
  %12 = alloca i32, align 4
  %13 = alloca float, align 4
  %14 = alloca float, align 4
  %15 = alloca i32, align 4
  %16 = alloca float, align 4
  store ptr %0, ptr %6, align 8
  store i32 %1, ptr %7, align 4
  store i32 %2, ptr %8, align 4
  store float %3, ptr %9, align 4
  store float %4, ptr %10, align 4
  store i32 0, ptr %11, align 4
  br label %17

17:                                               ; preds = %92, %5
  %18 = load i32, ptr %11, align 4
  %19 = load i32, ptr %8, align 4
  %20 = icmp slt i32 %18, %19
  br i1 %20, label %21, label %95

21:                                               ; preds = %17
  store i32 0, ptr %12, align 4
  br label %22

22:                                               ; preds = %88, %21
  %23 = load i32, ptr %12, align 4
  %24 = load i32, ptr %7, align 4
  %25 = icmp slt i32 %23, %24
  br i1 %25, label %26, label %91

26:                                               ; preds = %22
  %27 = load i32, ptr %12, align 4
  %28 = sitofp i32 %27 to float
  %29 = load i32, ptr %7, align 4
  %30 = sitofp i32 %29 to float
  %31 = fdiv float %30, 2.000000e+00
  %32 = fsub float %28, %31
  %33 = fmul float %32, 3.000000e+00
  %34 = load i32, ptr %7, align 4
  %35 = sitofp i32 %34 to float
  %36 = fdiv float %33, %35
  store float %36, ptr %13, align 4
  %37 = load i32, ptr %11, align 4
  %38 = sitofp i32 %37 to float
  %39 = load i32, ptr %8, align 4
  %40 = sitofp i32 %39 to float
  %41 = fdiv float %40, 2.000000e+00
  %42 = fsub float %38, %41
  %43 = fmul float %42, 3.000000e+00
  %44 = load i32, ptr %8, align 4
  %45 = sitofp i32 %44 to float
  %46 = fdiv float %43, %45
  store float %46, ptr %14, align 4
  store i32 0, ptr %15, align 4
  br label %47

47:                                               ; preds = %60, %26
  %48 = load float, ptr %13, align 4
  %49 = load float, ptr %13, align 4
  %50 = load float, ptr %14, align 4
  %51 = load float, ptr %14, align 4
  %52 = fmul float %50, %51
  %53 = call float @llvm.fmuladd.f32(float %48, float %49, float %52)
  %54 = fcmp olt float %53, 4.000000e+00
  br i1 %54, label %55, label %58

55:                                               ; preds = %47
  %56 = load i32, ptr %15, align 4
  %57 = icmp slt i32 %56, 255
  br label %58

58:                                               ; preds = %55, %47
  %59 = phi i1 [ false, %47 ], [ %57, %55 ]
  br i1 %59, label %60, label %78

60:                                               ; preds = %58
  %61 = load float, ptr %13, align 4
  %62 = load float, ptr %13, align 4
  %63 = load float, ptr %14, align 4
  %64 = load float, ptr %14, align 4
  %65 = fmul float %63, %64
  %66 = fneg float %65
  %67 = call float @llvm.fmuladd.f32(float %61, float %62, float %66)
  %68 = load float, ptr %9, align 4
  %69 = fadd float %67, %68
  store float %69, ptr %16, align 4
  %70 = load float, ptr %13, align 4
  %71 = fmul float 2.000000e+00, %70
  %72 = load float, ptr %14, align 4
  %73 = load float, ptr %10, align 4
  %74 = call float @llvm.fmuladd.f32(float %71, float %72, float %73)
  store float %74, ptr %14, align 4
  %75 = load float, ptr %16, align 4
  store float %75, ptr %13, align 4
  %76 = load i32, ptr %15, align 4
  %77 = add nsw i32 %76, 1
  store i32 %77, ptr %15, align 4
  br label %47, !llvm.loop !6

78:                                               ; preds = %58
  %79 = load i32, ptr %15, align 4
  %80 = load ptr, ptr %6, align 8
  %81 = load i32, ptr %11, align 4
  %82 = load i32, ptr %7, align 4
  %83 = mul nsw i32 %81, %82
  %84 = load i32, ptr %12, align 4
  %85 = add nsw i32 %83, %84
  %86 = sext i32 %85 to i64
  %87 = getelementptr inbounds i32, ptr %80, i64 %86
  store i32 %79, ptr %87, align 4
  br label %88

88:                                               ; preds = %78
  %89 = load i32, ptr %12, align 4
  %90 = add nsw i32 %89, 1
  store i32 %90, ptr %12, align 4
  br label %22, !llvm.loop !8

91:                                               ; preds = %22
  br label %92

92:                                               ; preds = %91
  %93 = load i32, ptr %11, align 4
  %94 = add nsw i32 %93, 1
  store i32 %94, ptr %11, align 4
  br label %17, !llvm.loop !9

95:                                               ; preds = %17
  ret void
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare float @llvm.fmuladd.f32(float, float, float) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @app() #0 {
  %1 = alloca [73728 x i32], align 16
  %2 = alloca float, align 4
  %3 = alloca float, align 4
  %4 = alloca float, align 4
  %5 = alloca float, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  store float 0xBFE6666660000000, ptr %2, align 4
  store float 0x3FD14A2340000000, ptr %3, align 4
  store float 0x3F60624DE0000000, ptr %4, align 4
  store float 0x3F589374C0000000, ptr %5, align 4
  br label %10

10:                                               ; preds = %0, %73
  %11 = getelementptr inbounds [73728 x i32], ptr %1, i64 0, i64 0
  %12 = load float, ptr %2, align 4
  %13 = load float, ptr %3, align 4
  call void @compute_julia(ptr noundef %11, i32 noundef 384, i32 noundef 192, float noundef %12, float noundef %13)
  store i32 0, ptr %6, align 4
  br label %14

14:                                               ; preds = %46, %10
  %15 = load i32, ptr %6, align 4
  %16 = icmp slt i32 %15, 192
  br i1 %16, label %17, label %49

17:                                               ; preds = %14
  store i32 0, ptr %7, align 4
  br label %18

18:                                               ; preds = %42, %17
  %19 = load i32, ptr %7, align 4
  %20 = icmp slt i32 %19, 384
  br i1 %20, label %21, label %45

21:                                               ; preds = %18
  %22 = load i32, ptr %6, align 4
  %23 = mul nsw i32 %22, 1536
  %24 = sdiv i32 %23, 4
  %25 = load i32, ptr %7, align 4
  %26 = add nsw i32 %24, %25
  %27 = sext i32 %26 to i64
  %28 = getelementptr inbounds [73728 x i32], ptr %1, i64 0, i64 %27
  %29 = load i32, ptr %28, align 4
  store i32 %29, ptr %8, align 4
  %30 = load i32, ptr %8, align 4
  %31 = icmp eq i32 %30, 255
  br i1 %31, label %32, label %33

32:                                               ; preds = %21
  br label %37

33:                                               ; preds = %21
  %34 = load i32, ptr %8, align 4
  %35 = mul nsw i32 %34, 9
  %36 = srem i32 %35, 256
  br label %37

37:                                               ; preds = %33, %32
  %38 = phi i32 [ 0, %32 ], [ %36, %33 ]
  store i32 %38, ptr %9, align 4
  %39 = load i32, ptr %7, align 4
  %40 = load i32, ptr %6, align 4
  %41 = load i32, ptr %9, align 4
  call void @simPutPixel(i32 noundef %39, i32 noundef %40, i32 noundef %41)
  br label %42

42:                                               ; preds = %37
  %43 = load i32, ptr %7, align 4
  %44 = add nsw i32 %43, 1
  store i32 %44, ptr %7, align 4
  br label %18, !llvm.loop !10

45:                                               ; preds = %18
  br label %46

46:                                               ; preds = %45
  %47 = load i32, ptr %6, align 4
  %48 = add nsw i32 %47, 1
  store i32 %48, ptr %6, align 4
  br label %14, !llvm.loop !11

49:                                               ; preds = %14
  call void (...) @simFlush()
  %50 = load float, ptr %4, align 4
  %51 = load float, ptr %2, align 4
  %52 = fadd float %51, %50
  store float %52, ptr %2, align 4
  %53 = load float, ptr %5, align 4
  %54 = load float, ptr %3, align 4
  %55 = fadd float %54, %53
  store float %55, ptr %3, align 4
  %56 = load float, ptr %2, align 4
  %57 = fcmp ogt float %56, 0xBFE3333340000000
  br i1 %57, label %61, label %58

58:                                               ; preds = %49
  %59 = load float, ptr %2, align 4
  %60 = fcmp olt float %59, 0xBFE99999A0000000
  br i1 %60, label %61, label %64

61:                                               ; preds = %58, %49
  %62 = load float, ptr %4, align 4
  %63 = fneg float %62
  store float %63, ptr %4, align 4
  br label %64

64:                                               ; preds = %61, %58
  %65 = load float, ptr %3, align 4
  %66 = fcmp ogt float %65, 0x3FD6666660000000
  br i1 %66, label %70, label %67

67:                                               ; preds = %64
  %68 = load float, ptr %3, align 4
  %69 = fcmp olt float %68, 0x3FC99999A0000000
  br i1 %69, label %70, label %73

70:                                               ; preds = %67, %64
  %71 = load float, ptr %5, align 4
  %72 = fneg float %71
  store float %72, ptr %5, align 4
  br label %73

73:                                               ; preds = %70, %67
  br label %10
}

declare void @simPutPixel(i32 noundef, i32 noundef, i32 noundef) #2

declare void @simFlush(...) #2

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #2 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"Ubuntu clang version 18.1.3 (1ubuntu1)"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = distinct !{!8, !7}
!9 = distinct !{!9, !7}
!10 = distinct !{!10, !7}
!11 = distinct !{!11, !7}
