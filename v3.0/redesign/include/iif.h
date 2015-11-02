/*************************************************************************
  > File Name: iif.h
  > Author: Li Jiaying
  > Mail: lijiaying1989@gmail.com 
  > Created Time: Wed Oct 28 01:32:44 2015
 ************************************************************************/
#ifndef _IIF_H_
#define _IIF_H_
#include <stdarg.h>
#include <iostream>

// _TEST_ simplest output
//#define _TEST0_	 
#define _TEST1_	 
//#define _TEST2_
//#define _TEST3_

#ifdef _TEST1_
#define _TEST0_	 
#endif

#ifdef _TEST2_
#define _TEST1_	 
#define _TEST0_	 
#endif

#ifdef _TEST3_
#define _TEST1_	 
#define _TEST2_
#define _TEST0_	 
#endif

#endif
