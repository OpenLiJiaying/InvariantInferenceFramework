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

Trace<int>* LT;
TraceSet<int>* TS;
Solution<int>* inputs;

enum{NEG=-1, XUSE, POS, QST, CNE};

void run_target(Solution<int>* sol)
{
	LT = new Trace<int>();
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
	TS = new TraceSet<int>[5]();
	TS = &TS[XUSE];
	inputs = new Solution<int>();

	Equation* p = NULL;
	bool bCon = false;
	srand(time(NULL));
	SVM_algo *psvm = new SVM_algo(print_null);




	int rnd;
	for (rnd = 1; rnd <= max_iter; rnd++) {
		psvm->equation = NULL;
		TS[POS].reset();
		TS[NEG].reset();
		/*
		*	The first round is very special, so we put this round apart with its following rounds.
		*	1> We used random values as inputs for program executions in the first round.
		*	2> We need to make sure there are at last two classes of generated traces. "positive" and "negative"
		*/
	init:
		if (rnd == 1) {
			//std::cout << "[1]******************************************************" << std::endl;
			std::cout << "[" << rnd << "]-----------------------------------------------------------------------------------------------------"
				<< std::endl;
			std::cout << "\t(1) running programs... [" << inputs_init << "] {" ;
			for (int i = 0; i < inputs_init; i++) {
				Equation::linearSolver(NULL, inputs);
				std::cout << inputs <<" | "; 
				run_target(inputs);
			}

			if (TS[CNE].length > 0) {
				std::cout << "}\nProgram BUG! Encountered a counter-example." << std::endl;
				std::cout << &TS[CNE] << std::endl;
				return -1;
			}

			if (TS[NEG].length == 0 || TS[POS].length == 0) {
				if (TS[NEG].length == 0) std::cout << "Negative 0" << std::endl;
				if (TS[POS].length == 0) std::cout << "Positive 0" << std::endl;
				goto init;
			}
				
		}
		else {
			std::cout << "[" << rnd << "]-----------------------------------------------------------------------------------------------------"
				<< std::endl;
			std::cout << "\t(1) running programs...[" << inputs_aft << "] {";
			for (int i = 0; i < inputs_aft; i++) {
				Equation::linearSolver(p, inputs);
				std::cout << inputs << " | ";
				run_target(inputs);
			}
		}
		std::cout << "}" << std::endl;

		if (TS[CNE].length > 0) {
			std::cout << "Program BUG! Encountered a counter-example." << std::endl;
			std::cout << &TS[CNE] << std::endl;
			return -1;
		}

		std::cout << "\t(2) start training process...[";
		std::cout << "+" << TS[POS].length << " | -" << TS[NEG].length << " | ";
		psvm->insertFromTraceSet<int>(&TS[POS]);
		psvm->insertFromTraceSet<int>(&TS[NEG]);
		std::cout << psvm->size() << "]" << std::endl;
		//	std::cout << psvm->problem.l << "]" << std::endl;
		//	std::cout << &(psvm->problem) << std::endl;
		//	std::cout << "SVM_PROBLEM: " << std::endl;
		//	std::cout << &(psvm->problem) << std::endl; 
		psvm->train();
		std::cout << "\t |-->> " << psvm; //<< std::endl;


		/*
		*	check on its own training data.
		*	There should be no prediction errors.
		*/
		std::cout << "\t(3) checking on training traces.";
		double passRat = psvm->predictOnProblem();
		std::cout << " [" << passRat * 100 << "%]";
		if (passRat < 1) {
			std::cout << " [FAIL] The problem is not linear separable.. Trying to solve is by SVM-I algo" << std::endl;
			std::cerr << "*******************************USING SVM_I NOW******************************" << std::endl;
			psvm = new SVM_I_algo(print_null);
			rnd = 1;
			goto init;
		}
		std::cout << " [PASS]" << std::endl;


		/*
		*	Check on Question traces.
		*	There should not exists one traces, in which a negative state is behind a positive state.
		*/
		std::cout << "\t(4) checking on Question traces.";
		 
		std::cout << " [" << TS[QST].length << "]";
		if (TS[QST].length != 0) {
			for (Trace<int>* lt = TS[QST].first; lt != NULL; lt = lt->next) {
				if (lt->length <= 1) continue;
				int pre = -1, cur = 0;
				std::cout << std::endl;
				std::cout << "\t\t##" << lt << " >>> ";
				for (State<int>* ls = lt->first; ls != NULL; ls = ls->next) {
					cur = Equation::calc<int>(psvm->equation, ls->values);
					std::cout << ((cur >= 0) ? "+" : "-");
					if ((pre > 0) && (cur < 0)) {
						std::cerr << "Predict wrongly on Question traces." << std::endl;
						return -1;
					}
					pre = cur;
				}
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
				break;
			}
			std::cout << "[FT]";
			bCon = true;
		}
		else {
			std::cout << ((bCon == true) ? "[T" : "[F") << "F] ";
			bCon = false;
		}
		std::cout << "\t [FAIL] neXt round " << std::endl;
		if (p != NULL) {
			delete p;
		}
		p = psvm->equation;	
	}



	// finish classification...
	std::cout << "*********************************************************************************************************" << std::endl;
	if (rnd == max_iter)
		std::cout << "[THE END] Reach the maximum rounds of iteration[" << rnd 
		<< "]-------------------------------------------------------" << std::endl;
	else 
		std::cout << "[THE END] Finally get converged after [" << rnd 
		<< "] iterations----------------------------------------------------" << std::endl;

	
	Equation equs[8];
	int equ_num = psvm->roundoff(equs);
	std::cout << "Hypothesis Invairant: {\n";
	std::cout << "\t\t" << &equs[0] << std::endl;
	for (int i = 1; i < equ_num; i++)
		std::cout << "\t  /\\ " << &equs[i] << std::endl;
	std::cout << "}" << std::endl;
	delete p;
	delete psvm->equation;
	return 0;


start_svm_i:
	SVM_I_algo *psvmi = new SVM_I_algo(print_null);
	psvmi->insertFromTraceSet<int>(&TS[POS]);
	psvmi->insertFromTraceSet<int>(&TS[NEG]);


	return 0;
}
