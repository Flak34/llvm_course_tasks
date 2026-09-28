; ModuleID = 'app.c'
source_filename = "app.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: nofree norecurse nosync nounwind memory(argmem: write) uwtable
define dso_local void @compute_julia(ptr nocapture noundef writeonly %0, i32 noundef %1, i32 noundef %2, float noundef %3, float noundef %4) local_unnamed_addr #0 {
  %6 = icmp sgt i32 %2, 0
  br i1 %6, label %7, label %28

7:                                                ; preds = %5
  %8 = icmp sgt i32 %1, 0
  %9 = sitofp i32 %1 to float
  %10 = fmul float %9, 5.000000e-01
  %11 = sitofp i32 %2 to float
  %12 = fmul float %11, 5.000000e-01
  %13 = zext nneg i32 %2 to i64
  %14 = zext nneg i32 %1 to i64
  br label %15

15:                                               ; preds = %7, %29
  %16 = phi i64 [ 0, %7 ], [ %30, %29 ]
  br i1 %8, label %17, label %29

17:                                               ; preds = %15
  %18 = trunc i64 %16 to i32
  %19 = sitofp i32 %18 to float
  %20 = fsub float %19, %12
  %21 = fmul float %20, 3.000000e+00
  %22 = fdiv float %21, %11
  %23 = fmul float %22, %22
  %24 = trunc i64 %16 to i32
  %25 = mul i32 %24, %1
  %26 = zext i32 %25 to i64
  %27 = getelementptr i32, ptr %0, i64 %26
  br label %32

28:                                               ; preds = %29, %5
  ret void

29:                                               ; preds = %57, %15
  %30 = add nuw nsw i64 %16, 1
  %31 = icmp eq i64 %30, %13
  br i1 %31, label %28, label %15, !llvm.loop !5

32:                                               ; preds = %17, %57
  %33 = phi i64 [ 0, %17 ], [ %60, %57 ]
  %34 = trunc i64 %33 to i32
  %35 = sitofp i32 %34 to float
  %36 = fsub float %35, %10
  %37 = fmul float %36, 3.000000e+00
  %38 = fdiv float %37, %9
  %39 = tail call float @llvm.fmuladd.f32(float %38, float %38, float %23)
  %40 = fcmp olt float %39, 4.000000e+00
  br i1 %40, label %41, label %57

41:                                               ; preds = %32, %41
  %42 = phi float [ %52, %41 ], [ %23, %32 ]
  %43 = phi i32 [ %51, %41 ], [ 0, %32 ]
  %44 = phi float [ %50, %41 ], [ %22, %32 ]
  %45 = phi float [ %48, %41 ], [ %38, %32 ]
  %46 = fneg float %42
  %47 = tail call float @llvm.fmuladd.f32(float %45, float %45, float %46)
  %48 = fadd float %47, %3
  %49 = fmul float %45, 2.000000e+00
  %50 = tail call float @llvm.fmuladd.f32(float %49, float %44, float %4)
  %51 = add nuw nsw i32 %43, 1
  %52 = fmul float %50, %50
  %53 = tail call float @llvm.fmuladd.f32(float %48, float %48, float %52)
  %54 = fcmp olt float %53, 4.000000e+00
  %55 = icmp ult i32 %43, 254
  %56 = select i1 %54, i1 %55, i1 false
  br i1 %56, label %41, label %57, !llvm.loop !7

57:                                               ; preds = %41, %32
  %58 = phi i32 [ 0, %32 ], [ %51, %41 ]
  %59 = getelementptr i32, ptr %27, i64 %33
  store i32 %58, ptr %59, align 4, !tbaa !8
  %60 = add nuw nsw i64 %33, 1
  %61 = icmp eq i64 %60, %14
  br i1 %61, label %29, label %32, !llvm.loop !12
}

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(i64 immarg, ptr nocapture) #1

; Function Attrs: mustprogress nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare float @llvm.fmuladd.f32(float, float, float) #2

; Function Attrs: noreturn nounwind uwtable
define dso_local void @app() local_unnamed_addr #3 {
  %1 = alloca [73728 x i32], align 16
  call void @llvm.lifetime.start.p0(i64 294912, ptr nonnull %1) #5
  br label %2

2:                                                ; preds = %54, %0
  %3 = phi <2 x float> [ <float 0x3F589374C0000000, float 0x3F60624DE0000000>, %0 ], [ %63, %54 ]
  %4 = phi <2 x float> [ <float 0x3FD14A2340000000, float 0xBFE6666660000000>, %0 ], [ %55, %54 ]
  %5 = extractelement <2 x float> %4, i64 1
  %6 = extractelement <2 x float> %4, i64 0
  br label %7

7:                                                ; preds = %17, %2
  %8 = phi i64 [ 0, %2 ], [ %18, %17 ]
  %9 = trunc i64 %8 to i32
  %10 = sitofp i32 %9 to float
  %11 = fadd float %10, -9.600000e+01
  %12 = fmul float %11, 3.000000e+00
  %13 = fdiv float %12, 1.920000e+02
  %14 = fmul float %13, %13
  %15 = mul nuw nsw i64 %8, 384
  %16 = getelementptr i32, ptr %1, i64 %15
  br label %20

17:                                               ; preds = %45
  %18 = add nuw nsw i64 %8, 1
  %19 = icmp eq i64 %18, 192
  br i1 %19, label %50, label %7, !llvm.loop !5

