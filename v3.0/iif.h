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



#ifndef VARS 
#define VARS (2)
#endif

//extern int max_trace_pnt;
//extern int inputs_init;
//extern int inputs_aft;
//extern int inputs_max;



struct node
{
	public:
		int value[VARS];
		//	int label;
		long long hash_value;

		long long  hash(){
			hash_value = 0;
			for(int i = 0; i < VARS; i++)
				hash_value = hash_value << 4 + value[i];
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
			return out;
		}
};

//extern struct node* positive_set;
//extern struct node* negative_set;
//extern struct node* counter_example_set;
//extern struct node trace[];

extern bool bPassP;
extern bool bPassQ;

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


int insert_data(int, ...);



#endif
