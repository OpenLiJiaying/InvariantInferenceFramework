#include"header.h"
#include <iostream>

int m(int* a){
	int x = a[0];
	int y = a[1];
	//assume (x + y >= 0)
	assume (x + y > 0);
//	record_values(x, y);
	while (y > 0) {
		curPSC->addNewProgramState(new ProgramState<int>(x,y));
		record_values(x, y);
		x++;
		y--;
	}

	// assert (y > 0);
	curPSC->addNewProgramState(new ProgramState<int>(x,y));
	record_values(x, y);
	assert (x > 0);
}

