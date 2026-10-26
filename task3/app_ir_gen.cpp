#include "sim.h"

#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

double hexToDouble(uint64_t hexValue) {
  double result;
  std::memcpy(&result, &hexValue, sizeof(double));
  return result;
}

Function* defineComputeJuliaFunc(LLVMContext& context, IRBuilder<>& builder, Module* module) {
  // types
  Type *voidType = Type::getVoidTy(context);
  Type *int32Type = Type::getInt32Ty(context);
  Type* int64Type = Type::getInt64Ty(context);
  Type* ptrType = PointerType::get(context, 0);
  Type* floatType = Type::getFloatTy(context);

  // declare float @llvm.fmuladd.f32(float, float, float) #2
  FunctionType* llvmFmuladdType =
    FunctionType::get(floatType, {floatType, floatType, floatType}, false);
  FunctionCallee llvmFmuladdFunc =
    module->getOrInsertFunction("llvm.fmuladd.f32", llvmFmuladdType);

  //   define void @compute_julia(ptr %0, i32 %1, i32 %2, float %3, float %4) {
  FunctionType *computeJuliaType = FunctionType::get(voidType,
    {ptrType, int32Type, int32Type, floatType, floatType}, false);
  Function *computeJuliaFunc =
      Function::Create(computeJuliaType, Function::ExternalLinkage, "compute_julia", module);
  Value* val0 = computeJuliaFunc->getArg(0);
  Value* val1 = computeJuliaFunc->getArg(1);
  Value* val2 = computeJuliaFunc->getArg(2);
  Value* val3 = computeJuliaFunc->getArg(3);
  Value* val4 = computeJuliaFunc->getArg(4);

  BasicBlock *BB0 = BasicBlock::Create(context, "", computeJuliaFunc);
  BasicBlock *BB7 = BasicBlock::Create(context, "", computeJuliaFunc);
  BasicBlock *BB15 = BasicBlock::Create(context, "", computeJuliaFunc);
  BasicBlock *BB17 = BasicBlock::Create(context, "", computeJuliaFunc);
  BasicBlock *BB28 = BasicBlock::Create(context, "", computeJuliaFunc);
  BasicBlock *BB29 = BasicBlock::Create(context, "", computeJuliaFunc);
  BasicBlock *BB32 = BasicBlock::Create(context, "", computeJuliaFunc);
  BasicBlock *BB41 = BasicBlock::Create(context, "", computeJuliaFunc);
  BasicBlock *BB57 = BasicBlock::Create(context, "", computeJuliaFunc);

  // 0:
  builder.SetInsertPoint(BB0);
  //   %6 = icmp sgt i32 %2, 0
  Value* val6 = builder.CreateICmpSGT(val2, builder.getInt32(0));
  //   br i1 %6, label %7, label %28
  builder.CreateCondBr(val6, BB7, BB28);

  // 7:                                                ; preds = %5
  builder.SetInsertPoint(BB7);
  //   %8 = icmp sgt i32 %1, 0
  Value* val8 = builder.CreateICmpSGT(val1, builder.getInt32(0));
  //   %9 = sitofp i32 %1 to float
  Value* val9 = builder.CreateSIToFP(val1, floatType);
  //   %10 = fmul float %9, 5.000000e-01
  Value* val10 = builder.CreateFMul(val9, ConstantFP::get(floatType, 0.5));
  //   %11 = sitofp i32 %2 to float
  Value* val11 = builder.CreateSIToFP(val2, floatType);
  //   %12 = fmul float %11, 5.000000e-01
  Value* val12 = builder.CreateFMul(val11, ConstantFP::get(floatType, 0.5));
  //   %13 = zext nneg i32 %2 to i64
  Value* val13 = builder.CreateZExt(val2, int64Type, "", true);
  //   %14 = zext nneg i32 %1 to i64
  Value* val14 = builder.CreateZExt(val1, int64Type, "", true);
  //   br label %15
  builder.CreateBr(BB15);

  // 15:                                               ; preds = %7, %29
  builder.SetInsertPoint(BB15);
  //   %16 = phi i64 [ 0, %7 ], [ %30, %29 ]
  PHINode* val16 = builder.CreatePHI(int64Type, 2);
  //   br i1 %8, label %17, label %29
  builder.CreateCondBr(val8, BB17, BB29);

  // 17:                                               ; preds = %15
  builder.SetInsertPoint(BB17);
  //   %18 = trunc i64 %16 to i32
  Value* val18 = builder.CreateTrunc(val16, int32Type);
  //   %19 = sitofp i32 %18 to float
  Value* val19 = builder.CreateSIToFP(val18, floatType);
  //   %20 = fsub float %19, %12
  Value* val20 = builder.CreateFSub(val19, val12);
  //   %21 = fmul float %20, 3.000000e+00
  Value* val21 = builder.CreateFMul(val20, ConstantFP::get(floatType, 3.0));
  //   %22 = fdiv float %21, %11
  Value* val22 = builder.CreateFDiv(val21, val11);
  //   %23 = fmul float %22, %22
  Value* val23 = builder.CreateFMul(val22, val22);
  //   %24 = trunc i64 %16 to i32
  Value* val24 = builder.CreateTrunc(val16, int32Type);
  //   %25 = mul i32 %24, %1
  Value* val25 = builder.CreateMul(val24, val1);
  //   %26 = zext i32 %25 to i64
  Value* val26 = builder.CreateZExt(val25, int64Type);
  //   %27 = getelementptr i32, ptr %0, i64 %26
  Value* val27 = builder.CreateGEP(int32Type, val0, {val26});
  //   br label %32
  builder.CreateBr(BB32);

  // 28:                                               ; preds = %29, %5
  builder.SetInsertPoint(BB28);
  //   ret void
  builder.CreateRetVoid();

  // 29:                                               ; preds = %57, %15
  builder.SetInsertPoint(BB29);
  //   %30 = add nuw nsw i64 %16, 1
  Value* val30 = builder.CreateAdd(val16, builder.getInt64(1), "", true, true);
  //   %31 = icmp eq i64 %30, %13
  Value* val31 = builder.CreateICmpEQ(val30, val13);
  //   br i1 %31, label %28, label %15, !llvm.loop !5
  builder.CreateCondBr(val31, BB28, BB15);

  // 32:                                               ; preds = %17, %57
  builder.SetInsertPoint(BB32);
  //   %33 = phi i64 [ 0, %17 ], [ %60, %57 ]
  PHINode* val33 = builder.CreatePHI(int64Type, 2);
  //   %34 = trunc i64 %33 to i32
  Value* val34 = builder.CreateTrunc(val33, int32Type);
  //   %35 = sitofp i32 %34 to float
  Value* val35 = builder.CreateSIToFP(val34, floatType);
  //   %36 = fsub float %35, %10
  Value* val36 = builder.CreateFSub(val35, val10);
  //   %37 = fmul float %36, 3.000000e+00
  Value* val37 = builder.CreateFMul(val36, ConstantFP::get(floatType, 3.0));
  //   %38 = fdiv float %37, %9
  Value* val38 = builder.CreateFDiv(val37, val9);
  //   %39 = tail call float @llvm.fmuladd.f32(float %38, float %38, float %23)
  Value* val39 = builder.CreateCall(llvmFmuladdFunc, {val38, val38, val23});
  //   %40 = fcmp olt float %39, 4.000000e+00
  Value* val40 = builder.CreateFCmpOLT(val39, ConstantFP::get(floatType, 4.0));
  //   br i1 %40, label %41, label %57
  builder.CreateCondBr(val40, BB41, BB57);

  // 41:                                               ; preds = %32, %41
  builder.SetInsertPoint(BB41);
  //   %42 = phi float [ %52, %41 ], [ %23, %32 ]
  PHINode* val42 = builder.CreatePHI(floatType, 2);
  //   %43 = phi i32 [ %51, %41 ], [ 0, %32 ]
  PHINode* val43 = builder.CreatePHI(int32Type, 2);
  //   %44 = phi float [ %50, %41 ], [ %22, %32 ]
  PHINode* val44 = builder.CreatePHI(floatType, 2);
  //   %45 = phi float [ %48, %41 ], [ %38, %32 ]
  PHINode* val45 = builder.CreatePHI(floatType, 2);
  //   %46 = fneg float %42
  Value* val46 = builder.CreateFNeg(val42);
  //   %47 = tail call float @llvm.fmuladd.f32(float %45, float %45, float %46)
  Value* val47 = builder.CreateCall(llvmFmuladdFunc, {val45, val45, val46});
  //   %48 = fadd float %47, %3
  Value* val48 = builder.CreateFAdd(val47, val3);
  //   %49 = fmul float %45, 2.000000e+00
  Value* val49 = builder.CreateFMul(val45, ConstantFP::get(floatType, 2.0));
  //   %50 = tail call float @llvm.fmuladd.f32(float %49, float %44, float %4)
  Value* val50 = builder.CreateCall(llvmFmuladdFunc, {val49, val44, val4});
  //   %51 = add nuw nsw i32 %43, 1
  Value* val51 = builder.CreateAdd(val43, builder.getInt32(1), "", true, true);
  //   %52 = fmul float %50, %50
  Value* val52 = builder.CreateFMul(val50, val50);
  //   %53 = tail call float @llvm.fmuladd.f32(float %48, float %48, float %52)
  Value* val53 = builder.CreateCall(llvmFmuladdFunc, {val48, val48, val52});
  //   %54 = fcmp olt float %53, 4.000000e+00
  Value* val54 = builder.CreateFCmpOLT(val53, ConstantFP::get(floatType, 4.0));
  //   %55 = icmp ult i32 %43, 254
  Value* val55 = builder.CreateICmpULT(val43, builder.getInt32(254));
  //   %56 = select i1 %54, i1 %55, i1 false
  Value* val56 = builder.CreateSelect(val54, val55, builder.getInt1(false));
  //   br i1 %56, label %41, label %57, !llvm.loop !7
  builder.CreateCondBr(val56, BB41, BB57);

  // 57:                                               ; preds = %41, %32
  builder.SetInsertPoint(BB57);
  //   %58 = phi i32 [ 0, %32 ], [ %51, %41 ]
  PHINode* val58 = builder.CreatePHI(int32Type, 2);
  //   %59 = getelementptr i32, ptr %27, i64 %33
  Value* val59 = builder.CreateGEP(int32Type, val27, {val33});
  //   store i32 %58, ptr %59, align 4, !tbaa !8
  builder.CreateStore(val58, val59);
  //   %60 = add nuw nsw i64 %33, 1
  Value* val60 = builder.CreateAdd(val33, builder.getInt64(1), "", true, true);
  //   %61 = icmp eq i64 %60, %14
  Value* val61 = builder.CreateICmpEQ(val60, val14);
  //   br i1 %61, label %29, label %32, !llvm.loop !12
  builder.CreateCondBr(val61, BB29, BB32);

  // link PHI-nodes
  //   %16 = phi i64 [ 0, %7 ], [ %30, %29 ]
  val16->addIncoming(builder.getInt64(0), BB7);
  val16->addIncoming(val30, BB29);
  //   %33 = phi i64 [ 0, %17 ], [ %60, %57 ]
  val33->addIncoming(builder.getInt64(0), BB17);
  val33->addIncoming(val60, BB57);
  //   %42 = phi float [ %52, %41 ], [ %23, %32 ]
  val42->addIncoming(val52, BB41);
  val42->addIncoming(val23, BB32);
  // %43 = phi i32 [ %51, %41 ], [ 0, %32 ]
  val43->addIncoming(val51, BB41);
  val43->addIncoming(builder.getInt32(0), BB32);
  //   %44 = phi float [ %50, %41 ], [ %22, %32 ]
  val44->addIncoming(val50, BB41);
  val44->addIncoming(val22, BB32);
  //   %45 = phi float [ %48, %41 ], [ %38, %32 ]
  val45->addIncoming(val48, BB41);
  val45->addIncoming(val38, BB32);
  //   %58 = phi i32 [ 0, %32 ], [ %51, %41 ]
  val58->addIncoming(builder.getInt32(0), BB32);
  val58->addIncoming(val51, BB41);

  return computeJuliaFunc;
}

