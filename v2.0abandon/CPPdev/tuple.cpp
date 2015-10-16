#include "tuple.h"
#include <iostream>
using namespace std;

static Tuple* Tuple::make_tuple(int first, ...) {
	Tuple* tmp = new Tuple();
	va_list ap;
	va_start(ap, first);
	tmp->s[0] = first;
	cout << tmp->s[0] << "==";
	for (int i = 1; i < dim; i++) {
		tmp->s[i] = va_arg(ap, int);
		cout << tmp->s[i] << "==";
	}
	cout << endl;
	return tmp;
}

std::size_t Tuple::hash(){
	std::size_t t = 0;
	for (int i = 0; i < dim; i++) {
		t = t << 4 | s[i];
	}
	return t;
}

static Tuple* Tuple::rand_gen()
{
	int length = maxi - mini + 1;
	Tuple* t = new Tuple();
	for (int i = 0; i < dim; i++)
		t->s[i] = rand() % length + mini;
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

