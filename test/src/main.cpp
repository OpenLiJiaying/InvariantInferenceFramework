/*************************************************************************
  > File Name: main.cpp
  > Author: Li Jiaying
  > Mail: lijiaying1989@gmail.com 
  > Created Time: Wed Oct 28 01:47:17 2015
 ************************************************************************/
#include <iostream>
#include <float.h>
#include <string.h>
#include "../include/header.h"

int minv = -100, maxv = 100;
void print_null(const char *s) {}

States global_states_sets[4];
States* gsets = &global_states_sets[1];

double Pset[max_items][vars], Nset[max_items][vars], Qset[q_items][vars];
int pIndex, nIndex, qIndex;
int qNum;


double agent_label[max_items * 2];
double* agent_set[max_items * 2];



Solution inputs;
enum{NEG=-1, QST, POS, CNE};

void run_target(Solution inp)
{
	//	std::cout << "inputs" << sol <<  std::endl;
	before_loop();
	m_double(inp.x);
	after_loop();
}


#ifdef __OPT
int fromSetToProblem(double** set, int length, int label, svm_problem& pro)
{
	int start = pro.l;
	for (int i = 0; i < length; i++) 
	{
		pro.y[start + i] = label;
		pro.x[start + i] = (svm_node*)&set[i];
	}
	pro.l += length;
	return length;
}
#endif


