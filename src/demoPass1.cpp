/*
 * @Description: A Simple demo pass for learning llvm
 * Reference link:
 * https://stackoverflow.com/questions/48068157/llvm-replace-operand-with-function
 * 先做分支覆盖率，再改进为 MD/DC 覆盖率
 * 采用 LLVM Pass
 * @Author: Fishermanykx
 * @Date: 2021-03-25 16:10:20
 * @LastEditors: Fishermanykx
 * @LastEditTime: 2021-05-11 09:27:37
 */

#include "ProjHeaders.h"

using namespace llvm;

namespace {
struct TraverseFunc : public FunctionPass {
  static char ID;
  TraverseFunc() : FunctionPass(ID) {}

  bool runOnFunction(Function &CurFunc) override {
    bool hasChanged = false;
    errs() << "In function: " << CurFunc.getName().str() << "\n";

    // Declare Coverage Func
    LLVMContext &context =
        CurFunc.getContext();  // Function::getContext returns a mutable
                               // reference to LLVMContext
    // getParent() will always go one step up in the LLVM IR hierarchy
    FunctionCallee IcmpFunc = CurFunc.getParent()->getOrInsertFunction(
        "calCoverage", Type::getVoidTy(context));
    FunctionCallee InitFunc = CurFunc.getParent()->getOrInsertFunction(
        "init", Type::getVoidTy(context));

    // Traverse BasicBlocks of each Function
    for (auto &bb : CurFunc) {
      // std::cout << "Basic block whose name is: " << bb.getName().str()
      // << std::endl;

      // getTerminator(): Returns a pointer to the terminator instruction that
      // appears at the end of the BasicBlock.
      Instruction *ti = bb.getTerminator();
      // ti->printAsOperand(errs());
      // std::cout << std::endl;
      if (ti) errs() << *ti << "\n";
      // dyn_cast: 检测 ti 是否为 llvm::BranchInst 类型，如果不是返回空指针
      // 如果是返回指向该类型的指针
      auto *bri = llvm::dyn_cast<llvm::BranchInst>(ti);

      if (!bri) continue;
      if (!bri->isConditional()) continue;

      auto *cond =
          bri->getCondition();  // Condition of the branch instruction (br)
      // cond->printAsOperand(errs());
      // std::cout << std::endl;

      // please note that this cast is for integer comparisons
      // adjust according to your needs
      auto cmpi = llvm::dyn_cast<llvm::ICmpInst>(cond);  // icmp instruction
      if (!cmpi) continue;

      auto *opA = cmpi->getOperand(0);  // Operand 0 and its definition
      auto *opB = cmpi->getOperand(1);  // Operand 1 and its definition

      auto argsA = llvm::ArrayRef<llvm::Value *>(&opA, 1);
      auto argsB = llvm::ArrayRef<llvm::Value *>(&opB, 1);

      hasChanged |= true;
    }
    return hasChanged;
  }
};
}  // namespace

char TraverseFunc::ID = 0;

// Register for opt
static RegisterPass<TraverseFunc> X("traverseFunc", "Traverse function pass");

// Register for clang
static RegisterStandardPasses Y(PassManagerBuilder::EP_EarlyAsPossible,
                                [](const PassManagerBuilder &Builder,
                                   legacy::PassManagerBase &PM) {
                                  PM.add(new TraverseFunc());
                                });
