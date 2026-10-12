; ModuleID = 'app.c'
source_filename = "app.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@0 = private unnamed_addr constant [42 x i8] c"icmp <- <argument const>\0Aicmp <- <const>\0A\00", align 1
@1 = private unnamed_addr constant [28 x i8] c"sitofp <- <argument const>\0A\00", align 1
@2 = private unnamed_addr constant [32 x i8] c"fmul <- sitofp\0Afmul <- <const>\0A\00", align 1
@3 = private unnamed_addr constant [26 x i8] c"zext <- <argument const>\0A\00", align 1
@4 = private unnamed_addr constant [27 x i8] c"fmul <- fdiv\0Afmul <- fdiv\0A\00", align 1
@5 = private unnamed_addr constant [38 x i8] c"mul <- trunc\0Amul <- <argument const>\0A\00", align 1
@6 = private unnamed_addr constant [13 x i8] c"zext <- mul\0A\00", align 1
@7 = private unnamed_addr constant [57 x i8] c"getelementptr <- <argument const>\0Agetelementptr <- zext\0A\00", align 1
@8 = private unnamed_addr constant [17 x i8] c"sitofp <- trunc\0A\00", align 1
@9 = private unnamed_addr constant [29 x i8] c"fsub <- sitofp\0Afsub <- fmul\0A\00", align 1
@10 = private unnamed_addr constant [30 x i8] c"fmul <- fsub\0Afmul <- <const>\0A\00", align 1
@11 = private unnamed_addr constant [29 x i8] c"fdiv <- fmul\0Afdiv <- sitofp\0A\00", align 1
@12 = private unnamed_addr constant [104 x i8] c"llvm.fmuladd.f32 <- fdiv\0Allvm.fmuladd.f32 <- fdiv\0Allvm.fmuladd.f32 <- fmul\0Allvm.fmuladd.f32 <- <const>\0A\00", align 1
@13 = private unnamed_addr constant [27 x i8] c"fneg <- fmul\0Afneg <- fmul\0A\00", align 1
@14 = private unnamed_addr constant [154 x i8] c"llvm.fmuladd.f32 <- fadd\0Allvm.fmuladd.f32 <- fdiv\0Allvm.fmuladd.f32 <- fadd\0Allvm.fmuladd.f32 <- fdiv\0Allvm.fmuladd.f32 <- fneg\0Allvm.fmuladd.f32 <- <const>\0A\00", align 1
@15 = private unnamed_addr constant [51 x i8] c"fadd <- llvm.fmuladd.f32\0Afadd <- <argument const>\0A\00", align 1
@16 = private unnamed_addr constant [43 x i8] c"fmul <- fadd\0Afmul <- fdiv\0Afmul <- <const>\0A\00", align 1
@17 = private unnamed_addr constant [153 x i8] c"llvm.fmuladd.f32 <- fmul\0Allvm.fmuladd.f32 <- llvm.fmuladd.f32\0Allvm.fmuladd.f32 <- fdiv\0Allvm.fmuladd.f32 <- <argument const>\0Allvm.fmuladd.f32 <- <const>\0A\00", align 1
@18 = private unnamed_addr constant [42 x i8] c"add <- add\0Aadd <- <const>\0Aadd <- <const>\0A\00", align 1
@19 = private unnamed_addr constant [51 x i8] c"fmul <- llvm.fmuladd.f32\0Afmul <- llvm.fmuladd.f32\0A\00", align 1
@20 = private unnamed_addr constant [104 x i8] c"llvm.fmuladd.f32 <- fadd\0Allvm.fmuladd.f32 <- fadd\0Allvm.fmuladd.f32 <- fmul\0Allvm.fmuladd.f32 <- <const>\0A\00", align 1
@21 = private unnamed_addr constant [42 x i8] c"fcmp <- llvm.fmuladd.f32\0Afcmp <- <const>\0A\00", align 1
@22 = private unnamed_addr constant [45 x i8] c"icmp <- add\0Aicmp <- <const>\0Aicmp <- <const>\0A\00", align 1
@23 = private unnamed_addr constant [49 x i8] c"select <- fcmp\0Aselect <- icmp\0Aselect <- <const>\0A\00", align 1
@24 = private unnamed_addr constant [78 x i8] c"getelementptr <- getelementptr\0Agetelementptr <- <const>\0Agetelementptr <- add\0A\00", align 1
@25 = private unnamed_addr constant [54 x i8] c"store <- <const>\0Astore <- add\0Astore <- getelementptr\0A\00", align 1
@26 = private unnamed_addr constant [26 x i8] c"icmp <- add\0Aicmp <- zext\0A\00", align 1
@27 = private unnamed_addr constant [19 x i8] c"alloca <- <const>\0A\00", align 1
@28 = private unnamed_addr constant [102 x i8] c"llvm.lifetime.start.p0 <- <const>\0Allvm.lifetime.start.p0 <- alloca\0Allvm.lifetime.start.p0 <- <const>\0A\00", align 1
@29 = private unnamed_addr constant [76 x i8] c"extractelement <- <const>\0Aextractelement <- fadd\0Aextractelement <- <const>\0A\00", align 1
@30 = private unnamed_addr constant [42 x i8] c"mul <- <const>\0Amul <- add\0Amul <- <const>\0A\00", align 1
@31 = private unnamed_addr constant [61 x i8] c"fadd <- <const>\0Afadd <- select\0Afadd <- <const>\0Afadd <- fadd\0A\00", align 1
@32 = private unnamed_addr constant [48 x i8] c"shufflevector <- fadd\0Ashufflevector <- <const>\0A\00", align 1
@33 = private unnamed_addr constant [32 x i8] c"fneg <- <const>\0Afneg <- select\0A\00", align 1
@34 = private unnamed_addr constant [39 x i8] c"fcmp <- shufflevector\0Afcmp <- <const>\0A\00", align 1
@35 = private unnamed_addr constant [48 x i8] c"shufflevector <- fcmp\0Ashufflevector <- <const>\0A\00", align 1
@36 = private unnamed_addr constant [41 x i8] c"or <- shufflevector\0Aor <- shufflevector\0A\00", align 1
@37 = private unnamed_addr constant [64 x i8] c"select <- or\0Aselect <- fneg\0Aselect <- <const>\0Aselect <- select\0A\00", align 1
@38 = private unnamed_addr constant [38 x i8] c"add <- <const>\0Aadd <- add\0Aadd <- mul\0A\00", align 1
@39 = private unnamed_addr constant [71 x i8] c"getelementptr <- alloca\0Agetelementptr <- <const>\0Agetelementptr <- add\0A\00", align 1
@40 = private unnamed_addr constant [23 x i8] c"load <- getelementptr\0A\00", align 1
@41 = private unnamed_addr constant [30 x i8] c"icmp <- load\0Aicmp <- <const>\0A\00", align 1
@42 = private unnamed_addr constant [28 x i8] c"mul <- load\0Amul <- <const>\0A\00", align 1
@43 = private unnamed_addr constant [29 x i8] c"srem <- mul\0Asrem <- <const>\0A\00", align 1
@44 = private unnamed_addr constant [31 x i8] c"trunc <- <const>\0Atrunc <- add\0A\00", align 1
@45 = private unnamed_addr constant [42 x i8] c"add <- <const>\0Aadd <- add\0Aadd <- <const>\0A\00", align 1
@46 = private unnamed_addr constant [29 x i8] c"icmp <- add\0Aicmp <- <const>\0A\00", align 1

