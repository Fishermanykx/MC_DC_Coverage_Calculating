/*
 * @Description: 条件覆盖率
 * 最终实现的不是条件覆盖率，只是某种计算形式
 * @Author: Fishermanykx
 * @Date: 2021-05-03 09:28:02
 * @LastEditors: Fishermanykx
 * @LastEditTime: 2021-05-12 16:12:00
 */

#include "ProjHeaders.h"

using namespace llvm;

namespace {
struct ConditionCoverage : public FunctionPass {
  static char ID;
  ConditionCoverage() : FunctionPass(ID) {}

  int totalConditions = 0;
  bool runOnFunction(Function &curFunc) override {
    bool hasChanged = false;

    LLVMContext &context = curFunc.getContext();
    // Declaration of External Functions to be inserted
    FunctionCallee InitFunc = curFunc.getParent()->getOrInsertFunction(
        "init", Type::getVoidTy(context), Type::getInt8PtrTy(context));
    FunctionCallee ICmpFunc = curFunc.getParent()->getOrInsertFunction(
        "calCoverage", Type::getVoidTy(context));
    FunctionCallee ReturnFunc = curFunc.getParent()->getOrInsertFunction(
        "getTotalConditions", Type::getVoidTy(context),
        Type::getInt32Ty(context));

    std::string funcNameStr = curFunc.getName().str();
    char *funcName = (char *)funcNameStr.c_str();

    for (auto bb = curFunc.begin(); bb != curFunc.end(); ++bb) {
      for (auto ii = bb->begin(); ii != bb->end(); ++ii) {
        IRBuilder<> builder(&*ii);

        // Insert Init func in the front of each Function
        if (ii == bb->begin() && bb == curFunc.begin() &&
            funcNameStr == "main") {
          builder.SetInsertPoint(&*bb, ++builder.GetInsertPoint());
          Value *strVal = builder.CreateGlobalStringPtr(funcName);
          Value *callArgs[] = {strVal};

          // printf("Init func called\n");
          builder.CreateCall(InitFunc, callArgs);
          continue;
        }

        // auto *curInst = dyn_cast<BranchInst>(ii);
        // if (!curInst) continue;
        // if (!curInst->isConditional()) continue;

        // auto *cond = curInst->getCondition();
        // ICmpInst *cmpInst = dyn_cast<ICmpInst>(cond);
        // if (!cmpInst) continue;

        if (ICmpInst *op = dyn_cast<ICmpInst>(&*ii)) {
          ++totalConditions;
          builder.SetInsertPoint(&*bb, ++builder.GetInsertPoint());
          builder.CreateCall(ICmpFunc);
        }
      }
    }

    printf("In function %s, total branch num is %d\n", funcName,
           totalConditions);

    // Get terminator basicblock of the funcrtion
    for (auto bb = curFunc.begin(); bb != curFunc.end(); ++bb) {
      for (auto ii = bb->begin(); ii != bb->end(); ++ii) {
        if (ReturnInst *RI = dyn_cast<ReturnInst>(ii)) {
          // bb is a terminator basicblock
          IRBuilder<> termBuilder(&*ii);
          termBuilder.SetInsertPoint(&*bb, termBuilder.GetInsertPoint());
          Value *args[] = {termBuilder.getInt32(totalConditions)};
          termBuilder.CreateCall(ReturnFunc, args);
        }
      }
    }
    hasChanged = true;

    return hasChanged;
  }
};
}  // namespace

char ConditionCoverage::ID = 0;
static RegisterPass<ConditionCoverage> X("ConditionCoverage",
                                         "Condition Coverage calculation pass");

// Register for clang
static RegisterStandardPasses Y(PassManagerBuilder::EP_EarlyAsPossible,
                                [](const PassManagerBuilder &Builder,
                                   legacy::PassManagerBase &PM) {
                                  PM.add(new ConditionCoverage());
                                });