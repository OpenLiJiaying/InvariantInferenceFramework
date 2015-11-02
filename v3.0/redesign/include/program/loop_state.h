#ifndef _LOOP_STATE_H_
#define _LOOP_STATE_H_
#include <stdarg.h>
#include <iostream>

extern const int vars;
extern char** LabelTable;

template<class T>
class LoopState{
	public:
		~LoopState() {
			if (values != NULL)
				delete [] values;
		}

		LoopState(T first, ...) {
			if (values == NULL) values = new T [vars];
			va_list ap;
			va_start(ap, first);
			values[0] = first;
			for (int i = 1; i < vars; i++) {
				values[i] = va_arg(ap, T);
			}
			va_end(ap);
		}

		friend std::ostream& operator << (std::ostream& out, const LoopState* ps){
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

		int labeling(int label) 
		{
			this->label = label;
		}
		
		LoopState<T> *next;
	
		T *values;
		int label; // -1, 0, 1 or otherss
	private:
//		static int num = vars;
};


#endif
