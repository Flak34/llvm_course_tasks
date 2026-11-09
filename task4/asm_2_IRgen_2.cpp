#include "sim.h"
#include "common.h"

#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include <memory>

using namespace llvm;

static FunctionCallee simPutPixelFunc, simFlushFunc;

Value *emitCmp(bool f, Value *x, Value *y, CmpInst::Predicate ip,
               CmpInst::Predicate fp) {
  Value *c = f ? B->CreateFCmp(fp, x, y) : B->CreateICmp(ip, x, y);
  return B->CreateZExt(c, I64T);
}

Value *emitBinOp(const std::string &s, bool f, Value *x, Value *y) {
  if (s == "ADD")
    return f ? B->CreateFAdd(x, y) : B->CreateAdd(x, y);
  if (s == "SUB")
    return f ? B->CreateFSub(x, y) : B->CreateSub(x, y);
  if (s == "MUL")
    return f ? B->CreateFMul(x, y) : B->CreateMul(x, y);
  if (s == "DIV")
    return f ? B->CreateFDiv(x, y) : B->CreateSDiv(x, y);
  if (s == "OR")
    return B->CreateOr(x, y);
  if (s == "AND")
    return B->CreateAnd(x, y);
  if (s == "SREM")
    return B->CreateSRem(x, y);
  exitWithError("Unknown binary operation: " + s);
}

Value *emitCmpOp(const std::string &s, bool f, Value *x, Value *y) {
  if (s == "CMP_EQ")
    return emitCmp(f, x, y, CmpInst::ICMP_EQ, CmpInst::FCMP_OEQ);
  if (s == "CMP_GT")
    return emitCmp(f, x, y, CmpInst::ICMP_SGT, CmpInst::FCMP_OGT);
  if (s == "CMP_LT")
    return emitCmp(f, x, y, CmpInst::ICMP_SLT, CmpInst::FCMP_OLT);
  exitWithError("Unknown comparison: " + s);
}

