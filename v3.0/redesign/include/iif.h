/*************************************************************************
  > File Name: iif.h
  > Author: Li Jiaying
  > Mail: lijiaying1989@gmail.com 
  > Created Time: Wed Oct 28 01:32:44 2015
 ************************************************************************/
#ifndef _IIF_H_
#define _IIF_H_
#include <stdarg.h>
#include <iostream>

// _TEST_ simplest output
//#define _TEST0_	 
#define _TEST1_	 
//#define _TEST2_
//#define _TEST3_

#ifdef _TEST1_
#define _TEST0_	 
#endif

#ifdef _TEST2_
#define _TEST1_	 
#define _TEST0_	 
#endif

#ifdef _TEST3_
#define _TEST1_	 
#define _TEST2_
#define _TEST0_	 
#endif



extern bool _passP;
extern bool _passQ;

#ifndef VARS 
const int vars = 2;
#else
const int vars = VARS;
#endif
//


extern int maxv;
extern int minv;

const int max_trace_pnt = 1024;
const int inputs_init = 4 * vars;
const int inputs_aft = 2 * vars;
const int max_inputs = inputs_init;
const int max_set_idx = 10240;
const int max_iter = 32;



struct node
{
	public:
		double value[vars];
		//	int label;
		long long hash_value;

		node(): hash_value(0){}

		long long  hash(){
			hash_value = 0;
			for(int i = 0; i < vars; i++)
				hash_value = (hash_value << 8) + (int)value[i];
			return hash_value;
		}
		void copy(double* a) {
			for (int i = 0; i < vars; i++)
				value[i] = a[i];
		}
		friend std::ostream& operator<< (std::ostream& out, const node& n) {
			out << "(" << n.value[0];
			for (int i = 1; i < vars; i++)
				out << ", " << n.value[i];
			out << ")";
#ifdef _TEST2_
			out << "h[" << n.hash_value <<"]";
#endif
			return out;
		}
		static int compare(const void* a, const void* b) {
			struct node* pa = (node*)a;
			struct node* pb = (node*)b;
			if (pa->hash_value == 0) pa->hash();
			if (pb->hash_value == 0) pb->hash();
			return pa->hash_value > pb->hash_value;
		}
};


extern struct node positive_set[];
extern struct node negative_set[];
extern struct node* counter_example_set;
extern int positive_idx;
extern int negative_idx;
extern bool positive_set_changed;
extern bool negative_set_changed;

extern int inputs[];
extern struct node trace[];
extern int trace_idx;


struct coef{
	double theta0;
	double theta[vars];
};


// function lists
int m(int*);
/*
   int loop1(int);
   int loop2(int, int);
   int loop3(int, int, int);
   int loop4(int, int, int, int);
   */
int record_values(int, ...);
int before_loop();
int after_loop();
void nice_set_print(); 


int linear_solver(const struct coef, int*);

#endif
