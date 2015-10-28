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
#include "svm_linker.h"
#include "svm.h"


struct node positive_set[max_set_idx];
struct node negative_set[max_set_idx];
struct node* counter_example_set = NULL;
int positive_idx = 0;
int negative_idx = 0;
bool positive_set_changed = false;
bool negative_set_changed = false;

#ifdef _TEST1_
void nice_set_print()
{
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
static void nice_set_print(){}
#endif 





int inputs[vars];
struct node trace[max_trace_pnt];
int trace_idx = 0;

bool _passP = false;
bool _passQ = false;



int record_values(int first, ...)
{
	if (trace_idx >= max_trace_pnt)
		return -1;
	va_list ap;
	va_start(ap, first);
	trace[trace_idx].value[0] = first;
#ifdef _TEST_
	std::cout << "insert: (" << first;
#endif
	for (int i = 1; i < vars; i++) {
		trace[trace_idx].value[i] = va_arg(ap, int);
#ifdef _TEST_
		std::cout << ", " << trace[trace_idx].value[i];
#endif
	}
	va_end(ap);
#ifdef _TEST_
	std::cout << ")" << std::endl;
#endif
	trace_idx++;
	return 0;
}


int before_loop()
{
	_passP = false;
	_passQ = false;
	trace_idx = 0;
	return 0;
}


int after_loop()
{
#ifdef _TEST3_
	std::cout << "after_loop" << _passP << "--" << _passQ << std::endl;
#endif
	if (_passP && _passQ) {
		positive_set_changed = true;
		for (int i = 0; i < trace_idx; i++){
			positive_set[positive_idx].copy(trace[i].value);
//			positive_set[positive_idx].hash();
#ifdef _TEST3_
			std::cout << positive_set[positive_idx] << "->";
#endif
			positive_idx++;
		}
#ifdef _TEST3_
		std::cout << std::endl;
#endif
		trace_idx = 0;
		return 0;
	}
	if (!_passP && !_passQ) {
		negative_set_changed = true;
		for (int i = 0; i < trace_idx; i++) {
			negative_set[negative_idx].copy(trace[i].value);
//			negative_set[negative_idx].hash();
#ifdef _TEST3_
			std::cout << negative_set[negative_idx] << "->";
#endif
			negative_idx++;
		}
#ifdef _TEST3_
		std::cout << std::endl;
#endif
		trace_idx = 0;
		return 0;
	}
	if (_passP && !_passQ) {
		counter_example_set = trace;
		std::cerr << "Encounter counter examples." << std::endl;
		std::cerr << "Trace: " << std::endl << "init:";
		for (int i = 0; i < trace_idx; i++)
			std::cout << negative_set[negative_idx++] << "-->";
		std::cout << "end" << std::endl;
		exit(-1);
	}
	if (!_passP && _passQ) {
		//		for (int i = 0; i < trace_idx; i++)
		//			question_set[question_idx++].copy(trace[i]);
		trace_idx = 0;
		return 0;
	}
	return -1;
}



