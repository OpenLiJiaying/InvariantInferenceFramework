#ifndef _EQUATION_H_
#define _EQUATION_H_
#include "header.h"
#include <iomanip>

extern int maxv;
extern int minv;


class Equation{
	public:
		Equation()
		{
		}

		Equation(double a0, ...)
		{
			va_list ap;
			va_start(ap, a0);
			theta0 = a0;

			for (int i = 1; i < vars; i++) {
				theta[i] = va_arg(ap, double);
			}
			va_end(ap);
		}

		friend std::ostream& operator << (std::ostream& out, const Equation* equ)
		{
			out << std::setprecision(16) << equ->theta[0] << " {0}";
			for (int j = 1; j < vars; j++)
				out << "  +  " << /*std::setprecision(16) <<*/ equ->theta[j] 
					<< /*std::setprecision(std::fixed) <<*/ "{" <<j << "}";
			out << " >= " << /*std::setprecision(16) <<*/ - equ->theta0;
			out << /*std::setprecision(std::fixed) <<*/ "\n";
		}

		int* linearSolver() 
		{
			int *x = new int[vars];
			int pick;
			double reminder;
solve:
			pick = rand() % vars;
			reminder = -theta0;
			for (int i = 0; i < vars; i++) {
				x[i] = rand() % (maxv - minv + 1) + minv;
				if (i != pick)
					reminder -= x[i] * theta[i];
			}
			x[pick] = nearbyint(reminder / theta[pick]); // + rand() % 3 - 1);
			if (x[pick] > maxv || x[pick] < minv) {
				goto solve;
			}
			return x;
		}

		double theta0;
		double theta[vars];
	private:
};


#endif
