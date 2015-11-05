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

#ifdef __OPT
template<class T>
static int insertTrace(int label, Trace<T>* t)
{
	if (label == 1) {
		for (State<T>* s = t->first; s != NULL; s = s->next) {
			for (int i = 0; i < vars; i++)
				Pset[pIndex][i] = s->values[i];
			pIndex++;
		}
	} else if (label == -1) {
		for (State<T>* s = t->first; s != NULL; s = s->next) {
			for (int i = 0; i < vars; i++)
				Nset[nIndex][i] = s->values[i];
			nIndex++;
		}
	}
	return 0;
}
#endif


int after_loop()
{
	int label = 0;
	if (_passP && _passQ) {
		label = 1;
#ifdef __OPT
		insertTrace<int>(1, LT);
		return 0;
#endif
	} else if (!_passP && !_passQ) {
		label = -1; 
#ifdef __OPT
		insertTrace<int>( -1, LT);
		return 0;
#endif
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
