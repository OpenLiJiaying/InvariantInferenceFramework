#ifndef _HEADER_H_
#define _HEADER_H_

//#define VARS 3

#ifndef VARS 
const int vars = 2;
#else
const int vars = VARS;
#endif

#include <stdarg.h>
#include <iostream>
#include <cstdlib>
#include <cmath>
//#include <cstdio>

#include "equation.h"
#include "program/state.h"
#include "program/trace.h"
#include "program/trace_set.h"
#include "assert.h"
#include "svm.h"
#include "instrumentation.h"


extern bool _passP;
extern bool _passQ;

extern int maxv;
extern int minv;

const int inputs_init = 4 * vars;
const int inputs_aft = 2 * vars;
const int max_iter = 64;


#endif
