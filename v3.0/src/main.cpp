/*************************************************************************
  > File Name: main.cpp
  > Author: Li Jiaying
  > Mail: lijiaying1989@gmail.com 
  > Created Time: Wed Oct 28 01:47:17 2015
 ************************************************************************/
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <time.h>
#include <float.h>
#include "iif.h"
#include "svm_linker.h"
#include "svm.h"

//int min = -100, max = 100;
int min = -10, max = 10;

int main(int argc, char** argv)
{
	if (argc < 1) {
		std::cout << "Arguments less than 2.\n";
		exit(-1);
	}	
	if (argc >= 3) {
		min = atoi(argv[1]);
		max = atoi(argv[2]);
	}

	srand(time(NULL));
	for (int i = 0; i < inputs_init; i++) {
		for (int j = 0; j < vars; j++) {
			inputs[j] = rand() % (max - min + 1) + min;
			before_loop();
			m(inputs);
			after_loop();
		}
	}
	if (positive_set_changed) { 
		qsort(positive_set, positive_idx, sizeof(node), node::compare);
		positive_set_changed = false;
	}
	if (negative_set_changed) { 
		qsort(negative_set, negative_idx, sizeof(node), node::compare);
		negative_set_changed = false;
	}
	nice_set_print();

#ifdef _TEST1_
	std::cout << "*********************************************************" << std::endl;
	std::cout << "converting data into svm format..." << std::endl;
#endif
	svm_linker sl;
	sl.add_node_set(positive_set, positive_idx, 1);
	sl.add_node_set(negative_set, negative_idx, -1);

	struct svm_parameter param;
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
	
	struct svm_model* model = svm_train((const struct svm_problem *)&sl, &param);
	svm_save_model("model_file", model);
	svm_model_visualization(model);
	print_svm_samples((const struct svm_problem*)&sl);
	svm_free_and_destroy_model(&model);

	return 0;
}
