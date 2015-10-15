/*************************************************************************
  > File Name: test.c
  > Author: Li Jiaying
  > Mail: lijiaying1989@gmail.com 
  > Created Time: 2015年10月04日 星期日 21时32分15秒
 ************************************************************************/

#include <stdio.h>

int main()
{
	int xa = -198640688;
	int ya = 492751876;
	int x, y;
	int za = xa + 2 * ya;
	printf ("%d + 2 * %d = %d\n", xa, ya, za);
	x = xa + 2*ya;
	y = -2*xa + ya;

	x++;
	if (0) y = y+x;
	else y = y-x;

	xa = x - 2*y;
	ya = 2*x + y;
	za = xa + 2 * ya;
	printf ("%d + 2 * %d = %d\n", xa, ya, za);
	return 0;
}