20:                                               ; preds = %45, %7
  %21 = phi i64 [ 0, %7 ], [ %48, %45 ]
  %22 = trunc i64 %21 to i32
  %23 = sitofp i32 %22 to float
  %24 = fadd float %23, -1.920000e+02
  %25 = fmul float %24, 3.000000e+00
  %26 = fdiv float %25, 3.840000e+02
  %27 = tail call float @llvm.fmuladd.f32(float %26, float %26, float %14)
  %28 = fcmp olt float %27, 4.000000e+00
  br i1 %28, label %29, label %45

29:                                               ; preds = %20, %29
  %30 = phi float [ %40, %29 ], [ %14, %20 ]
  %31 = phi i32 [ %39, %29 ], [ 0, %20 ]
  %32 = phi float [ %38, %29 ], [ %13, %20 ]
  %33 = phi float [ %36, %29 ], [ %26, %20 ]
  %34 = fneg float %30
  %35 = tail call float @llvm.fmuladd.f32(float %33, float %33, float %34)
  %36 = fadd float %5, %35
  %37 = fmul float %33, 2.000000e+00
  %38 = tail call float @llvm.fmuladd.f32(float %37, float %32, float %6)
  %39 = add nuw nsw i32 %31, 1
  %40 = fmul float %38, %38
  %41 = tail call float @llvm.fmuladd.f32(float %36, float %36, float %40)
  %42 = fcmp olt float %41, 4.000000e+00
  %43 = icmp ult i32 %31, 254
  %44 = select i1 %42, i1 %43, i1 false
  br i1 %44, label %29, label %45, !llvm.loop !7

45:                                               ; preds = %29, %20
  %46 = phi i32 [ 0, %20 ], [ %39, %29 ]
  %47 = getelementptr i32, ptr %16, i64 %21
  store i32 %46, ptr %47, align 4, !tbaa !8
  %48 = add nuw nsw i64 %21, 1
  %49 = icmp eq i64 %48, 384
  br i1 %49, label %17, label %20, !llvm.loop !12

50:                                               ; preds = %17, %64
  %51 = phi i64 [ %65, %64 ], [ 0, %17 ]
  %52 = mul nuw nsw i64 %51, 384
  %53 = trunc i64 %51 to i32
  br label %67

54:                                               ; preds = %64
  tail call void (...) @simFlush() #5
  %55 = fadd <2 x float> %3, %4
  %56 = shufflevector <2 x float> %55, <2 x float> poison, <4 x i32> <i32 0, i32 1, i32 0, i32 1>
  %57 = fneg <2 x float> %3
  %58 = fcmp olt <4 x float> %56, <float 0x3FC99999A0000000, float 0xBFE99999A0000000, float 0x3FD6666660000000, float 0xBFE3333340000000>
  %59 = fcmp ogt <4 x float> %56, <float 0x3FC99999A0000000, float 0xBFE99999A0000000, float 0x3FD6666660000000, float 0xBFE3333340000000>
  %60 = shufflevector <4 x i1> %59, <4 x i1> poison, <2 x i32> <i32 2, i32 3>
  %61 = shufflevector <4 x i1> %58, <4 x i1> poison, <2 x i32> <i32 0, i32 1>
  %62 = or <2 x i1> %60, %61
  %63 = select <2 x i1> %62, <2 x float> %57, <2 x float> %3
  br label %2

64:                                               ; preds = %76
  %65 = add nuw nsw i64 %51, 1
  %66 = icmp eq i64 %65, 192
  br i1 %66, label %54, label %50, !llvm.loop !13

67:                                               ; preds = %50, %76
  %68 = phi i64 [ 0, %50 ], [ %79, %76 ]
  %69 = add nuw nsw i64 %68, %52
  %70 = getelementptr inbounds [73728 x i32], ptr %1, i64 0, i64 %69
  %71 = load i32, ptr %70, align 4, !tbaa !8
  %72 = icmp eq i32 %71, 255
  br i1 %72, label %76, label %73

73:                                               ; preds = %67
  %74 = mul nsw i32 %71, 9
  %75 = srem i32 %74, 256
  br label %76

76:                                               ; preds = %67, %73
  %77 = phi i32 [ %75, %73 ], [ 0, %67 ]
  %78 = trunc i64 %68 to i32
  tail call void @simPutPixel(i32 noundef %78, i32 noundef %53, i32 noundef %77) #5
  %79 = add nuw nsw i64 %68, 1
  %80 = icmp eq i64 %79, 384
  br i1 %80, label %64, label %67, !llvm.loop !14
}

declare void @simPutPixel(i32 noundef, i32 noundef, i32 noundef) local_unnamed_addr #4

declare void @simFlush(...) local_unnamed_addr #4

attributes #0 = { nofree norecurse nosync nounwind memory(argmem: write) uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { mustprogress nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #3 = { noreturn nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{!"Ubuntu clang version 18.1.3 (1ubuntu1)"}
!5 = distinct !{!5, !6}
!6 = !{!"llvm.loop.mustprogress"}
!7 = distinct !{!7, !6}
!8 = !{!9, !9, i64 0}
!9 = !{!"int", !10, i64 0}
!10 = !{!"omnipotent char", !11, i64 0}
!11 = !{!"Simple C/C++ TBAA"}
!12 = distinct !{!12, !6}
!13 = distinct !{!13, !6}
!14 = distinct !{!14, !6}
