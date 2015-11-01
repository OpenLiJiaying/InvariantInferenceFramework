#ifndef _INSTRUMENTATION_H_
#define _INSTRUMENTATION_H_
#include <stdarg.h>
#include <iostream>
#include "header.h"

extern ProgramStateChain<int> *curPSC;

// function lists
int m(int*);

int record_values(int, ...);
int before_loop();
int after_loop();

#endif
