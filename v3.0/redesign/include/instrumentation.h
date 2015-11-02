#ifndef _INSTRUMENTATION_H_
#define _INSTRUMENTATION_H_
#include "header.h"

#define recordi(first, args...) LT->addLoopState(new LoopState<int>(first, ##args))
#define recordd(first, args...) LT->addLoopState(new LoopState<double>(first, ##args))


// function lists
int m(int*);


int before_loop();
int after_loop();

#endif
