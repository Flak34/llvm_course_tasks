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

int64_t IR_REG[REG_COUNT];
double FR_REG[REG_COUNT];

void do_MOV(int dst, int src) { IR_REG[dst] = IR_REG[src]; }
void do_MOV_const(int dst, int64_t imm) { IR_REG[dst] = imm; }
void do_FMOV(int dst, int src) { FR_REG[dst] = FR_REG[src]; }
void do_FMOV_const(int dst, double imm) { FR_REG[dst] = imm; }
void do_ADD_const(int dst, int src, int64_t imm) { IR_REG[dst] = IR_REG[src] + imm; }
void do_MUL_const(int dst, int src, int64_t imm) { IR_REG[dst] = IR_REG[src] * imm; }
void do_SREM_const(int dst, int src, int64_t imm) { IR_REG[dst] = IR_REG[src] % imm; }
void do_OR(int dst, int a, int b) { IR_REG[dst] = IR_REG[a] | IR_REG[b]; }
void do_AND(int dst, int a, int b) { IR_REG[dst] = IR_REG[a] & IR_REG[b]; }
void do_CMP_EQ_const(int dst, int src, int64_t imm) { IR_REG[dst] = IR_REG[src] == imm; }
void do_CMP_GT_const(int dst, int src, int64_t imm) { IR_REG[dst] = IR_REG[src] > imm; }
void do_CMP_LT_const(int dst, int src, int64_t imm) { IR_REG[dst] = IR_REG[src] < imm; }
void do_MULADD(int dst, int a, int b, int c) {
  IR_REG[dst] = IR_REG[a] * IR_REG[b] + IR_REG[c];
}
void do_MULADD_const(int dst, int a, int64_t imm, int c) {
  IR_REG[dst] = IR_REG[a] * imm + IR_REG[c];
}
void do_INC_EQ(int cond, int cnt, int limit) {
  IR_REG[cnt]++;
  IR_REG[cond] = IR_REG[cnt] == IR_REG[limit];
}
void do_INC_EQ_const(int cond, int cnt, int64_t limit) {
  IR_REG[cnt]++;
  IR_REG[cond] = IR_REG[cnt] == limit;
}
void do_SITOFP(int dst, int src) { FR_REG[dst] = static_cast<double>(IR_REG[src]); }
void do_LOAD_SCALE(int dst, int p, int idx, int64_t scale) {
  IR_REG[dst] = *reinterpret_cast<int64_t *>(IR_REG[p] + IR_REG[idx] * scale);
}
void do_STORE_SCALE(int p, int idx, int64_t scale, int src) {
  *reinterpret_cast<int64_t *>(IR_REG[p] + IR_REG[idx] * scale) = IR_REG[src];
}
void do_PUT_PIXEL(int x, int y, int c) {
  simPutPixel(static_cast<int>(IR_REG[x]), static_cast<int>(IR_REG[y]),
              static_cast<int>(IR_REG[c]));
}
void do_FLUSH() { simFlush(); }
void do_FADD(int dst, int a, int b) { FR_REG[dst] = FR_REG[a] + FR_REG[b]; }
void do_FSUB(int dst, int a, int b) { FR_REG[dst] = FR_REG[a] - FR_REG[b]; }
void do_FMUL(int dst, int a, int b) { FR_REG[dst] = FR_REG[a] * FR_REG[b]; }
void do_FMUL_const(int dst, int src, double imm) { FR_REG[dst] = FR_REG[src] * imm; }
void do_FDIV(int dst, int a, int b) { FR_REG[dst] = FR_REG[a] / FR_REG[b]; }
void do_FNEG(int dst, int src) { FR_REG[dst] = -FR_REG[src]; }
void do_FMULADD(int dst, int a, int b, int c) {
  FR_REG[dst] = FR_REG[a] * FR_REG[b] + FR_REG[c];
}
void do_FMULSUB(int dst, int a, int b, int c) {
  FR_REG[dst] = FR_REG[a] * FR_REG[b] - FR_REG[c];
}
void do_FMUL2ADD(int dst, int a, int b, int c) {
  FR_REG[dst] = 2 * FR_REG[a] * FR_REG[b] + FR_REG[c];
}
void do_FCMP_LT_const(int dst, int src, double imm) { IR_REG[dst] = FR_REG[src] < imm; }
void do_FCMP_GT_const(int dst, int src, double imm) { IR_REG[dst] = FR_REG[src] > imm; }
void do_SELECT_F(int dst, int cond, int t, int f) {
  FR_REG[dst] = IR_REG[cond] ? FR_REG[t] : FR_REG[f];
}

