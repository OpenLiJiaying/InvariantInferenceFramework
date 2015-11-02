#ifndef _HEADER_H_
#define _HEADER_H_

#ifndef VARS 
const int vars = 2;
#else
const int vars = VARS;
#endif

#include <stdarg.h>
#include <iostream>
//#include <ctime>
#include <cstdlib>
#include <cmath>
//#include <cstdio>
//#include <time.h>

#include "equation.h"
#include "loop_state.h"
#include "loop_trace.h"
#include "trace_set.h"
#include "assert.h"
#include "svm.h"
//#include "svm_linker.h"
#include "instrumentation.h"



extern bool _passP;
extern bool _passQ;



extern LoopTrace<int>* LP;
extern int maxv;
extern int minv;

const int max_trace_pnt = 1024;
const int inputs_init = 4 * vars;
const int inputs_aft = 2 * vars;
const int max_inputs = inputs_init;
const int max_set_idx = 10240;
const int max_iter = 32;



#endif