int main(int argc, char** argv)
{
	gsets[-1].label = -1;
	gsets[0].label = 0;
	gsets[1].label = 1;
	gsets[2].label = 2;
	if (argc < 1) {
		std::cout << "Arguments less than 2.\n";
		exit(-1);
	}	
	if (argc >= 3) {
		minv = atoi(argv[1]);
		maxv = atoi(argv[2]);
	}

	srand(time(NULL));
	bool bSvmI = false;

	int oldpIndex = 0, oldnIndex = 0;
	pIndex = 0;
	nIndex = 0;
	qIndex = 0;
	qNum = 0;

	for (int i = 0; i < 2 * max_items; i++)
		agent_label[i] = -1;


	for (int i = 0; i < inputs_init; i++) {
		Equation::linearSolver(NULL, &inputs);
		std::cout << inputs << " | ";
		run_target(inputs);
	}
	std::cout << std::endl;

	for (int i = -1; i < 2; i++)
		std::cout << gsets[i] << std::endl;

	return 0;




/*	TS = new TraceSet<int>[4]();
	TS = &TS[1];
	inputs = new Solution<int>();

	Equation* p = NULL;
	bool bCon = false;
	srand(time(NULL));
	SVM_algo *psvm = new SVM_algo(print_null);

#ifdef __OPT
	psvm->problem.y = agent_label;
	psvm->problem.x = (svm_node**)agent_set;
#endif

	int rnd;
	for (rnd = 1; rnd <= max_iter; rnd++) {
		psvm->equation = NULL;
#ifndef __OPT
		TS[POS].reset();
		TS[NEG].reset();
#endif
*/
		/*
		 *	The first round is very special, so we put this round apart with its following rounds.
		 *	1> We used random values as inputs for program executions in the first round.
		 *	2> We need to make sure there are at last two classes of generated traces. "positive" and "negative"
		 */
/*
init:
		if (rnd == 1) {
			//std::cout << "[1]******************************************************" << std::endl;
			std::cout << "[" << rnd << "]-----------------------------------------------------------------------------------------------------"
				<< std::endl;
			std::cout << "\t(1) running programs... [" << inputs_init << "] {" ;
			for (int i = 0; i < inputs_init; i++) {
				Equation::linearSolver(NULL, inputs);
				std::cout << *inputs <<" | "; 
				run_target(inputs);
			}


			if (TS[CNE].length > 0) {
				std::cout << "}\nProgram BUG! Encountered a counter-example." << std::endl;
				std::cout << TS[CNE] << std::endl;
				return -1;
			}

#ifndef __OPT
			if (TS[NEG].length == 0 || TS[POS].length == 0) {
				if (TS[NEG].length == 0) std::cout << "Negative 0" << std::endl;
				if (TS[POS].length == 0) std::cout << "Positive 0" << std::endl;
				goto init;
			}
#else
			if (pIndex == 0 || nIndex == 0) {
				if (pIndex == 0) std::cout << "Negative 0" << std::endl;
				if (nIndex == 0) std::cout << "Positive 0" << std::endl;
				goto init;
			}
#endif			
		}
		else {
			std::cout << "[" << rnd << "]-----------------------------------------------------------------------------------------------------"
				<< std::endl;
			std::cout << "\t(1) running programs...[" << inputs_aft << "] {";
			for (int i = 0; i < inputs_aft; i++) {
				Equation::linearSolver(p, inputs);
				std::cout << *inputs << " | ";
				run_target(inputs);
			}
		}
		std::cout << "}" << std::endl;

		if (TS[CNE].length > 0) {
			std::cout << "Program BUG! Encountered counter-example(s)." << std::endl;
			std::cout << TS[CNE] << std::endl;
			return -1;
		}

		std::cout << "\t(2) start training process...[";
#ifndef __OPT
		std::cout << "+" << TS[POS].length << " | -" << TS[NEG].length << " | ";
		psvm->insertFromTraceSet<int>(&TS[POS]);
		psvm->insertFromTraceSet<int>(&TS[NEG]);
#else
		std::cout << "+" << pIndex - oldpIndex << " | -" << nIndex - oldnIndex << " | ";
		memmove(agent_set + pIndex, agent_set + oldpIndex, oldnIndex * sizeof(double*));
		for (int i = oldpIndex; i < pIndex; i++) {
			agent_set[i] = Pset[i];
			agent_label[i] = 1;
		}
		for (int i = oldnIndex; i < nIndex; i++) {
			agent_set[pIndex + i] = Nset[i];
		}
		psvm->problem.l = pIndex + nIndex;
		oldpIndex = pIndex;
		oldnIndex = nIndex;
#endif

		std::cout << psvm->size() << "]" << std::endl;
		//	std::cout << psvm->problem.l << "]" << std::endl;
		//	std::cout << &(psvm->problem) << std::endl;
		//	std::cout << "SVM_PROBLEM: " << std::endl;
		//	std::cout << &(psvm->problem) << std::endl; 
		psvm->train();
		std::cout << "\t |-->> " << *psvm; //<< std::endl;
		*/

		/*
		 *	check on its own training data.
		 *	There should be no prediction errors.
		 */
/*		std::cout << "\t(3) checking on training traces.";
		double passRat = psvm->predictOnProblem();
		std::cout << " [" << passRat * 100 << "%]";
		if (passRat < 1) {
			std::cout << " [FAIL] \n The problem is not linear separable.. Trying to solve is by SVM-I algo" << std::endl;
			std::cerr << "*******************************USING SVM_I NOW******************************" << std::endl;
			bSvmI = true;
			break;
		}
		std::cout << " [PASS]" << std::endl;
*/

		/*
		 *	Check on Question traces.
		 *	There should not exists one traces, in which a negative state is behind a positive state.
		 */
//		std::cout << "\t(4) checking on Question traces.";
/*		std::cout << "[" << qNum << "]";
		if (qIndex != 0) {
			std::cout << std::endl;
			int num = 0;
			for (int i = 0; i < qIndex; i++) {
				num++;
				int pre = -1, cur = 0;
				std::cout << "\t\t" << num << ">";
				while (Qset[i][0] != PI) {
					cur = Equation::calc<double>(psvm->equation, Qset[i]);
					std::cout << "(" << Qset[i][0];
					for (int j = 1; j < vars; j++)
						std::cout << "," << Qset[i][j];
					std::cout << ")";
					std::cout << ((cur >= 0) ? "[+]" : "[-]");
					std::cout << " -> ";
					if ((pre > 0) && (cur < 0)) {
						std::cerr << "\n Predict wrongly on Question traces." << std::endl;
						return -1;
					}
					pre = cur;
					i++;
				}
				std::cout << "END" << std::endl;
			}
		}
		std::cout << "\t\t [PASS]" << std::endl;
*/

/*		std::cout << " [" << TS[QST].length << "]";
		if (TS[QST].length != 0) {
			for (Trace<int>* t = TS[QST].first; t != NULL; t = t->next) {
				if (t->length <= 1) continue;
				int pre = -1, cur = 0;
				std::cout << std::endl;
				std::cout << "\t\t##" << *t << " >>> ";
				for (State<int>* ls = t->first; ls != NULL; ls = ls->next) {
					cur = Equation::calc<int>(psvm->equation, ls->values);
					std::cout << ((cur >= 0) ? "+" : "-");
					if ((pre > 0) && (cur < 0)) {
						std::cerr << "\n Predict wrongly on Question traces." << std::endl;
						return -1;
					}
					pre = cur;
				}
			}
		}
		//std::cout << "\t    ";
		std::cout << " [PASS]" << std::endl;
*/

		/*
		 *	bCon is used to store the convergence check return value for the last time.
		 *	We only admit convergence if the three consecutive round are converged.
		 *	This is to prevent in some round the points are too right to adjust the classifier.
		 */
/*		std::cout << "\t(5) check convergence: ";
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


	if (!bSvmI) {
		// finish classification...
		std::cout << "*********************************************************************************************************" << std::endl;
		if (rnd == max_iter)
			std::cout << "[THE END] Reach the maximum rounds of iteration[" << rnd 
				<< "]-------------------------------------------------------" << std::endl;
		else 
			std::cout << "[THE END] Finally get converged after [" << rnd 
				<< "] iterations----------------------------------------------------" << std::endl;


		Equation equ;
		psvm->roundoff(&equ);
		std::cout << "Hypothesis Invairant: {\n";
		std::cout << "\t\t" << equ << std::endl;
		std::cout << "}" << std::endl;
		delete p;
		delete psvm->equation;
		return 0;
	}



	//start_svm_i:
	std::cout << "start SVM-I" << std::endl;
	SVM_I_algo * psvmi = new SVM_I_algo(print_null);

	rnd = 1;
	Equation **pEqu = NULL;
	int pEquNum = 0;
	while (rnd < 16) {
		if (rnd != 1) {
			std::cout << "[" << rnd << "]-----------------------------------------------------------------------------------------------------"
				<< std::endl;
			std::cout << "\t(1) running programs...[" << psvmi->equ_num << "] {";
			for (int i = 0; i < psvmi->equ_num; i++) {
				Equation::linearSolver(psvmi->equation[i], inputs);
				std::cout << *inputs << " | ";
				run_target(inputs);
			}
		}
#ifdef __OPT
		std::cout << "\t(2) start training process...[";
		std::cout << "+" << pIndex - oldpIndex << " | -" << nIndex - oldnIndex << " | ";
		memmove(agent_set + pIndex, agent_set + oldpIndex, oldnIndex * sizeof(double*));
		for (int i = oldpIndex; i < pIndex; i++) {
			agent_set[i] = Pset[i];
			agent_label[i] = 1;
		}
		for (int i = oldnIndex; i < nIndex; i++) {
			agent_set[pIndex + i] = Nset[i];
		}
		oldpIndex = pIndex;
		oldnIndex = nIndex;

		agent_set[pIndex + nIndex] = agent_set[pIndex];
		psvmi->problem1.l = pIndex;
		psvmi->problem2.l = nIndex;
		psvmi->problem1.y = agent_label;
		psvmi->problem2.y = agent_label + pIndex + 1;
		psvmi->problem1.x = (svm_node**)agent_set;
		psvmi->problem2.x = (svm_node**)(agent_set + pIndex + 1);
		std::cout << psvmi->size() << "]" << std::endl;
#else
		std::cout << "\t(2) start training process...[";
		std::cout << "+" << TS[POS].length << " | -" << TS[NEG].length << " | ";
		psvmi->insertFromTraceSet<int>(&TS[POS]);
		psvmi->insertFromTraceSet<int>(&TS[NEG]);
		std::cout << psvm->size() << "]" << std::endl;
		//	std::cout << psvm->problem.l << "]" << std::endl;
		//	std::cout << &(psvm->problem) << std::endl;
#endif
		psvmi->train();
		std::cout << "\t |-->> " << *psvmi; //<< std::endl;
*/
		/*
		 *	check on its own training data.
		 *	There should be no prediction errors.
		 */
/*		std::cout << "\t(3) checking on training traces.";
		double passRat = psvmi->predictOnProblem();
		std::cout << " [" << passRat * 100 << "%]";
		if (passRat < 1) {
			//std::cout << " [FAIL] at SVM-I algo" << std::endl;
			//exit(-1);
		}
		std::cout << " [PASS]" << std::endl;
*/

		/*
		 *	Check on Question traces.
		 *	There should not exists one traces, in which a negative state is behind a positive state.
		 */
//		std::cout << "\t(4) checking on Question traces.";
//		std::cout << " [" << TS[QST].length << "]";


		//std::cout << "\t    ";
//		std::cout << " NEED TO BE DONE HERE... [PASS]" << std::endl;


		/*
		 *	bCon is used to store the convergence check return value for the last time.
		 *	We only admit convergence if the three consecutive round are converged.
		 *	This is to prevent in some round the points are too right to adjust the classifier.
		 */
/*		std::cout << "\t(5) check convergence: ";
		int converge = 0;
		if (psvmi->equ_num != pEquNum)
			std::cout << "\t [FAIL] neXt round " << std::endl;
		else {
			for (int i = 0; i < pEquNum; i++){
				if (psvm->equation[i].isSimilar(pEqu[i]) != 0) {
					converge = -1;
					break;
				}
			}
			if (converge == 0) {
				std::cout << "[TT] \t[SUCCESS] rounding off" << std::endl;
				break;
			} else if (converge == -1) {
					std::cout << "[FT]";
					std::cout << "\t [FAIL] neXt round " << std::endl;
			}
		}

		pEqu = psvmi->equation;
		pEquNum = psvmi->equ_num;
		rnd++;
	}

	std::cout << "finish running svm-I for " << rnd << "times." << std::endl; 
	Equation equs[32];
	int equ_num = psvmi->roundoff(equs);
	std::cout << "Hypothesis Invairant: {\n";
	std::cout << "\t     " << equs[0] << std::endl;
	for (int i = 1; i < equ_num; i++)
		std::cout << "\t  /\\ " << equs[i] << std::endl;
	std::cout << "}" << std::endl;
	*/
	return 0;
}