static const std::map<std::string, void *> doPtrs = {
    {"do_MOV", reinterpret_cast<void *>(do_MOV)},
    {"do_MOV_const", reinterpret_cast<void *>(do_MOV_const)},
    {"do_FMOV", reinterpret_cast<void *>(do_FMOV)},
    {"do_FMOV_const", reinterpret_cast<void *>(do_FMOV_const)},
    {"do_ADD_const", reinterpret_cast<void *>(do_ADD_const)},
    {"do_MUL_const", reinterpret_cast<void *>(do_MUL_const)},
    {"do_SREM_const", reinterpret_cast<void *>(do_SREM_const)},
    {"do_OR", reinterpret_cast<void *>(do_OR)},
    {"do_AND", reinterpret_cast<void *>(do_AND)},
    {"do_CMP_EQ_const", reinterpret_cast<void *>(do_CMP_EQ_const)},
    {"do_CMP_GT_const", reinterpret_cast<void *>(do_CMP_GT_const)},
    {"do_CMP_LT_const", reinterpret_cast<void *>(do_CMP_LT_const)},
    {"do_MULADD", reinterpret_cast<void *>(do_MULADD)},
    {"do_MULADD_const", reinterpret_cast<void *>(do_MULADD_const)},
    {"do_INC_EQ", reinterpret_cast<void *>(do_INC_EQ)},
    {"do_INC_EQ_const", reinterpret_cast<void *>(do_INC_EQ_const)},
    {"do_SITOFP", reinterpret_cast<void *>(do_SITOFP)},
    {"do_LOAD_SCALE", reinterpret_cast<void *>(do_LOAD_SCALE)},
    {"do_STORE_SCALE", reinterpret_cast<void *>(do_STORE_SCALE)},
    {"do_PUT_PIXEL", reinterpret_cast<void *>(do_PUT_PIXEL)},
    {"do_FLUSH", reinterpret_cast<void *>(do_FLUSH)},
    {"do_FADD", reinterpret_cast<void *>(do_FADD)},
    {"do_FSUB", reinterpret_cast<void *>(do_FSUB)},
    {"do_FMUL", reinterpret_cast<void *>(do_FMUL)},
    {"do_FMUL_const", reinterpret_cast<void *>(do_FMUL_const)},
    {"do_FDIV", reinterpret_cast<void *>(do_FDIV)},
    {"do_FNEG", reinterpret_cast<void *>(do_FNEG)},
    {"do_FMULADD", reinterpret_cast<void *>(do_FMULADD)},
    {"do_FMULSUB", reinterpret_cast<void *>(do_FMULSUB)},
    {"do_FMUL2ADD", reinterpret_cast<void *>(do_FMUL2ADD)},
    {"do_FCMP_LT_const", reinterpret_cast<void *>(do_FCMP_LT_const)},
    {"do_FCMP_GT_const", reinterpret_cast<void *>(do_FCMP_GT_const)},
    {"do_SELECT_F", reinterpret_cast<void *>(do_SELECT_F)}};

std::map<std::string, FunctionCallee> doCache;