Value *emitMulOp(const std::string &s, bool f, Value *x, Value *y, Value *w) {
  Value *two = f ? static_cast<Value *>(ConstantFP::get(F64T, 2.0))
                 : static_cast<Value *>(ConstantInt::get(I64T, 2));
  if (s == "MULADD")
    return f ? B->CreateFAdd(B->CreateFMul(x, y), w)
             : B->CreateAdd(B->CreateMul(x, y), w);
  if (s == "MULSUB")
    return f ? B->CreateFSub(B->CreateFMul(x, y), w)
             : B->CreateSub(B->CreateMul(x, y), w);
  if (s == "MUL2ADD")
    return f ? B->CreateFAdd(B->CreateFMul(B->CreateFMul(two, x), y), w)
             : B->CreateAdd(B->CreateMul(B->CreateMul(two, x), y), w);
  exitWithError("Unknown multiply-add operation: " + s);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    outs() << "[ERROR] Need 1 argument: file with assembler code\n";
    return 1;
  }
  in.open(argv[1]);
  if (!in.is_open()) {
    outs() << "[ERROR] Can't open " << argv[1] << '\n';
    return 1;
  }

  mod = std::make_unique<Module>("top", ctx);
  B = std::make_unique<IRBuilder<>>(ctx);
  VoidT = Type::getVoidTy(ctx);
  I32T = Type::getInt32Ty(ctx);
  I64T = Type::getInt64Ty(ctx);
  F64T = Type::getDoubleTy(ctx);
  PtrT = PointerType::get(ctx, 0);
  IRGTy = ArrayType::get(I64T, REG_COUNT);
  FRGTy = ArrayType::get(F64T, REG_COUNT);
  IRG = new GlobalVariable(*mod, IRGTy, false, GlobalValue::PrivateLinkage,
                           ConstantAggregateZero::get(IRGTy), "IR_REG");
  FRG = new GlobalVariable(*mod, FRGTy, false, GlobalValue::PrivateLinkage,
                           ConstantAggregateZero::get(FRGTy), "FR_REG");
  simPutPixelFunc = mod->getOrInsertFunction(
      "simPutPixel", FunctionType::get(VoidT, {I32T, I32T, I32T}, false));
  simFlushFunc =
      mod->getOrInsertFunction("simFlush", FunctionType::get(VoidT, false));

  FuncInfo *curFunc = nullptr;
  for (std::string s = tokc(); !s.empty(); s = tokc()) {
    if (s == "FUNC") {
      FuncInfo fi;
      std::string name = readFuncHeader(fi.argIdx, fi.argFloat);
      std::vector<Type *> ptypes;
      for (bool f : fi.argFloat)
        ptypes.push_back(f ? F64T : I64T);
      fi.fn = Function::Create(FunctionType::get(VoidT, ptypes, false),
                               Function::ExternalLinkage, name, mod.get());
      curFunc = &funcs[name];
      *curFunc = std::move(fi);
      continue;
    }
    if (s.back() == ':') {
      std::string label = s.substr(0, s.size() - 1);
      curFunc->bbs[label] = BasicBlock::Create(ctx, label, curFunc->fn);
      curFunc->labels.push_back(label);
      continue;
    }
    if (s == "CALL") {
      while (true) {
        std::string t = peekc();
        if (t.empty() || t.back() == ':' || ARITY.count(t))
          break;
        tokc();
      }
      continue;
    }
    auto it = ARITY.find(s);
    if (it == ARITY.end())
      exitWithError("Unknown instruction: " + s);
    for (int k = 0; k < it->second; k++)
      tokc();
  }

  in.clear();
  in.close();
  in.open(argv[1]);
  pend.clear();

  BasicBlock *curBB = nullptr;
  curFunc = nullptr;

  for (std::string s = tokc(); !s.empty(); s = tokc()) {
    if (s == "FUNC") {
      std::vector<int> ai;
      std::vector<bool> af;
      std::string name = readFuncHeader(ai, af);
      curFunc = &funcs[name];
      curBB = nullptr;

      EB = nullptr;
      continue;
    }
    if (s.back() == ':') {
      std::string label = s.substr(0, s.size() - 1);
      BasicBlock *bb = curFunc->bbs[label];
      if (curBB && !curBB->getTerminator())
        B->CreateBr(bb);
      B->SetInsertPoint(bb);
      curBB = bb;

      if (!EB) {
        BasicBlock &entry = curFunc->fn->getEntryBlock();
        EB = std::make_unique<IRBuilder<>>(&entry, entry.begin());
        Function::arg_iterator it = curFunc->fn->arg_begin();
        for (size_t i = 0; i < curFunc->argIdx.size(); ++i, ++it)
          B->CreateStore(&*it, regAddr(curFunc->argIdx[i], curFunc->argFloat[i]));
      }
      continue;
    }
    if (curBB->getTerminator()) {
      curBB = BasicBlock::Create(ctx, "unreachable", curFunc->fn);
      B->SetInsertPoint(curBB);
    }
    if (s == "MOV") {
      Operand d = getOperand(), v = getOperand();
      if (d.isFloat)
        storeDoubleToReg(d.idxInReg, loadDoubleFromReg(v));
      else
        storeIntToReg(d.idxInReg, loadIntFromReg(v));
      continue;
    }
    if (BIN_OPS.count(s)) {
      Operand d = getOperand(), a = getOperand(), b = getOperand();
      bool f = a.isFloat || b.isFloat;
      Value *x = f ? loadDoubleFromReg(a) : loadIntFromReg(a);
      Value *y = f ? loadDoubleFromReg(b) : loadIntFromReg(b);
      Value *r = s.compare(0, 3, "CMP") == 0 ? emitCmpOp(s, f, x, y)
                                             : emitBinOp(s, f, x, y);
      if (f && s.compare(0, 3, "CMP") != 0)
        storeDoubleToReg(d.idxInReg, r);
      else
        storeIntToReg(d.idxInReg, r);
      continue;
    }
    if (MUL_OPS.count(s)) {
      Operand d = getOperand(), a = getOperand(), b = getOperand(),
              c = getOperand();
      bool f = a.isFloat || b.isFloat || c.isFloat;
      Value *r = emitMulOp(s, f, f ? loadDoubleFromReg(a) : loadIntFromReg(a), f ? loadDoubleFromReg(b) : loadIntFromReg(b),
                           f ? loadDoubleFromReg(c) : loadIntFromReg(c));
      if (f)
        storeDoubleToReg(d.idxInReg, r);
      else
        storeIntToReg(d.idxInReg, r);
      continue;
    }
    if (s == "NEG") {
      Operand d = getOperand(), a = getOperand();
      if (a.isFloat)
        storeDoubleToReg(d.idxInReg, B->CreateFNeg(loadDoubleFromReg(a)));
      else
        storeIntToReg(d.idxInReg, B->CreateNeg(loadIntFromReg(a)));
      continue;
    }
    if (s == "SELECT") {
      Operand d = getOperand(), c = getOperand(), t = getOperand(),
              fv = getOperand();
      Value *cond =
          B->CreateICmpNE(loadIntFromReg(c), ConstantInt::get(I64T, 0));
      if (d.isFloat)
        storeDoubleToReg(d.idxInReg, B->CreateSelect(cond, loadDoubleFromReg(t), loadDoubleFromReg(fv)));
      else
        storeIntToReg(d.idxInReg, B->CreateSelect(cond, loadIntFromReg(t), loadIntFromReg(fv)));
      continue;
    }
    if (s == "SITOFP") {
      Operand d = getOperand(), v = getOperand();
      storeDoubleToReg(d.idxInReg, B->CreateSIToFP(loadIntFromReg(v), F64T));
      continue;
    }
    if (s == "INC_EQ") {
      Operand c = getOperand(), n = getOperand(), l = getOperand();
      Value *nv = B->CreateAdd(loadIntFromReg(n), ConstantInt::get(I64T, 1));
      storeIntToReg(n.idxInReg, nv);
      storeIntToReg(c.idxInReg,
           emitCmp(false, nv, loadIntFromReg(l), CmpInst::ICMP_EQ, CmpInst::FCMP_OEQ));
      continue;
    }
    if (s == "ALLOCI") {
      Operand d = getOperand(), sz = getOperand();
      Value *ptr = EB->CreateAlloca(ArrayType::get(I64T, sz.i));
      B->CreateStore(EB->CreatePtrToInt(ptr, I64T), regAddr(d.idxInReg, false));
      continue;
    }
    if (s == "LOAD_SCALE") {
      Operand d = getOperand(), p = getOperand(), o = getOperand(),
              sc = getOperand();
      Value *off = B->CreateMul(loadIntFromReg(o), ConstantInt::get(I64T, sc.i));
      Value *addr =
          B->CreateGEP(B->getInt8Ty(), B->CreateIntToPtr(loadIntFromReg(p), PtrT), off);
      storeIntToReg(d.idxInReg, B->CreateLoad(I64T, addr));
      continue;
    }
    if (s == "STORE_SCALE") {
      Operand p = getOperand(), o = getOperand(), sc = getOperand(),
              v = getOperand();
      Value *off = B->CreateMul(loadIntFromReg(o), ConstantInt::get(I64T, sc.i));
      Value *addr =
          B->CreateGEP(B->getInt8Ty(), B->CreateIntToPtr(loadIntFromReg(p), PtrT), off);
      B->CreateStore(loadIntFromReg(v), addr);
      continue;
    }
    if (s == "JMP") {
      std::string label = tokc();
      B->CreateBr(curFunc->bbs[label]);
      continue;
    }
    if (s == "RET") {
      B->CreateRetVoid();
      continue;
    }
    if (s == "JNZ") {
      std::string label = tokc();
      Operand c = getOperand();
      Value *cond = B->CreateICmpNE(loadIntFromReg(c), ConstantInt::get(I64T, 0));
      BasicBlock *cont = BasicBlock::Create(ctx, "cont", curFunc->fn);
      B->CreateCondBr(cond, curFunc->bbs[label], cont);
      B->SetInsertPoint(cont);
      curBB = cont;
      continue;
    }
    if (s == "CALL") {
      std::string name = tokc();
      FuncInfo &cf = funcs[name];
      std::vector<Value *> args;
      for (size_t i = 0; i < cf.argIdx.size(); i++) {
        Operand a = getOperand();
        args.push_back(cf.argFloat[i] ? loadDoubleFromReg(a) : loadIntFromReg(a));
      }
      B->CreateCall(cf.fn, args);
      continue;
    }
    if (s == "PUT_PIXEL") {
      Operand x = getOperand(), y = getOperand(), c = getOperand();
      Value *args[] = {B->CreateTrunc(loadIntFromReg(x), I32T),
                       B->CreateTrunc(loadIntFromReg(y), I32T),
                       B->CreateTrunc(loadIntFromReg(c), I32T)};
      B->CreateCall(simPutPixelFunc, args);
      continue;
    }
    if (s == "FLUSH") {
      B->CreateCall(simFlushFunc);
      continue;
    }
    exitWithError("Unknown instruction: " + s);
  }

  outs() << "\n#[LLVM IR]:\n";
  mod->print(outs(), nullptr);
  bool ok = true;
  for (auto &[name, fi] : funcs) {
    bool bad = verifyFunction(*fi.fn, &outs());
    outs() << "[VERIFICATION] " << name << ": " << (bad ? "FAIL" : "OK")
           << '\n';
    ok = ok && !bad;
  }
  if (!ok)
    return 1;

  outs() << "\n#[Running code]\n";
  InitializeNativeTarget();
  InitializeNativeTargetAsmPrinter();

  ExecutionEngine *ee = EngineBuilder(std::move(mod)).create();
  if (!ee) {
    outs() << "[ERROR] Failed to create ExecutionEngine\n";
    return 1;
  }

  ee->InstallLazyFunctionCreator([](const std::string &name) -> void * {
    if (name == "simPutPixel")
      return reinterpret_cast<void *>(simPutPixel);
    if (name == "simFlush")
      return reinterpret_cast<void *>(simFlush);
    return nullptr;
  });
  ee->finalizeObject();

  simInit();
  ee->runFunction(funcs["app"].fn, {});
  outs() << "#[Code was run]\n";
  simExit();
  return EXIT_SUCCESS;
}
