/*
 * @Description:
 * @Author: Fishermanykx
 * @Date: 2021-04-08 14:51:50
 * @LastEditors: Fishermanykx
 * @LastEditTime: 2021-05-12 16:07:20
 */
#include <stdio.h>

int func(int a, int b) {
  if (a >= 0 && b > a) {
    a += 1;
  }

  return a;
}

void output(char *s) { printf("%s\n", s); }

int main(void) {
  int a = 0;
  int b = 1;
  int c = 1;

  /*   for (int i = 0; i < 3; ++i) {
      if (a >= 0 && b > 0) {
        a = func(a);
      } else {
        a--;
      }
    } */

  a = func(a, b);

  if (a < b && b >= c && c >= a) {
    a = a / (b - c + 1);
  }

  return 0;
}
