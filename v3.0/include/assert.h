#include <stdio.h>
#include <stdlib.h>

int _passP = 0;
int _passQ = 0;
int _t_condition = 0;

#undef assume
#undef assert
#ifdef NDEBUG
#define assert(expr)  ((void) 0)
#else
#define assert(expr) do { \
if (_t_condition == 0) {\
	_passP = (expr)? 1 : 0;\
} else if (_t_condition == 1) { \
_passQ = (expr)? 1 : 0;\
if (_passP && _passQ) printf ("+ 1 ");\
else if (_passP && !_passQ) { \
		printf ("x 1 ");\
		fprintf(stderr, "encounter a counter-example. assertion failed: %s, file %s, line %i\n", \
		#expr, __FILE__, __LINE__);\
	exit(2);\
	}\
else if (!_passP && _passQ) printf ("- -1 ");\
else printf ("+ -1 ");\
} else {\
if(!(expr)) \
	fprintf(stderr, "Assertion failed: %s, file %s, line %i\n", \
	#expr, __FILE__, __LINE__);\
}\
_t_condition++;\
} while (0)
#endif
