/*************************************************************************
  > File Name: svm_linker.h
  > Author: Li Jiaying
  > Mail: lijiaying1989@gmail.com 
  > Created Time: 2015年10月28日 星期三 17时04分41秒
 ************************************************************************/
#ifndef _SVM_LINKER_H_
#define _SVM_LINKER_H_

#include "iif.h"

struct svm_linker
{
	public:
		int l;
		double* y;
		double** x;

		svm_linker() : l(0){
			y = new double[2 * max_set_idx];
			x = new double* [2 * max_set_idx];
		}
		~svm_linker() {
			if (y) delete[] y;
			if (x) delete[] x;
		}
		int add_node_set(node*, int, double);
		int keep_and_reset_rest(int); 

};

#endif
