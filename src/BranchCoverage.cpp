/*
 * @Description: 分支覆盖率
 * @Author: Fishermanykx
 * @Date: 2021-05-03 09:28:02
 * @LastEditors: Fishermanykx
 * @LastEditTime: 2021-05-11 10:00:55
 */

#include "ProjHeaders.h"

using namespace llvm;

namespace {
struct BranchCoverage : public FunctionPass {
  static char ID;
  BranchCoverage() : FunctionPass(ID) {}

  bool runOnFunction(Function &curFunc) override {
    bool hasChanged = false;
    errs() << "In function: " << curFunc.getName().str() << "\n";

    LLVMContext &context = curFunc.getContext();
    // Declaration of External Functions to be inserted
    FunctionCallee InitFunc = curFunc.getParent()->getOrInsertFunction(
        "Init", Type::getVoidTy(context));
    FunctionCallee IcmpFunc = curFunc.getParent()->getOrInsertFunction(
        "calCoverage", Type::getVoidTy(context));

    for (BasicBlock &bb : curFunc) {
      Instruction *terminatorInst = bb.getTerminator();
      // // for insert
      // IRBuilder<> builder(&*bb.begin());
      // builder.SetInsertPoint(&*(&bb), ++builder.GetInsertPoint());
      // builder.CreateCall(InitFunc);

      for (Instruction &ii : bb) {
        auto *curInst = dyn_cast<BranchInst>(&ii);
        if (!curInst) continue;
        if (!curInst->isConditional()) continue;

        auto *cond = curInst->getCondition();
        auto cmpInst = dyn_cast<ICmpInst>(cond);
        if (!cmpInst) continue;

        // IRBuilder<> cmpBuilder(&*(&ii));
        // // Insert after ii
        // cmpBuilder.SetInsertPoint(&*(&bb), ++cmpBuilder.GetInsertPoint());
        // cmpBuilder.CreateCall(IcmpFunc);
        // std::cout << "insert function called" << std::endl;
      }
    }

    hasChanged = true;

    return hasChanged;
  }
};
}  // namespace

char BranchCoverage::ID = 0;
static RegisterPass<BranchCoverage> X("BranchCoverage",
                                      "Branch Coverage calculation pass");

// Register for clang
static RegisterStandardPasses Y(PassManagerBuilder::EP_EarlyAsPossible,
                                [](const PassManagerBuilder &Builder,
                                   legacy::PassManagerBase &PM) {
                                  PM.add(new BranchCoverage());
                                });