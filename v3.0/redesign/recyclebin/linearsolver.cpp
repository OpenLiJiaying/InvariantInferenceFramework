/*************************************************************************
  > File Name: linearsolver.cpp
  > Author: Li Jiaying
  > Mail: lijiaying1989@gmail.com 
  > Created Time: Fri 18 Sep 2015 01:22:56 AM
 ************************************************************************/
#include <cstdlib>
#include <cmath>
#include "header.h"

//extern int maxv;
//extern int minv;
//extern const int vars;


// dot_product(theta, x) + theta0 = 0
// find x to satisfy the above fomula.
int linear_solver(const Equation co, int* x)
{
	double theta0 = co.theta0;
	const double* theta = co.theta;
	int pick;
	double reminder;
start_solve:
	pick = rand() % vars;
	reminder = -theta0;
	for (int i = 0; i < vars; i++) {
		x[i] = rand() % (maxv - minv + 1) + minv;
		if (i != pick)
			reminder -= x[i] * theta[i];
	}
	x[pick] = nearbyint(reminder / theta[pick]); // + rand() % 3 - 1);
	if (x[pick] > maxv || x[pick] < minv) {
		goto start_solve;
	}

	return 0;
}
