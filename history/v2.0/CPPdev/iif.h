#ifndef _IIF_H_
#define _IIF_H_

namespace iif{
#define MAX_DIM  16
#define MAX_BITS  32

#define EXIT_POS 0
#define EXIT_NEG -1
#define EXIT_CNT -2
#define EXIT_IGN -3

extern int dim;
extern int min;
extern int max;

extern int num_tests_begin;
extern int num_tests_after;
extern int num_iters;

}
#endif
