/*************************************************************************
  > File Name: convergeCheck.cpp
  > Author: Li Jiaying
  > Mail: lijiaying1989@gmail.com 
  > Created Time: Fri Sep 18 03:33:56 2015
 ************************************************************************/
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <limits>
using namespace std;

#ifndef BOUND
#define BOUND  (0)
#endif

/*
 * call format:
 *		./convergecheck num
 * num:	number of variables
 *		if we have variable x1, x2; this num should be set as 2
 *		in the program input, we should cin>> 
 *			ratio x1 x2 rvalue
 *			ratio x1 x2 rvalue
 *	
 * return value:
 *		0: converged
 *		1: not converged.
 */
int main(int argc, char** argv){	// argv[1] = no. of variables
	int length = 3;
	if (argc >= 2)
		length = atoi(argv[1]) + 2;  //accuracy var1 var2 rvalue
	double *prev = new double[length], *cur = new double[length];

	double bound;
	bound = BOUND > 0? BOUND : (-1 * BOUND);

	for (int i = 0; i < length; i++)
		cin >> prev[i];
	for (int i = 0; i < length; i++)
		cin >> cur[i];


/*	if (BOUND == 0) {
		for (int i = 0; i < length; i++) 
			if (prev[i] != cur[i]) 
				return 1;
		return 0;
	}
*/
	for (int i = 0; i < length; i++) {
		if ((prev[i] - cur[i]) > BOUND || (cur[i] - prev[i]) > BOUND) {
			return 1;	// not converged
		}
	}

	return 0; // converged.
}

