/*************************************************************************
  > File Name: linearsolver.cpp
  > Author: Li Jiaying
  > Mail: lijiaying1989@gmail.com 
  > Created Time: Fri 18 Sep 2015 01:22:56 AM
 ************************************************************************/
#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cmath>
using namespace std;

#ifndef MAX
#define MAX (100)
#endif

#ifndef MIN
#define MIN (-100)
#endif

int main(int argc, char** argv) // var_no out_no
{
	if (argc < 2) {
		printf ("ERROR!");
		return -1;
	}
	int var_no = atoi(argv[1]);
	int out_no = 1;
	int seed = time(NULL);
	if (argc >= 3) 
		out_no = atoi(argv[2]);
	if (argc >= 4) 
		seed = atoi(argv[3]);
	srand(seed);
	double* constraints = new double [var_no];
	double rvalue;
	int* solutions = new int [var_no];
	for (int i = 0; i < var_no; i++) {
		cin >> constraints[i];
	}
	cin >> rvalue;
	rvalue = -1 * rvalue;

	while (out_no--) {
		int pickup = rand() % var_no;
		double reminder = rvalue;
		for (int i = 0; i < var_no; i++) {
			solutions[i] = rand() % (MAX - MIN) + MIN;
			if (i != pickup)
				reminder -= solutions[i] * constraints[i];
		}
		solutions[pickup] = nearbyint(reminder / constraints[pickup]); // + rand() % 3 - 1);
		if (solutions[pickup] > MAX || solutions[pickup] < MIN) {
			out_no++;
			continue;
		}

		for (int k = 0; k < var_no; k++) 
			cout<< k+1 << ":" << solutions[k] << " ";
		cout << endl;
	}
	delete solutions;
	return 0;
}
