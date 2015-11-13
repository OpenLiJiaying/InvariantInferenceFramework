#include <iostream>
#include "tuple.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
using namespace iif;

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


void Tuple::to_char_array(char** argv)
{
	for (int i = 0; i < dim; i++) 
		strcpy(argv[i], std::to_string(s[i]).c_str());
	strcpy(argv[dim], "\0");
}