Function* defineAppFunc(LLVMContext& context, IRBuilder<>& builder, Module* module) {
  // types
  Type *voidType = Type::getVoidTy(context);
  Type *int32Type = Type::getInt32Ty(context);
  Type* int64Type = Type::getInt64Ty(context);
  Type* ptrType = PointerType::get(context, 0);
  Type* floatType = Type::getFloatTy(context);
  Type* twoFloatsVectorType = VectorType::get(floatType, ElementCount::getFixed(2));
  Type* int1Type = Type::getInt1Ty(context);
  Type* fourBoolsVectorType = VectorType::get(int1Type, ElementCount::getFixed(4));

  // declare void @simPutPixel(i32, i32, i32)
  ArrayRef<Type *> simPutPixelParamTypes = {int32Type, int32Type, int32Type};
  FunctionType *simPutPixelType =
      FunctionType::get(voidType, simPutPixelParamTypes, false);
  FunctionCallee simPutPixelFunc =
      module->getOrInsertFunction("simPutPixel", simPutPixelType);

  // declare void @simFlush()
  FunctionType *simFlushType = FunctionType::get(voidType, false);
  FunctionCallee simFlushFunc =
      module->getOrInsertFunction("simFlush", simFlushType);

  // declare void @llvm.lifetime.start.p0(i64 immarg, ptr nocapture)
  FunctionType* llvmLifetimeStartType =
    FunctionType::get(voidType, {int64Type, ptrType}, false);
  FunctionCallee llvmLifetimeStartFunc =
    module->getOrInsertFunction("llvm.lifetime.start.p0", llvmLifetimeStartType);

  // declare void @compute_julia(ptr %0, i32 %1, i32 %2, float %3, float %4)
  FunctionType* computeJuliaType =
    FunctionType::get(voidType, {ptrType, int32Type, int32Type, floatType, floatType}, false);
  FunctionCallee computeJuliaFunc =
    module->getOrInsertFunction("compute_julia", computeJuliaType);

  // define void @app() {
  FunctionType *appFuncType = FunctionType::get(voidType, false);
  Function *appFunc =
      Function::Create(appFuncType, Function::ExternalLinkage, "app", module);

  BasicBlock *BB0 = BasicBlock::Create(context, "", appFunc);
  BasicBlock *BB2 = BasicBlock::Create(context, "", appFunc);
  BasicBlock *BB7 = BasicBlock::Create(context, "", appFunc);
  BasicBlock *BB11 = BasicBlock::Create(context, "", appFunc);
  BasicBlock *BB21 = BasicBlock::Create(context, "", appFunc);
  BasicBlock *BB24 = BasicBlock::Create(context, "", appFunc);
  BasicBlock *BB30 = BasicBlock::Create(context, "", appFunc);
  BasicBlock *BB33 = BasicBlock::Create(context, "", appFunc);

  // 0:
  builder.SetInsertPoint(BB0);
  //   %1 = alloca [73728 x i32], align 16
  Value* val1 = builder.CreateAlloca(int32Type, builder.getInt32(73728));
  //   call void @llvm.lifetime.start.p0(i64 294912, ptr nonnull %1) #5
  builder.CreateCall(llvmLifetimeStartFunc, {builder.getInt64(294912), val1});
  //   br label %2
  builder.CreateBr(BB2);

  // 2:                                                ; preds = %11, %0
  builder.SetInsertPoint(BB2);
  //   %3 = phi <2 x float> [ <float 0x3F589374C0000000, float 0x3F60624DE0000000>, %0 ], [ %20, %11 ]
  PHINode *val3 = builder.CreatePHI(twoFloatsVectorType, 2);
  //   %4 = phi <2 x float> [ <float 0x3FD14A2340000000, float 0xBFE6666660000000>, %0 ], [ %12, %11 ]
  PHINode *val4 = builder.CreatePHI(twoFloatsVectorType, 2);
  //   %5 = extractelement <2 x float> %4, i64 0
  Value* val5 = builder.CreateExtractElement(val4, builder.getInt64(0));
  //   %6 = extractelement <2 x float> %4, i64 1
  Value* val6 = builder.CreateExtractElement(val4, builder.getInt64(1));
  //   call void @compute_julia(ptr noundef nonnull %1, i32 noundef 384, i32 noundef 192, float noundef %6, float noundef %5) #6
  builder.CreateCall(computeJuliaFunc, {val1, builder.getInt32(384), builder.getInt32(192), val6, val5});
  //   br label %7
  builder.CreateBr(BB7);

  // 7:                                                ; preds = %2, %21
  builder.SetInsertPoint(BB7);
  //   %8 = phi i64 [ 0, %2 ], [ %22, %21 ]
  PHINode *val8 = builder.CreatePHI(int64Type, 2);
  //   %9 = mul nuw nsw i64 %8, 384
  Value* val9 = builder.CreateMul(val8, builder.getInt64(384), "", true, true);
  //   %10 = trunc i64 %8 to i32
  Value* val10 = builder.CreateTrunc(val8, int32Type);
  //   br label %24
  builder.CreateBr(BB24);

  // 11:                                               ; preds = %21
  builder.SetInsertPoint(BB11);
  //   tail call void (...) @simFlush() #7
  builder.CreateCall(simFlushFunc);
  //   %12 = fadd <2 x float> %3, %4
  Value* val12 = builder.CreateFAdd(val3, val4);
  //   %13 = shufflevector <2 x float> %12, <2 x float> poison, <4 x i32> <i32 0, i32 1, i32 0, i32 1>
  Value* val13 = builder.CreateShuffleVector(val12, PoisonValue::get(twoFloatsVectorType), {0, 1, 0, 1});
  //   %14 = fneg <2 x float> %3
  Value* val14 = builder.CreateFNeg(val3);
  //   %15 = fcmp olt <4 x float> %13, <float 0x3FC99999A0000000, float 0xBFE99999A0000000, float 0x3FD6666660000000, float 0xBFE3333340000000>
  Value* val15 = builder.CreateFCmpOLT(val13, ConstantVector::get({
    ConstantFP::get(floatType, hexToDouble(0x3FC99999A0000000)),
    ConstantFP::get(floatType, hexToDouble(0xBFE99999A0000000)),
    ConstantFP::get(floatType, hexToDouble(0x3FD6666660000000)),
    ConstantFP::get(floatType, hexToDouble(0xBFE3333340000000))})
  );
  //   %16 = fcmp ogt <4 x float> %13, <float 0x3FC99999A0000000, float 0xBFE99999A0000000, float 0x3FD6666660000000, float 0xBFE3333340000000>
  Value* val16 = builder.CreateFCmpOGT(val13, ConstantVector::get({
    ConstantFP::get(floatType, hexToDouble(0x3FC99999A0000000)),
    ConstantFP::get(floatType, hexToDouble(0xBFE99999A0000000)),
    ConstantFP::get(floatType, hexToDouble(0x3FD6666660000000)),
    ConstantFP::get(floatType, hexToDouble(0xBFE3333340000000))})
  );
  //   %17 = shufflevector <4 x i1> %16, <4 x i1> poison, <2 x i32> <i32 2, i32 3>
  Value* val17 = builder.CreateShuffleVector(val16, PoisonValue::get(fourBoolsVectorType), {2, 3});
  //   %18 = shufflevector <4 x i1> %15, <4 x i1> poison, <2 x i32> <i32 0, i32 1>
  Value* val18 = builder.CreateShuffleVector(val15, PoisonValue::get(fourBoolsVectorType), {0, 1});
  //   %19 = or <2 x i1> %17, %18
  Value* val19 = builder.CreateOr(val17, val18);
  //   %20 = select <2 x i1> %19, <2 x float> %14, <2 x float> %3
  Value* val20 = builder.CreateSelect(val19, val14, val3);
  //   br label %2
  builder.CreateBr(BB2);

  // 21:                                               ; preds = %33
  builder.SetInsertPoint(BB21);
  //   %22 = add nuw nsw i64 %8, 1
  Value* val22 = builder.CreateAdd(val8, builder.getInt64(1), "", true, true);
  //   %23 = icmp eq i64 %22, 192
  Value* val23 = builder.CreateICmpEQ(val22, builder.getInt64(192));
  //   br i1 %23, label %11, label %7, !llvm.loop !13
  builder.CreateCondBr(val23, BB11, BB7);

  // 24:                                               ; preds = %7, %33
  builder.SetInsertPoint(BB24);
  //   %25 = phi i64 [ 0, %7 ], [ %36, %33 ]
  PHINode *val25 = builder.CreatePHI(int64Type, 2);
  //   %26 = add nuw nsw i64 %25, %9
  Value* val26 = builder.CreateAdd(val25, val9, "", true, true);
  //   %27 = getelementptr inbounds [73728 x i32], ptr %1, i64 0, i64 %26
  Value* val27 = builder.CreateInBoundsGEP(ArrayType::get(int32Type, 73728), val1, {builder.getInt64(0), val26});
  //   %28 = load i32, ptr %27, align 4, !tbaa !8
  Value* val28 = builder.CreateAlignedLoad(int32Type, val27, MaybeAlign(4));
  //   %29 = icmp eq i32 %28, 255
  Value* val29 = builder.CreateICmpEQ(val28, builder.getInt32(255));
  //   br i1 %29, label %33, label %30
  builder.CreateCondBr(val29, BB33, BB30);

  // 30:                                               ; preds = %24
  builder.SetInsertPoint(BB30);
  //   %31 = mul nsw i32 %28, 9
  Value* val31 = builder.CreateNSWMul(val28, builder.getInt32(9));
  //   %32 = srem i32 %31, 256
  Value* val32 = builder.CreateSRem(val31, builder.getInt32(256));
  //   br label %33
  builder.CreateBr(BB33);

  // 33:                                               ; preds = %24, %30
  builder.SetInsertPoint(BB33);
  //   %34 = phi i32 [ %32, %30 ], [ 0, %24 ]
  PHINode *val34 = builder.CreatePHI(int32Type, 2);
  //   %35 = trunc i64 %25 to i32
  Value* val35 = builder.CreateTrunc(val25, int32Type);
  //   tail call void @simPutPixel(i32 noundef %35, i32 noundef %10, i32 noundef %34) #7
  builder.CreateCall(simPutPixelFunc, {val35, val10, val34});
  //   %36 = add nuw nsw i64 %25, 1
  Value* val36 = builder.CreateAdd(val25, builder.getInt64(1), "", true, true);
  //   %37 = icmp eq i64 %36, 384
  Value* val37 = builder.CreateICmpEQ(val36, builder.getInt64(384));
  //   br i1 %37, label %21, label %24, !llvm.loop !14
  builder.CreateCondBr(val37, BB21, BB24);

  // Link PHI nodes
  //   %3 = phi <2 x float> [ <float 0x3F589374C0000000, float 0x3F60624DE0000000>, %0 ], [ %20, %11 ]
  val3->addIncoming(ConstantVector::get({
    ConstantFP::get(floatType, hexToDouble(0x3F589374C0000000)),
    ConstantFP::get(floatType, hexToDouble(0x3F60624DE0000000))}), BB0);
  val3->addIncoming(val20, BB11);
  //   %4 = phi <2 x float> [ <float 0x3FD14A2340000000, float 0xBFE6666660000000>, %0 ], [ %12, %11 ]
  val4->addIncoming(ConstantVector::get({
    ConstantFP::get(floatType, hexToDouble(0x3FD14A2340000000)),
    ConstantFP::get(floatType, hexToDouble(0xBFE6666660000000))}), BB0);
  val4->addIncoming(val12, BB11);
  //   %8 = phi i64 [ 0, %2 ], [ %22, %21 ]
  val8->addIncoming(builder.getInt64(0), BB2);
  val8->addIncoming(val22, BB21);
  //   %25 = phi i64 [ 0, %7 ], [ %36, %33 ]
  val25->addIncoming(builder.getInt64(0), BB7);
  val25->addIncoming(val36, BB33);
  //   %34 = phi i32 [ %32, %30 ], [ 0, %24 ]
  val34->addIncoming(val32, BB30);
  val34->addIncoming(builder.getInt32(0), BB24);

  return appFunc;
}

