/*
 * @Description: Calculate MC/DC coverage
 * @Author: Fishermanykx
 * @Date: 2021-05-12 16:15:37
 * @LastEditors: Fishermanykx
 * @LastEditTime: 2021-05-27 16:03:27
 */
#include "ProjHeaders.h"

using namespace llvm;

namespace {
struct MCDCCoverage : public FunctionPass {
  static char ID;
  MCDCCoverage() : FunctionPass(ID) {}

  int totalConds = 0;
  // std::vector<std::string> scannedFunc;

  void printVector(std::vector<std::string> &v) {
    errs() << "--------- Single Decision ----------\n";
    for (std::vector<std::string>::iterator it = v.begin(); it != v.end();
         ++it) {
      errs() << *it << "\t";
    }
    errs() << "\n";
    errs() << "-----------------------\n";
  }

  bool runOnFunction(Function &curFunc) override {
    bool hasChanged = false;

    LLVMContext &Cxt = curFunc.getContext();
    // Declaration of runtime lib functions
    FunctionCallee mainInit = curFunc.getParent()->getOrInsertFunction(
        "mainInit", Type::getVoidTy(Cxt));
    FunctionCallee updateNameFunc = curFunc.getParent()->getOrInsertFunction(
        "updateNameTable", Type::getVoidTy(Cxt), Type::getInt8PtrTy(Cxt),
        Type::getInt8PtrTy(Cxt));
    FunctionCallee updateCondFunc = curFunc.getParent()->getOrInsertFunction(
        "updateCondTable", Type::getVoidTy(Cxt), Type::getInt8PtrTy(Cxt),
        Type::getInt8PtrTy(Cxt), Type::getInt8PtrTy(Cxt), Type::getInt32Ty(Cxt),
        Type::getInt32Ty(Cxt));
    FunctionCallee ReturnFunc = curFunc.getParent()->getOrInsertFunction(
        "getTotalConditions", Type::getVoidTy(Cxt), Type::getInt32Ty(Cxt));
    FunctionCallee scanSigFunc = curFunc.getParent()->getOrInsertFunction(
        "getScanSig", Type::getVoidTy(Cxt));

    std::string funcNameStr = curFunc.getName().str();
    char *funcName = (char *)funcNameStr.c_str();

    // int scanned = 0;

    // if (find(scannedFunc.begin(), scannedFunc.end(), funcNameStr) !=
    //     scannedFunc.end()) {
    //   scanned = 1;
    // }

    // Get conditions whose basicblock is actually executed
    std::string termLabel = "if.end";
    std::string thenLabel = "if.then";
    // int deciCnt = 0;

    for (Function::iterator bb = curFunc.begin(); bb != curFunc.end(); ++bb) {
      std::string bbNameStr = bb->getName().str();
      const char *bbName = bbNameStr.c_str();

      for (BasicBlock::iterator ii = bb->begin(); ii != bb->end(); ++ii) {
        // Insert Init func in the front of main Func
        if (ii == bb->begin() && bb == curFunc.begin() &&
            funcNameStr == "main") {
          IRBuilder<> builder(&*ii);
          builder.SetInsertPoint(&*bb, ++builder.GetInsertPoint());
          builder.CreateCall(mainInit);
        }

        // If one Icmp Inst is executed, pass its bbName and funcName to
        // updateCondFunc and infer val of previous Cond
        if (ICmpInst *op = dyn_cast<ICmpInst>(&*ii)) {
          Value *lhs = op->getOperand(0);
          Value *rhs = op->getOperand(1);
          std::string handlerStr =
              op->getPredicateName(op->getPredicate()).str();
          const char *handler = handlerStr.c_str();

          IRBuilder<> builder(&*ii);
          builder.SetInsertPoint(&*bb, ++builder.GetInsertPoint());

          Value *bbNameVal = builder.CreateGlobalStringPtr(bbName);
          Value *funcNameVal = builder.CreateGlobalStringPtr(funcName);
          Value *handlerVal = builder.CreateGlobalStringPtr(handler);

          Value *callArgs[] = {funcNameVal, bbNameVal, handlerVal, lhs, rhs};

          builder.CreateCall(updateCondFunc, callArgs);
        }
      }
    }

    /*       // If cur BasicBlock is if.end*
          if (bbNameStr.find(termLabel) != std::string::npos) {
            auto ii = bb->begin();
            IRBuilder<> builder(&*ii);
            builder.SetInsertPoint(&*bb, builder.GetInsertPoint());

            Value *funcNameVal = builder.CreateGlobalStringPtr(funcName);
            Value *args[] = {funcNameVal, builder.getInt32(deciCnt)};

            builder.CreateCall(updateCondFunc, args);

            // std::cout << "In Func: " << funcNameStr << std::endl;
            // std::cout << "Branch cnt: " << deciCnt << std::endl;

            ++deciCnt;
          }
        } */

    // Get Name of each basicblock and construct name table
    std::vector<std::string> singleDecision;

    for (Function::iterator bb = curFunc.begin(); bb != curFunc.end(); ++bb) {
      std::string bbNameStr = bb->getName().str();
      const char *bbName = bbNameStr.c_str();

      if (bbNameStr.find(termLabel) != std::string::npos) {
        // printVector(singleDecision);
        totalConds += singleDecision.size();
        // if (scanned) {
        //   (std::vector<std::string>()).swap(singleDecision);
        //   singleDecision.push_back(bbNameStr);

        //   break;
        // }

        // If it's the end of a branch, pass it to rtlib
        auto ii = bb->begin();
        IRBuilder<> builder(&*ii);
        for (std::vector<std::string>::iterator it = singleDecision.begin();
             it != singleDecision.end(); ++it) {
          builder.SetInsertPoint(&*bb, builder.GetInsertPoint());
          Value *bbNameVal = builder.CreateGlobalStringPtr((*it).c_str());
          Value *funcNameVal = builder.CreateGlobalStringPtr(funcName);
          Value *callArgs[] = {funcNameVal, bbNameVal};
          builder.CreateCall(updateNameFunc, callArgs);
        }
        // clear
        (std::vector<std::string>()).swap(singleDecision);
        // Add new start (i.e. if.end)
        singleDecision.push_back(bbNameStr);
      } else {
        singleDecision.push_back(bbNameStr);
      }
    }

    // Get Number of conditions
    for (auto bb = curFunc.begin(); bb != curFunc.end(); ++bb) {
      for (auto ii = bb->begin(); ii != bb->end(); ++ii) {
        if (ReturnInst *RI = dyn_cast<ReturnInst>(ii)) {
          if (funcNameStr != "main") {
            // bb is a terminator basicblock
            IRBuilder<> termBuilder(&*ii);
            termBuilder.SetInsertPoint(&*bb, termBuilder.GetInsertPoint());
            Value *args[] = {termBuilder.getInt32(totalConds)};
            termBuilder.CreateCall(ReturnFunc, args);
          }
          if (funcNameStr == "func") {
            IRBuilder<> sigBuilder(&*ii);
            sigBuilder.SetInsertPoint(&*bb, sigBuilder.GetInsertPoint());
            sigBuilder.CreateCall(scanSigFunc);
          }
        }
      }
    }

    hasChanged |= true;
    return hasChanged;
  }
};
}  // namespace

// Register for opt
char MCDCCoverage::ID = 0;
static RegisterPass<MCDCCoverage> X("MCDCCoverage",
                                    "MC/DC Coverage calculation pass");

// Register for clang
static RegisterStandardPasses Y(PassManagerBuilder::EP_EarlyAsPossible,
                                [](const PassManagerBuilder &Builder,
                                   legacy::PassManagerBase &PM) {
                                  PM.add(new MCDCCoverage());
                                });