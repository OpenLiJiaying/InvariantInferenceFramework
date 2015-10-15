#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <klee/klee.h>


int main (int argc, char** argv) {
	int x;
	int y;
	int xa;
	int ya;
	klee_make_symbolic(&xa, sizeof(xa), "xa");
	klee_make_symbolic(&ya, sizeof(ya), "ya");
	klee_assume(xa + 2 * ya >= 0);
	klee_assert(xa + 2 * ya >= -1);
	return 0;
}

