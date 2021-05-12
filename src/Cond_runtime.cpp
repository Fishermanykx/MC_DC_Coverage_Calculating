/*
 * @Description:
 * @Author: Fishermanykx
 * @Date: 2021-04-12 21:52:34
 * @LastEditors: Fishermanykx
 * @LastEditTime: 2021-05-12 16:11:21
 */
#include <cmath>
#include <cstdio>
#include <iostream>

int covBranch = 0;
int totalConditionNum;

extern "C" void initExit() {
  // std::cout << "Now covered " << covBranch << " branch" << std::endl;
  // std::cout << "Func calCoverage ended" << std::endl;
  // std::cout << std::endl;
  double conditionCoverage = covBranch * 1.0 / pow(2.0, totalConditionNum);
  printf("Has %d conditions, covered %d conditions, condition coverage = %lf\n",
         totalConditionNum, covBranch, conditionCoverage);
}

extern "C" void init(char* funcName) {
  // covBranch = 0;

  printf("Init function called\n");

  printf("In function: %s\n", funcName);

  std::atexit(initExit);
}

extern "C" void calCoverage() {
  // printf("calCoverage function called\n");

  covBranch++;
  // printf("Now covered %d branch(es)\n", covBranch);

  // std::atexit(Exit);
}

extern "C" void getTotalConditions(int totalConditions) {
  totalConditionNum = totalConditions;
}