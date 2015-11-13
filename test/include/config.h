/*************************************************************************
    > File Name: config.h
    > Author: Li Jiaying
    > Mail: lijiaying1989@gmail.com 
    > Created Time: 2015年11月05日 星期四 11时50分51秒
 ************************************************************************/

//#define __PRT

#define VARS (1)

#ifndef VARS 
#define VARS (2)
#endif

const int max_items = 100000;
const int q_items = 1000;

const int init_exes = 4 * VARS;
const int after_exes = 2 * VARS;
const int random_exes = 1;
const int max_iter = 32;
