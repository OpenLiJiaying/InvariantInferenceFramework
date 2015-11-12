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

#include "states.h"
#include "instrumentation.h"

extern States* gsets;
#define PI (3.1415926) // as a stop sentinel for questions chain

extern bool _passP;
extern bool _passQ;

extern int maxv;
extern int minv;


extern double temp_states[256][VARS];

extern double Pset[max_items][VARS];
extern double Nset[max_items][VARS];
extern double Qset[q_items][VARS];
extern int pIndex, nIndex, qIndex;
extern int qNum;

#endif /*end of #ifndef _HEADER_H_*/