void callDo(const std::string &name, std::vector<Value *> args) {
  if (!doPtrs.count(name))
    exitWithError("Unsupported do_* call: " + name);
  auto it = doCache.find(name);
  if (it == doCache.end()) {
    std::vector<Type *> ts;
    for (Value *a : args)
      ts.push_back(a->getType());
    it = doCache
             .emplace(name, mod->getOrInsertFunction(
                                name, FunctionType::get(VoidT, ts, false)))
             .first;
  }
  B->CreateCall(it->second, args);
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
  mod->getOrInsertGlobal("IR_REG", IRGTy);
  IRG = mod->getNamedGlobal("IR_REG");
  mod->getOrInsertGlobal("FR_REG", FRGTy);
  FRG = mod->getNamedGlobal("FR_REG");

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
      if (d.isFloat) {
        if (v.isConst)
          callDo("do_FMOV_const",
                 {B->getInt32(d.idxInReg), ConstantFP::get(F64T, v.f)});
        else
          callDo("do_FMOV",
                 {B->getInt32(d.idxInReg), B->getInt32(v.idxInReg)});
      } else {
        if (v.isConst)
          callDo("do_MOV_const",
                 {B->getInt32(d.idxInReg), ConstantInt::get(I64T, v.i)});
        else
          callDo("do_MOV",
                 {B->getInt32(d.idxInReg), B->getInt32(v.idxInReg)});
      }
      continue;
    }
    if (s == "NEG") {
      Operand d = getOperand(), a = getOperand();
      if (a.isConst)
        exitWithError("NEG with constant is not supported");
      callDo("do_FNEG", {B->getInt32(d.idxInReg), B->getInt32(a.idxInReg)});
      continue;
    }
    if (BIN_OPS.count(s)) {
      Operand d = getOperand(), a = getOperand(), b = getOperand();
      if (a.isConst)
        exitWithError(s + " with constant in first operand is not supported");
      bool f = a.isFloat || b.isFloat;
      if (b.isConst) {
        std::string fn = f ? "do_F" + s + "_const" : "do_" + s + "_const";
        Value *imm = f ? static_cast<Value *>(ConstantFP::get(F64T, b.f))
                       : static_cast<Value *>(ConstantInt::get(I64T, b.i));
        callDo(fn, {B->getInt32(d.idxInReg), B->getInt32(a.idxInReg), imm});
      } else {
        std::string fn = f ? "do_F" + s : "do_" + s;
        callDo(fn, {B->getInt32(d.idxInReg), B->getInt32(a.idxInReg),
                    B->getInt32(b.idxInReg)});
      }
      continue;
    }
    if (MUL_OPS.count(s)) {
      Operand d = getOperand(), a = getOperand(), b = getOperand(),
              c = getOperand();
      if (a.isConst || c.isConst)
        exitWithError(s + " with constant in first or last operand is not supported");
      bool f = a.isFloat || b.isFloat || c.isFloat;
      if (b.isConst) {
        if (s != "MULADD")
          exitWithError(s + " with constant is not supported");
        std::string fn = f ? "do_FMULADD_const" : "do_MULADD_const";
        Value *imm = f ? static_cast<Value *>(ConstantFP::get(F64T, b.f))
                       : static_cast<Value *>(ConstantInt::get(I64T, b.i));
        callDo(fn, {B->getInt32(d.idxInReg), B->getInt32(a.idxInReg), imm,
                    B->getInt32(c.idxInReg)});
      } else {
        std::string fn = f ? "do_F" + s : "do_" + s;
        callDo(fn, {B->getInt32(d.idxInReg), B->getInt32(a.idxInReg),
                    B->getInt32(b.idxInReg), B->getInt32(c.idxInReg)});
      }
      continue;
    }
    if (s == "SELECT") {
      Operand d = getOperand(), c = getOperand(), t = getOperand(),
              f = getOperand();
      if (!d.isFloat)
        exitWithError("SELECT with integer destination is not supported");
      callDo("do_SELECT_F",
             {B->getInt32(d.idxInReg), B->getInt32(c.idxInReg),
              B->getInt32(t.idxInReg), B->getInt32(f.idxInReg)});
      continue;
    }
    if (s == "SITOFP") {
      Operand d = getOperand(), v = getOperand();
      if (v.isConst)
        exitWithError("SITOFP with constant is not supported");
      callDo("do_SITOFP",
             {B->getInt32(d.idxInReg), B->getInt32(v.idxInReg)});
      continue;
    }
    if (s == "INC_EQ") {
      Operand c = getOperand(), n = getOperand(), l = getOperand();
      if (l.isConst)
        callDo("do_INC_EQ_const",
               {B->getInt32(c.idxInReg), B->getInt32(n.idxInReg),
                ConstantInt::get(I64T, l.i)});
      else
        callDo("do_INC_EQ",
               {B->getInt32(c.idxInReg), B->getInt32(n.idxInReg),
                B->getInt32(l.idxInReg)});
      continue;
    }
    if (s == "ALLOCI") {
      Operand d = getOperand(), sz = getOperand();
      Value *ptr = EB->CreateAlloca(ArrayType::get(I64T, sz.i));
      storeIntToReg(d.idxInReg, EB->CreatePtrToInt(ptr, I64T));
      continue;
    }
    if (s == "LOAD_SCALE") {
      Operand d = getOperand(), p = getOperand(), o = getOperand(),
              sc = getOperand();
      callDo("do_LOAD_SCALE",
             {B->getInt32(d.idxInReg), B->getInt32(p.idxInReg),
              B->getInt32(o.idxInReg), ConstantInt::get(I64T, sc.i)});
      continue;
    }
    if (s == "STORE_SCALE") {
      Operand p = getOperand(), o = getOperand(), sc = getOperand(),
              v = getOperand();
      callDo("do_STORE_SCALE",
             {B->getInt32(p.idxInReg), B->getInt32(o.idxInReg),
              ConstantInt::get(I64T, sc.i), B->getInt32(v.idxInReg)});
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
      Value *v = B->CreateLoad(I64T, regAddr(c.idxInReg, false));
      Value *cond = B->CreateICmpNE(v, ConstantInt::get(I64T, 0));
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
      callDo("do_PUT_PIXEL", {B->getInt32(x.idxInReg), B->getInt32(y.idxInReg),
                              B->getInt32(c.idxInReg)});
      continue;
    }
    if (s == "FLUSH") {
      callDo("do_FLUSH", {});
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
    auto it = doPtrs.find(name);
    return it != doPtrs.end() ? it->second : nullptr;
  });
  ee->addGlobalMapping(IRG, IR_REG);
  ee->addGlobalMapping(FRG, FR_REG);
  ee->finalizeObject();

  simInit();
  ee->runFunction(funcs["app"].fn, {});
  outs() << "#[Code was run]\n";
  simExit();
  return EXIT_SUCCESS;
}
