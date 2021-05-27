/*
 * @Description:
 * @Author: Fishermanykx
 * @Date: 2021-04-08 14:51:50
 * @LastEditors: Fishermanykx
 * @LastEditTime: 2021-05-27 16:02:21
 */
#include <stdio.h>

int func(int a, int b) {
  int c = a;

  if (a >= 1 && b > a) {
    c += 1;
  }

  if ((a >= 1 && c <= 2) || (b > 3 && b < 5)) {
    a = c;
  }

  return a;
}

void output(char *s) { printf("%s\n", s); }

int main(void) {
  // 给出一系列测试用例，调用 Func 函数。最终计算的是 Func 函数的 MC/DC 覆盖率
  int a, b;

  // Test case 1
  a = 0;
  b = 1;
  func(a, b);

  // Test case 2
  a = 1;
  b = 1;
  func(a, b);

  // Test case 3
  a = 2;
  b = 3;
  func(a, b);

  // Test case 4
  a = 2;
  b = 4;
  func(a, b);

  // // Test case 5
  // a = 2;
  // b = 5;
  // func(a, b);

  // if (a >= 0 && b > a) {
  //   a += 1;
  // }

  return 0;
}
