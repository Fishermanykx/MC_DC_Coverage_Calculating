/*
 * @Description: 确认： Clang 版本和 CMakeLists 无误
 * @Author: Fishermanykx
 * @Date: 2021-03-26 14:11:09
 * @LastEditors: Fishermanykx
 * @LastEditTime: 2021-04-03 10:48:11
 */
#include "llvm/IR/Function.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;

namespace {
struct Hello : public FunctionPass {
  static char ID;
  Hello() : FunctionPass(ID) {}
  bool runOnFunction(Function &F) override {
    errs() << "Hello: ";
    errs().write_escaped(F.getName()) << '\n';
    return false;
  }
};
}  // namespace

char Hello::ID = 0;

// Register for opt
static RegisterPass<Hello> X("hello", "Hello World Pass");

// Register for clang
static RegisterStandardPasses Y(PassManagerBuilder::EP_EarlyAsPossible,
                                [](const PassManagerBuilder &Builder,
                                   legacy::PassManagerBase &PM) {
                                  PM.add(new Hello());
                                });
