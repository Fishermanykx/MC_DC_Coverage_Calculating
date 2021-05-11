/*
 * @Description:
 * @Author: Fishermanykx
 * @Date: 2021-04-12 21:52:34
 * @LastEditors: Fishermanykx
 * @LastEditTime: 2021-05-11 10:24:25
 */
#include <iostream>

int covBranch;

extern "C" void Exit() {
  std::cout << "covered " << covBranch << " branch" << std::endl;
}

extern "C" void init() {
  covBranch = 0;
  std::atexit(Exit);
}

extern "C" void coverage() { covBranch++; }
