#ifndef _INSTRUMENTATION_H_
#define _INSTRUMENTATION_H_
#include "header.h"
#include <stdarg.h>
#include "program/state.h"
#include "program/trace.h"

#ifdef WIN32
#define recordi(first, args, ...) LT->addState(new State<int>(first, ##args))
#define recordd(first, args, ...) LT->addState(new State<double>(first, ##args))
#endif

#ifdef linux
#define recordi(first, args ...) LT->addState(new State<int>(first, ##args))
#define recordd(first, args ...) LT->addState(new State<double>(first, ##args))
#endif


// function lists
int m(int*);


int before_loop();
int after_loop();

#endif
