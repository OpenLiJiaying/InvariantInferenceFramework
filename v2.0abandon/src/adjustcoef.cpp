/*************************************************************************
    > File Name: adjustcoef.cpp
    > Author: Li Jiaying
    > Mail: lijiaying1989@gmail.com 
    > Created Time: 2015年10月15日 星期四 11时24分30秒
 ************************************************************************/

#include <iostream>
#include <fstream>
#include <limits>
#include <cmath>
using namespace std;

#define MAX_DIM 128
#define UPBOUND (1.0/1000)

double roundoff(double x) 
{
	int inx = nearbyint(x);
//	cout << x << "->" << inx << "(" << x * (1-UPBOUND) << ", " << x * (1+UPBOUND) << ")" << endl;
	if ((inx >= x * (1-UPBOUND) && inx <= x * (1+UPBOUND)) 
			|| (inx <= x * (1-UPBOUND) && inx >= x * (1+UPBOUND))) 
		return double(inx);

	return x;
}


int main(int argc, char** argv)
{
	if (argc < 3) {
		cout << "adjustcoef needs more arguments..." << endl;
		return -1;
	}

	ifstream if1(argv[1]);
	ofstream of1(argv[2]);
	cout.precision(std::numeric_limits<double>::digits10);
	of1.precision(std::numeric_limits<double>::digits10);

	int dimension = 0;
	double coef[MAX_DIM + 1];
	if1 >> dimension;
	of1 << dimension << "\t";
	double min = 99999;
	for (int i = 0; i <= dimension; i++) {
		if1 >> coef[i];
		min = abs(coef[i]) < min? abs(coef[i]) : min;
	}
//	cout << "min:" << min << endl;
	for (int i = 0; i <= dimension; i++) {
		coef[i]  = roundoff(coef[i] / min);
		of1 << coef[i] << "\t";
	}
	
	cout << coef[0] << "[0]";
	for (int i = 1; i < dimension; i++) {
		cout << "  +  " << coef[i] << "[" << i << "]";
	}
	cout << " >= " << -coef[dimension] << endl;

	if1.close();
	of1.close();
	return 0;
}
