#ifndef _TRACE_SET_H_
#define _TRACE_SET_H_

#include "state.h"
#include "trace.h"
#include "../equation.h"
#include <iostream>

extern const int vars;

template <class type>
class TraceSet
{
	public:
		TraceSet()
		{
			first = NULL;
			last = NULL;
			length = 0;
		};

		~TraceSet() 
		{
/*			Trace<type>* p = first;
			Trace<type>* pp; 
			while (p != NULL) {
				pp = p->next;
				delete p;
				p = pp;
			}
*/		}

		int addLoopTrace(Trace<type>* lt = LT) 
		{
			length += lt->length;
			if (first == NULL) {
				last = first = lt;
				last->next = NULL;
				return 0;
			}
			last->next = lt;
			last = lt;
			last->next = NULL;
			return 0;
		}

		friend std::ostream& operator << (std::ostream& out, const TraceSet ts)
		{
			if (ts.first == NULL) {
				out << "NULL";
				return out;
			}
			Trace<type>* p = ts.first;

			while (p != NULL) {
				out  << *p << "\n" /*<< " ||-->|| "*/;
				p = p->next;
			}
			return out;
		}

		int labeling(int label) 
		{
			if (first == NULL) {
				return 0;
			}
			this->label = label;
			Trace<type>* p = first;
			while (p != NULL) {
				p->labeling(label);
				p = p->next;
			}
			return 0;
		}

		int reset()
		{
			first = NULL;
			last = NULL;
			length = 0;
			return 0;
		}
		Trace<type>* first;
		Trace<type>* last;
		int label;
		int length;
};


extern TraceSet<int>* TS;

#endif
