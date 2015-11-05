#ifndef _HEADER_H_
#define _HEADER_H_

#include "config.h"

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

extern double PS[max_items][vars];
extern double NS[max_items][vars];
extern int pIndex, nIndex;

#endif
