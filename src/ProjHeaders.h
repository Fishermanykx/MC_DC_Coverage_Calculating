/*
 * @Description: Headers for the proj
 * @Author: Fishermanykx
 * @Date: 2021-03-25 15:38:46
 * @LastEditors: Fishermanykx
 * @LastEditTime: 2021-05-27 16:06:47
 */

#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"