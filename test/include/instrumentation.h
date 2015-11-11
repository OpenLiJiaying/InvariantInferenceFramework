#ifndef _INSTRUMENTATION_H_
#define _INSTRUMENTATION_H_
#include "header.h"
#include <stdarg.h>

int add_state_int(int first ...);
int add_state_double(double first ...);

#ifdef WIN32
	#if VARS == 1
		int add_state_int(int first);
		int add_state_double(double first);
		#define recordi(first) add_state_int(first)
		#define recordd(first) add_state_double(first)
	#else
		int add_state_int(int first ...);
		int add_state_double(double first ...);
		#define recordi(first, args, ...) add_state_int(first, ##args)
		#define recordd(first, args, ...) add_state_double(first, ##args)
	#endif
#endif

#ifdef linux
	int add_state_int(int first, ...);
	int add_state_double(double first, ...);
	#define recordi(first, args ...) add_state_int(first, ##args)
	#define recordd(first, args ...) add_state_double(first, ##args)
#endif


// function lists

int m(int*);
int m_int(int*);
int m_double(double*);


int before_loop();
int after_loop();

#endif
