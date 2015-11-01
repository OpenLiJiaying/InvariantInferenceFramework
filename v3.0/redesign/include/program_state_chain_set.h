/*************************************************************************
  > File Name: iif.h
  > Author: Li Jiaying
  > Mail: lijiaying1989@gmail.com 
  > Created Time: Wed Oct 28 01:32:44 2015
 ************************************************************************/
#ifndef _PROGRAM_STATE_CHAIN_SET_H_
#define _PROGRAM_STATE_CHAIN_SET_H_
#include <stdarg.h>
#include <iostream>
#include "header.h"

template <class type>
class ProgramStateChainSet
{
	public:
		ProgramStateChainSet()
		{
			first = NULL;
			last = NULL;
		};

		~ProgramStateChainSet() 
		{
			ProgramStateChain<type>* p = first;
			ProgramStateChain<type>* pp; 
			while (p != NULL) {
				pp = p->next;
				delete p;
				p = pp;
			}
		}

		void addNewProgramStateChain(ProgramStateChain<type>* ps) 
		{
			if (first == NULL) {
				last = first = ps;
				first->next = NULL;
				return;
			}
			last->next = ps;
			last = ps;
		}

		friend std::ostream& operator << (std::ostream& out, const ProgramStateChainSet* psc)
		{
			out << "SET: ";
			if (psc->first == NULL) {
				out << "NULL";
				return out;
			}
			ProgramStateChain<type>* p = psc->first;
			while (p != NULL) {
				out << p <<  "\n ||-->|| ";
				p = p->next;
			}
			out << "END";
			return out;
		}

		ProgramStateChain<type>* first;
		ProgramStateChain<type>* last;
};


#endif
