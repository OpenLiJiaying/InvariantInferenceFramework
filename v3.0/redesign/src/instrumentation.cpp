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


//int trace_idx = 0;

bool _passP = false;
bool _passQ = false;
char lt[4][10] =  { "Negative", "Question", "Positive", "Bugtrace"};
char(*LabelTable)[10] = &lt[1];

int before_loop()
{
	//std::cout << "---> before_loop";
	_passP = false;
	_passQ = false;
	//std::cout << "[done]";
	return 0;
}

template<class T>
static int insertTrace(double** set, int& index,  Trace<T>* t)
{
	for (State<T>* s = t->first; s != NULL; s = s->next) {
		for (int i = 0; i < vars; i++)
			PS[index][i] = s->values[i];
		index++;
	}
}


int after_loop()
{
	int label = 0;
	//std::cout << "---> after_loop.";
	if (_passP && _passQ) {
		label = 1;
		//insertTrace<int>((double**)PS, pIndex, LT);
	} else if (!_passP && !_passQ) {
		label = -1; 
		//insertTrace<int>((double**)NS, nIndex, LT);
	} else if (!_passP && _passQ) {
		label = 0; 
	} else if (_passP && !_passQ) {
		label = 2;
	}
	LT->labeling(label);
	TS[label].addLoopTrace(LT);
	//std::cout << "[done]" << LabelTable[label] << std::endl;
	return 0;
}
