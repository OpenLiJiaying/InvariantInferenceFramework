#ifndef _EQUATION_H_
#define _EQUATION_H_
#include "header.h"
#include <iomanip>


extern int maxv;
extern int minv;

const double UPBOUND = pow(0.1, 3);

inline double _roundoff(double x)
{
	int inx = nearbyint(x);
	if ((inx >= x * (1 - UPBOUND) && inx <= x * (1 + UPBOUND))
		|| (inx <= x * (1 - UPBOUND) && inx >= x * (1 + UPBOUND)))
		return double(inx);
	return x;
}


class Solution{
	public:
		Solution() {}

		Solution(double a0, ...)
		{
			va_list ap;
			va_start(ap, a0);
			x[0] = a0;

			for (int i = 1; i < vars; i++) {
				x[i] = va_arg(ap, double);
			}
			va_end(ap);
		}

		friend std::ostream& operator << (std::ostream& out, const Solution sol)
		{
			out << "(" << sol.x[0];
			for (int j = 1; j < vars; j++)
				out << ", " << sol.x[j];
			out << ")";
			return out;
		}


		double x[vars];
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

		friend std::ostream& operator << (std::ostream& out, const Equation equ)
		{
			out << std::setprecision(16) << equ.theta[0] << " {0}";
			for (int j = 1; j < vars; j++)
				out << "  +  " << equ.theta[j] << "{" <<j << "}";
			out << " >= " << - equ.theta0;
			return out;
		}

		static int linearSolver(const Equation* equ, Solution* sol) 
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
			//std::cout << "solved the equation to get one solution";
			return 0;
		}


		static double calc(const Equation* equ, double* sol)
		{
			if (sol == NULL) return -1;
			if (equ == NULL) return -1;
			double res = equ->theta0;
			for (int i = 0; i < vars; i++) {
				res += equ->theta[i] * sol[i];
			}
			return res;
		}

		int isSimilar(const Equation* e, int precision = 4)
		{
			if (e == NULL) return -1;
			if ((theta0 == 0) && (e->theta0 == 0))
				return -1;
			double ratio = theta0 / e->theta0;
			double down, up;
			if (ratio >= 0) {
				down = ratio * (1 - pow(0.1, precision));
				up = ratio * (1 + pow(0.1, precision));
			}
			else {
				up = ratio * (1 - pow(0.1, precision));
				down = ratio * (1 + pow(0.1, precision));
			}
			//std::cout << "[" << down << ", " << ratio << ", " << up << "]" << std::endl;
			for (int i = 0; i < vars; i++) {
				if ((theta[i] < e->theta[i] * down) || (theta[i] > e->theta[i] * up))
					return -1;
			}
			return 0;
		}

		int roundoff(Equation* e)
		{
			double min = std::abs(theta0);
			for (int i = 0; i < vars; i++)
				min = (std::abs(theta[i]) < min) ? std::abs(theta[i]) : min;
			//if (min <= pow(0.1, 5)) min = 1;
			for (int i = 0; i < vars; i++)
				e->theta[i] = _roundoff(theta[i] / min);
			e->theta0 = _roundoff(theta0 / min);
			return 0;
		}

		double theta0;
		double theta[vars];
	private:
};

#endif