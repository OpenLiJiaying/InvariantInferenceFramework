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
#include "../include/svm.h"
#include "../include/svm_i.h"
#include "../include/svm_ii.h"
#include "../include/color.h"

int minv = -100, maxv = 100;
void print_null(const char *s) {}

States global_states_sets[4];
States* gsets = &global_states_sets[1];



double training_label[max_items * 2];
double* training_set[max_items * 2];
Solution inputs;


void run_target(Solution inp)
{
	//	std::cout << "inputs" << sol <<  std::endl;
	before_loop();
	m_double(inp.x);
	after_loop();
	if (gsets[COUNT_EXAMPLE].traces_num() > 0) {
		std::cout << "}\nProgram BUG! Program have encountered a Counter-Example trace." << std::endl;
		std::cout << gsets[COUNT_EXAMPLE] << std::endl;
		exit(-1);
	}
}



void init_gsets()
{
	gsets[NEGATIVE].label = -1;
	gsets[QUESTION].label = 0;
	gsets[POSITIVE].label = 1;
	gsets[COUNT_EXAMPLE].label = 2;
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

	srand(time(NULL)); // initialize seed for rand() function
	init_gsets();


	for (int i = 0; i < 2 * max_items; i++)
		training_label[i] = -1;


	//for (int i = -1; i < 3; i++)
	//	std::cout << gsets[i] << std::endl;

	int rnd;
	bool b_similar_last_time = false;
	Equation* p = NULL;

	SVM svm(print_null);
	svm.problem.x = (svm_node**)(training_set);
	svm.problem.y = training_label;

	int pre_positive_size = 0, pre_negative_size = 0, pre_question_size = 0;
	int cur_positive_size = 0, cur_negative_size = 0, cur_question_size = 0;

	for (rnd = 1; rnd <= max_iter; rnd++) {
		svm.main_equation = NULL;
		/*
		*	The first round is very special, so we put this round apart with its following rounds.
		*	1> We used random values as inputs for program executions in the first round.
		*	2> We need to make sure there are at last two classes of generated traces. "positive" and "negative"
		*/

	init:
		std::cout << "[" << rnd << "]--------------------------------------------------" << "-------------------------------------------------------------" << std::endl;
		if (rnd == 1) {
			std::cout << "\t(1) execute programs... [" << init_exes << "] {";
			for (int i = 0; i < init_exes; i++) {
				Equation::linearSolver(NULL, inputs);
				std::cout << inputs;
				if (i < init_exes - 1) std::cout << "|";
				run_target(inputs);
			}
			std::cout << "}" << std::endl;

			if (gsets[POSITIVE].traces_num() == 0 || gsets[NEGATIVE].traces_num() == 0) {
				if (gsets[POSITIVE].traces_num() == 0) std::cout << "[0] Positive trace, execute program again." << std::endl;
				if (gsets[NEGATIVE].traces_num() == 0) std::cout << "[0] Negative trace, execute program again." << std::endl;
				goto init;
			}
		}
		else {
			std::cout << "\t(1) execute programs...[" << after_exes << "] {";
			for (int i = 0; i < after_exes; i++) {
				Equation::linearSolver(p, inputs);
				std::cout << inputs;
				if (i < after_exes - 1) std::cout << " | ";
				run_target(inputs);
			}
			std::cout << "}" << std::endl;
		}

		std::cout << "\t(2) prepare training data... ";
		svm.prepare_training_data(gsets, pre_positive_size, pre_negative_size);
		std::cout << std::endl;

		std::cout << "\t(3) start training... ";
		svm.train();
		std::cout << "|-->> ";
		set_console_color(std::cout);
		std::cout << svm << std::endl;
		unset_console_color(std::cout);

		

		/*
		*	check on its own training data.
		*	There should be no prediction errors.
		*/
		std::cout << "\t(4) checking training traces.";
		double passRat = svm.predict_on_training_set();
		std::cout << " [" << passRat * 100 << "%]";
		if (passRat < 1) {
			std::cout << " [FAIL] \n The problem is not linear separable.. Trying to solve is by SVM-I algo" << std::endl;
			std::cerr << "*******************************USING SVM_I NOW******************************" << std::endl;
			//bSvmI = true;
			break;
		}
		std::cout << " [PASS]" << std::endl;


		/*
		*	Check on Question traces.
		*	There should not exists one traces, in which a negative state is behind a positive state.
		*/
		std::cout << "\t(5) checking question traces.";
		std::cout << " [" << gsets[QUESTION].traces_num() << "]";
		for (int i = 0; i < gsets[QUESTION].p_index; i++) {
			int pre = -1, cur = 0;
			std::cout << ".";
			//std::cout << "\t\t" << i << ">";
			//gsets[QUESTION].print_trace(i);
			for (int j = gsets[QUESTION].index[i]; j < gsets[QUESTION].index[i + 1]; j++) {
				cur = Equation::calc(svm.main_equation, gsets[QUESTION].values[j]);
				//std::cout << ((cur >= 0) ? "+" : "-");
				if ((pre > 0) && (cur < 0)) {
					// deal with wrong question trace.
					// Trace back to print out the whole trace and the predicted labels.
					std::cerr << "\t\t[FAIL]\n \t  Predict wrongly on Question traces." << std::endl;
					gsets[QUESTION].print_trace(i);
					for (int j = gsets[QUESTION].index[i]; j < gsets[QUESTION].index[i + 1]; j++) {
						cur = Equation::calc(svm.main_equation, gsets[QUESTION].values[j]);
						std::cout << ((cur >= 0) ? "+" : "-");
					}
					std::cout << std::endl;
					return -1;
				}
				pre = cur;
			}
			//std::cout << "END" << std::endl;
		}
		std::cout << " [PASS]" << std::endl;


		/*
		*	bCon is used to store the convergence check return value for the last time.
		*	We only admit convergence if the three consecutive round are converged.
		*	This is to prevent in some round the points are too right to adjust the classifier.
		*/
		std::cout << "\t(6) check convergence:        ";
		if (svm.main_equation->is_similar(p) == 0) {
			if (b_similar_last_time == true) {
				std::cout << "[TT]  [SUCCESS] rounding off" << std::endl;
				Equation equ;
				p->roundoff(equ);
				set_console_color(std::cout);
				std::cout << "Hypothesis Invairant: {\n";
				std::cout << "\t\t" << equ << std::endl;
				std::cout << "}" << std::endl;
				unset_console_color(std::cout);
				delete p;
				delete svm.main_equation;
				return 0;
				break;
			}
			std::cout << "[FT]";
			b_similar_last_time = true;
		}
		else {
			std::cout << ((b_similar_last_time == true) ? "[T" : "[F") << "F] ";
			b_similar_last_time = false;
		}
		std::cout << "  [FAIL] neXt round " << std::endl;
		if (p != NULL) {
			delete p;
		}
		p = svm.main_equation;
	}

	return 0;

}































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
