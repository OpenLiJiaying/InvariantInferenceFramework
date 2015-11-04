#include "../include/header.h"
//#include <iostream>

#define ex1 m
int nondet() {
	return rand() % 4;
	return rand() % 20;
}




int f1(int* a){
	int x = a[0];
	int y = a[1];
	assume (x + y > 0);
	while (y > 0) {
		recordi(x, y);
		x++;
		y--;
	}

	recordi(x, y);
	assert (x > 0);
	return 0;
}


int f2(int* a) {
	int x = a[0];
	int y = a[1];
	assume((x + y >= 0) && (x - y >= 0));
	while (y > 0) {
		recordi(x, y);
		x++;
		y--;
	}

	recordi(x, y);
	assume((x + y >= 0) && (x - y >= 0));
	return 0;
}



int f3(int* a) {
	int x = a[0];
	int y = a[1];
	int z = a[2];
	assume(x + y + z >= 0);
	while (z> 0) {
		recordi(x, y, z);
		x++;
		y++;
		z -= 2;
	}

	recordi(x, y, z);
	assert(x + y >= 0);
	return 0;
}


int ex1(int* a) {
	int x;
	int y;
	int xa = a[0];
	int ya = a[1];

	assume(xa + 2 * ya >= 0);
	while (nondet()) {
		recordi(xa, ya);
		x = xa + 2 * ya;
		y = -2 * xa + ya;

		x++;
		if (nondet()) y = y + x;
		else y = y - x;

		xa = x - 2 * y;
		ya = 2 * x + y;
	}
	recordi(xa, ya);
	assert(xa + 2 * ya >= 0);
	return 0;
}