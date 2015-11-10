#ifndef _STATES_H_
#define _STATES_H_
#include "header.h"
#include <iostream>
#include "string.h"


class States{
	public:
		~States() {
			if (values != NULL)
				delete [] values;
			if (index != NULL)
				delete[] index;
		}

		States() : max_items(100000) {
			values = new double [max_items][vars];
			index = new int [max_items];
			p_index = 0;
		}


		int add_states(double st[][vars], int len){
			if (index[p_index] + len >= max_items) {
				std::cerr << "exceed maximium program states." << std::endl;
				exit(-1);
			}
			memcpy(values[index[p_index]], st, sizeof(double) * vars * len);
			
			index[p_index+1] = index[p_index] + len;
			p_index++;
			return 0;
		}

		
		friend std::ostream& operator << (std::ostream& out, const States s){
			std::cout << "lable[" << s.label << "]:" << std::endl;
			for (int i = 0; i < s.p_index; i++) {
				std::cout << "\tTr." << i << ":";
				for (int j = s.index[i]; j < s.index[i + 1]; j++) {
					std::cout << "(" << s.values[j][0];
					for (int k = 1; k < vars; k++)
						std::cout << "," << s.values[j][i];
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
