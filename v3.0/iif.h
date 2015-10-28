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
//#define _TEST1_	 
#define _TEST2_
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


#ifndef VARS 
#define VARS (2)
#endif

//extern int max_trace_pnt;
//extern int inputs_init;
//extern int inputs_aft;
//extern int inputs_max;

//extern struct node* positive_set;
//extern struct node* negative_set;
//extern struct node* counter_example_set;
//extern struct node trace[];

extern bool _passP;
extern bool _passQ;

#undef assume
#undef assert

#define assume(expr) do { \
	_passP = (expr)? true : false;\
} while(0)

#define assert(expr) do { \
	_passQ = (expr)? true : false;\
} while(0)








// function lists
int m(int*);
/*
   int loop1(int);
   int loop2(int, int);
   int loop3(int, int, int);
   int loop4(int, int, int, int);
   int loop5(int, int, int, int, int);
   int loop6(int, int, int, int, int, int);
   int loop7(int, int, int, int, int, int, int);
   int loop8(int, int, int, int, int, int, int, int);
   */
int record_values(int, ...);







struct node
{
	public:
		int value[VARS];
		//	int label;
		long long hash_value;

		node(): hash_value(0){}

		long long  hash(){
			hash_value = 0;
			for(int i = 0; i < VARS; i++)
				hash_value = (hash_value << 8) + value[i];
			return hash_value;
		}
		void copy(int* a) {
			for (int i = 0; i < VARS; i++)
				value[i] = a[i];
		}
	friend std::ostream& operator<< (std::ostream& out, const node& n) {
			out << "(" << n.value[0];
			for (int i = 1; i < VARS; i++)
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




#endif
