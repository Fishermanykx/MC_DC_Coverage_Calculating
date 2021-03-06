/*
 * @Description: Calculate MC/DC coverage
 * @Author: Fishermanykx
 * @Date: 2021-05-12 16:15:37
 * @LastEditors: Fishermanykx
 * @LastEditTime: 2021-06-25 10:42:36
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
    FunctionCallee funcInit = curFunc.getParent()->getOrInsertFunction(
        "funcInit", Type::getVoidTy(Cxt));
    FunctionCallee updateNameFunc = curFunc.getParent()->getOrInsertFunction(
        "updateNameTable", Type::getVoidTy(Cxt), Type::getInt8PtrTy(Cxt),
        Type::getInt8PtrTy(Cxt), Type::getInt32Ty(Cxt));
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
    std::string termLabel = ".end";
    std::string ifTermLabel = "if.end";
    std::string thenLabel = "if.then";
    std::string whileTermLabel = "while.body";
    std::string forTermLabel = "for.body";
    // int deciCnt = 0;

    std::vector<std::vector<std::string> > singleFuncNameTable;

    for (Function::iterator bb = curFunc.begin(); bb != curFunc.end(); ++bb) {
      std::string bbNameStr = bb->getName().str();
      const char *bbName = bbNameStr.c_str();

      for (BasicBlock::iterator ii = bb->begin(); ii != bb->end(); ++ii) {
        // Insert Init func in the front of main Func
        if (ii == bb->begin() && bb == curFunc.begin()) {
          if (funcNameStr == "main") {
            IRBuilder<> builder(&*ii);
            builder.SetInsertPoint(&*bb, ++builder.GetInsertPoint());
            builder.CreateCall(mainInit);
          } else {
            IRBuilder<> builder(&*ii);
            builder.SetInsertPoint(&*bb, ++builder.GetInsertPoint());
            // Value *funcNameVal = builder.CreateGlobalStringPtr(funcName);
            // Value *args[] = {funcNameVal};
            builder.CreateCall(funcInit);
          }
        }

        // If one Icmp Inst is executed, pass its bbName and funcName to
        // updateCondFunc and infer val of previous Cond
        if (CmpInst *op = dyn_cast<CmpInst>(&*ii)) {
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

    // Get Name of each basicblock and construct name table
    std::vector<std::string> singleDecision;

    for (Function::iterator bb = curFunc.begin(); bb != curFunc.end(); ++bb) {
      std::string bbNameStr = bb->getName().str();
      const char *bbName = bbNameStr.c_str();
      auto startInst = bb->begin();

      int isWhileBody = (bbNameStr.find(whileTermLabel) != std::string::npos);
      int isForBody = (bbNameStr.find(forTermLabel) != std::string::npos);
      int isIfEnd = (bbNameStr.find(ifTermLabel) != std::string::npos);
      int isIfThen = (bbNameStr.find("if.then") != std::string::npos);

      if (startInst->getOpcode() == Instruction::PHI) {
        // If bbName is land.end* or lor.end*, skip
        continue;
      }
      if (bbNameStr.find("for.end") != std::string::npos ||
          bbNameStr.find("while.end") != std::string::npos) {
        continue;
      }

      // Check if new decision begins
      int newDeciBegin = 0;
      if (bbNameStr.find("body") != std::string::npos || isIfEnd || isIfThen) {
        // If is while.body or for.body, check if a new decision begins
        for (auto ii = bb->begin(); ii != bb->end(); ++ii) {
          if (ICmpInst *op = dyn_cast<ICmpInst>(&*ii)) {
            newDeciBegin = 1;
            break;
          }
        }
      }

      if (isIfThen || isWhileBody || isForBody) {
        // If is if.then or *.body, a decision ends, update nameTable
        // printVector(singleDecision);

        totalConds += singleDecision.size();
        // if (scanned) {
        //   (std::vector<std::string>()).swap(singleDecision);
        //   singleDecision.push_back(bbNameStr);

        //   break;
        // }

        // If it's the end of a branch, pass it to rtlib

        /*         IRBuilder<> builder(&*startInst);
                int firstLoop = 1;
                for (std::vector<std::string>::iterator it =
           singleDecision.begin(); it != singleDecision.end(); ++it) {
                  builder.SetInsertPoint(&*bb, builder.GetInsertPoint());
                  Value *bbNameVal =
           builder.CreateGlobalStringPtr((*it).c_str()); Value *funcNameVal =
           builder.CreateGlobalStringPtr(funcName); Value *callArgs[] =
           {funcNameVal, bbNameVal, builder.getInt32(firstLoop)};
                  builder.CreateCall(updateNameFunc, callArgs);
                  firstLoop = 0;
                } */
        singleFuncNameTable.push_back(singleDecision);

        // clear
        (std::vector<std::string>()).swap(singleDecision);
        // Add a new start if new decision begins
        if (newDeciBegin) {
          singleDecision.push_back(bbNameStr);
        }
      } else {
        if ((isIfEnd && newDeciBegin) || (!isIfEnd)) {
          singleDecision.push_back(bbNameStr);
        }
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
          if (!singleFuncNameTable.empty()) {
            IRBuilder<> builder(&*ii);
            int numDecis = singleFuncNameTable.size();
            for (int i = 0; i < numDecis; ++i) {
              std::vector<std::string> singleDeci = singleFuncNameTable[i];
              int firstLoop = 1;
              for (std::vector<std::string>::iterator s_it = singleDeci.begin();
                   s_it != singleDeci.end(); ++s_it) {
                Value *bbNameVal =
                    builder.CreateGlobalStringPtr((*s_it).c_str());
                Value *funcNameVal = builder.CreateGlobalStringPtr(funcName);
                Value *callArgs[] = {funcNameVal, bbNameVal,
                                     builder.getInt32(firstLoop)};
                builder.CreateCall(updateNameFunc, callArgs);
                firstLoop = 0;
              }
            }
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