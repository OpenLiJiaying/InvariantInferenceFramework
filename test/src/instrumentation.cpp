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
#include "../include/header.h"


bool _passP = false;
bool _passQ = false;
char lt[4][10] =  { "Negative", "Question", "Positive", "Bugtrace"};
char(*LabelTable)[10] = &lt[1];

double temp_states[256][vars];
int temp_index;


int add_state_I(int first ...)
{
	va_list ap;
	va_start(ap, first);
	temp_states[temp_index][0] = first;
	for (int i = 1; i < vars; i++) {
		temp_states[temp_index][i] = va_arg(ap, int);
	}
	va_end(ap);
	temp_index++;
	return 0;
}

int add_state_D(double first, ...)
{
	va_list ap;
	va_start(ap, first);
	temp_states[temp_index][0] = first;
	for (int i = 1; i < vars; i++) {
		temp_states[temp_index][i] = va_arg(ap, double);
	}
	va_end(ap);
	temp_index++;
	return 0;
}


int before_loop()
{
	//std::cout << "---> before_loop";
	temp_index = 0;
	_passP = false;
	_passQ = false;
	//std::cout << "[done]";
	return 0;
}


int after_loop()
{
	int label = 0;
	if (_passP && _passQ) {
		label = 1;
	} else if (!_passP && !_passQ) {
		label = -1; 
	} else if (!_passP && _passQ) {
		label = 0; 
	} else if (_passP && !_passQ) {
		label = 2;
	}

	gsets[label].add_states(temp_states, temp_index);
	//TS[label].addLoopTrace(LT);
	return 0;
}


int m_double(double* p)
{
	int a[vars];
	for (int i = 0; i < vars; i++)
		a[i] = static_cast<int>(p[i]);
	return m_int(a);
}

int m_int(int* p)
{
	return m(p);
}