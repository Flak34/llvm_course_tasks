#ifndef ASM_COMMON_H
#define ASM_COMMON_H

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <limits>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "llvm/IR/Constants.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"

struct Operand {
  bool isConst = false;
  bool isFloat = false;
  int idxInReg = 0;
  int64_t i = 0;
  double f = 0.0;
};

struct FuncInfo {
  llvm::Function *fn = nullptr;
  std::vector<int> argIdx;
  std::vector<bool> argFloat;
  std::vector<std::string> labels;
  std::map<std::string, llvm::BasicBlock *> bbs;
};

static constexpr int REG_COUNT = 101;

static llvm::LLVMContext ctx;
static std::unique_ptr<llvm::Module> mod;
static std::unique_ptr<llvm::IRBuilder<>> B;
static std::unique_ptr<llvm::IRBuilder<>> EB;
static llvm::Type *VoidT, *I32T, *I64T, *F64T, *PtrT;
static llvm::ArrayType *IRGTy, *FRGTy;
static llvm::GlobalVariable *IRG, *FRG;

static llvm::Value *regAddr(int idx, bool f) {
  return B->CreateConstGEP2_32(f ? FRGTy : IRGTy, f ? FRG : IRG, 0, idx);
}

static llvm::Value *loadIntFromReg(const Operand &o) {
  if (o.isConst)
    return llvm::ConstantInt::get(I64T, o.i);
  return B->CreateLoad(I64T, regAddr(o.idxInReg, false));
}

static llvm::Value *loadDoubleFromReg(const Operand &o) {
  if (o.isConst)
    return llvm::ConstantFP::get(F64T, o.f);
  return B->CreateLoad(F64T, regAddr(o.idxInReg, true));
}

static void storeIntToReg(int idx, llvm::Value *v) {
  B->CreateStore(v, regAddr(idx, false));
}

static void storeDoubleToReg(int idx, llvm::Value *v) {
  B->CreateStore(v, regAddr(idx, true));
}

static std::ifstream in;
static std::vector<std::string> pend;
static std::map<std::string, FuncInfo> funcs;

static std::string tok() {
  if (!pend.empty()) {
    std::string s = pend.back();
    pend.pop_back();
    return s;
  }
  std::string s;
  while (in >> s) {
    if (s.front() == ';') {
      in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      continue;
    }
    break;
  }
  return s;
}

static std::string tokc() {
  std::string s = tok();
  while (!s.empty() && s.back() == ',') s.pop_back();
  return s;
}

static std::string peekc() {
  pend.push_back(tokc());
  return pend.back();
}

static Operand parseOperand(const std::string &s) {
  Operand o;
  if (s.rfind("IR", 0) == 0) {
    o.idxInReg = std::stoi(s.substr(2));
    return o;
  }
  if (s.rfind("FR", 0) == 0) {
    o.idxInReg = std::stoi(s.substr(2));
    o.isFloat = true;
    return o;
  }
  o.isConst = true;
  if (s.find('.') != std::string::npos) {
    o.isFloat = true;
    o.f = std::stod(s);
  } else if (s.rfind("0x", 0) == 0) {
    o.isFloat = true;
    uint64_t bits = std::stoull(s, nullptr, 16);
    memcpy(&o.f, &bits, sizeof(o.f));
  } else {
    o.i = std::stoll(s);
  }
  return o;
}

static Operand getOperand() { return parseOperand(tokc()); }

static const std::map<std::string, int> ARITY = {
    {"MOV", 2},        {"ADD", 3},        {"SUB", 3},       {"MUL", 3},
    {"DIV", 3},        {"NEG", 2},        {"OR", 3},        {"AND", 3},
    {"SREM", 3},       {"SITOFP", 2},     {"CMP_EQ", 3},    {"CMP_GT", 3},
    {"CMP_LT", 3},     {"SELECT", 4},     {"MULADD", 4},    {"MULSUB", 4},
    {"MUL2ADD", 4},    {"INC_EQ", 3},     {"FLUSH", 0},     {"RET", 0},
    {"LOAD_SCALE", 4}, {"STORE_SCALE", 4}, {"ALLOCI", 2},   {"JMP", 1},
    {"JNZ", 2},        {"PUT_PIXEL", 3}};

static const std::set<std::string> BIN_OPS = {
    "ADD", "SUB", "MUL", "DIV", "OR", "AND", "SREM", "CMP_EQ", "CMP_GT",
    "CMP_LT"};

static const std::set<std::string> MUL_OPS = {"MULADD", "MULSUB", "MUL2ADD"};

static std::vector<std::string> split(const std::string &s, char sep) {
  std::vector<std::string> out;
  size_t pos = 0;
  while (true) {
    size_t next = s.find(sep, pos);
    if (next == std::string::npos) {
      out.push_back(s.substr(pos));
      break;
    }
    out.push_back(s.substr(pos, next - pos));
    pos = next + 1;
  }
  return out;
}

static std::string readFuncHeader(std::vector<int> &ai, std::vector<bool> &af) {
  std::string decl;
  while (true) {
    std::string t = tok();
    if (t.empty())
      break;
    decl += t;
    if (t.find(')') != std::string::npos)
      break;
  }
  size_t p1 = decl.find('(');
  std::string name = decl.substr(0, p1);
  std::string args = decl.substr(p1 + 1, decl.find(')') - p1 - 1);
  if (!args.empty())
    for (const std::string &a : split(args, ',')) {
      Operand o = parseOperand(a);
      ai.push_back(o.idxInReg);
      af.push_back(o.isFloat);
    }
  return name;
}

[[noreturn]] static void exitWithError(const std::string &msg) {
  llvm::errs() << "[ERROR] " << msg << '\n';
  exit(1);
}

#endif
