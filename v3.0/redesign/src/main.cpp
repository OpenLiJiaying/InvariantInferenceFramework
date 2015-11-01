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
//void print_null(const char *s) {}
TraceSet<int>* TS;

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
	TS = new TraceSet<int>();
/*
 * struct svm_parameter param;
	param.svm_type = C_SVC;
	param.kernel_type = LINEAR;
	param.degree = 3;
	param.gamma = 0;	// 1/num_features
	param.coef0 = 0;
	param.nu = 0.5;
	param.cache_size = 100;
	//	param.C = 1;
	param.C = DBL_MAX;
	param.eps = 1e-3;
	param.p = 0.1;
	param.shrinking = 1;
	param.probability = 0;
	param.nr_weight = 0;
	param.weight_label = NULL;
	param.weight = NULL;
	svm_set_print_string_function(print_null);
*/

	int rnd = 1;
	srand(time(NULL));
#ifdef _TEST0_
	std::cout << "[1]******************************************************" << std::endl;
	std::cout << "\t(1) running programs... [" << inputs_init <<"]" << std::endl;
#endif
	for (int i = 0; i < inputs_init; i++) {
		for (int j = 0; j < vars; j++) {
			inputs[j] = rand() % (maxv - minv + 1) + minv;
		}
		before_loop();
		m(inputs);
		after_loop();
	}
	std::cout << "\t(2) program Trace Set... {" << TS <<"\n}" << std::endl;

/*
start_processing:	
	if (positive_set_changed) { 
		qsort(positive_set, positive_idx, sizeof(node), node::compare);
		positive_set_changed = false;
	}
	if (negative_set_changed) { 
		qsort(negative_set, negative_idx, sizeof(node), node::compare);
		negative_set_changed = false;
	}
	//	nice_set_print();

#ifdef _TEST0_
	std::cout << "\t(2) converting data into svm format..." << std::endl;
#endif
	svm_linker sl;
	sl.add_node_set(positive_set, positive_idx, 1);
	sl.add_node_set(negative_set, negative_idx, -1);

#ifdef _TEST0_
	std::cout << "\t(3) svm training...[" << sl.l << "]" << std::endl;
#endif
	struct svm_model* model = svm_train((const struct svm_problem *)&sl, &param);
//	svm_save_model("model_file", model);
	struct coef co;
	svm_model_visualization(model, &co);
	printf(" %.16g [0]", co.theta[0]);
	for (int j = 1; j < vars; j++)
		printf ("  +  %.16g [%d]", co.theta[j], j);
	printf (" >= %.16g\n", -co.theta0);

	print_svm_samples((const struct svm_problem*)&sl);
	svm_free_and_destroy_model(&model);

	rnd++;
	if (rnd <= max_iter) {
#ifdef _TEST0_
		std::cout << "[" << rnd << "]*********************************************************" << std::endl;
		std::cout << "\t(1) running programs...[" << inputs_aft << "]" << std::endl;
#endif
		for (int i = 0; i < inputs_aft; i++) {
			linear_solver(co, inputs);
			before_loop();
			m(inputs);
			after_loop();
		}
		goto start_processing;

	}

*/


	return 0;
}
