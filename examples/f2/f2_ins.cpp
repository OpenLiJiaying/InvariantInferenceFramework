#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<iostream>
using namespace std;

int nondet() {
	return rand() % 20;
}

void main () {
	srand(time(NULL));
	int x, y, z, w;
	x=y=z=w=0;


	while (nondet() ) {

		if (nondet()) {x++; y = y+2;}
		else if (nondet()) {
			if (x >= 4) {x++; y = y+3; z = z+10; w = w+10;}
		}
		else if (x >= z && w >= y+1) {x = -x; y = -y; }
		x = x;  /* this works around a VC gen bug */
	}

	if (3*x <= y-1)
		goto ERROR;

	return;
ERROR:;
}
