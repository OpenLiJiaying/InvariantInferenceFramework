/*************************************************************************
  > File Name: main.cpp
  > Author: Li Jiaying
  > Mail: lijiaying1989@gmail.com 
  > Created Time: Wed Oct 28 01:47:17 2015
 ************************************************************************/
#include <iostream>
#include <float.h>
#include "../include/header.h"

int minv = -200, maxv = 200;
void print_null(const char *s) {}

LoopTrace<int>* LT;
TraceSet<int>* TS;
Solution<int>* inputs;

enum{NEG=-1, XUSE, POS, QST, CNE};

void run_target(Solution<int>* sol)
{
	LT = new LoopTrace<int>();
//	std::cout << "inputs" << sol <<  std::endl;
	before_loop();
	m(sol->x);
	after_loop();
}


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
	Equation* p = NULL;
	bool bCon = false;
	srand(time(NULL));
	SVM_algo *psvm = new SVM_algo(print_null);
	//std::cout << "[1]******************************************************" << std::endl;
	std::cout << "[" << rnd << "]-----------------------------------------------------------------------------------------------------" << std::endl;
	std::cout << "\t(1) running programs... [" << inputs_init <<"]" << std::endl;



init:
	/* 
	 *	The first round is very special, so we put this round apart with its following rounds.
	 *	1> We used random values as inputs for program executions in the first round.
	 *	2> We need to make sure there are at last two classes of generated traces. "positive" and "negative" 
	 */
	for (int i = 0; i < inputs_init; i++) {
		Equation::linearSolver(NULL, inputs);
		run_target(inputs);
	}

	if (TS[-1].first == NULL || TS[1].first == NULL)
		goto init;



start_svm:	
	std::cout << "\t(2) start training process...[";
	psvm->insertFromTraceSet<int>(&TS[POS]);
	psvm->insertFromTraceSet<int>(&TS[NEG]);
	std::cout << psvm->problem.l << "]" << std::endl;
//	std::cout << &(psvm->problem) << std::endl;
//	std::cout << "SVM_PROBLEM: " << std::endl;
//	std::cout << &(psvm->problem) << std::endl; 
	psvm->classify();
	std::cout << "\t    RESULT: " << psvm->equation << std::endl;


	/*	
	 *	check on its own training data.
	 *	There should be no prediction errors.
	 */
	std::cout << "\t(3) checking on training traces.";
	double passRat = psvm->predictOnProblem();
	std::cout << " [" << passRat * 100 << "%]";
	if (passRat < 1) {
		std::cout << " [FAIL] The problem is not linear separable.. Trying to solve is by SVM-I algo" << std::endl;
		goto start_svm_i;
	}
	std::cout << " [PASS]" << std::endl;


	/*
	*	Check on Question traces.
	*	There should not exists one traces, in which a negative state is behind a positive state.
	*/
	std::cout << "\t(4) checking on Question traces.";
	if (TS[QST].first == NULL) std::cout << "No such traces...";
	//std::cout << std::endl;
	for (LoopTrace<int>* lt = TS[QST].first; lt != NULL; lt = lt->next) {
		if (lt->length <= 1) continue;
		int pre = -1, cur = 0;
		std::cout << std::endl;
		std::cout << "\t\t##" << lt << " >>> ";
		for (LoopState<int>* ls = lt->first; ls != NULL; ls = ls->next) {
			cur = Equation::calc<int>(psvm->equation, ls->values);
			std::cout << ((cur >= 0) ? "+" : "-");
			if ((pre > 0) && (cur < 0)) {
				std::cerr << "Predict wrongly on Question traces." << std::endl;
				return -1;
			}
			pre = cur;
		}
	}
	//std::cout << "\t    ";
	std::cout << " [PASS]" << std::endl;



	
	/*
	 *	bCon is used to store the convergence check return value for the last time.
	 *	We only admit convergence if the three consecutive round are converged.
	 *	This is to prevent in some round the points are too right to adjust the classifier.
	 */
	std::cout << "\t(5) check convergence: ";
	if (psvm->equation->isSimilar(p) == 0) {
		if (bCon == true) {
			std::cout << "[TT] \t[SUCCESS] rounding off" << std::endl;
			goto svm_end;
		}
		std::cout << "[FT] \t[FAIL] neXt round" << std::endl;
		bCon = true;
	} else {
		std::cout << ((bCon == true) ? "[T" : "[F") << "F] ";
		bCon = false;
	}
	std::cout << "\t [FAIL] neXt round " << std::endl;
	if (p != NULL) {
		delete p;
	}
	p = psvm->equation;
	psvm->equation = NULL;




	rnd++;
	if (rnd <= max_iter) {
		std::cout << "[" << rnd << "]-----------------------------------------------------------------------------------------------------" << std::endl;
		std::cout << "\t(1) running programs...[" << inputs_aft << "]" << std::endl;
		for (int i = 0; i < inputs_aft; i++) {
			Equation::linearSolver(p, inputs);
			
			run_target(inputs);
		}
		goto start_svm;

	}


svm_end:
	if (rnd == max_iter)
		std::cout << "[THE END] Reach the maximum rounds of iteration[" << rnd 
		<< "]-------------------------------------------------------" << std::endl;
	else 
		std::cout << "[THE END] Finally get converged after [" << rnd 
		<< "] iterations----------------------------------------------------" << std::endl;

	std::cout << "*********************************************************************************************************" << std::endl;
	psvm->equation->roundoff(p);
	std::cout << "\t\t"<< p << std::endl;
	delete p;
	delete psvm->equation;


start_svm_i:
	//

	return 0;
}
