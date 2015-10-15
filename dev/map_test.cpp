/*************************************************************************
  > File Name: test_shell.cpp
  > Author: Li Jiaying
  > Mail: lijiaying1989@gmail.com 
  > Created Time: 2015年10月15日 星期四 13时29分26秒
 ************************************************************************/
#include <cstdlib>
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
		for (int i = 0; i < dim; i++)
			tmp->s[i] = va_arg(ap, int);
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
	
	static std::pair<std::size_t, Tuple> make_pair(Tuple& t) {
		return new std::pair<std::size_t, Tuple> (t.hash(), t);
	}
};








int main(int argc, char** argv, char** envp)
{
	dim = 3;
	Tuple k1, k2;
	k1.s[0] = 12;
	k1.s[1] = 16;
	k1.s[2] = 205;
	k2.s[0] = 2;
	k2.s[1] = 341;
	k2.s[2] = 212;
	Tuple k3 = Tuple::make_tuple(34,14,521);
	Tuple k4 = Tuple::make_tuple(-12, 2, -243);
	std::unordered_map<std::size_t, Tuple> map;
	map.insert(Tuple::make_pair(k1));
	map.insert(Tuple::make_pair(k2));
	map.insert(Tuple::make_pair(k3));
	map.insert(Tuple::make_pair(k4));
	for (auto& x: map)
		cout << x.first << ":" << x.second.s[0] << ", " << x.second.s[1] << ", " << x.second.s[2] << endl;

	cout << endl;
	auto m = map.find(t1.first);
	if (m == map.end()) 
		cout << "not found" << endl;
	else
		cout << m->first << ":" << m->second.s[0] << ", " << m->second.s[1] << ", " << m->second.s[2] << endl;

	return 0;
}
