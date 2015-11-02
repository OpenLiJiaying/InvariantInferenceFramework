/*************************************************************************
  > File Name: main.cpp
  > Author: Li Jiaying
  > Mail: lijiaying1989@gmail.com 
  > Created Time: Wed Oct 28 01:47:17 2015
 ************************************************************************/
#include <iostream>
#include <float.h>
#include "header.h"

int minv = -200, maxv = 200;
void print_null(const char *s) {}
TraceSet<int>* TS;

int inputs[vars];



int main(int argc, char** argv)
{
	if (argc < 1) {
		std::cout << "Arguments less than 2.\n";
		exit(-1);
	}	
	if (argc >= 3) {
		minv = atoi(argv[1]);
		maxv = atoi(argv[2]);
	}
	TS = new TraceSet<int>[3]();

	int rnd = 1;
	srand(time(NULL));
#ifdef _TEST0_
	std::cout << "[1]******************************************************" << std::endl;
	std::cout << "\t(1) running programs... [" << inputs_init <<"]" << std::endl;
#endif
init:
	for (int i = 0; i < inputs_init; i++) {
		for (int j = 0; j < vars; j++) {
			inputs[j] = rand() % (maxv - minv + 1) + minv;
		}
		before_loop();
		m(inputs);
		after_loop();
	}
//	std::cout << "\t(2) program Trace Set... " /*<<"{" << TS <<"\n}"*/ << std::endl;

	SVM_algo *psvm = new SVM_algo(print_null);
	std::cout << "\t(2) converting data into svm format..." << std::endl;
	if (TS[0].first == NULL || TS[1].first == NULL)
		goto init;

start_processing:	
	std::cout << "START TRAINING..." << std::endl;
//	std::cout << &TS[1] << std::endl;
	psvm->insertFromTraceSet2SVMProblem<int>(&TS[1]);
//	std::cout << &(psvm->problem) << std::endl;
	std::cout << "Positive done" << std::endl;
//	std::cout << &TS[0] << std::endl;
	psvm->insertFromTraceSet2SVMProblem<int>(&TS[0]);
//	std::cout << &(psvm->problem) << std::endl;
	std::cout << "after converting" << std::endl;
//	std::cout << "SVM_PROBLEM: " << std::endl;
//	std::cout << &(psvm->problem) << std::endl; 
	psvm->classify();
	std::cout << "after classify" << std::endl;
	std::cout << "RESULT: " << psvm->equation << std::endl;

/*
	std::cout << "\t(3) svm training...[" << sl.l << "]" << std::endl;
	svm_free_and_destroy_model(&model);
*/

	rnd++;
	if (rnd <= max_iter) {
#ifdef _TEST0_
		std::cout << "[" << rnd << "]*********************************************************" << std::endl;
		std::cout << "\t(1) running programs...[" << inputs_aft << "]" << std::endl;
#endif
		std::cout << inputs_aft << std::endl;
		for (int i = 0; i < inputs_aft; i++) {
			psvm->equation->linearSolver(inputs);
			for (int j = 0; j < vars; j++)
				std::cout << " " << inputs[j];
			std::cout << std::endl;
			before_loop();
			m(inputs);
			after_loop();
			std::cout << "NEXT INPUTS:  ";
		}
		goto start_processing;

	}



	return 0;
}
