#include"header.h"
#include <iostream>
extern LoopTrace<int>* LP;

int m(int* a){
	int x = a[0];
	int y = a[1];
	//assume (x + y >= 0)
	assume (x + y > 0);
//	record_values(x, y);
	while (y > 0) {
		LT->addNewLoopState(new LoopState<int>(x,y));
//		record_values(x, y);
		x++;
		y--;
	}

	// assert (y > 0);
	LT->addNewLoopState(new LoopState<int>(x,y));
//	record_values(x, y);
	assert (x > 0);
}

