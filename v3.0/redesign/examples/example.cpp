#include "../include/header.h"
//#include <iostream>

int m(int* a){
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
