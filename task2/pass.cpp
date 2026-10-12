#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
using namespace llvm;

struct MyModPass : PassInfoMixin<MyModPass> {
  Type *voidType;
  Type *int8PtrTy;
  Type *int32Ty;
  Type *int64Ty;

  static bool isFuncLogger(const StringRef name) {
    return name == "instructionLogger";
  }

  static bool needLogUser(const Instruction& inst) {
    if (inst.isTerminator()) {
      return false;
    }

    if (isa<PHINode>(&inst)) {
      return false;
    }

    if (isa<CallInst>(&inst) && !isa<IntrinsicInst>(&inst)) {
      return false;
    }

    return true;
  }

  static std::vector<std::string> getOperandsName(const Use& use, IRBuilder<>& builder) {
    if (auto inst = dyn_cast<Instruction>(&use)) {
      if (auto call = dyn_cast<CallInst>(&use)) {
        return {call->getCalledFunction()->getName().str()};
      }

      if (auto phi = dyn_cast<PHINode>(&use)) {
        std::vector<std::string> res;
        for (auto& op : phi->operands()) {
          auto ops = getOperandsName(op, builder);
          res.insert(res.end(), ops.begin(), ops.end());
        }
        return res;
      }

      return {inst->getOpcodeName()};
    }

    if (isa<Constant>(&use)) {
      return {"<const>"};
    }

    if (isa<Argument>(&use)) {
      return {"<argument const>"};
    }

    use->print(outs());

    return {"<unknown operand>"};
  }

  void insertLog(Module &M, Function &F, IRBuilder<> &builder) const {
    ArrayRef instructionLoggerParamTypes = {int8PtrTy};
    FunctionType *instructionLoggerFuncType =
        FunctionType::get(voidType, instructionLoggerParamTypes, false);
    FunctionCallee binOptLogFunc =
        M.getOrInsertFunction("instructionLogger", instructionLoggerFuncType);

    for (auto &B : F) {
      for (auto &I : B) {
        if (!needLogUser(I)) {
          continue;
        }

        std::string userName = I.getOpcodeName();
        if (auto call = dyn_cast<CallInst>(&I)) {
          userName = call->getCalledFunction()->getName().str();
        }

        builder.SetInsertPoint(&I);
        std::string log;

        for (auto &use : I.operands()) {
          auto opNames = getOperandsName(use, builder);
          for (const auto& op : opNames) {
            log.append(userName + " <- " + op + "\n");
          }
        }

        Value *args[] = {builder.CreateGlobalStringPtr(log)};
        builder.CreateCall(binOptLogFunc, args);
      }
    }
  }

  PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {
    outs() << "[Module] " << M.getName() << '\n';

    LLVMContext &Ctx = M.getContext();
    IRBuilder<> builder(Ctx);
    voidType = Type::getVoidTy(Ctx);
    int8PtrTy = Type::getInt8Ty(Ctx)->getPointerTo();
    int32Ty = Type::getInt32Ty(Ctx);
    int64Ty = Type::getInt64Ty(Ctx);

    for (auto &F : M) {
      if (isFuncLogger(F.getName()) || F.isDeclaration()) {
        continue;
      }

      outs() << "[Function] " << F.getName() << " (arg_size: " << F.arg_size() << ")\n";

      insertLog(M, F, builder);

      outs() << '\n';
      bool verif = verifyFunction(F, &outs());
      outs() << "[VERIFICATION] " << (verif ? "FAIL\n\n" : "OK\n\n");
    }
    outs() << '\n';
    return PreservedAnalyses::none();
  };
};

PassPluginLibraryInfo getPassPluginInfo() {
  const auto callback = [](PassBuilder &PB) {
    PB.registerOptimizerLastEPCallback([](ModulePassManager &MPM, auto) {
    // PB.registerPipelineStartEPCallback([](ModulePassManager &MPM, auto) {
      MPM.addPass(MyModPass{});
      return true;
    });
  };

  return {LLVM_PLUGIN_API_VERSION, "MyPlugin", "0.0.1", callback};
};

/* When a plugin is loaded by the driver, it will call this entry point to
obtain information about this plugin and about how to register its passes.
*/
extern "C" LLVM_ATTRIBUTE_WEAK PassPluginLibraryInfo llvmGetPassPluginInfo() {
  return getPassPluginInfo();
}
