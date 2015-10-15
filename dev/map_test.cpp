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
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;

#define MAXDIM  16
static int dim = 2;

struct Tuple {
	int s[MAXDIM];
	bool operator==(const Tuple &p) const {
		for (int i = 0; i < dim; i++) 
			if (s[i] != p.s[i])
				return false;
		return true;
	}
};


std::size_t my_hash(const Tuple& p){
	std::size_t t = 0;
	for (int i = 0; i < dim; i++) {
		t = t << 4 | p.s[i];
	}
	return t;
}




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
	std::unordered_map<std::size_t, Tuple> map;
	std::pair<std::size_t, Tuple> t1 (my_hash(k1), k1);
	std::pair<std::size_t, Tuple> t2 (my_hash(k2), k2);
	map.insert(t1);
	map.insert(t2);
	for (auto& x: map)
		cout << x.first << ":" << x.second.s[0] << ", " << x.second.s[1] << ", " << x.second.s[2] << endl;
	return 0;
/*
   pid_t pid;
   int status;

   pid = fork();
   if (pid > 0) {
   waitpid(pid, &status, 0);
   } else {
   execve("./test.sh", argv, envp);
   }
   cout << "exit value is " << status << endl;
   */
	return 0;
}
