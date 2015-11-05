/*************************************************************************
    > File Name: config.h
    > Author: Li Jiaying
    > Mail: lijiaying1989@gmail.com 
    > Created Time: 2015年11月05日 星期四 11时50分51秒
 ************************************************************************/

//#define VARS 3
#define __OPT

#ifndef VARS 
const int vars = 2;
#else
const int vars = VARS;
#endif

const int max_items = 100000;

const int inputs_init = 4 * vars;
const int inputs_aft = 2 * vars;
const int max_iter = 64;
