#include "tuple.h"
#include <iostream>
using namespace std;


std::size_t Tuple::hash(){
	std::size_t t = 0;
	for (int i = 0; i < dim; i++) {
		t = t << 4 | s[i];
	}
	return t;
}


string Tuple::to_string()
{
	string str = "";
	for (int i = 0; i < dim; i++) 
		str += std::to_string(s[i]) + " ";
	str += "\0";
	return str;
}

