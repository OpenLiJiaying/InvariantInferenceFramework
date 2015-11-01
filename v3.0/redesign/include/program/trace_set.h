#ifndef _TRACE_SET_H_
#define _TRACE_SET_H_
#include <stdarg.h>
#include <iostream>
#include "header.h"


template <class type>
class TraceSet
{
	public:
		TraceSet()
		{
			first = NULL;
			last = NULL;
		};

		~TraceSet() 
		{
			LoopTrace<type>* p = first;
			LoopTrace<type>* pp; 
			while (p != NULL) {
				pp = p->next;
				delete p;
				p = pp;
			}
		}

		void addNewLoopTrace(LoopTrace<type>* lt = LT) 
		{
			if (first == NULL) {
				last = first = lt;
				first->next = NULL;
				return;
			}
			last->next = lt;
			last = lt;
		}

		friend std::ostream& operator << (std::ostream& out, const TraceSet* ts)
		{
			if (ts->first == NULL) {
				out << "NULL";
				return out;
			}
			LoopTrace<type>* p = ts->first;
			while (p != NULL) {
				out << "\n ||-->|| " << p;
				p = p->next;
			}
			return out;
		}

		int labeling(int label) 
		{
			if (first == NULL) {
				return 0;
			}
			LoopTrace<type>* p = first;
			while (p != NULL) {
				p->labeling(label);
				p = p->next;
			}
			return 0;
		}
		LoopTrace<type>* first;
		LoopTrace<type>* last;
//		int label;
};


extern TraceSet<int>* TS;

#endif
