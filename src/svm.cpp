#include "svm.h"
#include "svm_core.h"

SVM::SVM(void (*f) (const char*)) {
	problem.l = 0;

	main_equation = NULL;
	model = NULL;

	param.svm_type = C_SVC;
	param.kernel_type = LINEAR;
	param.degree = 3;
	param.gamma = 0;	// 1/num_features
	param.coef0 = 0;
	param.nu = 0.5;
	param.cache_size = 100;
	//	param.C = 1;
	//param.C = DBL_MAX;
	param.C = 1000;
	param.eps = 1e-3;
	param.p = 0.1;
	param.shrinking = 1;
	param.probability = 0;
	param.nr_weight = 0;
	param.weight_label = NULL;
	param.weight = NULL;
	if (f != NULL)
		svm_set_print_string_function(f);

	for (int i = 0; i < 2 * max_items; i++)
		training_label[i] = -1;
	problem.x = (svm_node**)(training_set);
	problem.y = training_label;
}

SVM::~SVM() {
	if (model != NULL)
		delete model;
	//if (problem.y != NULL)
	//	delete problem.y;
	// here we should check x[i] for each.
	// be careful about whether it is imported from double trace set or int trace set.
	// these two cases should be handled separatly.
}

int SVM::prepare_training_data(States* gsets, int& pre_positive_size, int& pre_negative_size) {
	int cur_positive_size = gsets[POSITIVE].size();
	int cur_negative_size = gsets[NEGATIVE].size();

	std::cout << "+[";
	std::cout << cur_positive_size - pre_positive_size << "|";
	std::cout << cur_negative_size - pre_negative_size << "";
	std::cout << "] ==> [";
	std::cout << cur_positive_size << "+|";
	std::cout << cur_negative_size << "-";
	std::cout << "]";

	//double** training_set = (double**)(problem.x);
	//double* training_label = (double*)(problem.y);
	// prepare new training data set
	// training set & label layout:
	// data :  0 | positive states | negative states | ...
	// label:    | 1, 1, ..., 1, . | -1, -1, ..., -1, -1, -1, ...
	// move the negative states from old OFFSET: [pre_positive_size] to new OFFSET: [cur_positive_size]
	memmove(training_set + cur_positive_size, training_set + pre_positive_size, pre_negative_size * sizeof(double*));
	// add new positive states at OFFSET: [pre_positive_size]
	for (int i = pre_positive_size; i < cur_positive_size; i++) {
		training_set[i] = gsets[POSITIVE].values[i];
		training_label[i] = 1;
	}
	// add new negative states at OFFSET: [cur_positive_size + pre_negative_size]
	for (int i = pre_negative_size; i < cur_negative_size; i++) {
		training_set[cur_positive_size + i] = gsets[NEGATIVE].values[i];
	}
	problem.l = cur_positive_size + cur_negative_size;
	pre_positive_size = cur_positive_size;
	pre_negative_size = cur_negative_size;
	return 0;
}


int SVM::train() {
	if (problem.y == NULL || problem.x == NULL)
		return -1;
	const char* error_msg = svm_check_parameter(&problem, &param);
	if (error_msg) {
		std::cout << "ERROR: " << error_msg << std::endl;
		exit(-1);
	}
	model = svm_train(&problem, &param);
	/*for (int i = 0; i < 100000; i++)
	  if (sin(i) + cos(i) > 1.414)
	  std::cout << ".";
	  */
	main_equation = new Equation();
	svm_model_visualization(model, main_equation);
	svm_free_and_destroy_model(&model);
	return 0;
}


double SVM::predict_on_training_set() {
	if (problem.l <= 0) return 0;
	int pass = 0;
	for (int i = 0; i < problem.l; i++) {
		pass += (Equation::calc(main_equation, (double*)problem.x[i]) * problem.y[i] > 0) ? 1 : 0;
	}
	return static_cast<double>(pass) / problem.l;
}


int SVM::check_question_set(States& qset) {
	if (main_equation == NULL) return -1;
	std::cout << " [" << qset.traces_num() << "]";
	for (int i = 0; i < qset.p_index; i++) {
		int pre = -1, cur = 0;
		std::cout << ".";
		//std::cout << "\t\t" << i << ">";
		//gsets[QUESTION].print_trace(i);
		for (int j = qset.index[i]; j < qset.index[i + 1]; j++) {
			cur = Equation::calc(main_equation, qset.values[j]);
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


std::ostream& operator << (std::ostream& out, const SVM& svm) {
	out << "SVM: ";
	out << *svm.main_equation; // << std::endl;
	return out;
}


int SVM::size()
{
	return problem.l;
}


Equation* SVM::roundoff(int& num)
{
	num = 1;
	Equation* equs = new Equation[1];
	main_equation->roundoff(equs[0]);
	return equs;
}

int SVM::predict(double* v, int label) {
	if (main_equation == NULL) return -2;
	if (v == NULL) return -2;
	double res = Equation::calc(main_equation, v);
	if (res >= 0) return 1;
	else return -1;
}