; Function Attrs: nofree norecurse nosync nounwind optsize memory(argmem: write) uwtable
define dso_local void @compute_julia(ptr nocapture noundef writeonly %0, i32 noundef %1, i32 noundef %2, float noundef %3, float noundef %4) local_unnamed_addr #0 {
  call void @instructionLogger(ptr @0)
  %6 = icmp sgt i32 %2, 0
  br i1 %6, label %7, label %28

7:                                                ; preds = %5
  call void @instructionLogger(ptr @0)
  %8 = icmp sgt i32 %1, 0
  call void @instructionLogger(ptr @1)
  %9 = sitofp i32 %1 to float
  call void @instructionLogger(ptr @2)
  %10 = fmul float %9, 5.000000e-01
  call void @instructionLogger(ptr @1)
  %11 = sitofp i32 %2 to float
  call void @instructionLogger(ptr @2)
  %12 = fmul float %11, 5.000000e-01
  call void @instructionLogger(ptr @3)
  %13 = zext nneg i32 %2 to i64
  call void @instructionLogger(ptr @3)
  %14 = zext nneg i32 %1 to i64
  br label %15

15:                                               ; preds = %7, %29
  %16 = phi i64 [ 0, %7 ], [ %30, %29 ]
  br i1 %8, label %17, label %29