int main() {
  LLVMContext context;
  // ; ModuleID = 'app.c'
  // source_filename = "app.c"
  Module *module = new Module("app.c", context);
  IRBuilder<> builder(context);

  auto computeJuliaFunc = defineComputeJuliaFunc(context, builder, module);
  auto appFunc = defineAppFunc(context, builder, module);

  // Dump LLVM IR
  module->print(outs(), nullptr);
  outs() << '\n';
  bool verif = verifyModule(*module, &outs());
  outs() << "[VERIFICATION] " << (verif ? "FAIL\n\n" : "OK\n\n");

  // LLVM IR Interpreter
  outs() << "[EE] Run\n";
  InitializeNativeTarget();
  InitializeNativeTargetAsmPrinter();

  ExecutionEngine *ee = EngineBuilder(std::unique_ptr<Module>(module)).create();
  ee->InstallLazyFunctionCreator([](const std::string &fnName) -> void * {
    if (fnName == "simPutPixel") {
      return reinterpret_cast<void *>(simPutPixel);
    }
    if (fnName == "simFlush") {
      return reinterpret_cast<void *>(simFlush);
    }
    outs() << "[ExecutionEngine] Can't find function " << fnName
           << ". Catch the Segmentation fault:)\n";
    return nullptr;
  });
  ee->finalizeObject();

  simInit();

  ArrayRef<GenericValue> noargs;
  GenericValue v = ee->runFunction(appFunc, noargs);
  outs() << "[EE] Result: " << v.IntVal << '\n';

  simExit();
  return EXIT_SUCCESS;
}