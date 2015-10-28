#include"iif.h"

int m(int* a){
	int x = a[0];
	int y = a[1];
	//assume (x + y >= 0)
	assume (x + y > 0);
//	record_values(x, y);
	while (y > 0) {
		record_values(x, y);
		x++;
		y--;
	}

	// assert (y > 0);
	record_values(x, y);
	assert (x > 0);

	return 0;
}

