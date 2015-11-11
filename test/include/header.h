#ifndef _HEADER_H_
#define _HEADER_H_

#include "config.h"

#include <stdarg.h>
#include <iostream>
#include <cstdlib>
#include <cmath>
//#include <cstdio>

#include "equation.h"
#include "iif_assert.h"
#include "svm.h"
#include "states.h"
#include "instrumentation.h"

extern States* gsets;
#define PI (3.1415926) // as a stop sentinel for questions chain

extern bool _passP;
extern bool _passQ;

extern int maxv;
extern int minv;

#ifdef __OPT
//extern double set[max_items][vars];
//extern double slabel[max_items];
//extern int sIndex;

extern double temp_states[256][vars];

extern double Pset[max_items][vars];
extern double Nset[max_items][vars];
extern double Qset[q_items][vars];
extern int pIndex, nIndex, qIndex;
extern int qNum;
#endif

#endif /*end of #ifndef _HEADER_H_*/