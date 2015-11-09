#ifndef _LOOP_TRACE_H_
#define _LOOP_TRACE_H_
//#include "../header.h"
//#include <iostream>
#include "state.h"


template <class T>
class Trace
{
	public:
		Trace()
		{
			first = NULL;
			last = NULL;
			length = 0;
		};

		~Trace() 
		{
/*			State<T>* p = first;
			State<T>* pp; 
			while (p != NULL) {
				pp = p->next;
				delete p;
				p = pp;
			}
*/		}

		int addState(State<T>* ps) 
		{
			length++;
			//std::cout << ">" << ps;
			if (first == NULL) {
				last = first = ps;
				first->next = NULL;
			} else {
				last->next = ps;
				last = ps;
				last->next = NULL;
			}
			//std::cout << ".";
			return 0;
		}

		friend std::ostream& operator << (std::ostream& out, const Trace t)
		{
			out << LabelTable[t.label] << " trace: ";
			if (t.first == NULL) {
				out << "NULL";
				return out;
			}
			State<T>* p = t.first;
			while (p != NULL) {
				out << *p << " -> ";
				p = p->next;
			}
			out << "END";
			out << "[" << t.length <<"]";
			return out;
		}

		int labeling(int label) 
		{
			this->label = label;
			if (first == NULL)
				return 0;
			State<T>* p = first;
			while (p != NULL) {
				p->labeling(label);
				p = p->next;
			}
			return 0;
		}

		State<T>* first;
		int label;
		int length;
		Trace<T>* next;
	private:
		State<T>* last;
};

extern Trace<int>* LT;

#endif
