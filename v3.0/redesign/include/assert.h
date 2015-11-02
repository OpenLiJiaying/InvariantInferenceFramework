#ifndef _ASSERT_H_
#define _ASSERT_H_

extern bool _passP;
extern bool _passQ;

//#undef assume
//#undef assert

#define assume(expr) do { \
	_passP = (expr)? true : false;\
} while(0)

#define assert(expr) do { \
	_passQ = (expr)? true : false;\
} while(0)


#endif
