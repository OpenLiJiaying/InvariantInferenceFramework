#ifndef _EQUATION_H_
#define _EQUATION_H_
#include "header.h"
#include <iomanip>

extern int maxv;
extern int minv;

template<class T>
class Solution{
	public:
		Solution() {}

		Solution(T a0, ...)
		{
			va_list ap;
			va_start(ap, a0);
			x[0] = a0;

			for (int i = 1; i < vars; i++) {
				x[i] = va_arg(ap, T);
			}
			va_end(ap);
		}

		friend std::ostream& operator << (std::ostream& out, const Solution* sol)
		{
			out << "(" << sol->x[0];
			for (int j = 1; j < vars; j++)
				out << ", " << sol->x[j];
			out << ")";
			return out;
		}

		T x[vars];
};


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
			//out << /*std::setprecision(std::fixed) <<*/ "\n";
			return out;
		}

		static int linearSolver(const Equation* equ, Solution<int>* sol) 
		{
			if (sol == NULL) return -1;
			if (equ == NULL) {
				for (int i = 0; i < vars; i++)
					sol->x[i] = rand() % (maxv - minv + 1) + minv;
				return 0;
			}
			int pick;
			double reminder;
solve:
			pick = rand() % vars;
			reminder = - equ->theta0;
			for (int i = 0; i < vars; i++) {
				sol->x[i] = rand() % (maxv - minv + 1) + minv;
				if (i != pick)
					reminder -= sol->x[i] * equ->theta[i];
			}
			sol->x[pick] = nearbyint(reminder / equ->theta[pick]); // + rand() % 3 - 1);
			if (sol->x[pick] > maxv || sol->x[pick] < minv) {
				goto solve;
			}
			std::cout << "solved the equation to get one solution";
			return 0;
		}

		template<class T>
		static double calc(const Equation* equ, T* sol)
		{
			if (sol == NULL) return -1;
			if (equ == NULL) return -1;
			double res = equ->theta0;
			for (int i = 0; i < vars; i++) {
				res += equ->theta[i] * sol[i];
			}
			return res;
		}

		double theta0;
		double theta[vars];
	private:
};

#endif
