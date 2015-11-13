#ifndef _LIBSVM_I_H_
#define _LIBSVM_I_H_
#include "svm.h"
#include <iostream>
//#include <stdarg.h>
//#include "float.h"



class SVM_I : public SVM
{
private:
	const int max_equ;

public:
	svm_model* model;
	Equation* equations;
	int equ_num;
	svm_parameter param;
	States* negatives; 

	SVM_I(void(*f) (const char*) = NULL, Equation* eq = NULL, int equ = 8) : SVM(f), max_equ(equ) {
		negatives = NULL;
		main_equation = eq;
		equ_num = 0;
		equations = new Equation[max_equ];
		model = NULL;
	}

	~SVM_I()
	{
		if (model != NULL)
			svm_free_and_destroy_model(&model);
		delete []equations;
	}


	virtual int prepare_training_data(States* gsets, int& pre_positive_size, int& pre_negative_size) {
		negatives = &gsets[NEGATIVE];
		int cur_positive_size = gsets[POSITIVE].size();
		int cur_negative_size = gsets[NEGATIVE].size();

		std::cout << "+[";
		std::cout << cur_positive_size - pre_positive_size << "|";
		std::cout << cur_negative_size - pre_negative_size << "";
		std::cout << "] ==> [";
		std::cout << cur_positive_size << "+|";
		std::cout << cur_negative_size << "-";
		std::cout << "]";


		// prepare new training data set
		// training set & label layout:
		// data :  0 | positive states ...
		// add new positive states at OFFSET: [pre_positive_size]
		for (int i = pre_positive_size; i < cur_positive_size; i++) {
			training_set[i] = gsets[POSITIVE].values[i];
			training_label[i] = 1;
		}

		problem.l = cur_positive_size;
		pre_positive_size = cur_positive_size;
		pre_negative_size = cur_negative_size;
		return 0;
	}


	int train()
	{
		if (problem.y == NULL || problem.x == NULL || negatives == NULL)
			return -1;

		for (equ_num = 0; equ_num < max_equ; equ_num++) {
			int misidx = -1;
			int ret = getMisclassified(misidx);
			if (ret == -1) return -1;  // something wrong in misclassified.
			if ((ret == 0) && (misidx == -1)) {	// can divide all the negative points correctly
				std::cout << "finish classified..." << std::endl;
				return 0;
			}

			// there is some point which is misclassified by current dividers.
			// "negatives->values[misidx]" 
			std::cout << "add " << misidx << "-th element: (";
			for (int i = 0; i < VARS; i++)
				std::cout << negatives->values[misidx][i] << ",";
			std::cout << ")   ";
			int length = problem.l;
			problem.y[length] = -1;
			problem.x[length] = (svm_node*)negatives->values[misidx];
			problem.l++;

			std::cout << "." << equ_num; // << std::endl;

			model = svm_train(&problem, &param);
			svm_model_visualization(model, &equations[equ_num]);
			std::cout << equations[equ_num];

			double precision = check_postives_and_one_negative();
			svm_free_and_destroy_model(&model);
			problem.l--;
			std::cout << " On training set precision: " << precision * 100 << "%" << std::endl;
			if (precision < 1) std::cout << "CAN NOT DIVIDE THE PICKED NEGATIVE FROM POSITIVES..." << std::endl;
			//std::cout << "\n On whole set precision: " << predictOnProblem() * 100 << "%" << std::endl;
		}
		return 0;
	}

	double predict_on_training_set()
	{
		int negatives_size = negatives->size();
		int total = problem.l + negatives_size;
		int pass = 0;
		for (int i = 0; i < problem.l; i++) {
			pass += (predict((double*)problem.x[i]) >= 0) ? 1 : 0;
		}
		if (negatives_size > 0) {
			for (int i = 0; i < negatives_size; i++) {
				pass += (predict(negatives->values[i]) < 0) ? 1 : 0;
			}
		}
		return (double)pass / total;
	}

	virtual int check_question_set(States& qset) {
		if (main_equation == NULL) return -1;
		std::cout << " [" << qset.traces_num() << "]";
		for (int i = 0; i < qset.p_index; i++) {
			int pre = -1, cur = 0;
			std::cout << ".";
			//std::cout << "\t\t" << i << ">";
			//gsets[QUESTION].print_trace(i);
			for (int j = qset.index[i]; j < qset.index[i + 1]; j++) {
				cur = predict(qset.values[j]);
				//std::cout << ((cur >= 0) ? "+" : "-");
				if ((pre > 0) && (cur < 0)) {
					// deal with wrong question trace.
					// Trace back to print out the whole trace and the predicted labels.
					std::cerr << "\t\t[FAIL]\n \t  Predict wrongly on Question traces." << std::endl;
					qset.print_trace(i);
					for (int j = qset.index[i]; j < qset.index[i + 1]; j++) {
						cur = Equation::calc(main_equation, qset.values[j]);
						std::cout << ((cur >= 0) ? "+" : "-");
					}
					std::cout << std::endl;
					return -1;
				}
				pre = cur;
			}
			//std::cout << "END" << std::endl;
		}
		std::cout << " [PASS]";
		return 0;
	}

	

	friend std::ostream& operator << (std::ostream& out, const SVM_I& svm_i) {
		out << "SVM-I: ";
		out << std::setprecision(16);
		out << "{ \n\t    " << *svm_i.main_equation;
		for (int i = 0; i < svm_i.equ_num; i++) {
			out << " \n\t /\\ " << svm_i.equations[i];
		}
		out << "}\n";
		return out;
	}


	int size()
	{
		if (negatives == NULL)
			return problem.l;
		return problem.l + negatives->size();
	}

private:
	virtual int predict(double* v)
	{
		std::cout << "(";
		for (int i = 0; i < VARS; i++)
			std::cout << v[i] << ",";
		std::cout << ")";
		if (v == NULL) return -2;
		/*
		* We use conjunction of positive as predictor.
		* For example, (A >= 0) /\ (B >= 0) /\ (C >= 0) /\ ...
		* Only when the give input pass all the equationss, it returns 1;
		* Otherwise, -1 will be returned.
		*/
		double res = Equation::calc(main_equation, v);
		if (res < 0) {
			std::cout << "[-]";
			return -1;
		}
		for (int i = 0; i < equ_num; i++) {
			res = Equation::calc(&equations[i], v);
			if (res < 0) {
				std::cout << "[-]";
				return -1;
			}
		}
		std::cout << "[+]";
		return 1;
	}

	double check_postives_and_one_negative()
	{
		int total = problem.l;
		int pass = 0;
		std::cout << "\t";
		for (int i = 0; i < problem.l; i++) {
			pass += (predict((double*)(problem.x[i])) * problem.y[i] >= 0) ? 1 : 0;
		}
		std::cout << std::endl << pass << "/" << total << "..";
		return (double)pass / total;
	}


	int getMisclassified(int& idx) // negative points may be misclassified.
	{
		if (negatives == NULL)
			return -1;
		if ((equ_num <= 0) && (main_equation == NULL)) {
			idx = 0;
			return -1;
		}

		int negatives_size = negatives->size();
		for (int i = 0; i < negatives_size; i++) {
			if (predict(negatives->values[i]) >= 0) {
				std::cout << "\nMISclassified" << i << std::endl;
				idx = i;
				return 0;
			}
		}

		// there should be no negatives misclassified.
		std::cout << "No misclassified.\n";
		idx = -1;
		return 0;	
	}
};


#endif /* _LIBSVM_I_H */
