#include"iif.h"

int m(int* a){
	int x = a[0];
	int y = a[1];
	//Precondition: x + y >= 0
	//assume (x + y >= 0)
	if (x + y > 0)
		bPassP = true;
	while (y > 0) {
		insert_data(x, y);
		x++;
		y--;
	}

	// Postcondition: y > 0
	// assert (y > 0);
	insert_data(x, y);
	if (x > 0)
		bPassQ = true;

	return 0;
}

