/*************************************************************************
  > File Name: iif.cpp
  > Author: Li Jiaying
  > Mail: lijiaying1989@gmail.com 
  > Created Time: Wed Oct 28 01:47:17 2015
 ************************************************************************/
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <time.h>
#include "iif.h"

//#define _TEST_


const int max_trace_pnt = 1024;
const int inputs_init = 4 * VARS;
const int inputs_aft = 2 * VARS;
const int max_inputs = inputs_init;

const int max_set_idx = 10240;
struct node positive_set[max_set_idx];
struct node negative_set[max_set_idx];
struct node* counter_example_set = NULL;
int positive_idx = 0;
int negative_idx = 0;

struct node inputs;
struct node trace[max_trace_pnt];
int trace_idx = 0;

bool bPassP = false;
bool bPassQ = false;
int min = -100, max = 100;


	int 
insert_data(int first, ...)
{
	if (trace_idx >= max_trace_pnt)
		return -1;
	va_list ap;
	va_start(ap, first);
	trace[trace_idx].value[0] = first;
#ifdef _TEST_
	std::cout << "insert: (" << first;
#endif
	for (int i = 1; i < VARS; i++) {
		trace[trace_idx].value[i] = va_arg(ap, int);
#ifdef _TEST_
		std::cout << ", " << trace[trace_idx].value[i];
#endif
	}
#ifdef _TEST_
	std::cout << ")" << std::endl;
#endif
	trace_idx++;
	return 0;
}


static int before_loop()
{
	bPassP = false;
	bPassQ = false;
	trace_idx = 0;
	return 0;
}

static int after_loop()
{
#ifdef _TEST_
	std::cout << "after_loop" << bPassP << "--" << bPassQ << std::endl;
#endif
	if (bPassP && bPassQ) {
		for (int i = 0; i < trace_idx; i++){
			positive_set[positive_idx].copy(trace[i].value);
#ifdef _TEST_
			std::cout << positive_set[positive_idx] << "->";
#endif
			positive_idx++;
		}
#ifdef _TEST_
		std::cout << std::endl;
#endif
		trace_idx = 0;
		return 0;
	}
	if (!bPassP && !bPassQ) {
		for (int i = 0; i < trace_idx; i++) {
			negative_set[negative_idx].copy(trace[i].value);
#ifdef _TEST_
			std::cout << negative_set[negative_idx] << "->";
#endif
			negative_idx++;
		}
#ifdef _TEST_
		std::cout << std::endl;
#endif
		trace_idx = 0;
		return 0;
	}
	if (bPassP && !bPassQ) {
		counter_example_set = trace;
		std::cerr << "Encounter counter examples." << std::endl;
		std::cerr << "Trace: " << std::endl << "init:";
		for (int i = 0; i < trace_idx; i++)
			std::cout << negative_set[negative_idx++] << "-->";
		std::cout << "end" << std::endl;
		exit(-1);
	}
	if (!bPassP && bPassQ) {
		//		for (int i = 0; i < trace_idx; i++)
		//			question_set[question_idx++].copy(trace[i]);
		trace_idx = 0;
		return 0;
	}
	return -1;
}


#ifndef _TEST_
void set_print() {
	std::cout << "Positive Set [" << positive_idx << "]:" << std::endl;
	for (int i = 0; i < positive_idx; i++)
		std::cout << positive_set[i] << " ";
	std::cout << std::endl;
	std::cout << "Negative Set [" << negative_idx << "]:" << std::endl;
	for (int i = 0; i < negative_idx; i++)
		std::cout << negative_set[i] << " ";
	std::cout << std::endl;
}
#else
void set_print(){}
#endif 


	int 
main(int argc, char** argv)
{
	if (argc < 1) {
		std::cout << "Arguments less than 2.\n";
		exit(-1);
	}	
	if (argc >= 3) {
		min = atoi(argv[1]);
		max = atoi(argv[2]);
	}

	srand(time(NULL));
	for (int i = 0; i < inputs_init; i++) {
		for (int j = 0; j < VARS; j++) {
			inputs.value[j] = rand() % (max - min + 1) + min;
			before_loop();
			m(inputs.value);
			after_loop();
		}
	}
	set_print();
	return 0;
}
