#ifndef _STATES_H_
#define _STATES_H_
#include "header.h"
#include <iostream>
#include "string.h"


class States{
	public:
		~States() {
//			if (values != NULL)
//				delete [] values[vars];
//			if (index != NULL)
//				delete[] index;
		}

		States() : max_items(100000) {
			values = new double [max_items][vars];
			index = new int [max_items];
			index[0] = 0;
			p_index = 0;
		}


		int add_states(double st[][vars], int len){
			if (index[p_index] + len >= max_items) {
				std::cerr << "exceed maximium program states." << std::endl;
				exit(-1);
			}
			
			/*
			std::cout << "+++";
			for (int i = 0; i < len; i++)  {
				std::cout << "{";
				for (int j = 0 ; j < vars; j++)
					std::cout << st[i][j] << ",";
				std::cout << "}-";
			}
			std::cout << std::endl;
			*/

			memcpy(values[index[p_index]], st, sizeof(double) * vars * len);
			//memmove(values[index[p_index]], st, sizeof(double) * vars * len);
			
			index[p_index+1] = index[p_index] + len;
			p_index++;
			return 0;
		}

		
		friend std::ostream& operator << (std::ostream& out, const States ss){
			std::cout << "lable[" << ss.label << "]:" << std::endl;
			for (int i = 0; i < ss.p_index; i++) {
				std::cout << "\tTr." << i << ":";
				for (int j = ss.index[i]; j < ss.index[i + 1]; j++) {
					std::cout << "(" << ss.values[j][0];
					for (int k = 1; k < vars; k++)
						std::cout << "," << ss.values[j][k];
					std::cout << ")->";
				}
				std::cout << "eot." << std::endl;
			}
			return out;
		}

	
		double (*values)[vars];
		int* index;
		int p_index;
		int label;
	private:
		const int max_items;
};


#endif
