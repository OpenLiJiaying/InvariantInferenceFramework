/*************************************************************************
  > File Name: test_shell.cpp
  > Author: Li Jiaying
  > Mail: lijiaying1989@gmail.com 
  > Created Time: 2015年10月15日 星期四 13时29分26秒
 ************************************************************************/
#include <stdlib.h>
#include <iostream>
#include <functional>
#include <unordered_map>
#include <stdarg.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;

#define MAXDIM  16
static int dim = 2;

struct Tuple {
	static struct Tuple make_tuple(int first, ...) {
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
		return *tmp;
	}

	int s[MAXDIM];
	/*	bool operator==(const Tuple &p) const {
		for (int i = 0; i < dim; i++) 
		if (s[i] != p.s[i])
		return false;
		return true;
		}
		*/
	std::size_t hash(){
		std::size_t t = 0;
		for (int i = 0; i < dim; i++) {
			t = t << 4 | s[i];
		}
		return t;
	}
};






int main(int argc, char** argv, char** envp)
{
	dim = 3;
	cout << sizeof(std::size_t) << endl;
	auto k1 = Tuple::make_tuple(12, 16, 205);
	Tuple k2 = Tuple::make_tuple(2, 341, 212);
	Tuple k3 = Tuple::make_tuple(34,14,521);
	Tuple k4 = Tuple::make_tuple(-12, 2, -243);
	std::unordered_map<std::size_t, Tuple> map;
	map.insert(make_pair(k1.hash(), k1));
	map.insert(make_pair(k2.hash(), k2));
	map.insert(make_pair(k3.hash(), k3));
	map.insert(make_pair(k4.hash(), k4));
	for (auto& x: map)
		cout << x.first << ":" << x.second.s[0] << ", " << x.second.s[1] << ", " << x.second.s[2] << endl;

	cout << endl;
	auto m1 = map.find(k1.hash());
	auto m2 = map.find(Tuple::make_tuple(12, 16, 203).hash());
	if (m1 == map.end()) 
		cout << "not found" << endl;
	else
		cout << "m1 found" << m1->first << ":" << m1->second.s[0] << ", " << m1->second.s[1] << ", " << m1->second.s[2] << endl;
	if (m2 == map.end()) 
		cout << "not found" << endl;
	else
		cout << "m2 found" << m2->first << ":" << m2->second.s[0] << ", " << m2->second.s[1] << ", " << m2->second.s[2] << endl;

	return 0;
}
