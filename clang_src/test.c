/*
 * @Description: 待测程序
 * @Author: Fishermanykx
 * @Date: 2021-03-25 15:36:42
 * @LastEditors: Fishermanykx
 * @LastEditTime: 2021-03-25 15:37:05
 */
#include <stdio.h>

int main() {
  int x = 0;
  int y = 0;

  if (x < y) {
    x++;
    if (x == y) {
      printf("%d", x);
    }
  }
  return 0;
}