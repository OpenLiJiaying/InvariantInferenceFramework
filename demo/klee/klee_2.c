#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<klee/klee.h>


int main (int argc, char** argv) {
	int x;
	int y;
	int xa;
	int ya;
	int cond;
	klee_make_symbolic(&xa, sizeof(xa), "xa");
	klee_make_symbolic(&ya, sizeof(ya), "ya");
	klee_make_symbolic(&cond, sizeof(cond), "cond");

	klee_assume(xa + 2 * ya >= -1);
	x = xa + 2*ya;
	y = -2*xa + ya;

	x++;
	if (cond) y = y+x;
	else y = y-x;

	xa = x - 2*y;
	ya = 2*x + y;
	klee_assert(xa + 2 * ya >= -1);
	return 0;
}

