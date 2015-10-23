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

namespace iif{
class Tuple {
	public:
		static Tuple* make_tuple(int first, ...)
		{
			Tuple* tmp = new Tuple();
			va_list ap;
			va_start(ap, first);
			tmp->s[0] = first;
			cout << tmp->s[0] << "==";
			for (int i = 1; i < dim; i++) {
				tmp->s[i] = va_arg(ap, int);
				cout << tmp->s[i] << "==";
			}
			cout << endl;
			return tmp;
		}

		static Tuple* rand_gen()
		{
			int length = max - min + 1;
			Tuple* t = new Tuple();
			for (int i = 0; i < dim; i++)
				t->s[i] = rand() % length + min;
			return t;
		}

		std::size_t hash();
		
		string to_string();

		void to_char_array(char** argv);

		int s[MAX_DIM];
		int label;
};
}

#endif
