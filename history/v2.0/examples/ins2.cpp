#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<iostream>
using namespace std;


bool passP = false;
bool passQ = false;

int main (int argc, char** argv) {
	if (argc < 3) return -1;
	int x = atoi(argv[1]);
	int y = atoi(argv[2]);

	//Precondition: x + y >= 0
	//assume (x + y >= 0)
	if (x + y > 0)
		passP = true;
	
	while (y > 0) {
		x++;
		y--;
	}

	// Postcondition: y > 0
	// assert (y > 0);
	if (x > 0)
		passQ = true;
	if (passP) { if (passQ) return 1;
		else return 2;
	} else if (passQ) return 3;
	return 4;
}