17:                                               ; preds = %15
  call void @instructionLogger(ptr @44)
  %18 = trunc i64 %16 to i32
  call void @instructionLogger(ptr @8)
  %19 = sitofp i32 %18 to float
  call void @instructionLogger(ptr @9)
  %20 = fsub float %19, %12
  call void @instructionLogger(ptr @10)
  %21 = fmul float %20, 3.000000e+00
  call void @instructionLogger(ptr @11)
  %22 = fdiv float %21, %11
  call void @instructionLogger(ptr @4)
  %23 = fmul float %22, %22
  call void @instructionLogger(ptr @44)
  %24 = trunc i64 %16 to i32
  call void @instructionLogger(ptr @5)
  %25 = mul i32 %24, %1
  call void @instructionLogger(ptr @6)
  %26 = zext i32 %25 to i64
  call void @instructionLogger(ptr @7)
  %27 = getelementptr i32, ptr %0, i64 %26
  br label %32

28:                                               ; preds = %29, %5
  ret void

29:                                               ; preds = %57, %15
  call void @instructionLogger(ptr @45)
  %30 = add nuw nsw i64 %16, 1
  call void @instructionLogger(ptr @26)
  %31 = icmp eq i64 %30, %13
  br i1 %31, label %28, label %15, !llvm.loop !5

32:                                               ; preds = %17, %57
  %33 = phi i64 [ 0, %17 ], [ %60, %57 ]
  call void @instructionLogger(ptr @44)
  %34 = trunc i64 %33 to i32
  call void @instructionLogger(ptr @8)
  %35 = sitofp i32 %34 to float
  call void @instructionLogger(ptr @9)
  %36 = fsub float %35, %10
  call void @instructionLogger(ptr @10)
  %37 = fmul float %36, 3.000000e+00
  call void @instructionLogger(ptr @11)
  %38 = fdiv float %37, %9
  call void @instructionLogger(ptr @12)
  %39 = tail call float @llvm.fmuladd.f32(float %38, float %38, float %23)
  call void @instructionLogger(ptr @21)
  %40 = fcmp olt float %39, 4.000000e+00
  br i1 %40, label %41, label %57

41:                                               ; preds = %32, %41
  %42 = phi float [ %52, %41 ], [ %23, %32 ]
  %43 = phi i32 [ %51, %41 ], [ 0, %32 ]
  %44 = phi float [ %50, %41 ], [ %22, %32 ]
  %45 = phi float [ %48, %41 ], [ %38, %32 ]
  call void @instructionLogger(ptr @13)
  %46 = fneg float %42
  call void @instructionLogger(ptr @14)
  %47 = tail call float @llvm.fmuladd.f32(float %45, float %45, float %46)
  call void @instructionLogger(ptr @15)
  %48 = fadd float %47, %3
  call void @instructionLogger(ptr @16)
  %49 = fmul float %45, 2.000000e+00
  call void @instructionLogger(ptr @17)
  %50 = tail call float @llvm.fmuladd.f32(float %49, float %44, float %4)
  call void @instructionLogger(ptr @18)
  %51 = add nuw nsw i32 %43, 1
  call void @instructionLogger(ptr @19)
  %52 = fmul float %50, %50
  call void @instructionLogger(ptr @20)
  %53 = tail call float @llvm.fmuladd.f32(float %48, float %48, float %52)
  call void @instructionLogger(ptr @21)
  %54 = fcmp olt float %53, 4.000000e+00
  call void @instructionLogger(ptr @22)
  %55 = icmp ult i32 %43, 254
  call void @instructionLogger(ptr @23)
  %56 = select i1 %54, i1 %55, i1 false
  br i1 %56, label %41, label %57, !llvm.loop !7

57:                                               ; preds = %41, %32
  %58 = phi i32 [ 0, %32 ], [ %51, %41 ]
  call void @instructionLogger(ptr @24)
  %59 = getelementptr i32, ptr %27, i64 %33
  call void @instructionLogger(ptr @25)
  store i32 %58, ptr %59, align 4, !tbaa !8
  call void @instructionLogger(ptr @45)
  %60 = add nuw nsw i64 %33, 1
  call void @instructionLogger(ptr @26)
  %61 = icmp eq i64 %60, %14
  br i1 %61, label %29, label %32, !llvm.loop !12
}

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(i64 immarg, ptr nocapture) #1

; Function Attrs: mustprogress nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare float @llvm.fmuladd.f32(float, float, float) #2

; Function Attrs: noreturn nounwind optsize uwtable
define dso_local void @app() local_unnamed_addr #3 {
  call void @instructionLogger(ptr @27)
  %1 = alloca [73728 x i32], align 16
  call void @instructionLogger(ptr @28)
  call void @llvm.lifetime.start.p0(i64 294912, ptr nonnull %1) #5
  br label %2

