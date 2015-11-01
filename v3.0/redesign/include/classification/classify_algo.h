/*************************************************************************
    > File Name: classify_algo.h
    > Author: Li Jiaying
    > Mail: lijiaying1989@gmail.com 
    > Created Time: 2015年11月01日 星期日 16时13分01秒
 ************************************************************************/
#ifndef _CLASSIFY_ALGO_H_
#define _CLASSIFY_ALGO_H_
#include "header.h"

class ClassifyAlgo
{
	private:
//		void* dataset;
//		void* labelset;
	public:
		virtual int classify() = 0;
		int predict(void*);
};


#endif
