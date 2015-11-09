#ifndef _LOOP_STATE_H_
#define _LOOP_STATE_H_
#include "../header.h"
#include <iostream>


extern char (*LabelTable)[10];

template<class T>
class State{
	public:
		~State() {
//			if (values != NULL)
//				delete [] values;
		}

		State(T first, ...) {
			//if (values == NULL) 
			values = new T [vars];
			va_list ap;
			va_start(ap, first);
			values[0] = first;
			for (int i = 1; i < vars; i++) {
				values[i] = va_arg(ap, T);
			}
			va_end(ap);
		}

		friend std::ostream& operator << (std::ostream& out, const State s){
			if (s.values == NULL) { 
				out << "(NULL)";
				return out;
			}
			out << "(" << s.values[0];
			for (int i = 1; i < vars; i++) {
				out << ", " << s.values[i];
			}
			out << ")";
			return out;
		}

		int labeling(int label) 
		{
			this->label = label;
			return 0;
		}
		
		State<T> *next;
	
		T *values;
		int label; // -1, 0, 1 or otherss
	private:
};


#endif