2:                                                ; preds = %11, %0
  %3 = phi <2 x float> [ <float 0x3F589374C0000000, float 0x3F60624DE0000000>, %0 ], [ %20, %11 ]
  %4 = phi <2 x float> [ <float 0x3FD14A2340000000, float 0xBFE6666660000000>, %0 ], [ %12, %11 ]
  call void @instructionLogger(ptr @29)
  %5 = extractelement <2 x float> %4, i64 0
  call void @instructionLogger(ptr @29)
  %6 = extractelement <2 x float> %4, i64 1
  call void @compute_julia(ptr noundef nonnull %1, i32 noundef 384, i32 noundef 192, float noundef %6, float noundef %5) #6
  br label %7

7:                                                ; preds = %2, %21
  %8 = phi i64 [ 0, %2 ], [ %22, %21 ]
  call void @instructionLogger(ptr @30)
  %9 = mul nuw nsw i64 %8, 384
  call void @instructionLogger(ptr @44)
  %10 = trunc i64 %8 to i32
  br label %24

11:                                               ; preds = %21
  tail call void (...) @simFlush() #7
  call void @instructionLogger(ptr @31)
  %12 = fadd <2 x float> %3, %4
  call void @instructionLogger(ptr @32)
  %13 = shufflevector <2 x float> %12, <2 x float> poison, <4 x i32> <i32 0, i32 1, i32 0, i32 1>
  call void @instructionLogger(ptr @33)
  %14 = fneg <2 x float> %3
  call void @instructionLogger(ptr @34)
  %15 = fcmp olt <4 x float> %13, <float 0x3FC99999A0000000, float 0xBFE99999A0000000, float 0x3FD6666660000000, float 0xBFE3333340000000>
  call void @instructionLogger(ptr @34)
  %16 = fcmp ogt <4 x float> %13, <float 0x3FC99999A0000000, float 0xBFE99999A0000000, float 0x3FD6666660000000, float 0xBFE3333340000000>
  call void @instructionLogger(ptr @35)
  %17 = shufflevector <4 x i1> %16, <4 x i1> poison, <2 x i32> <i32 2, i32 3>
  call void @instructionLogger(ptr @35)
  %18 = shufflevector <4 x i1> %15, <4 x i1> poison, <2 x i32> <i32 0, i32 1>
  call void @instructionLogger(ptr @36)
  %19 = or <2 x i1> %17, %18
  call void @instructionLogger(ptr @37)
  %20 = select <2 x i1> %19, <2 x float> %14, <2 x float> %3
  br label %2

21:                                               ; preds = %33
  call void @instructionLogger(ptr @45)
  %22 = add nuw nsw i64 %8, 1
  call void @instructionLogger(ptr @46)
  %23 = icmp eq i64 %22, 192
  br i1 %23, label %11, label %7, !llvm.loop !13

24:                                               ; preds = %7, %33
  %25 = phi i64 [ 0, %7 ], [ %36, %33 ]
  call void @instructionLogger(ptr @38)
  %26 = add nuw nsw i64 %25, %9
  call void @instructionLogger(ptr @39)
  %27 = getelementptr inbounds [73728 x i32], ptr %1, i64 0, i64 %26
  call void @instructionLogger(ptr @40)
  %28 = load i32, ptr %27, align 4, !tbaa !8
  call void @instructionLogger(ptr @41)
  %29 = icmp eq i32 %28, 255
  br i1 %29, label %33, label %30

30:                                               ; preds = %24
  call void @instructionLogger(ptr @42)
  %31 = mul nsw i32 %28, 9
  call void @instructionLogger(ptr @43)
  %32 = srem i32 %31, 256
  br label %33

33:                                               ; preds = %24, %30
  %34 = phi i32 [ %32, %30 ], [ 0, %24 ]
  call void @instructionLogger(ptr @44)
  %35 = trunc i64 %25 to i32
  tail call void @simPutPixel(i32 noundef %35, i32 noundef %10, i32 noundef %34) #7
  call void @instructionLogger(ptr @45)
  %36 = add nuw nsw i64 %25, 1
  call void @instructionLogger(ptr @46)
  %37 = icmp eq i64 %36, 384
  br i1 %37, label %21, label %24, !llvm.loop !14
}

; Function Attrs: optsize
declare void @simPutPixel(i32 noundef, i32 noundef, i32 noundef) local_unnamed_addr #4

; Function Attrs: optsize
declare void @simFlush(...) local_unnamed_addr #4

declare void @instructionLogger(ptr)

attributes #0 = { nofree norecurse nosync nounwind optsize memory(argmem: write) uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { mustprogress nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #3 = { noreturn nounwind optsize uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { optsize "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { nounwind }
attributes #6 = { optsize }
attributes #7 = { nounwind optsize }

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
