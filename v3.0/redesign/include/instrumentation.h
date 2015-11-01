#ifndef _INSTRUMENTATION_H_
#define _INSTRUMENTATION_H_
#include "header.h"


// function lists
int m(int*);

int record_values(int, ...);
int before_loop();
int after_loop();

#endif
