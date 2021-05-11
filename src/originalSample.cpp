/*
 * @Description:
 * @Author: Fishermanykx
 * @Date: 2021-05-11 10:01:12
 * @LastEditors: Fishermanykx
 * @LastEditTime: 2021-05-11 10:11:30
 */
#include "ProjHeaders.h"

using namespace llvm;

namespace {
struct OriginalSample : public FunctionPass {
  static char ID;
  OriginalSample() : FunctionPass(ID) {}

  bool runOnFunction(Function &CurFunc) override {
    bool hasChanged = false;

    LLVMContext &Ctx = CurFunc.getContext();
    FunctionCallee ICmpFunc = CurFunc.getParent()->getOrInsertFunction(
        "coverage", Type::getVoidTy(Ctx));
    FunctionCallee InitFunc =
        CurFunc.getParent()->getOrInsertFunction("init", Type::getVoidTy(Ctx));

    for (auto bb = CurFunc.begin(); bb != CurFunc.end(); ++bb) {
      for (auto ii = bb->begin(); ii != bb->end(); ++ii) {
        IRBuilder<> builder(&*ii);
        builder.SetInsertPoint(&*bb, ++builder.GetInsertPoint());

        if (ii == bb->begin() && bb == CurFunc.begin()) {
          builder.CreateCall(InitFunc);
        } else if (ICmpInst *op = dyn_cast<ICmpInst>(&*ii)) {
          builder.CreateCall(ICmpFunc);
        }
      }
    }

    hasChanged |= true;
    return hasChanged;
  }
};
}  // namespace

char OriginalSample::ID = 0;

// Register for opt
static RegisterPass<OriginalSample> X("originalSample", "Original Sample Pass");

// Register for clang
static RegisterStandardPasses Y(PassManagerBuilder::EP_EarlyAsPossible,
                                [](const PassManagerBuilder &Builder,
                                   legacy::PassManagerBase &PM) {
                                  PM.add(new OriginalSample());
                                });
