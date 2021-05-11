/*
 * @Description:
 * @Author: Fishermanykx
 * @Date: 2021-04-08 14:51:50
 * @LastEditors: Fishermanykx
 * @LastEditTime: 2021-04-08 16:24:23
 */
#include <stdio.h>

/*
int func(int a) {
  a += 1;
  return a;
}
*/

int main(void) {
  int a = 0;
  int b = 1;
	int c = 1;

	/*
  for (int i = 0; i < 3; ++i) {
    if (a >= 0 && b > 0) {
      a = func(a);
    } else {
      a--;
    }
  }*/
	if (a==0) {
		if (b == c) {
			a = a / (b - c + 1);
		}
	}

  return 0;
}
