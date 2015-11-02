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
#include "header.h"

//int trace_idx = 0;

bool _passP = false;
bool _passQ = false;
char *lt[10] =  { "Negative", "UnSet", "Positive", "Question", "Bugtrace"};
char** LabelTable = &lt[1];

int before_loop()
{
	std::cout << "---> before_loop";
	_passP = false;
	_passQ = false;
	std::cout << "[done]";
	return 0;
}


int after_loop()
{
	int label = 0;
	std::cout << "---> after_loop.";
	if (_passP && _passQ) {
		label = 1; 
	} else if (!_passP && !_passQ) {
		label = -1; 
	} else if (!_passP && _passQ) {
		label = 2; 
	} else if (_passP && !_passQ) {
		label = 0;
		std::cout << "Program BUG! Encountered a counter-example." << std::endl;
		std::cout << "loop traces: " << LT << std::endl;
		return -1;
	}
	LT->labeling(label);
	TS[label].addLoopTrace(LT);
	std::cout << "[done]" << LabelTable[label] << "\n";
	return 0;
}
