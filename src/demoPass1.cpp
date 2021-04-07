/*
 * @Description: A Simple demo pass for learning llvm
 * Reference link:
 * https://stackoverflow.com/questions/48068157/llvm-replace-operand-with-function
 *
 * @Author: Fishermanykx
 * @Date: 2021-03-25 16:10:20
 * @LastEditors: Fishermanykx
 * @LastEditTime: 2021-04-07 14:06:40
 */

#include "ProjHeaders.h"

using namespace llvm;

namespace {
struct TraverseFunc : public FunctionPass {
  static char ID;
  TraverseFunc() : FunctionPass(ID) {}

  bool runOnFunction(Function &CurFunc) override {
    bool hasChanged = false;

    for (auto &bb : CurFunc) {
      auto *ti = bb.getTerminator();
      auto *bri = llvm::dyn_cast<llvm::BranchInst>(ti);
      if (!bri) continue;
      if (!bri->isConditional()) continue;

      auto *cond = bri->getCondition();

      // please note that this cast is for integer comparisons
      // adjust according to your needs
      auto cmpi = llvm::dyn_cast<llvm::ICmpInst>(cond);
      if (!cmpi) continue;

      auto *opA = cmpi->getOperand(0);
      auto *opB = cmpi->getOperand(1);

      auto argsA = llvm::ArrayRef<llvm::Value *>(&opA, 1);
      auto argsB = llvm::ArrayRef<llvm::Value *>(&opB, 1);

      cmpi->setOperand(1, opA);
      cmpi->setOperand(1, opB);
      hasChanged |= true;
    }
    return hasChanged;
  }
};
}  // namespace

char TraverseFunc::ID = 0;
static RegisterPass<TraverseFunc> X("traverseFunc", "Traverse function pass");
