/*************************************************************************
  > File Name: iif.h
  > Author: Li Jiaying
  > Mail: lijiaying1989@gmail.com 
  > Created Time: Wed Oct 28 01:32:44 2015
 ************************************************************************/
#ifndef _PROGRAM_STATE_CHAIN_H_
#define _PROGRAM_STATE_CHAIN_H_
#include <stdarg.h>
#include <iostream>
#include "program_state.h"

template <class type>
class ProgramStateChain
{
	public:
		ProgramStateChain()
		{
			first = NULL;
			last = NULL;
		};

		~ProgramStateChain() 
		{
			ProgramState<type>* p = first;
			ProgramState<type>* pp; 
			while (p != NULL) {
				pp = p->next;
				delete p;
				p = pp;
			}
		}

		void addNewProgramState(ProgramState<type>* ps) 
		{
			if (first == NULL) {
				last = first = ps;
				first->next = NULL;
				return;
			}
			last->next = ps;
			last = ps;
		}

		friend std::ostream& operator << (std::ostream& out, const ProgramStateChain* psc)
		{
			out << "state chain: ";
			if (psc->first == NULL) {
				out << "NULL";
				return out;
			}
			ProgramState<type>* p = psc->first;
			while (p != NULL) {
				out << p << " --> ";
				p = p->next;
			}
			out << "END";
			return out;
		}

		void labelChain(int label) 
		{
			this->label = label;
		}

		ProgramState<type>* first;
		ProgramState<type>* last;
		int label;
};


#endif
