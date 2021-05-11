# !/bin/bash

#================================================================
#   Copyright (C) 2021 * Ltd. All rights reserved.
#   
#   File name   : run.sh
#   Author      : Fishermanykx
#   Created date: 2021-05-11 10:56:22
#   Description : 插桩代码运行脚本
#
#================================================================

g++ runtime.cpp -fPIC -shared -o libruntime.so
clang -emit-llvm -S -fno-discard-value-names -c test.c -o test.ll
# opt -load build/src/libBranchCoverage.so -BranchCoverage -S test.ll -o test.instrumented.ll
opt -load build/src/liboriginalSample.so -originalSample -S test.ll -o test.instrumented.ll
clang -o test libruntime.so test.instrumented.ll
./test
