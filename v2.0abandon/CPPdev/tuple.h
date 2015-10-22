#ifndef _TUPLE_H_
#define _TUPLE_H_
#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <string>
#include <stdarg.h>
#include <unistd.h>
#include "iif.h"
using namespace std;

class Tuple {
	public:
		static Tuple* make_tuple(int first, ...);
		static Tuple* rand_gen();
		std::size_t hash();
		string to_string();

		int s[MAX_DIM];
};

#endif
