#ifndef _SVM_H_
#define _SVM_H_
#include "svm_core.h"

class SVM // : public ClassifyAlgo
{
	public:
		svm_model* model;
		Equation* main_equation;
		svm_parameter param;
		svm_problem problem;
		double training_label[max_items * 2];
		double* training_set[max_items * 2];

		SVM(void (*f) (const char*) = NULL);

		virtual ~SVM(); 

		virtual int prepare_training_data(States* gsets, int& pre_positive_size, int& pre_negative_size);

		virtual int train();

		virtual double predict_on_training_set();

		virtual int check_question_set(States& qset);

		friend std::ostream& operator << (std::ostream& out, const SVM& svm);

		virtual int size();

		virtual Equation* roundoff(int& num);

	private:
		virtual int predict(double* v, int label = 0);
};

#endif /* _SVM_H */
