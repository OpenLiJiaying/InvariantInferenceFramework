#ifndef _LOOP_TRACE_H_
#define _LOOP_TRACE_H_
#include "header.h"


template <class T>
class LoopTrace
{
	public:
		LoopTrace()
		{
			first = NULL;
			last = NULL;
			length = 0;
		};

		~LoopTrace() 
		{
			LoopState<T>* p = first;
			LoopState<T>* pp; 
			while (p != NULL) {
				pp = p->next;
				delete p;
				p = pp;
			}
		}

		int addLoopState(LoopState<T>* ps) 
		{
			length++;
			std::cout << ">" << ps;
			if (first == NULL) {
				last = first = ps;
				first->next = NULL;
			} else {
				last->next = ps;
				last = ps;
				last->next = NULL;
			}
			std::cout << ".";
			return 0;
		}

		friend std::ostream& operator << (std::ostream& out, const LoopTrace* lt)
		{
			out << LabelTable[lt->label] << " trace: ";
			if (lt->first == NULL) {
				out << "NULL";
				return out;
			}
			LoopState<T>* p = lt->first;
			while (p != NULL) {
				out << p << " --> ";
				p = p->next;
			}
			out << "END";
			out << "[" << lt->length <<"]";
			return out;
		}

		int labeling(int label) 
		{
			this->label = label;
			if (first == NULL)
				return 0;
			LoopState<T>* p = first;
			while (p != NULL) {
				p->labeling(label);
				p = p->next;
			}
			return 0;
		}

		LoopState<T>* first;
		int label;
		int length;
		LoopTrace<T>* next;
	private:
		LoopState<T>* last;
};

extern LoopTrace<int>* LT;

#endif
