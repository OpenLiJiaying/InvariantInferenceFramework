#ifndef _INSTRUMENTATION_H_
#define _INSTRUMENTATION_H_
#include "header.h"
#include <stdarg.h>
#include "program/loop_state.h"
#include "program/loop_trace.h"

#ifdef WIN32
#define recordi(first, args, ...) LT->addLoopState(new LoopState<int>(first, ##args))
#define recordd(first, args, ...) LT->addLoopState(new LoopState<double>(first, ##args))
#endif

#ifdef linux
#define recordi(first, args ...) LT->addLoopState(new LoopState<int>(first, ##args))
#define recordd(first, args ...) LT->addLoopState(new LoopState<double>(first, ##args))
#endif


// function lists
int m(int*);


int before_loop();
int after_loop();

#endif
