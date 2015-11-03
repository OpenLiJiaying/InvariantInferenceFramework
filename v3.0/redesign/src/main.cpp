/*************************************************************************
  > File Name: main.cpp
  > Author: Li Jiaying
  > Mail: lijiaying1989@gmail.com 
  > Created Time: Wed Oct 28 01:47:17 2015
 ************************************************************************/
#include <iostream>
#include <float.h>
#include "../include/header.h"

int minv = -100, maxv = 100;
void print_null(const char *s) {}

LoopTrace<int>* LT;
TraceSet<int>* TS;

Solution<int>* inputs;



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
	TS = new TraceSet<int>[4]();
	TS = &TS[1];
	inputs = new Solution<int>();

	int rnd = 1;
	srand(time(NULL));
	SVM_algo *psvm = new SVM_algo(print_null);
	std::cout << "[1]******************************************************" << std::endl;
	std::cout << "\t(1) running programs... [" << inputs_init <<"]" << std::endl;
init:
	for (int i = 0; i < inputs_init; i++) {
		Equation::linearSolver(NULL, inputs);
		LT = new LoopTrace<int>();
		before_loop();
		m(inputs->x);
		after_loop();
	}

	if (TS[-1].first == NULL || TS[1].first == NULL)
		goto init;

start_svm:	
	std::cout << "\t(2) start training process...[" << psvm->problem.l << "]" << std::endl;
	psvm->insertFromTraceSet<int>(&TS[1]);
	psvm->insertFromTraceSet<int>(&TS[-1]);
//	std::cout << &(psvm->problem) << std::endl;
//	std::cout << "after converting" << std::endl;
//	//	std::cout << "SVM_PROBLEM: " << std::endl;
//	std::cout << &(psvm->problem) << std::endl; 
	psvm->classify();
//	std::cout << "after classify" << std::endl;
	std::cout << "\t(3) RESULT: " << psvm->equation; // << std::endl;
	double passRat = psvm->predictOnProblem();
	std::cout << " \t passrate=[" << passRat * 100 << "%]." << std::endl;
	if (passRat < 1) {
		std::cout << "The problem is not linear separable.. Trying to solve is by SVM-I algo" << std::endl;
		goto start_svm_i;
	}


	rnd++;
	if (rnd <= max_iter) {
		std::cout << "[" << rnd << "]*********************************************************" << std::endl;
		std::cout << "\t(1) running programs...[" << inputs_aft << "]" << std::endl;
		for (int i = 0; i < inputs_aft; i++) {
			//std::cout << "NEXT INPUTS:  ";
			//Equation::linearSolver(psvm->equation, inputs);
			Equation::linearSolver(NULL, inputs);
			//std::cout << inputs <<  std::endl;
			LT = new LoopTrace<int>();
			before_loop();
			m(inputs->x);
//			std::cout << LT;
			after_loop();
//			std::cout << LT << std::endl;
		}
		goto start_svm;

	}

	std::cout << "[THE END] Reaching the maximum round of iteration[" << rnd << "]************************" << std::endl;

start_svm_i:
	//

	return 0;
}
