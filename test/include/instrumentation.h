#ifndef _INSTRUMENTATION_H_
#define _INSTRUMENTATION_H_
#include "header.h"
#include <stdarg.h>

int add_state_I(int first ...);
int add_state_I(double first ...);

#ifdef WIN32
	#if VARS == 1
		#define recordi(first) add_state_I(first)
		#define recordd(first) add_state_D(first)
	#else
		#define recordi(first, args, ...) add_state_I(first, ##args)
		#define recordd(first, args, ...) add_state_D(first, ##args)
	#endif
#endif

#ifdef linux
#define recordi(first, args ...) add_state_I(first, ##args)
#define recordd(first, args ...) add_state_D(first, ##args)
#endif


// function lists

int m(int*);
int m_int(int*);
int m_double(double*);


int before_loop();
int after_loop();

#endif
