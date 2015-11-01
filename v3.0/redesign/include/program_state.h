#ifndef _PROGRAM_STATE_H_
#define _PROGRAM_STATE_H_
#include <stdarg.h>
#include <iostream>


extern const int vars;


template<class type>
class ProgramState{
	public:
		~ProgramState() {
			if (values != NULL)
				delete [] values;
		}

		ProgramState(type first, ...) {
//			std::cout << "##mark " << first;
			if (values == NULL) values = new type [vars];
			va_list ap;
			va_start(ap, first);
			values[0] = first;
			for (int i = 1; i < vars; i++) {
				values[i] = va_arg(ap, type);
			}
			va_end(ap);
		}

		friend std::ostream& operator << (std::ostream& out, const ProgramState* ps){
			if (ps->values == NULL) { 
				out << "(NULL)";
				return out;
			}
			out << "(" << ps->values[0];
			for (int i = 1; i < vars; i++) {
				out << ", " << ps->values[i];
			}
			out << ")";
			return out;
		}
		
		ProgramState<type> *next;
	private:
		type *values;
		int label; // -1, 0, 1 or otherss
//		static int num = vars;
};


#endif
